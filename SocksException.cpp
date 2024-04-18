#include "SocksException.h"

namespace Socks
{

    namespace excep {
        SocksException::SocksException(std::string reason)
            : reason(reason)
        {
        }
        std::string SocksException::what()
        {
            return reason.c_str() ? reason : "Unknown exception";
        }
    }
}