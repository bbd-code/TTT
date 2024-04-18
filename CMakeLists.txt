cmake_minimum_required(VERSION 3.1)
project(TicTacToeServer VERSION 1.0)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(Server Server/Source.cpp
                PNet/Constants.h
                PNet/IncludeMe.h
                PNet/IPEndpoint.h   PNet/IPEndpoint.cpp
                PNet/IPVersion.h
                PNet/Network.h      PNet/Network.cpp
                PNet/Packet.h       PNet/Packet.cpp
                PNet/PacketException.h
                PNet/PacketType.h
                PNet/PResult.h
                PNet/Socket.h       PNet/Socket.cpp
                PNet/SocketHandle.h
                PNet/SocketOption.h)
                
set_target_properties(Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
                
