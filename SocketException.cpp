#include "SocketException.h"


namespace Socks
{
    namespace excep
    {

        std::string SocketException::what()
        {
            return "Socket exception - [" + SocksException::what() + "]";
        }
    }
}