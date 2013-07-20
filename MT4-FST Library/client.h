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

class Client
{
    static bool ResponseOK(std::string response);
    static std::string Command(int id, std::string cmd);

public:
    static bool Tick(int id, const char *symbol, int period, int time, double bid, double ask, int spread, double tickvalue,
                     int bartime, double open, double high, double low, double close, int volume, int bartime10,
                     double accountBalance, double accountEquity, double accountProfit, double accountFreeMargin,
                     int positionTicket, int positionType, double positionLots, double positionOpenPrice, int positionOpenTime,
                     double positionStopLoss, double positionTakeProfit, double positionProfit, char *positionComment,
					 char *parameters);
};
