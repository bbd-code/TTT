#include "Server.h"

namespace SocksServer {

	bool Server::init(IPEndpoint endpoint, bool is_sock_blocking)
	{
		if (!Socks::Net::initialize())
			return false;

		listening_socket = Socket(endpoint.get_ip_version());


		if (listening_socket.st_create() == SR::OK)
		{
			if (listening_socket.st_listen(endpoint) == SR::OK)
				return true;

			listening_socket.st_close();
		}

		return false;
	}

	Server::~Server()
	{
		Socks::Net::shutdown();
	}
	ServerStatus Server::get_status() const
	{
		return status;
	}
	void Server::set_status(const ServerStatus& status)
	{
		Server::status = status;
	}
}