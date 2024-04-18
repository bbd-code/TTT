#include "Connection.h"

namespace Socks
{
	Connection::Connection(Socket connection, IPEndpoint ip_endpoint)
		:connection(connection), ip_endpoint(ip_endpoint)
	{

	}
	Socket Connection::get_socket()
	{
		return connection;
	}

	IPEndpoint Connection::get_ip_endpoint()
	{
		return ip_endpoint;
	}

	void Connection::close()
	{
		connection.st_close();
	}


	

}