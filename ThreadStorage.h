#pragma once


#ifndef THREAD_STORAGE_H
#define THREAD_STORAGE_H

#include<thread>
#include<vector>

namespace SocksServer
{

	namespace threads
	{
		struct ThreadStorage final
		{
			static std::vector<std::thread> thread_pool;
		};
	}
}
#endif // !THREAD_STORAGE_H
