#pragma once


#ifndef SOCKET_H
#define SOCKET_H

#define WIN32_LEAN_AND_MEAN

#include<WinSock2.h>
#include<string>
#include"IPVersion.h"
#include"SocketResult.h"
#include"IPEndpoint.h"
#include"SocketOption.h"
#include<assert.h>
#include<sstream>

namespace Socks
{
	using SocketHandle = SOCKET;

	class Socket
	{

		using Buffer = std::ostringstream;

	public:
		Socket(IPVersion ip_vers = IPVersion::IPv4, SocketHandle handle = INVALID_SOCKET);
		SocketResult st_create(bool blocking = false);
		SocketResult st_close();
		SocketResult st_bind(IPEndpoint endpoint);
		SocketResult st_listen(IPEndpoint endpoint, int backlog = 5);
		SocketResult st_accept(Socket& out_socket, IPEndpoint* endpoint = nullptr);
		SocketResult st_connect(IPEndpoint endpoint);
		SocketResult st_send(const void* data, int number_of_bytes, int& bytes_sent);
		SocketResult st_send(std::string buffer);
		SocketResult st_recieve(void* destination, int number_of_bytes, int& bytes_recv);
		SocketResult st_recieve(std::string& buffer, int timeout = 0);
		/*SocketResult send_sock(Packet& packet);
		SocketResult recieve_sock(Packet& packet);*/
		SocketResult st_send_all(const void* data, int number_of_bytes);
		SocketResult st_recieve_all(void* destination, int number_of_bytes);
		SocketHandle get_handle();
		IPVersion get_ip_version();
		u_long get_buf_size();
		
		bool operator == (const Socket& other);


	private:
		SocketResult set_blocking(bool blocking);
		SocketResult set_sock_opt(SocketOption option, BOOL value);

	private:
		IPVersion ipversion;
		SocketHandle handle;

	};

}
#endif // !SOCKET_H
