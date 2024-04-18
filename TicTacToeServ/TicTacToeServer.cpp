#include "TicTacToeServer.h"


namespace SocksServer::GameServer {


    


    void TicTacToeServer::frame()
    {
        FileDptorArray use_fd = master_fd;

#pragma region Main loop
        if (WSAPoll(use_fd.data(), use_fd.size(), 1) > 0)
        {
            
            FileDptor listen_dptor = use_fd[0];

            if (listen_dptor.revents & POLLRDNORM)
            {
                Socket new_conn;
                IPEndpoint endpoint;

                if (listening_socket.st_accept(new_conn, &endpoint) == SR::OK)
                {
                    Player accepted_player(new_conn, endpoint);

                    connections.push_back(accepted_player);

                    Player& accepted_conn = connections[connections.size() - 1];
                    FileDptor new_conn_fd = {};

                    new_conn_fd.fd = accepted_conn.get_socket().get_handle();
                    new_conn_fd.events = POLLRDNORM;
                    new_conn_fd.revents = 0;

                    master_fd.push_back(new_conn_fd);

#ifndef NDEBUG
                    std::cout << "Connection accepted" << std::endl;
#endif // !NDEBUG


                }

            }


            for (int i = 1; i < use_fd.size(); ++i)
            {
                int conn_index = i - 1;

                Player& connection = connections[conn_index];

                /*TODO: —делать проверку на обычное отсоединение от сети*/
                
                if (use_fd[i].revents & POLLERR)
                {
                    clear_from_room(connection);
                    close_conn(i, use_fd, connection, conn_index);
                    std::cout << "error" << std::endl;
                    continue;
                }
                if (use_fd[i].revents & POLLHUP)
                {
                    clear_from_room(connection);
                    close_conn(i, use_fd, connection, conn_index);
                    std::cout << "error" << std::endl;
                    continue;
                }
                if (use_fd[i].revents & POLLNVAL)
                {
                    clear_from_room(connection);
                    close_conn(i, use_fd, connection, conn_index);
                    std::cout << "error" << std::endl;
                    continue;
                }
                if (use_fd[i].revents & POLLRDNORM)
                {
                    if (connection.get_socket().st_recieve(buffer) != SR::OK)
                    {
                        close_conn(i, use_fd, connection, conn_index);
                        continue;
                    }

                    switch (parse_request())
                    {
                    case ParseResult::GET_ROOMS:
                        response_get_rooms(connection);
#ifndef NDEBUG
                        std::cout << "Request get-rooms" << std::endl;
#endif // !NDEBUG
                        break;

                    case ParseResult::PICK_ROOM:
                        response_pick_room(connection);
#ifndef NDEBUG
                        
                        std::cout << "Request pick-room" << std::endl;
#endif // !NDEBUG
                        break;

                    case ParseResult::PICK_SIDE:
                        response_pick_side(connection);
#ifndef NDEBUG
                        std::cout << "Request pick-side" << std::endl;
#endif // !NDEBUG
                        break;

                    case ParseResult::TURN:
                        response_turn(connection);
#ifndef NDEBUG
                        std::cout << "Request turn" << std::endl;
#endif // !NDEBUG
                        break;

                    case ParseResult::QUIT:
                        response_quit(connection);
                      
#ifndef NDEBUG
                        std::cout << "Request quit" << std::endl;
#endif // !NDEBUG
                        break;

                    default:
                        close_conn(i, use_fd, connection, conn_index);
                        break;
                    }


                }

                buffer.clear();
            }
        }
#pragma endregion 
    }

    


    ServerStatus TicTacToeServer::on_connect(Socket& connection)
    {
        return ServerStatus::OK;
    }

    void TicTacToeServer::close_conn(int& i, FileDptorArray& use_fd, Player& connection, int conn_index)
    {
        master_fd.erase(master_fd.begin() + i);
        use_fd.erase(use_fd.begin() + i);
        connection.get_socket().st_close();
        connections.erase(connections.begin() + conn_index);
        --i;
    }

    ServerStatus TicTacToeServer::on_disconnect(Socket& connection)
    {
        return ServerStatus();
    }

    ServerStatus TicTacToeServer::close_conn(Socket& connection)
    {
        return ServerStatus();
    }

    void TicTacToeServer::init_rooms()
    {
        /*TODO:for (int i = 0; i < 2; ++i)
            rooms.push_back(Room());*/

        rooms.push_back(Room("Room1"));
        rooms.push_back(Room("Room2"));


    }

    TicTacToeServer::
        ParseResult TicTacToeServer::parse_request()
    {
        try {

            /*command from client*/

            Json json = Json::parse(buffer);

            if (json.contains("command"))
            {
                if (json.at("command") == "get-rooms")
                {
                    return ParseResult::GET_ROOMS;
                }
                else if (json.at("command") == "pick-room")
                {
                    return ParseResult::PICK_ROOM;
                }
                else if (json.at("command") == "pick-side")
                {
                    return ParseResult::PICK_SIDE;
                }
                else if (json.at("command") == "turn")
                {
                    return ParseResult::TURN;
                }
                else if (json.at("command") == "quit")
                {
                    return ParseResult::QUIT;
                }
            }
        }
        catch (Json::exception& ex)
        {
            std::cout << ex.what() << std::endl;
        }


        return ParseResult::PARSE_ERROR;
    }

    ServerStatus TicTacToeServer::broadcast_to_clients(Room& room, std::string message)
    {
        for (auto& connection : room.get_players())
        {
            connection.get_socket().st_send(message + "\n");
        }

        return ServerStatus::OK;
    }

    ServerStatus TicTacToeServer::broadcast_to_clients(Room& room, Player& miss_client, std::string message)
    {
        for (auto& connection : room.get_players())
        {
            if (connection.operator==(miss_client))
                continue;
            connection.get_socket().st_send(message + "\n");
        }

        return ServerStatus::OK;
    }

    

    void TicTacToeServer::listen_sock_fd()
    {
        FileDptor listening_fd = {};

        listening_fd.fd = listening_socket.get_handle();
        listening_fd.events = POLLRDNORM;
        listening_fd.revents = 0;

        master_fd.push_back(listening_fd);

    }

    void TicTacToeServer::response_get_rooms(Player& connection)
    {
        try
        {
            Json response_json;

            response_json["rooms-names"];
            response_json["conn-count"];


            for (auto& room : rooms) {
                response_json["rooms-names"].push_back(room.get_name());
                response_json["conn-count"].push_back(std::to_string(room.get_conn_count()));
            }

            connection.get_socket().st_send(response_json.dump() + "\n");
        }
        catch (Json::exception& ex)
        {
            std::cout << ex.what() << std::endl;
            connection.get_socket().st_send("error\n");
        }
    }
    /*TODO:*/
    void TicTacToeServer::response_pick_room(Player& connection)
    {
        try
        {
            Json request_json = Json::parse(buffer);

            for (auto& room : rooms)
            {
                if (request_json.at("pickedRoom") == room.get_name())
                {
                    if (room.is_full())
                    {
                        connection.get_socket().st_send("room-full\n");
#ifndef NDEBUG
                        std::cout << "Room is full" << std::endl;
#endif // !NDEBUG
                        return;
                    }
                    else
                    {
                        
                        if (!room.is_game_active())
                        {
                            Json response_json;
                            response_json["warning"] = "room-picked";
                            room.push_client(connection);
                            connection.get_socket().st_send(response_json.dump() + "\n");
                            connection.set_room_name(request_json["pickedRoom"]);

                            Json op_con_json;
                            op_con_json["warning"] = "op-conn";

                            broadcast_to_clients(room, connection, op_con_json.dump() + "\n");
                            
                            /*std::thread broadcast_to_clients_th([](ConnArray copy_conns, Player miss_client ,RoomsArray copy_rooms) mutable
                                {
                                    Json response_json;
                                    response_json["rooms-names"];
                                    response_json["conn-count"];

                                    for (auto& room : copy_rooms) {
                                        response_json["rooms-names"].push_back(room.get_name());
                                        response_json["conn-count"].push_back(std::to_string(room.get_conn_count()));
                                    }

                                    for (auto& player : copy_conns) {

                                        if (player.operator==(miss_client))
                                            continue;
                                        player.get_socket().st_send(response_json.dump() + "\n");
                                    }

                                }, connections, connection ,rooms);

                            broadcast_to_clients_th.detach();*/

                            return;
                        }
#ifndef NDEBUG
                        std::cout << "Game is active now" << std::endl;
#endif //!NDEBUG
                        return;
                    }

                }

            }
        }
        catch (Json::exception& ex)
        {
            std::cout << ex.what() << std::endl;
            connection.get_socket().st_send("error");

        }
    }

    void TicTacToeServer::response_pick_side(Player& connection)
    {
        try
        {
            Json request_json = Json::parse(buffer);
            Json warning_json;

            for (auto& room : rooms)
            {

                if (request_json["pickedRoom"] == room.get_name())
                {
                    if (room.is_full())
                    {
                        if (room.checkBusySides(request_json["side"]))
                        {
                            warning_json["warning"] = "side-pick-ok";
                            connection.get_socket().st_send(warning_json.dump() + "\n");
                            return;
                        }
                        else
                        {
                            warning_json["warning"] = "side-pick-error";
                            connection.get_socket().st_send(warning_json.dump() + "\n");
                            return;
                        }
                    }
                    else
                    {
                        warning_json["warning"] = "side-room-is-not-full";
                        connection.get_socket().st_send(warning_json.dump() + "\n");
                    }
                }

            }

        }
        catch (Json::exception& ex)
        {
            std::cout << ex.what() << std::endl;
        }


    }

    void TicTacToeServer::response_turn(Player& connection)
    {

        try
        {
            Json request_json = Json::parse(buffer);

            for (auto& room : rooms)
            {
                if (request_json["pickedRoom"] == room.get_name())
                {
                    if (room.is_full())
                    {
                        if (request_json["side"] == room.get_current_side())
                        {
                            request_json.erase("command");
                            request_json.erase("pickedRoom");

                            request_json["turn"] = room.next_turn();

                            broadcast_to_clients(room, request_json.dump() + "\n");

                            return;
                        }
                        else
                        {
                            Json response_json;
                            response_json["warning"] = "no-ur-turn";
                            connection.get_socket().st_send(response_json.dump() + "\n");
#ifndef NDEBUG
                            std::cout << "Warning no your turn" << std::endl;
#endif //!NDEBUG
                            return;
                        }
                    }
                    else
                    {
                        Json wresponse_json;
                        wresponse_json["warning"] = "wait";
                        connection.get_socket().st_send(wresponse_json.dump());
#ifndef NDEBUG
                        std::cout << "Warning wait" << std::endl;
#endif //!NDEBUG
                        return;
                    }

                    
                }

            }
        }
        catch (Json::exception& ex)
        {
            std::cout << ex.what() << std::endl;
            connection.get_socket().st_send("error");
        }


    }

    void TicTacToeServer::response_quit(Player& connection)
    {
        try
        {
            Json request_json = Json::parse(buffer);

            for (auto& room : rooms) {

                if (request_json["pickedRoom"] == room.get_name())
                {
                    room.erase_client(connection);

                    Json response_json;
                    response_json["warning"] = "op-disc";
                    broadcast_to_clients(room, connection, response_json.dump() + "\n");

                    /*std::thread broadcast_to_clients_th([](ConnArray copy_conns, RoomsArray copy_rooms) mutable
                        {
                            Json response_json;
                            response_json["rooms-names"];
                            response_json["conn-count"];

                            for (auto& room : copy_rooms) {
                                response_json["rooms-names"].push_back(room.get_name());
                                response_json["conn-count"].push_back(std::to_string(room.get_conn_count()));
                            }

                            for (auto& player : copy_conns)
                                player.get_socket().st_send(response_json.dump() + "\n");
                        }, connections, rooms);

                    broadcast_to_clients_th.detach();*/

                    return;
                }
            }
        }
        catch (Json::exception& ex)
        {
            std::cout << ex.what() << std::endl;
            connection.get_socket().st_send("error\n");
        }
    }

    void TicTacToeServer::clear_from_room(Player& player)
    {

         for(auto& room : rooms)
         {
             if (room.get_name() == player.get_room_name())
             {
                 room.erase_client(player);
                 return;
             }
         }

    }



}

