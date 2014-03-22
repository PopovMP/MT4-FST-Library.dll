//==============================================================
// Forex Strategy Builder
// Copyright (c) Miroslav Popov. All rights reserved.
//==============================================================
// THIS CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
//==============================================================

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MTFST_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MTFST_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef  MTFST_EXPORTS
#define MTFST_API __declspec(dllexport)
#else
#define MTFST_API __declspec(dllimport)
#endif

#define LIBRARY_VERSION      "6.0"

#define FST_REQ_ERROR           -1
#define FST_REQ_NONE             0
#define FST_REQ_SYMBOL_INFO      1
#define FST_REQ_MARKET_INFO      2
#define FST_REQ_ACCOUNT_INFO     3
#define FST_REQ_BARS             4
#define FST_REQ_ORDERS           5
#define FST_REQ_ORDER_INFO       6
#define FST_REQ_ORDER_SEND       7
#define FST_REQ_ORDER_MODIFY     8
#define FST_REQ_ORDER_CLOSE      9
#define FST_REQ_ORDER_DELETE    10
#define FST_REQ_PING            11
#define FST_REQ_MARKET_INFO_ALL 12
#define FST_REQ_TERMINAL_INFO   13
#define FST_REQ_SET_LTF_META    14

#pragma pack(push, 1)
struct RateInfo
{
    double   time;
    double   open;
    double   low;
    double   high;
    double   close;
    double   volume;
};
#pragma pack(pop)

struct MqlString
{
    int   len;
    char *string;
};
