///////////////////////////////////////////////////////////////////////////////
// FileName:    DumpHex.cpp
// Created:     2009/04/10
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2009
//-----------------------------------------------------------------------------
// Information: DumpHex Implementation
///////////////////////////////////////////////////////////////////////////////

#include "LvCommon.h"
#include "DumpHex.h"
#include <vector>
#include "..\lua\lua.hpp"

#define BUF_SIZE    4096
#define IO_SIZE     0x10

DHERR WINAPI DumpHex(HANDLE hDst, HANDLE hSrc, PCWSTR script, DUMPROUTINE pfn, PVOID param)
{
    // 打开 lua
    lua_State *lua = luaL_newstate();
    if (NULL == lua)
        return DHERR_INIT_FAILED;
    // 打开 lua 库
    luaL_openlibs(lua);

    // 加载脚本文件
    USES_CONVERSION;
    int err = luaL_loadfile(lua, W2CA(script));
    if (0 != err)
    {
        lua_close(lua);
        return DHERR_LOAD_FAILED;
    }

    err = lua_pcall(lua, 0, LUA_MULTRET, 0);
    if (0 != err)
    {
        lua_close(lua);
        return DHERR_LOAD_FAILED;
    }

    DWORD cbLeft = GetFileSize(hSrc, NULL);
    DWORD cbDone = 0;

    vector<BYTE> inbuf(BUF_SIZE);
    vector<char> outbuf(BUF_SIZE);
    DWORD inptr = 0, outptr = 0;

    PCSTR txt = NULL;
    while (cbLeft > 0)
    {
        DWORD cbRead = 0;
        ReadFile(hSrc, &inbuf[0], BUF_SIZE, &cbRead, NULL);
        cbLeft -= cbRead;

        inptr = 0;
        while (inptr < cbRead)
        {
            // 开始转换字节
            DWORD cbIo = cbRead - inptr;
            if (cbIo > IO_SIZE)
                cbIo = IO_SIZE;

            // 调用脚本的转换函数
            lua_getglobal(lua, "bin2txt");
            lua_pushlstring(lua, reinterpret_cast<PCSTR>(&inbuf[inptr]), cbIo);
            if (0 != lua_pcall(lua, 1, 1, 0))
            {
                lua_close(lua);
                return DHERR_RUN_FAILED;
            }
            ATLASSERT(lua_isstring(lua, -1));

            inptr += cbIo;
            txt = lua_tostring(lua, -1);

            // 输出缓冲区是否已满？
            DWORD len = lstrlenA(txt);
            if (len > BUF_SIZE - outptr)
            {
                DWORD dwWritten;
                WriteFile(hDst, &outbuf[0], outptr, &dwWritten, NULL);
                outptr = 0;
            }

            // 输出字符
            lstrcpyA(&outbuf[outptr], txt);
            outptr += len;

            lua_pop(lua, 1);
        }

        cbDone += cbRead;
        if (NULL != pfn)
            pfn(cbDone, param);
    }

    if (outptr > 0)
    {
        DWORD dwWritten;
        WriteFile(hDst, &outbuf[0], outptr, &dwWritten, NULL);
        outptr = 0;
    }

    lua_close(lua);
    return DHERR_SUCCESS;
}
