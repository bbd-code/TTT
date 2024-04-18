#pragma once


#ifndef ROOM_H
#define ROOM_H

#include<string>
#include<vector>
#include<socks_serv/Socket.h>
#include<algorithm>
#include<SocksServer/servers/TicTacToeServ/Player.h>


namespace SocksServer::GameServer {

	class Room
	{
		using ClientArray = std::vector<Player>;
		using AvalibleSides = std::vector<std::string>;

	public:
		Room(std::string name);

		/*TODO: Сделать так чтобы клиенты не могли заходит за одну и ту же сторону*/


		void push_client(Player& player);
		void erase_client(Player& player);
		std::string get_name();
		std::string get_current_side();
		ClientArray get_players();
		size_t get_conn_count(); 
		bool checkBusySides(std::string side);
		int next_turn();
		bool is_full();
		bool is_game_active() const;
		

	private:
		void refresh_sides();
		
	private:

		std::string name;
		std::string current_side;
		int turn;

		
		ClientArray players;
		AvalibleSides sides;
	};

}

#endif // !ROOM_H
