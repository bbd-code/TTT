#pragma once


#ifndef IP_ENDPOINT_H
#define IP_ENDPOINT_H

#ifdef _WIN32
#include<WinSock2.h>
#include<WS2tcpip.h>
#else
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/tcp.h>

#endif // _WIN32

#include<string>
#include<vector>
#include"IPVersion.h"
#include<assert.h>

namespace Socks
{
	class IPEndpoint
	{
	public:
		IPEndpoint(const char* ip, u_short port);
		IPEndpoint(sockaddr* addr);
		IPEndpoint() = default;
		std::string get_hostname() const;
		std::string get_ipstring() const;
		std::vector<uint8_t> get_ip_bytes() const;
		IPVersion get_ip_version() const;
		sockaddr_in get_sockaddr_IPv4();
		sockaddr_in6 get_sockaddr_IPv6();
		u_short get_port();
		void print();


	private:
		std::string hostname;
		std::string ip_string;
		std::vector<uint8_t> ip_bytes;
		u_short port;
		IPVersion ipversion;
	};
}
#endif // !IP_ENDPOINT_H
