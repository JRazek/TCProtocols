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

    return 0;
}