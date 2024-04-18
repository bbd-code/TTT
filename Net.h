#pragma once


#ifndef NETWORK
#define NETWORK

#ifdef _WIN32
#include<WinSock2.h>
#endif // _WIN32

namespace Socks
{
	struct Net
	{
		static bool initialize();
		static void shutdown();
	};

}

#endif // NETWORK
