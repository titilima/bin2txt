///////////////////////////////////////////////////////////////////////////////
// FileName:    DumpHex.h
// Created:     2009/04/10
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2009
//-----------------------------------------------------------------------------
// Information: DumpHex Declaration
///////////////////////////////////////////////////////////////////////////////

#pragma once

typedef enum {
    DHERR_SUCCESS = 0,
    DHERR_INIT_FAILED,
    DHERR_LOAD_FAILED,
    DHERR_RUN_FAILED
} DHERR;

typedef void (WINAPI * DUMPROUTINE)(DWORD dwBytesDone, PVOID param);

DHERR WINAPI DumpHex(HANDLE hDst, HANDLE hSrc, PCWSTR script, DUMPROUTINE pfn, PVOID param);
