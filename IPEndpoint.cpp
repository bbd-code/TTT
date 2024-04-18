#include "IPEndpoint.h"
#include<string.h>

namespace Socks
{
	IPEndpoint::IPEndpoint(const char* ip, u_short port)
		: port(port), ipversion(IPVersion::INVALID)
	{
		in_addr addr = {};

		int result = inet_pton(AF_INET, ip, &addr);

		if (result == 1)
		{

			if (addr.s_addr != INADDR_NONE)
			{
				hostname = ip;
				ip_string = ip;
				ip_bytes.resize(sizeof(uint32_t));
				memcpy(&ip_bytes[0], &addr.s_addr, sizeof(uint32_t));
				ipversion = IPVersion::IPv4;
				return;
			}

		}

		addrinfo hints = {};
		hints.ai_family = AF_INET;

		addrinfo* hostinfo = nullptr;

		result = getaddrinfo(ip, NULL, &hints, &hostinfo);

		if (result == 0)
		{
			sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);

			ip_string.resize(16);
			inet_ntop(AF_INET, &host_addr->sin_addr, &ip_string[0], 16);

			hostname = ip;

			u_long ip_long = host_addr->sin_addr.s_addr;

			ip_bytes.resize(sizeof(uint32_t));

			memcpy(&ip_bytes[0], &ip_long, sizeof(uint32_t));

			ipversion = IPVersion::IPv4;

			freeaddrinfo(hostinfo);
			return;
		}

		in6_addr addr6 = {};


		result = inet_pton(AF_INET6, ip, &addr6);

		if (result == 1)
		{

			ip_string = ip;
			hostname = ip;

			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &addr6, 16);

			ipversion = IPVersion::IPv6;
			return;

		}
		addrinfo hintsv6 = {};
		hints.ai_family = AF_INET6;
		addrinfo* hostinfov6 = nullptr;
		result = getaddrinfo(ip, NULL, &hintsv6, &hostinfov6);

		if (result == 0)
		{
			sockaddr_in6* host_addr6 = reinterpret_cast<sockaddr_in6*>(hostinfov6->ai_addr);


			ip_string.resize(46);
			inet_ntop(AF_INET6, &host_addr6->sin6_addr, &ip_string[0], 46);

			hostname = ip;


			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &host_addr6->sin6_addr, 16);

			ipversion = IPVersion::IPv6;

			freeaddrinfo(hostinfov6);

			return;
		}


	}

	IPEndpoint::IPEndpoint(sockaddr* addr)
	{
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);

		if(addr->sa_family == AF_INET)
		{
			sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);
			ipversion = IPVersion::IPv4;
			port = ntohs(addrv4->sin_port);
			ip_bytes.resize(sizeof(uint32_t));
			memcpy(&ip_bytes[0], &addrv4->sin_addr, sizeof(uint32_t));
			ip_string.resize(16);
			inet_ntop(AF_INET, &addrv4->sin_addr, &ip_string[0], 16);
			hostname = ip_string;
		}
		else
		{
			sockaddr_in6* addrv6 = reinterpret_cast<sockaddr_in6*>(addr);
			ipversion = IPVersion::IPv6;
			port = ntohs(addrv6->sin6_port);
			ip_bytes.resize(16);
			memcpy(&ip_bytes, &addrv6->sin6_addr, 16);
			ip_string.resize(46);
			inet_ntop(AF_INET6, &addrv6->sin6_addr, &ip_string[0], 46);
			hostname = ip_string;
		}
	}

	std::string IPEndpoint::get_hostname() const
	{
		return hostname;
	}

	std::string IPEndpoint::get_ipstring() const
	{
		return ip_string;
	}

	std::vector<uint8_t> IPEndpoint::get_ip_bytes() const
	{
		return ip_bytes;
	}

	IPVersion IPEndpoint::get_ip_version() const
	{
		return ipversion;
	}

	sockaddr_in IPEndpoint::get_sockaddr_IPv4()
	{
		assert(ipversion == IPVersion::IPv4);
		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		memcpy(&addr.sin_addr, &ip_bytes[0], sizeof(uint32_t));
		addr.sin_port = htons(port);
		return addr;
	}

	sockaddr_in6 IPEndpoint::get_sockaddr_IPv6()
	{
		assert(ipversion == IPVersion::IPv6);
		sockaddr_in6 addr = {};
		addr.sin6_family = AF_INET6;
		memcpy(&addr.sin6_addr, &ip_bytes[0], 16);
		addr.sin6_port = htons(port);
		return addr;
	}

	u_short IPEndpoint::get_port()
	{
		return port;
	}
}
