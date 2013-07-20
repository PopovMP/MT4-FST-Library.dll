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

#define MAXLINE      1024
#define LOG_FILENAME "experts\\logs\\MT4-FST Library.log"

void LogMsg(const char *fmt, ...);
std::string Format(char *fmt, ...);
std::vector<std::string> Split(const std::string &str, const std::string &token);
char *Fixstr(char *str);
