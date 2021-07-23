//
// Created by user on 17.07.2021.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <zconf.h>
#include "TCPServer.h"
#include "../transferUtils/TransferObjectData.h"
#include "Socket.h"
//todo rsa crypt
TCPServer::TCPServer(){}

int TCPServer::killSocket(int socketID) {
    delete this->sockets[socketID];

    return shutdown(socketID, SHUT_RDWR);
}

int TCPServer::socketsCount() {
    return this->sockets.size();
}

TCPServer::~TCPServer() {
    for(int i = 0; i < this->sockets.size(); i ++){
        delete this->sockets[i];
    }
}
