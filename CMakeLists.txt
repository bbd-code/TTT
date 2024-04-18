cmake
cmake
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
    Player.cpp
    Player.h
    TicTacToeServer.cpp
    TicTacToeServer.h
)

# Добавляем файлы из папки Room
file(GLOB ROOM_SOURCES "Room/*.cpp")
file(GLOB ROOM_HEADERS "Room/*.h")

# Генерация исполняемого файла
add_executable(TicTacToeServer ${SOURCE_FILES} ${ROOM_SOURCES} ${ROOM_HEADERS})
