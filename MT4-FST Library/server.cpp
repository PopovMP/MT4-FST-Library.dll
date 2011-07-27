// Project:    MT4-FST Library.dll
// Solution:   Forex Strategy Trader
// Copyright:  (c) 2011 Miroslav Popov - All rights reserved!
// This code or any part of it cannot be used in other applications without a permission.
// Website:    http://forexsb.com
// Support:    http://forexsb.com/forum
// Contacts:   info@forexsb.com

#include "stdafx.h"
#include "server.h"
#include "utils.h"

using namespace std;

Server::Server(int id) : _pipe(SERVER_PIPE_NAME)
{
    _id          = id;
    _hasRequest  = false;
    _hasResponse = false;
    _thread      = NULL;
    _active      = true;

    try 
    {
        _pipe.Create(_id);
    }
    catch (Exception &e) 
    {
        if (e.code() == ERROR_PIPE_BUSY)
            _active = false;
        else 
        {
            LogMsg("Exception: '%s'", e.what());
            throw e;
        }
    }

    if (!InitializeCriticalSectionAndSpinCount(&_lock, 0x1000))
        throw Exception("Error creating the lock object!", GetLastError());

    if (_active)
    {
        _thread = CreateThread(NULL, 0, Run, this, 0, NULL);
        if (_thread == NULL)
            throw Exception("Error creating the server thread!", GetLastError());
    }
}

Server::~Server()
{
    if (_thread != NULL)
        if (!TerminateThread(_thread, 0))
            throw Exception("Error terminating the server thread!", GetLastError());

    DeleteCriticalSection(&_lock);
}

DWORD WINAPI Server::Run(LPVOID lpParameter)
{
    Server* pThis = (Server*)lpParameter;
    while (true)
    {
        try 
        {
            pThis->PostRequest(pThis->_pipe.Request());
            pThis->WaitForResponse();
            pThis->_pipe.Response(pThis->GetResponse());
        } 
        catch (Exception &e) 
        {
            LogMsg("Exception: %s Code: %d", e.what(), e.code());
            pThis->_pipe.Create(pThis->_id);
        }
    }

    return 0;
}

void Server::WaitForResponse()
{
    for (int i = 0; i < SERVER_REQUEST_COUNT && _hasRequest; i++)
        Sleep(SERVER_REQUEST_TIME);

    if (_hasRequest)
    {
        PostResponse("ER EA not running");
        return;
    }

    for (int i = 0; i < SERVER_RESPONSE_COUNT && !_hasResponse; i++)
        Sleep(SERVER_RESPONSE_TIME);

    if (!_hasResponse)
        PostResponse("ER EA not responding");
}

void Server::PostRequest(string request)
{
    if (_hasRequest)
        throw Exception("PostRequest overlap!");

    EnterCriticalSection(&_lock);
    _request     = request;
    _hasRequest  = true;
    _hasResponse = false;
    LeaveCriticalSection(&_lock);
}

void Server::PostResponse(string response)
{
    if (_hasResponse)
        throw Exception("PostResponse overlap!");

    EnterCriticalSection(&_lock);
    _response    = response;
    _hasResponse = true;
    _hasRequest  = false;
    LeaveCriticalSection(&_lock);
}

string Server::GetRequest()
{
    if (!_hasRequest)
        throw Exception("GetRequest with no request!");

    EnterCriticalSection(&_lock);
    _hasRequest = false;
    string rc = _request;
    LeaveCriticalSection(&_lock);

    return rc;
}

string Server::GetResponse()
{
    if (!_hasResponse)
        throw Exception("GetResponse with no response!");

    EnterCriticalSection(&_lock);
    _hasResponse = false;
    string rc = _response;
    LeaveCriticalSection(&_lock);

    return rc;
}
