//
// Created by user on 23.07.2021.
//

#include <cstdio>
#include <unistd.h>
#include <iostream>
#include "Listener.h"
#include "TCPServer.h"

struct TCPServer;

Listener::Listener(int id, TCPServer* tcpServer, u_short port):id(id) {
    this->tcpServer = tcpServer;
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

int Listener::listen(u_short clientsPendingCount) {
    std::lock_guard<std::mutex> lockGuard (this->mutex);

    if (::listen(listenerFileDescriptor, clientsPendingCount) < 0){
        perror("listen");
        return -1;
    }
    return 0;
}

Socket* Listener::acceptFirst(u_short BUFFER_SIZE) {
    std::lock_guard<std::mutex> lockGuard (this->mutex);

    int addrLen = sizeof(address);
    int socketFileDescriptor;

    if ((socketFileDescriptor = ::accept(this->listenerFileDescriptor, (struct sockaddr *)&address, (socklen_t*)&addrLen)) < 0){
        perror("accept");
        return nullptr;
    }
    Socket * socket1 = new Socket(this->tcpServer->socketsCount(), socketFileDescriptor, BUFFER_SIZE);
    return socket1;
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

