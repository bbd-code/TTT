#pragma once



#ifndef CONNECTION_H
#define CONNECTION_H

#include"Socket.h"


namespace Socks {

	class Connection
	{
	
	public:

		Connection(Socket connection, IPEndpoint ip_endpoint);
		Socket get_socket();
		IPEndpoint get_ip_endpoint();
		void close();

	protected:
		Socket connection;
		IPEndpoint ip_endpoint;

	};

}

#endif //!CONNECTION_H