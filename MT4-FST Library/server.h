// Project:    MT4-FST Library.dll
// Solution:   Forex Strategy Trader
// Copyright:  (c) 2011 Miroslav Popov - All rights reserved!
// This code or any part of it cannot be used in other applications without a permission.
// Website:    http://forexsb.com
// Support:    http://forexsb.com/forum
// Contacts:   info@forexsb.com

#pragma once

#include "pipe.h"

#define SERVER_REQUEST_TIME    50
#define SERVER_REQUEST_COUNT   10
#define SERVER_RESPONSE_TIME   20
#define SERVER_RESPONSE_COUNT 500

class Server
{
    ServerPipe _pipe;
    HANDLE     _thread;
    bool       _active;
    int        _id, _offset, _count;

    CRITICAL_SECTION _lock;
    std::string _request, _response;
    volatile bool _hasRequest, _hasResponse;

    static DWORD WINAPI Run(LPVOID lpParameter);
    void WaitForResponse();
    void PostRequest(std::string request);
    std::string GetResponse();

public:
    Server(int id);
    ~Server();

    void Offset(int offset) { _offset = offset; }
    void Count(int count) { _count = count; }
    int Offset() { return _offset; }
    int Count() { return _count; }

    bool IsActive() { return _active; }
    bool HasRequest() { return _hasRequest; }
    std::string GetRequest();
    void PostResponse(std::string response);
};
