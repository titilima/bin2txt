///////////////////////////////////////////////////////////////////////////////
// FileName:    MainDlg.h
// Created:     2009/04/10
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2009
//-----------------------------------------------------------------------------
// Information: Main Dialog
///////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CWinDataExchange<CMainDlg>
{
public:
    CMainDlg(void);
    enum { IDD = IDD_MAIN_DLG };
private:
    BOOL AutoName(void);
    void EnableControls(BOOL bEnable);
    void EnableDlgItem(int id, BOOL bEnable = TRUE);
    BOOL FillComboBox(void);
    void OpenDestFile(void);
public:
    BEGIN_MSG_MAP_EX(CMainDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnBtnExit)
        COMMAND_ID_HANDLER_EX(IDC_BTN_DST, OnBtnDst)
        COMMAND_ID_HANDLER_EX(IDC_BTN_SRC, OnBtnSrc)
        COMMAND_ID_HANDLER_EX(IDC_BTN_START, OnBtnStart)
        COMMAND_HANDLER_EX(IDC_EDIT_SRC, EN_UPDATE, OnSrcUpdate)
        COMMAND_HANDLER_EX(IDC_CMB_SCRIPT, CBN_SELCHANGE, OnScriptChange)
    END_MSG_MAP()
    BEGIN_DDX_MAP(CMainDlg)
        DDX_CONTROL_HANDLE(IDC_CMB_SCRIPT, m_cmbScripts)
        DDX_CONTROL_HANDLE(IDC_PROGRESS, m_pb)
        DDX_CONTROL(IDC_ST_HOMEPAGE, m_stHomePage)
    END_DDX_MAP()
private:
    BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
    void OnBtnDst(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnExit(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnSrc(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnBtnStart(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnScriptChange(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnSrcUpdate(UINT uNotifyCode, int nID, CWindow wndCtl);
private:
    static DWORD WINAPI DumpProc(PVOID param);
    static void WINAPI OnProgress(DWORD dwBytesDone, PVOID param);
private:
    CComboBox m_cmbScripts;
    CProgressBarCtrl m_pb;
    CHyperLink m_stHomePage;
    BOOL m_bAuto;            // 是否自动命名
};
