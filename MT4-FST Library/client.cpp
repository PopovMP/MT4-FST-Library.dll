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
#include "client.h"
#include "utils.h"

using namespace std;

bool Client::ResponseOK(string response)
{
    return response.length() == 2 && response.find("OK") == 0;
}

string Client::Command(int id, string command)
{
    ClientPipe pipe(CLIENT_PIPE_NAME);
    try 
    {
        pipe.Connect(id);
        string rc = pipe.Command(command);
        pipe.Close();
        
        if (!ResponseOK(rc))
        {
            LogMsg("CL > %s", command.c_str());
            LogMsg("CL < %s", command.c_str());
        }
        
        return rc;
    }
    catch (Exception &e)
    {
        pipe.Close();
        
        if (e.code() != ERROR_FILE_NOT_FOUND)
            LogMsg("Exception: %s Command: '%s'", e.what(), command.c_str());
        
        return string("ER ") + e.what();
    }
}

bool Client::Tick(int id, const char *symbol, int period, int time, double bid, double ask, int spread, double tickvalue,
                  int bartime, double open, double high, double low, double close, int volume, int bartime10,
                  double accountBalance, double accountEquity, double accountProfit, double accountFreeMargin,
                  int positionTicket, int positionType, double positionLots, double positionOpenPrice, int positionOpenTime,
                  double positionStopLoss, double positionTakeProfit, double positionProfit, char *positionComment,
				  char *parameters)
{
    string cmd = Format("TI %s %d %d %.5f %.5f %d %.5f %d %.5f %.5f %.5f %.5f %d %d %.2f %.2f %.2f %.2f %d %d %.2f %.5f %d %.5f %.5f %.2f %s %s",
                        symbol, period, time, bid, ask, spread, tickvalue, bartime, open, high, low, close, volume, bartime10,
                        accountBalance, accountEquity, accountProfit, accountFreeMargin, positionTicket, positionType,
                        positionLots, positionOpenPrice, positionOpenTime, positionStopLoss, positionTakeProfit, positionProfit, Fixstr(positionComment),
						parameters);
    
    string rc  = Command(id, cmd);

    return ResponseOK(rc);
}
