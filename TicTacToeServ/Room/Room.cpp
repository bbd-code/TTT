#include"Room.h"



namespace SocksServer::GameServer {

    Room::Room(std::string name)
        :name(name), current_side("Cross"), turn(1)
    {
        sides.push_back("Cross");
        sides.push_back("Zero");
    }

    void Room::push_client(Player& player){
        players.push_back(player);
    }

    void Room::erase_client(Player& erase_player)
    {
        std::erase_if(players, [&erase_player](Player& player)
            {
                return player.operator==(erase_player); 
            });

        turn = 1;
        current_side = "Cross";
        if(players.size() == 0)
            refresh_sides();
    }

    std::string Room::get_name()
    {
        return name;
    }

    std::string Room::get_current_side()
    {
        return current_side;
    }

    Room::ClientArray Room::get_players()
    {
        return players;
    }

    size_t Room::get_conn_count()
    {
        return players.size();
    }

    bool Room::checkBusySides(std::string picked_side)
    {
        for (auto& side : sides)
        {
            if (side == picked_side) 
            {

                std::erase_if(sides, [&picked_side](std::string& side)
                    {
                        return side == picked_side;
                    });

                return true;
            }
        }
        return false;
    }

    int Room::next_turn()
    {
        if (current_side == "Cross") current_side = "Zero";
        else                         current_side = "Cross";

        return ++turn;
    }

    bool Room::is_full()
    {
        if (players.size() == 2)
            return true;

        return false;
    }

    bool Room::is_game_active() const
    {
        if (turn > 1)
            return true;

        return false;
    }

    void Room::refresh_sides()
    {
        if(!sides.empty())
            sides.clear();

        sides.push_back("Cross");
        sides.push_back("Zero");
    }
}