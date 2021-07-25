cmake_minimum_required(VERSION 3.0.2)
project(TCProtocols)

set(CMAKE_CXX_STANDARD 17)

add_library(TCProtocols STATIC Server/TCPServer.cpp Server/TCPServer.h transferUtils/TransferObjectData.cpp transferUtils/TransferObjectData.h Client/TCPClient.cpp Client/TCPClient.h Server/Socket.cpp Server/Socket.h Server/Listener.cpp Server/Listener.h Logger/Logger.cpp Logger/Logger.h Logger/LEVEL.h)

#add_executable(TEST main.cpp main.cpp)

#target_link_libraries(TEST TCProtocols)

#install(DIRECTORY ./build/ DESTINATION /home/user/installCmakeTest
#        FILES_MATCHING PATTERN "*.a")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
