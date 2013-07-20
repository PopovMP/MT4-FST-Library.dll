//==============================================================
// Forex Strategy Builder
// Copyright (c) Miroslav Popov. All rights reserved.
//==============================================================
// THIS CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
//==============================================================

#include "stdafx.h"
#include "server.h"

using namespace std;

extern map<int, Server*> servers;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            for (map<int, Server*>::iterator i = servers.begin(); i != servers.end(); i++)
                delete i->second;
            break;
    }

    return TRUE;
}
