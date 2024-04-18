#pragma once


#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include<vector>
#include<thread>
#include<atomic>

namespace SocksServer
{
	namespace threads
	{
		struct ThreadManager final
		{
			static void join_all(std::vector<std::thread>& thread_pool);

			static void detach_all(std::vector<std::thread>& thread_pool);

			static void catch_main_thread();

			static std::atomic_bool can_start;

		};
	}
}
#endif // !THREAD_MANAGER_H
