#pragma once

#ifndef SERVER_H
#define SERVER_H

#include"IPEndpoint.h"
#include<iostream>
#include"Socket.h"
#include"Net.h"
#include"Constants.h"
#include"ServerStatus.h"
#include"ThreadManager.h"
#include"ThreadStorage.h"


namespace SocksServer
{

	class Server
	{
	protected:
		using IPEndpoint = Socks::IPEndpoint;
		using Socket = Socks::Socket;
		using SR = Socks::SocketResult;
	public:

		bool init(IPEndpoint endpoint, bool is_sock_blocking = false);
		virtual void frame() = 0;
		~Server();
		ServerStatus get_status() const;
		void set_status(const ServerStatus& status);
	protected:

		virtual ServerStatus on_connect(Socket& connection) = 0;
		virtual ServerStatus on_disconnect(Socket& connection) = 0; 
		virtual ServerStatus close_conn(Socket& connection) = 0;

	protected:
		Socket listening_socket;
		ServerStatus status;
		std::string buffer;
	};

}
#endif // !SERVER_H