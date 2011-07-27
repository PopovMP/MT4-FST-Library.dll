// Project:    MT4-FST Library.dll
// Solution:   Forex Strategy Trader
// Copyright:  (c) 2011 Miroslav Popov - All rights reserved!
// This code or any part of it cannot be used in other applications without a permission.
// Website:    http://forexsb.com
// Support:    http://forexsb.com/forum
// Contacts:   info@forexsb.com

#include "stdafx.h"
#include "MT4-FST Library.h"
#include "client.h"
#include "server.h"
#include "utils.h"

using namespace std;

map<int, Server*> servers;

///
/// Gets the library version.
///
MTFST_API char *__stdcall FST_LibraryVersion()
{
    return LIBRARY_VERSION;
}

///
/// Creates a server with selected ID.
///
MTFST_API void __stdcall FST_OpenConnection(int id)
{
    if (servers.find(id) != servers.end())
        delete servers[id];
    servers[id] = new Server(id);
}

///
/// Deletes a server with selected ID.
///
MTFST_API void __stdcall FST_CloseConnection(int id)
{
    if (servers.find(id) != servers.end()) {
        delete servers[id];
        servers.erase(id);
    }
}

///
/// Sends tick information.
/// Returns: Error -1; Tick received 1; No response 0. 
///
MTFST_API int __stdcall FST_Tick(int id, const char *symbol, int period, int ticktime, double bid, double ask, int spread, double tickvalue, const RateInfo *rates, int bars,
                                 double accountBalance, double accountEquity, double accountProfit, double accountFreeMargin, int positionTicket,
                                 int positionType, double positionLots, double positionOpenPrice, double positionStopLoss, double positionTakeProfit,
                                 double positionProfit, char *positionComment)
{
    if (symbol == NULL || rates == NULL || bars <= 0)
    {
        LogMsg("FST_Tick invalid arguments!");
        return -1;
    }

    int    bartime = rates[bars - 1].time;
    double open    = rates[bars - 1].open;
    double high    = rates[bars - 1].high;
    double low     = rates[bars - 1].low;
    double close   = rates[bars - 1].close;
    int    volume  = (int)rates[bars - 1].volume;

    int bartime10 = rates[bars - 11].time;

    bool bResponce = Client::Tick(id, symbol, period, ticktime, bid, ask, spread, tickvalue, bartime, open, high, low, close, volume, bartime10,
                                  accountBalance, accountEquity, accountProfit, accountFreeMargin, positionTicket, positionType,
                                  positionLots, positionOpenPrice, positionStopLoss, positionTakeProfit, positionProfit, positionComment);

    return bResponce ? 1 : 0;
}

///
/// Checks FST request if any.
///
MTFST_API int __stdcall FST_Request(int id, MqlString *symbol, int *iargs, int icount, double *dargs, int dcount, MqlString *parameters)
{
    if (servers.find(id) == servers.end())
        return FST_REQ_ERROR;
    Server *server = servers[id];

    if (!server->IsActive())
        return FST_REQ_ERROR;

    if (!server->HasRequest())
        return FST_REQ_NONE;

    if (symbol == NULL || symbol->string == NULL || iargs == NULL || dargs == NULL) 
    {
        LogMsg("FST_Request invalid arguments!");
        return FST_REQ_ERROR;
    }

    if (icount < 5 || dcount < 5)
    {
        LogMsg("FST_Request invalid argument size!");
        return FST_REQ_ERROR;
    }

    string request = server->GetRequest();
    if (request.length() < 2)
    {
        server->PostResponse("ER Invalid request");
        return FST_REQ_NONE;
    }

    string cmd = request.substr(0, 2);
    transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);
    vector<string> args = request.length() > 3 ? Split(request.substr(3), " ") : vector<string>(0);

    // Ping
    if (cmd == "PI")
    {
        if (args.size() != 0)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        return FST_REQ_PING;
    }

    // Symbol info
    else if (cmd == "SI")
    {
        if (args.size() != 1)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        if ((unsigned)symbol->length < args[0].length())
        {
            server->PostResponse("ER EA no space for symbol name");
            return FST_REQ_NONE;
        }

        strcpy(symbol->string, args[0].c_str());

        return FST_REQ_SYMBOL_INFO;
    } 

    // Market info element
    else if (cmd == "MI")
    {
        if (args.size() != 2)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        if ((unsigned)symbol->length < args[0].length())
        {
            server->PostResponse("ER EA no space for symbol name");
            return FST_REQ_NONE;
        }

        strcpy(symbol->string, args[0].c_str());
        iargs[0] = atoi(args[1].c_str());

        return FST_REQ_MARKET_INFO;
    } 

    // Market info all elements
    else if (cmd == "MA") 
    {
        if (args.size() != 1)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        if ((unsigned)symbol->length < args[0].length())
        {
            server->PostResponse("ER EA no space for symbol name");
            return FST_REQ_NONE;
        }

        strcpy(symbol->string, args[0].c_str());

        return FST_REQ_MARKET_INFO_ALL;
    } 

    // Account info
    else if (cmd == "AI") 
    {
        if (args.size() != 0)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        return FST_REQ_ACCOUNT_INFO;
    } 

    // Terminal info
    else if (cmd == "TE") 
    {
        if (args.size() != 0)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        return FST_REQ_TERMINAL_INFO;
    }

    // Bars
    else if (cmd == "BR")
    {
        if (args.size() != 4)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        if ((unsigned)symbol->length < args[0].length())
        {
            server->PostResponse("ER EA no space for symbol name");
            return FST_REQ_NONE;
        }

        strcpy(symbol->string, args[0].c_str());
        server->Offset(atoi(args[2].c_str()));
        server->Count(atoi(args[3].c_str()));

        iargs[0] = atoi(args[1].c_str()); // Period
        iargs[1] = atoi(args[3].c_str()); // Bars

        return FST_REQ_BARS;
    }

    // Orders
    else if (cmd == "OR")
    {
        if (args.size() != 0 && args.size() != 1) 
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        *(symbol->string) = 0;
        if (args.size() == 1)
        {
            if ((unsigned)symbol->length < args[0].length())
            {
                server->PostResponse("ER EA no space for symbol name");
                return FST_REQ_NONE;
            }

            strcpy(symbol->string, args[0].c_str());
        }

        return FST_REQ_ORDERS;
    }

    // Order Info
    else if (cmd == "OI")
    {
        if (args.size() != 1)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        iargs[0] = atoi(args[0].c_str());

        return FST_REQ_ORDER_INFO;
    }

    // Order Send
    else if (cmd == "OS")
    {
        if (args.size() != 10)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        if ((unsigned)symbol->length < args[0].length())
        {
            server->PostResponse("ER EA no space for symbol name");
            return FST_REQ_NONE;
        }

        strcpy(symbol->string, args[0].c_str());
        iargs[0] = atoi(args[1].c_str()); //type
        dargs[0] = atof(args[2].c_str()); //lots
        dargs[1] = atof(args[3].c_str()); //price
        iargs[1] = atoi(args[4].c_str()); //slippage
        dargs[2] = atof(args[5].c_str()); //stop loss
        dargs[3] = atof(args[6].c_str()); //take profit
        iargs[2] = atoi(args[7].c_str()); //magic
        iargs[3] = atoi(args[8].c_str()); //expire
        strcpy(parameters->string, args[9].c_str()); // Order parameters

        return FST_REQ_ORDER_SEND;
    }

    // Order Modify
    else if (cmd == "OM")
    {
        if (args.size() != 6)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        iargs[0] = atoi(args[0].c_str()); //ticket
        dargs[0] = atof(args[1].c_str()); //price
        dargs[1] = atof(args[2].c_str()); //stop loss
        dargs[2] = atof(args[3].c_str()); //take profit
        iargs[1] = atoi(args[4].c_str()); //expire
        strcpy(parameters->string, args[5].c_str()); // Order parameters

        return FST_REQ_ORDER_MODIFY;
    }

    // Order Close
    else if (cmd == "OC")
    {
        if (args.size() != 4)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        iargs[0] = atoi(args[0].c_str()); //ticket
        dargs[0] = atof(args[1].c_str()); //lots
        dargs[1] = atof(args[2].c_str()); //price
        iargs[1] = atoi(args[3].c_str()); //slippage

        return FST_REQ_ORDER_CLOSE;
    } 

    // Order Delete
    else if (cmd == "OD")
    {
        if (args.size() != 1)
        {
            server->PostResponse("ER Invalid number of arguments");
            return FST_REQ_NONE;
        }

        iargs[0] = atoi(args[0].c_str()); //ticket

        return FST_REQ_ORDER_DELETE;
    }

    server->PostResponse("ER Invalid command");

    return FST_REQ_NONE;
}

// Response
MTFST_API void __stdcall FST_Response(int id, int ok, int code)
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    if (!server->IsActive())
        return;

    if (ok != 0)
    {
        if (code != 0)
            server->PostResponse(Format("OK %d", code));
        else
            server->PostResponse("OK");
    } 
    else
    {
        server->PostResponse(Format("ER %d", code));
    }
}

// Ping
MTFST_API void __stdcall FST_Ping(int id, const char *symbol, int period, int time, double bid, double ask, int spread, double tickvalue, const RateInfo *rates, int bars,
                                  double accountBalance, double accountEquity, double accountProfit, double accountFreeMargin, int positionTicket,
                                  int positionType, double positionLots, double positionOpenPrice, double positionStopLoss, double positionTakeProfit,
                                  double positionProfit, char *positionComment)
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    if (!server->IsActive())
        return;

    int    bartime = rates[bars - 1].time;
    double open    = rates[bars - 1].open;
    double high    = rates[bars - 1].high;
    double low     = rates[bars - 1].low;
    double close   = rates[bars - 1].close;
    int    volume  = (int)rates[bars - 1].volume;

    int bartime10  = rates[bars - 11].time;

    string rc = Format("OK %s %d %d %.5f %.5f %d %.5f %d %.5f %.5f %.5f %.5f %d %d %.2f %.2f %.2f %.2f %d %d %.2f %.5f %.5f %.5f %.2f %s",
                        symbol, period, time, bid, ask, spread, tickvalue, bartime, open, high, low, close, volume,
                        bartime10, accountBalance, accountEquity, accountProfit, accountFreeMargin, positionTicket, positionType,
                        positionLots, positionOpenPrice, positionStopLoss, positionTakeProfit, positionProfit, Fixstr(positionComment));    
    server->PostResponse(rc);
}

// Symbol Info
MTFST_API void __stdcall FST_SymbolInfo(int id, char *symbol, double bid, double ask, int digits, double point, double spread, double stoplevel)
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    if (!server->IsActive())
        return;

    string rc = Format("OK %s %.5f %.5f %d %.5f %.5f %.5f", symbol, bid, ask, digits, point, spread, stoplevel);
    server->PostResponse(rc);
}

// Market Info Element
MTFST_API void __stdcall FST_MarketInfo(int id, char *symbol, int mode, double value)
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    if (!server->IsActive())
        return;

    string rc = Format("OK %s %d %.5f", symbol, mode, value);
    server->PostResponse(rc);
}

// Market Info All
MTFST_API void __stdcall FST_MarketInfoAll(int id, double point, double digits, double spread, double stoplevel, double lotsize, double tickvalue, double ticksize,
                                           double swaplong, double swapshort, double starting, double expiration, double tradeallowed, double minlot, 
                                           double lotstep, double maxlot, double swaptype, double profitcalcmode, double margincalcmode, double margininit,
                                           double marginmaintenance, double marginhedged, double marginrequired, double freezelevel)
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    if (!server->IsActive())
        return;

    string rc = Format("OK %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f",
                        point, digits, spread, stoplevel, lotsize, tickvalue, ticksize, swaplong, swapshort,  starting, expiration, tradeallowed,
                        minlot, lotstep, maxlot, swaptype, profitcalcmode, margincalcmode, margininit, marginmaintenance, marginhedged,
                        marginrequired, freezelevel);
    server->PostResponse(rc);
}

// Account Info
MTFST_API void __stdcall FST_AccountInfo(int id, char *name, int number, char *company, char *dserver, char *currency, int leverage, double balance,
                                         double equity, double profit, double credit, double margin, double freemarginmode, double freemargin,
                                         int stopoutmode, int stopout)
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    if (!server->IsActive())
        return;

    string rc = Format("OK %s %d %s %s %s %d %.2f %.2f %.2f %.2f %.2f %.2f %.2f %d %d",
                        Fixstr(name), number, Fixstr(company), Fixstr(dserver), Fixstr(currency), leverage, balance, equity, profit,
                        credit, margin, freemarginmode, freemargin, stopoutmode, stopout);
    server->PostResponse(rc);
}

// Terminal Info
MTFST_API void __stdcall FST_TerminalInfo(int id, char *name, char *company, char *path, char *expertversion )
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    if (!server->IsActive())
        return;

    string rc = Format("OK %s %s %s %s %s", Fixstr(name), Fixstr(company), Fixstr(path), Fixstr(expertversion), Fixstr(LIBRARY_VERSION));
    server->PostResponse(rc);
}

// Bars
MTFST_API void __stdcall FST_Bars(int id, char *symbol, int period, const RateInfo *rates, int bars)
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    if (!server->IsActive())
        return;

    if (bars <= 0) {
        server->PostResponse("ER Getting bars");
        return;
    }

    if (server->Offset() >= bars) {
        server->PostResponse("ER Bars not available");
        return;
    }

    string rc, header = Format("OK %s %d %d %d %d", symbol, period, bars, server->Offset(), server->Count()); //requested count
    int len = header.length(), count = 0;

    while (count <= server->Count() && server->Offset() + count < bars) {
        int i = bars - 1 - (server->Offset() + count);

        string s = Format(" %d %f %f %f %f %d", rates[i].time, rates[i].open, rates[i].high, rates[i].low, rates[i].close, (int)rates[i].volume);

        if (len + s.length() >= OUT_BUFFER_SIZE)
            break;

        rc  += s;
        len += s.length();
        count++;
    }

    header = Format("OK %s %d %d %d %d", symbol, period, bars, server->Offset(), count); //real count
    server->PostResponse(header + rc);
}

// Orders
MTFST_API void __stdcall FST_Orders(int id, char *symbol, int *tickets, int count)
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    string rc = strlen(symbol) ? Format("OK %s %d", symbol, count) : Format("OK %d", count);

    for (int i = 0; i < count; i++)
        rc += Format(" %d", tickets[i]);

    server->PostResponse(rc);
}

// Order Info
MTFST_API void __stdcall FST_OrderInfo(int id, int ticket, char *symbol, int type, double lots, double oprice, double stoploss, double takeprofit,
                                       int otime, int ctime, double cprice, double profit, int magic, int expire)
{
    if (servers.find(id) == servers.end())
        return;
    Server *server = servers[id];

    if (!server->IsActive())
        return;

    string rc = Format("OK %d %s %d %f %f %f %f %d %d %f %f %d %d",
                        ticket, symbol, type, lots, oprice, stoploss, takeprofit, otime, ctime, cprice, profit, magic, expire);
    server->PostResponse(rc);
}
