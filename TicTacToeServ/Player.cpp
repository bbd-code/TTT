#include "Player.h"

Player::Player()
	:Socks::Connection({}, {})
{
}

Player::Player(Socket& connection, IPEndpoint endpoint)
	:Socks::Connection(connection, endpoint)
{
}

void Player::set_room_name(std::string other_name)
{
	room_name = other_name;
}

std::string Player::get_room_name() const
{
	return room_name;
}

bool Player::operator==(const Player& other)
{
	return connection == other.connection;
}
