///////////////////////////////////////////////////////////////////////////////
// FileName:    bin2txt.cpp
// Created:     2009/04/10
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2009
//-----------------------------------------------------------------------------
// Information: Program Entry
///////////////////////////////////////////////////////////////////////////////

#include "LvCommon.h"
#include "MainDlg.h"

static wstring GetConfigFileName(void)
{
    WCHAR wzPath[MAX_PATH];
    GetModuleFileNameW(NULL, wzPath, MAX_PATH);
    PathRemoveFileSpecW(wzPath);
    PathAppendW(wzPath, L"bin2txt.ini");
    return wstring(wzPath);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR, int)
{
    AtlInitCommonControls(ICC_PROGRESS_CLASS);
    _Module.Init(NULL, hInstance);

    g_wsConfig = GetConfigFileName();

    CMainDlg dlg;
    dlg.DoModal(NULL);

    _Module.Term();
    return EXIT_SUCCESS;
}
