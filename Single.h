#pragma once



#ifndef SINGLE_H
#define SINGLE_H

#include"Server.h"
#include"ThreadStorage.h"

namespace SocksServer {



	struct Single
	{
		static void run(Server* server);

	};

}
#endif // !SINGLE_H