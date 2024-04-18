cmmake_minimum_required(VERSION 3.5)
project(TicTacToeServer)

set(CMAKE_CXX_STANDARD 17)

# Список исходных файлов
set(SOURCE_FILES
    AdminPanel.cpp
    AdminPanel.h
    Cluster.cpp
    Cluster.h
    Connection.cpp
    Connection.h
    Constants.h
    IPEndpoint.cpp
    IPEndpoint.h
    IPVersion.h
    Net.cpp
    Net.h
    PacketType.h
    Server.cpp
    Server.h
    ServerHandle.cpp
    ServerHandle.h
    ServerLoop.cpp
    ServerLoop.h
    ServerStatus.h
    Single.cpp
    Single.h
    Socket.cpp
    Socket.h
    SocketException.cpp
    SocketException.h
    SocketOption.h
    SocketResult.h
    ThreadManager.cpp
    ThreadManager.h
    ThreadStorage.h
    socks.h
  
)

# Добавляем файлы из папки Room
file(GLOB ROOM_SOURCES "TicTacToeServ/Room/*.cpp")
file(GLOB ROOM_HEADERS "TicTacToeServ/Room/*.h")
file(GLOB TTT_HEADERS "TicTacToeServ/*.h")
file(GLOB TTT_SOURCES "TicTacToe/*.cpp")

# Генерация исполняемого файла
add_executable(socks ${SOURCE_FILES})
add_executable(TicTacToeServer ${ROOM_SOURCES} ${ROOM_HEADERS} ${TTT_HEADERS} ${TTT_SOURCES})
set_target_properties(socks PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(TicTacToeServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
