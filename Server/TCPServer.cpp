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
#include "Listener.h"
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
    for(int i = 0; i < this->listeners.size(); i ++){
        delete this->listeners[i];
    }
    for(int i = 0; i < this->sockets.size(); i ++){
        delete this->sockets[i];
    }
}

int TCPServer::listen(in_port_t port) {
    listeners.push_back(new Listener(port));
    return 0;
}
