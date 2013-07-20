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
#include "pipe.h"

using namespace std;

Pipe::Pipe(wstring name)
{
    TCHAR _userName[100];
    DWORD bufCharCount = 100;
    GetUserName(_userName, &bufCharCount);

    _name = L"\\\\.\\pipe\\" + name + _userName + L"-";
    _pipe = INVALID_HANDLE_VALUE;
}

Pipe::~Pipe()
{
    Close();
}

void Pipe::Close()
{
    if (_pipe != INVALID_HANDLE_VALUE)
    {
        BOOL rc = CloseHandle(_pipe);
        _pipe = INVALID_HANDLE_VALUE;
        if (!rc)
            throw PipeException("Error closing the pipe!", GetLastError());
    }
}

string Pipe::Read()
{
    char buf[IN_BUFFER_SIZE + 1];
    DWORD read;
    if (!ReadFile(_pipe, buf, IN_BUFFER_SIZE, &read, NULL))
        throw PipeException("Error reading from pipe!", GetLastError());

    buf[read] = 0;
    return buf;
}

void Pipe::Write(string data)
{
    DWORD written;
    if (!WriteFile(_pipe, data.c_str(), data.length(), &written, NULL))
        throw PipeException("Error writing to pipe!", GetLastError());

    if (written != data.length())
        throw PipeException("Incomplete write to pipe!");
}


void ClientPipe::Connect(int id)
{
    Close();

    wstringstream pipename;
    pipename << _name << id;

    _pipe = CreateFile(pipename.str().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (_pipe == INVALID_HANDLE_VALUE)
        throw PipeException("Error connecting to pipe!", GetLastError());
}

string ClientPipe::Command(string cmd)
{
    Write(cmd);
    return Read();
}

void ServerPipe::Create(int id)
{
    Close();

    wstringstream pipename;
    pipename << _name << id;

    _pipe = CreateNamedPipe(pipename.str().c_str(),
        PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
        1, OUT_BUFFER_SIZE, IN_BUFFER_SIZE, PIPE_TIMEOUT, NULL);
    if (_pipe == INVALID_HANDLE_VALUE)
        throw PipeException("Error creating the pipe!", GetLastError());
}

string ServerPipe::Request()
{
    if (!ConnectNamedPipe(_pipe, NULL))
        throw PipeException("Error connecting client!", GetLastError());
    return Read();
}

void ServerPipe::Response(string response)
{
    Write(response);
    if (!DisconnectNamedPipe(_pipe))
        throw PipeException("Error disconnecting client!", GetLastError());
}
