// Project:    MT4-FST Library.dll
// Solution:   Forex Strategy Trader
// Copyright:  (c) 2011 Miroslav Popov - All rights reserved!
// This code or any part of it cannot be used in other applications without a permission.
// Website:    http://forexsb.com
// Support:    http://forexsb.com/forum
// Contacts:   info@forexsb.com

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
                     int positionTicket, int positionType, double positionLots, double positionOpenPrice,
                     double positionStopLoss, double positionTakeProfit, double positionProfit, char *positionComment);
};
