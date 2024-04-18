cmake_minimum_required(VERSION 3.1)
project(TicTacToeServer VERSION 1.0)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_executable( main.cpp
                Constants.h
                IncludeMe.h
                IPEndpoint.h   PNet/IPEndpoint.cpp
                IPVersion.h
                Net.h      Net.cpp
                
                PNet/PResult.h
                PNet/Socket.h       PNet/Socket.cpp
                PNet/SocketHandle.h
                PNet/SocketOption.h)
                
set_target_properties(Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
                
