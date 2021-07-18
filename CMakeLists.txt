cmake_minimum_required(VERSION 3.0.2)
project(TCProtocols)

set(CMAKE_CXX_STANDARD 17)

add_library(TCProtocols Server/TCPServer.cpp Server/TCPServer.h transferUtils/TransferObjectData.cpp transferUtils/TransferObjectData.h Client/TCPClient.cpp Client/TCPClient.h)

