#include "Socket.h"


namespace Socks
{
	Socket::Socket(IPVersion ip_vers, SocketHandle handle)
		: ipversion(ip_vers), handle(handle)
	{
		assert(ip_vers == IPVersion::IPv4 || ip_vers == IPVersion::IPv6);
	}
	SocketResult Socket::st_create(bool blocking)
	{

		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);

		if (handle != INVALID_SOCKET)
		{
			return SocketResult::GENERIC_ERROR;
		}


		handle = socket((ipversion == IPVersion::IPv4)  ? AF_INET : AF_INET6, SOCK_STREAM, IPPROTO_TCP);

		if (handle == INVALID_SOCKET)
			return SocketResult::GENERIC_ERROR;

		if (set_blocking(blocking) != SocketResult::OK)
			return SocketResult::GENERIC_ERROR;

		if (set_sock_opt(SocketOption::TCP_noDELAY, TRUE) != SocketResult::OK)
			return SocketResult::GENERIC_ERROR;

		return SocketResult::OK;
	}

	SocketResult Socket::st_close()
	{
		if (handle == INVALID_SOCKET)
			return SocketResult::GENERIC_ERROR;

		int result = closesocket(handle);

		if (result != 0)
			return SocketResult::GENERIC_ERROR;

		handle = INVALID_SOCKET;

		return SocketResult::OK;
	}

	SocketResult Socket::st_bind(IPEndpoint endpoint)
	{
		assert(ipversion == endpoint.get_ip_version());

		int result = 0;

		if (ipversion == IPVersion::IPv4)
		{
			sockaddr_in addr = endpoint.get_sockaddr_IPv4();
			result = bind(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
		}
		else
		{
			sockaddr_in6 addr6 = endpoint.get_sockaddr_IPv6();
			result = bind(handle, reinterpret_cast<sockaddr*>(&addr6), sizeof(sockaddr_in6));
		}

		if (result != 0)
			return SocketResult::GENERIC_ERROR;

		return SocketResult::OK;
	}

	SocketResult Socket::st_listen(IPEndpoint endpoint, int backlog)
	{
		if (ipversion == IPVersion::IPv6)
		{
			if (set_sock_opt(SocketOption::IPv6_ONLY, FALSE) != SocketResult::OK)
				return SocketResult::GENERIC_ERROR;
		}


		if (st_bind(endpoint) != SocketResult::OK)
			return SocketResult::GENERIC_ERROR;

		int result = listen(handle, backlog);

		if (result != 0)
			return SocketResult::GENERIC_ERROR;


		return SocketResult::OK;
	}

	SocketResult Socket::st_accept(Socket& out_socket, IPEndpoint* endpoint)
	{
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);

		SocketHandle accepted_conn = {};

		if (ipversion == IPVersion::IPv4)
		{
			sockaddr_in addr = {};
			int len = sizeof(sockaddr_in);
			accepted_conn = accept(handle, reinterpret_cast<sockaddr*>(&addr), &len);
			if (accepted_conn == INVALID_SOCKET)
				return SocketResult::GENERIC_ERROR;
			if (endpoint != nullptr)
			{
				*endpoint = IPEndpoint((sockaddr*)&addr);
			}
			out_socket = Socket(IPVersion::IPv4, accepted_conn);
		}
		
		else
		{
			sockaddr_in6 addr6 = {};
			int len = sizeof(sockaddr_in6);
			accepted_conn = accept(handle, reinterpret_cast<sockaddr*>(&addr6), &len);
			if (accepted_conn == INVALID_SOCKET)
				return SocketResult::GENERIC_ERROR;
			if (endpoint != nullptr)
			{
				*endpoint = IPEndpoint((sockaddr*)&addr6);
			}
			out_socket = Socket(IPVersion::IPv6, accepted_conn);
		}

		
		return SocketResult::OK;
	}

	SocketResult Socket::st_connect(IPEndpoint endpoint)
	{
		assert(ipversion == endpoint.get_ip_version());

		int result;

		if (ipversion == IPVersion::IPv4)
		{
			sockaddr_in addr = endpoint.get_sockaddr_IPv4();
			result = connect(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
		}
		else
		{
			sockaddr_in6 addr6 = endpoint.get_sockaddr_IPv6();
			result = connect(handle, reinterpret_cast<sockaddr*>(&addr6), sizeof(sockaddr_in6));
		}
		if (result != 0) return SocketResult::GENERIC_ERROR;


		return SocketResult::OK;
	}

	SocketResult Socket::st_send(const void* data, int number_of_bytes, int& bytes_sent)
	{
		bytes_sent = send(handle, reinterpret_cast<const char*>(data), number_of_bytes, NULL);

		if (bytes_sent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return SocketResult::GENERIC_ERROR;
		}

		return SocketResult::OK;
	}

	SocketResult Socket::st_send(std::string buffer)
	{
		int bytes_sent = send(handle, buffer.c_str(), buffer.size() + 1, NULL);

		if (bytes_sent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return SocketResult::GENERIC_ERROR;
		}
		return SocketResult::OK;
	}

	SocketResult Socket::st_recieve(void* destination, int number_of_bytes, int& bytes_recv)
	{
		bytes_recv = recv(handle, reinterpret_cast<char*>(destination), number_of_bytes, NULL);

		if (bytes_recv == 0)
		{
			return SocketResult::GENERIC_ERROR;
		}
		if (bytes_recv == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return SocketResult::GENERIC_ERROR;
		}
		return SocketResult::OK;
	}

	SocketResult Socket::st_send_all(const void* data, int number_of_bytes)
	{
		int total_bytes_sent = 0;
		while (total_bytes_sent < number_of_bytes)
		{
			int bytes_remaining = number_of_bytes - total_bytes_sent;
			int bytes_sent = 0;
			char* buffer_offset = const_cast<char*>(reinterpret_cast<const char*>(data)) + total_bytes_sent;
			SocketResult result = st_send(buffer_offset, number_of_bytes, bytes_sent);
			if (result != SocketResult::OK)
				return SocketResult::GENERIC_ERROR;
			total_bytes_sent += bytes_sent;
		}

		return SocketResult::OK;
	}

	SocketResult Socket::st_recieve_all(void* destination, int number_of_bytes)
	{
		int total_bytes_recv = 0;

		while (total_bytes_recv < number_of_bytes)
		{
			int bytes_remaining = number_of_bytes - total_bytes_recv;
			int bytes_recv = 0;
			char* buffer_offset = reinterpret_cast<char*>(destination) + total_bytes_recv;
			SocketResult result = st_recieve(buffer_offset, number_of_bytes, bytes_recv);
			if (result != SocketResult::OK)
			{
				return SocketResult::GENERIC_ERROR;
			}
			total_bytes_recv += bytes_recv;
		}

		return SocketResult::OK;
	}

	SocketResult Socket::st_recieve(std::string& buffer, int timeout)
	{
		Sleep(timeout);
		buffer.resize(get_buf_size());
		int bytes_recv = recv(handle, const_cast<char*>(buffer.c_str()), buffer.size() + 1, NULL);
		if (bytes_recv == 0)
		{
			return SocketResult::GENERIC_ERROR;
		}
		if (bytes_recv == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return SocketResult::GENERIC_ERROR;
		}

		return SocketResult::OK;
	}

	SocketResult Socket::set_blocking(bool blocking)
	{
		
		unsigned long block = 0;
		unsigned long non_block = 1;

		int result = ioctlsocket(handle, FIONBIO, blocking ? &block : &non_block);

		if (result == SOCKET_ERROR)
			return SocketResult::GENERIC_ERROR;

		return SocketResult::OK;
	}

	SocketHandle Socket::get_handle()
	{
		return handle;
	}

	IPVersion Socket::get_ip_version()
	{
		return ipversion;
	}

	u_long Socket::get_buf_size()
	{

		
		u_long bytes_avalible;
		/*Sleep(1000);*/
		ioctlsocket(handle, FIONREAD, &bytes_avalible);
		
		return bytes_avalible;
	}

	bool Socket::operator==(const Socket& other)
	{
		return ((this->handle == other.handle) && (this->ipversion == other.ipversion)) ? true : false;
	}

	SocketResult Socket::set_sock_opt(SocketOption option, BOOL value)
	{
		int result = 0;


		switch (option)
		{
		case SocketOption::TCP_noDELAY:
			result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&value), sizeof(value));
			break;
		case SocketOption::IPv6_ONLY:
			result = setsockopt(handle, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char*>(&value), sizeof(value));
			break;
		default:
			return SocketResult::GENERIC_ERROR;
			break;
		}

		if (result != 0) {
			int error = WSAGetLastError();
			return SocketResult::GENERIC_ERROR;
		}

		return SocketResult::OK;
	}

}
