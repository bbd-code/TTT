#include "AdminPanel.h"



void for_admin::AdminPanel::run(Server* server)
{
	int choise = 0;
	std::cout 
		<< "Admin panel initialized" << std::endl
		<< "Write 1 for stop server" << std::endl;


	while (true)
	{
		std::cin >> choise;
		
		switch (choise) {

		case AdminChoise::SHUTDOWN:
			server->set_status(ServerStatus::STOP);
			return;
		}
	}
}
