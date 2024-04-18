#include "Single.h"




void SocksServer::Single::run(Server* server)
{

	std::thread server_thread([&server] 
		{
			while (server->get_status() != ServerStatus::STOP)
				server->frame();
		});

	/*threads::ThreadStorage::thread_pool.emplace_back(server_thread);*/

}
