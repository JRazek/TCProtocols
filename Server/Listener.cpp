//
// Created by user on 23.07.2021.
//

#include <cstdio>
#include <unistd.h>
#include <iostream>
#include "Listener.h"
#include "TCPServer.h"

struct TCPServer;

Listener::Listener(int id, TCPServer* tcpServer, u_short port, u_short clientsPendingCount):id(id) {
    this->tcpServer = tcpServer;
    this->clientsPendingCount = clientsPendingCount;

    int opt = 1;
    if ((this->listenerFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
    }

    if (setsockopt(this->listenerFileDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
        perror("setsockopt");
    }

    this->address = {};

    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(port);
    this->address.sin_addr.s_addr = INADDR_ANY;

    if(bind(this->listenerFileDescriptor, (struct sockaddr*)&this->address, sizeof(this->address))){
        perror("bind failed");
    }
}

int Listener::listen() {
    if (::listen(listenerFileDescriptor, clientsPendingCount) < 0){
        perror("listen");
        return -1;
    }
    return 0;
}

int Listener::acceptFirst() {
    int addrLen = sizeof(address);
    int socketFileDescriptor;

    if ((socketFileDescriptor = ::accept(this->listenerFileDescriptor, (struct sockaddr *)&address, (socklen_t*)&addrLen)) < 0){
        perror("accept");
        return -1;
    }
    return socketFileDescriptor;
}

const int Listener::getId() const {
    return id;
}

void Listener::killListener() {
    shutdown(this->listenerFileDescriptor, SHUT_RDWR);
    close(this->listenerFileDescriptor);
}

Listener::~Listener() {
    this->killListener();
}

void Listener::run() {
    this->listen();

    std::thread thread([this]() mutable{
        int socketFD = this->acceptFirst();
        if(socketFD != -1){
            this->tcpServer->notifyAccept(socketFD);
        }
        return 0;
    });

    thread.detach();
}

