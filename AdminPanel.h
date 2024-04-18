#pragma once



#ifndef ADMIN_PANEL_H
#define ADMIN_PANEL_H


#include"Server.h"

namespace for_admin {



	class AdminPanel
	{
		using Server = SocksServer::Server;

		enum AdminChoise
		{
			SHUTDOWN = 1

			
		};

	public:

		virtual void run(Server* server);
		
	};

}

#endif // !ADMIN_PANEL_H