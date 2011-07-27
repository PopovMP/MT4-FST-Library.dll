// Project:    MT4-FST Library.dll
// Solution:   Forex Strategy Trader
// Copyright:  (c) 2011 Miroslav Popov - All rights reserved!
// This code or any part of it cannot be used in other applications without a permission.
// Website:    http://forexsb.com
// Support:    http://forexsb.com/forum
// Contacts:   info@forexsb.com

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
