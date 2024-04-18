#pragma once


#ifndef CONSTANTS_H
#define CONSTANTS_H



namespace Socks
{
	
	static const int max_packet_size = 8192;
	#ifndef _WIN32
	static const int INVALID_SOCKET = ~0;
	using int = BOOL;
	static const int SOCKET_ERROR = -1;

	#ifndef WSAGetLastError()
		#define WSAGetLastError() errno
	#endif // ! WSAGetLastError()
	#ifndef closesocket
		#define closesocket(fd) close(fd)
	#endif // !closesocket
	#endif //! _WIN32
}



#endif // !CONSTANTS_H
