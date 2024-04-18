#pragma once

#ifndef SOCKS_EXCEPTION_H
#define SOCKS_EXCEPTION_H

#include<string>

namespace Socks {

	namespace excep
	{

		class SocksException
		{
		public:
			SocksException(std::string reason);
			virtual std::string what();

		private:
			std::string reason;
		};
	}
}
#endif // !SOCKS_EXCEPTION_H
