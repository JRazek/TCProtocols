cmake_minimum_required(VERSION 3.0.2)
project(TCPServer)

set(CMAKE_CXX_STANDARD 17)

add_library(OpenCV Server/TCPServer.cpp Server/TCPServer.h transferUtils/TransferObjectData.cpp transferUtils/TransferObjectData.h Client/TCPClient.cpp Client/TCPClient.h)

