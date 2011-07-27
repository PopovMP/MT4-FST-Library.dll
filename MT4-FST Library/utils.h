// Project:    MT4-FST Library.dll
// Solution:   Forex Strategy Trader
// Copyright:  (c) 2011 Miroslav Popov - All rights reserved!
// This code or any part of it cannot be used in other applications without a permission.
// Website:    http://forexsb.com
// Support:    http://forexsb.com/forum
// Contacts:   info@forexsb.com

#pragma once

#define MAXLINE      1024
#define LOG_FILENAME "experts\\logs\\MT4-FST Library.log"

void LogMsg(const char *fmt, ...);
std::string Format(char *fmt, ...);
std::vector<std::string> Split(const std::string &str, const std::string &token);
char *Fixstr(char *str);
