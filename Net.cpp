#include "Net.h"

namespace Socks 
{

    bool Net::initialize()
    {
        WSAData ws_data;
        if (WSAStartup(MAKEWORD(2, 2), &ws_data) != 0)
            return false;
        if (LOBYTE(ws_data.wVersion) != 2 || HIBYTE(ws_data.wVersion) != 2)
            return false;
        return true;
    }

    void Net::shutdown()
    {
        WSACleanup();
    }
}