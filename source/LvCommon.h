///////////////////////////////////////////////////////////////////////////////
// FileName:    LvCommon.h
// Created:     2013/09/06
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2013
//-----------------------------------------------------------------------------
// Information: Pre-compiled Header & Global Variables
///////////////////////////////////////////////////////////////////////////////

#ifndef LVCOMMON_H
#define LVCOMMON_H

#pragma once

// ATL & WTL Headers
#include <atlbase.h>
#include <atltypes.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlddx.h>

// STL Headers
#include <string>

using namespace std;

// Local Headers
#include "resource.h"

// Global Variables
extern std::wstring g_wsConfig;

#endif // LVCOMMON_H
