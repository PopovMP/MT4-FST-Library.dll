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

#define PIPE_TIMEOUT     500
#define IN_BUFFER_SIZE   (2  * 1024)
#define OUT_BUFFER_SIZE  (50 * 1024)
#define SERVER_PIPE_NAME L"FST-MT4_"
#define CLIENT_PIPE_NAME L"MT4-FST_"

class Exception : public std::exception
{
    std::string _msg;
    int _code;

public:
    Exception() : std::exception()
    {
        _msg  = "Exception";
        _code = 0;
    }
    Exception(std::string msg) : std::exception()
    {
        _msg  = msg;
        _code = 0;
    }
    Exception(std::string msg, int code) : std::exception()
    {
        std::stringstream ss;
        ss << msg << " ErrorCode: " << code;
        _msg  = ss.str();
        _code = code;
    }

    virtual const char *what() const { return _msg.c_str(); }
    int code() const { return _code; }
};

class PipeException : public Exception
{
public:
    PipeException() : Exception("PipeException") { }
    PipeException(std::string msg) : Exception(msg) { }
    PipeException(std::string msg, int code) : Exception(msg, code) { }
};


class Pipe
{
protected:
    std::wstring _name;
    HANDLE _pipe;

    std::string Read();
    void Write(std::string data);

public:
    Pipe(std::wstring name);
    ~Pipe();
    void Close();
};


class ClientPipe : public Pipe
{
public:
    ClientPipe(std::wstring name) : Pipe(name) { }
    void Connect(int id);
    std::string Command(std::string command);
};


class ServerPipe : public Pipe
{
public:
    ServerPipe(std::wstring name) : Pipe(name) { }
    void Create(int id);
    std::string Request();
    void Response(std::string response);
};
