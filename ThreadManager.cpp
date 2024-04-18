#include "ThreadManager.h"

namespace SocksServer
{
	namespace threads
	{
		std::atomic_bool ThreadManager::can_start = false;

		void ThreadManager::join_all(std::vector<std::thread>& thread_pool)
		{
			for (auto& thread : thread_pool)
			{
				thread.join();
			}
		}
		void ThreadManager::detach_all(std::vector<std::thread>& thread_pool)
		{
			for (auto& thread : thread_pool)
			{
				thread.detach();
			}
		}
		void ThreadManager::catch_main_thread()
		{
			system("pause");
			system("cls");
		}
	}
}