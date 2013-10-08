///////////////////////////////////////////////////////////////////////////////
// FileName:    MainDlg.cpp
// Created:     2009/04/10
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2009
//-----------------------------------------------------------------------------
// Information: Main Dialog Implementation
///////////////////////////////////////////////////////////////////////////////

#include "LvCommon.h"
#include "MainDlg.h"
#include <atldlgs.h>
#include "DumpHex.h"

#include "Version.h"

#define SCRIPTS_DIR L"scripts\\"
#define SCRIPTS_LEN 8

typedef struct _tagThreadParam {
    CMainDlg* dlg;
    HANDLE hSrc;
    HANDLE hDst;
    WCHAR script[MAX_PATH];
} THREADPARAM, *PTHREADPARAM;

CMainDlg::CMainDlg(void)
{
    m_bAuto = TRUE;
}

BOOL CMainDlg::AutoName(void)
{
    if (!m_bAuto)
        return FALSE;

    TCHAR src[MAX_PATH];
    GetDlgItemText(IDC_EDIT_SRC, src, MAX_PATH);
    if (!PathFileExists(src))
        return FALSE;

    // 文件名称
    PTSTR p1 = _tcsrchr(src, _T('\\'));
    PTSTR p2 = _tcsrchr(p1, _T('.'));
    if (NULL == p2)
        p2 = _tcsrchr(p1, _T('\0'));
    else
        ++p2;

    // 脚本名称
    GetDlgItemText(IDC_CMB_SCRIPT, p2, MAX_PATH);
    lstrcpy(_tcsrchr(p2, _T('.')) + 1, _T("txt"));
    SetDlgItemText(IDC_EDIT_DST, src);
    return TRUE;
}

void CMainDlg::EnableControls(BOOL bEnable)
{
    EnableDlgItem(IDC_EDIT_SRC, bEnable);
    EnableDlgItem(IDC_BTN_SRC, bEnable);
    EnableDlgItem(IDC_EDIT_DST, bEnable);
    EnableDlgItem(IDC_BTN_DST, bEnable);
    EnableDlgItem(IDC_CMB_SCRIPT, bEnable);
}

void CMainDlg::EnableDlgItem(int id, BOOL bEnable)
{
    GetDlgItem(id).EnableWindow(bEnable);
}

BOOL CMainDlg::FillComboBox(void)
{
    m_cmbScripts.ResetContent();

    WCHAR wzFind[MAX_PATH];
    GetModuleFileName(NULL, wzFind, MAX_PATH);
    PathRemoveFileSpecW(wzFind);
    PathAppendW(wzFind, SCRIPTS_DIR L"*.lua");

    WIN32_FIND_DATAW wfd;
    HANDLE hFind = FindFirstFileW(wzFind, &wfd);
    if (INVALID_HANDLE_VALUE == hFind)
        return FALSE;

    USES_CONVERSION;
    do
    {
        m_cmbScripts.AddString(W2CT(wfd.cFileName));
    } while (FindNextFileW(hFind, &wfd));
    return TRUE;
}

void CMainDlg::OpenDestFile(void)
{
    if (BST_CHECKED != IsDlgButtonChecked(IDC_CHK_OPEN))
        return;

    TCHAR szFile[MAX_PATH];
    GetDlgItemText(IDC_EDIT_DST, szFile, MAX_PATH);
    ShellExecute(NULL, _T("open"), szFile, NULL, NULL, SW_SHOWNORMAL);
}

void CMainDlg::OnBtnDst(UINT, int, CWindow)
{
    const DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(FALSE, _T("*.txt"), NULL, dwFlags, _T("Text Files\0*.txt\0\0"));
    if (dlg.DoModal(m_hWnd))
    {
        // 用户设置目标文件后，禁用自动命名
        m_bAuto = FALSE;
        SetDlgItemText(IDC_EDIT_DST, dlg.m_szFileName);
    }
}

void CMainDlg::OnBtnExit(UINT, int, CWindow)
{
    EndDialog(EXIT_SUCCESS);
}

void CMainDlg::OnBtnSrc(UINT, int, CWindow)
{
    const DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(FALSE, NULL, NULL, dwFlags, _T("All Files\0*.*\0\0"));
    if (dlg.DoModal(m_hWnd))
    {
        // 重新打开文件后，允许自动命名
        m_bAuto = TRUE;
        SetDlgItemText(IDC_EDIT_SRC, dlg.m_szFileName);
        m_pb.SetPos(0);
    }
}

void CMainDlg::OnBtnStart(UINT, int, CWindow)
{
    PTHREADPARAM param = new THREADPARAM;
    param->dlg = this;

    TCHAR szFile[MAX_PATH];
    GetDlgItemText(IDC_EDIT_SRC, szFile, MAX_PATH);
    param->hSrc = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (INVALID_HANDLE_VALUE == param->hSrc)
    {
        MessageBox(_T("无法打开来源文件。"), _T("错误"), MB_ICONINFORMATION);
        return;
    }

    GetDlgItemText(IDC_EDIT_DST, szFile, MAX_PATH);
    param->hDst = CreateFile(szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE,
        NULL);
    if (INVALID_HANDLE_VALUE == param->hDst)
    {
        CloseHandle(param->hSrc);
        return;
    }

    GetModuleFileNameW(NULL, param->script, MAX_PATH);
    PathRemoveFileSpecW(param->script);
    PathAppendW(param->script, SCRIPTS_DIR);

    TCHAR sz[MAX_PATH];
    GetDlgItemText(IDC_CMB_SCRIPT, sz, MAX_PATH);

    USES_CONVERSION;
    PathAppendW(param->script, T2CW(sz));
    WritePrivateProfileStringW(L"Config", L"script", T2CW(sz), g_wsConfig.c_str());

    m_pb.SetRange32(0, (int)GetFileSize(param->hSrc, NULL));
    EnableDlgItem(IDC_BTN_START, FALSE);
    EnableControls(FALSE);
    HANDLE hThread = CreateThread(NULL, 0, DumpProc, param, 0, NULL);
    CloseHandle(hThread);
}

BOOL CMainDlg::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
    HICON hIcon = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON_MAIN));
    SetIcon(hIcon, TRUE);
    SetIcon(hIcon, FALSE);

    SetWindowText(BIN2TXT_WNDCAPTION);

    DoDataExchange(FALSE);

    if (!FillComboBox())
    {
        MessageBox(_T("没有可以使用的 lua 脚本，")
            _T("请确认应用程序的 scripts 文件夹下是否存在符合 bin2txt 规格的 lua 脚本。"),
            _T("错误"), MB_ICONINFORMATION);
        EndDialog(0);
    }

    // 读取上次使用的脚本
    WCHAR script[MAX_PATH];
    GetPrivateProfileStringW(L"Config", L"script", L"", script, MAX_PATH, g_wsConfig.c_str());

    USES_CONVERSION;
    int idx = m_cmbScripts.FindString(0, W2CT(script));
    if (CB_ERR == idx)
        idx = 0;
    m_cmbScripts.SetCurSel(idx);

    CheckDlgButton(IDC_CHK_OPEN, BST_CHECKED);
    return FALSE;
}

DWORD WINAPI CMainDlg::DumpProc(PVOID param)
{
    PTHREADPARAM p = (PTHREADPARAM)param;
    CMainDlg* dlg = p->dlg;

    DHERR err = DumpHex(p->hDst, p->hSrc, p->script, OnProgress, dlg);

    PCTSTR pszInfo = NULL;
    switch (err)
    {
    case DHERR_INIT_FAILED:
        pszInfo = _T("初始化失败。");
        break;
    case DHERR_LOAD_FAILED:
        pszInfo = _T("加载脚本失败。");
        break;
    case DHERR_RUN_FAILED:
        pszInfo = _T("运行脚本失败。");
        break;
    default:
        ATLASSERT(false);
    }

    if (NULL != pszInfo)
        dlg->MessageBox(pszInfo, _T("错误"), MB_ICONINFORMATION);
    else
        dlg->OpenDestFile();

    delete p;
    dlg->EnableDlgItem(IDC_BTN_START);
    dlg->EnableControls(TRUE);
    return 0;
}

void WINAPI CMainDlg::OnProgress(DWORD dwBytesDone, PVOID param)
{
    CMainDlg* dlg = (CMainDlg*)param;
    dlg->m_pb.SetPos((int)dwBytesDone);
}

void CMainDlg::OnScriptChange(UINT, int, CWindow)
{
    AutoName();
}

void CMainDlg::OnSrcUpdate(UINT, int, CWindow)
{
    AutoName();
}
