#pragma once
#include "SocksException.h"

namespace Socks
{
	namespace excep
	{

		class SocketException : public SocksException
		{
			// ������������ ����� SocksException
			SocketException(std::string reason);
			std::string what() override;

		
		};
	}
}