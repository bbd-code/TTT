#pragma once


#ifndef TIC_TAC_TOE_SERVER_H
#define TIC_TAC_TOE_SERVER_H


#include<socks_serv/Server.h>
#include<nlohmann/json.hpp>
#include<SocksServer/servers/TicTacToeServ/Room/Room.h>

namespace SocksServer {

	namespace GameServer {

		class TicTacToeServer : public Server
		{
			using RoomsArray = std::vector<Room>;
			using Json = nlohmann::json;
			using FileDptorArray = std::vector<WSAPOLLFD>;
			using FileDptor = WSAPOLLFD;
			using ConnArray = std::vector<Player>;


			enum class ParseResult
			{
				GET_ROOMS,
				PICK_ROOM,
				TURN,
				PICK_SIDE,
				QUIT,
				PARSE_ERROR

				/*TODO:*/
			};

		public:
		
			
			
			void frame() override;
			void listen_sock_fd();
			void init_rooms();

		protected:

			ServerStatus on_connect(Socket& connection) override;
			
			void close_conn(int& i, FileDptorArray& use_fd, Player& connection, int conn_index);
			ServerStatus on_disconnect(Socket& connection) override;
			ServerStatus close_conn(Socket& connection) override;
			ParseResult parse_request();
			ServerStatus broadcast_to_clients(Room& room, std::string message);
			ServerStatus broadcast_to_clients(Room& room, Player& miss_client, std::string message);
			void response_get_rooms(Player& connection);
			void response_pick_room(Player& connection);
			void response_pick_side(Player& connection);
			void response_turn(Player& connection);
			void response_quit(Player& connection);
			void clear_from_room(Player& player);

		private:


			RoomsArray rooms;
			FileDptorArray master_fd;
			ConnArray connections;
			
		};

	}

}



#endif // !TIC_TAC_TOE_SERVER_H



