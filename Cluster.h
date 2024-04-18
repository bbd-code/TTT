#pragma once


#ifndef CLUSTER
#define CLUSTER

#include"Server.h"
#include<initializer_list>
#include<string>
#include<thread>

namespace SocksServer
{
	class Cluster
	{
	public:
		void run();
		void add(Server& server);
		void add(std::initializer_list<Server>);
		std::thread& get_thread(int index) ;
		std::vector<std::thread>& get_thread_pool();
		std::vector<Server> get_servers() const;
	private:
		std::vector<Server> servers;
		std::vector<std::thread> thread_pool;
	};

}
#endif // !CLUSTER