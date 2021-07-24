//
// Created by user on 20.07.2021.
//

#include "Server/TCPServer.h"
#include "Client/TCPClient.h"
#include <future>
#include <iostream>

#include <zconf.h>

typedef unsigned char byte;

int main(){
    const int DATA_SIZE = 10000000;
    const int PACKETS_COUNT = 10;
    const u_short PORT = 12321;
    const size_t BUFFER_SIZE = 600000;
    const char * addr = "127.0.0.1";


    std::vector<byte> bytes;
    bytes.reserve(DATA_SIZE);
    for(int i = 0; i < DATA_SIZE; i++){
        bytes.push_back(i % 256);
    }

    TCPServer server;
    server.addListener(PORT);
    std::thread * serverRunnable = server.run();

//    TCPClient client1(addr, PORT);
//    TCPClient client2(addr, PORT);
//
//    std::this_thread::sleep_for (std::chrono::microseconds (100000l));
//    client1.connect();
//    client2.connect();
//    std::this_thread::sleep_for (std::chrono::microseconds (1000l));
//
//    client1.sendPacketsMetaData(1);
//    client1.sendPacket(bytes);

    serverRunnable->join();

//    TCPClient client1("10.0.0.8", PORT);
//    client1.connect();

    return 0;
}