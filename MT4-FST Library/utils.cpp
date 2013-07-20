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
#include "utils.h"

using namespace std;

void log_message(const char *filename, const char *message)
{
    FILE *f = fopen(filename, "a");
    if (!f)
        return;

    time_t tt = time(NULL);
    struct tm *ti = localtime(&tt);
    fprintf(f, "%02d.%02d.%d %02d:%02d:%02d %s\n", ti->tm_mday, ti->tm_mon + 1, ti->tm_year + 1900, ti->tm_hour, ti->tm_min, ti->tm_sec, message);

    fclose(f);
}

void log_doit(bool errnoflag, const char *fmt, va_list ap)
{
    char buf[MAXLINE];
    vsnprintf(buf, MAXLINE - 1, fmt, ap);

    if (errnoflag)
        _snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s", strerror(errno));

    log_message(LOG_FILENAME, buf);
}

void LogMsg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_doit(false, fmt, ap);
    va_end(ap);
}

string Format(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    char *out = static_cast<char *>(calloc(MAXLINE, sizeof(char)));
    int i = 1;
    while(vsnprintf(out, MAXLINE * i, fmt, ap) >= MAXLINE * i) 
    {
        i++;
        out = static_cast<char *>(realloc(out, MAXLINE * i * sizeof(char)));
    }
    string s = out;
    free(out);
    va_end(ap);
    return s;
}

vector<string> Split(const string &str, const string &token)
{
    vector<string> rc(0);
    string s;

    for(size_t i = 0; i < str.size(); i++)
    {
        if (str.substr(i, token.size()).compare(token) == 0) 
        {
            if (s.length() > 0)
                rc.push_back(s);
            s.clear();
            i += token.size() - 1;
        } 
        else
        {
            s.append(1, str[i]);
            if (i == (str.size() - 1))
                rc.push_back(s);
        }
    }
    return rc;
}

char *Fixstr(char *str)
{
    for(size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ') 
            str[i] = '_';
        else if (str[i] == '\\')
            str[i] = '|';
    }
    return str;
}
