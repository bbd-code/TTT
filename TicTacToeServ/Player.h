#pragma once



#ifndef PLAYER_H
#define PLAYER_H


#include<socks_serv/Connection.h>



class Player : public Socks::Connection
{
	using Socket = Socks::Socket;
	using IPEndpoint = Socks::IPEndpoint;
public:

	Player();

	Player(Socket& connection,IPEndpoint endpoint);

	void set_room_name(std::string room_name);

	std::string get_room_name() const;

	bool operator ==(const Player& other);



private:
	std::string room_name;
	

	
};

#endif // !PLAYER_H