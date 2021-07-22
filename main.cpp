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

    TCPServer tcpServer(PORT, BUFFER_SIZE);
    tcpServer.listen();

    TCPClient tcpClient(addr, PORT);

    std::future<int> accept = std::async(&TCPServer::accept, &tcpServer);

    while (tcpClient.connect() < 0){
        sleep(1);
    }

    accept.get();

  //  std::string s = tcpServer.getClientsIp(tcpServer.address);

    tcpClient.sendPacketsMetaData(PACKETS_COUNT);
    tcpServer.readPacketsMetadata();

    std::future<decltype(tcpServer.readPacketsAll())> fut = std::async(&TCPServer::readPacketsAll, &tcpServer);

    for(int i = 0; i < PACKETS_COUNT; i ++) {
        tcpClient.sendPacket(bytes);
    }
    //tcpServer.readPacketsAll();
    fut.get();


//    bool err = false;
//
//    if(response.second.size() == DATA_SIZE) {
//        for (int i = 0; i < DATA_SIZE; i++) {
//            if(response.second[i] != i%256)
//                err = true;
//        }
//    }else err = true;

   // delete[] bytes;
    return 0;
}