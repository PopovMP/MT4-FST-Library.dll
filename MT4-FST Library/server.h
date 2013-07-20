//==============================================================
// Forex Strategy Builder
// Copyright (c) Miroslav Popov. All rights reserved.
//==============================================================
// THIS CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
//==============================================================

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
    int        _id;
	int        _offset;
	int        _count;

    CRITICAL_SECTION _lock;
    std::string _request;
	std::string _response;
    volatile bool _hasRequest;
	volatile bool _hasResponse;

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
