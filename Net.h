#pragma once

#ifndef NETWORK
#define NETWORK

#include<WinSock2.h>


namespace Socks
{
	struct Net
	{
		static bool initialize();
		static void shutdown();
	};

}

#endif // NETWORK
