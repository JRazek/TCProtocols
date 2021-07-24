//
// Created by user on 23.07.2021.
//

#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include "Listener.h"
#include "TCPServer.h"
#include "../Logger/Logger.h"

struct TCPServer;

Listener::Listener(int id, TCPServer* tcpServer, u_short port, u_short clientsPendingCount):id(id), port(port) {
    this->tcpServer = tcpServer;
    this->clientsPendingCount = clientsPendingCount;
    this->listenerFileDescriptor = -1;

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
    std::lock_guard lockGuard(this->mutex);
    if ((socketFileDescriptor = ::accept(this->listenerFileDescriptor, (struct sockaddr *) &address,(socklen_t *) &addrLen)) < 0) {
        if(errno != 22)
            perror("accept");
    }
    return socketFileDescriptor;
}

void Listener::killListener() {
    shutdown(this->listenerFileDescriptor, SHUT_RDWR);
    std::lock_guard lockGuard(this->mutex);
    close(this->listenerFileDescriptor);
}

Listener::~Listener() {
    this->killListener();
}

std::thread * Listener::run() {
    std::thread * thread = new std::thread([this]() mutable {
        Logger::log("Listening on port " + std::to_string(this->port), LEVEL::INFO);
        if(this->listen() >= 0) {
            int socketFD;
            while ((socketFD = this->acceptFirst()) >= 0) {
                int addrLen = sizeof(address);
                int ip = getpeername(socketFD, (struct sockaddr *) &address,(socklen_t *) &addrLen);
                struct in_addr ip_addr;
                ip_addr.s_addr = ip;
                std::string ipStr = inet_ntoa(ip_addr);
                Logger::log("Accepted connection from host " + ipStr, LEVEL::INFO);
                this->tcpServer->notifyAccept(socketFD);
            }
            return 0;
        }
        Logger::log("Listening on port failed!  ", LEVEL::ERROR);
        return -1;
    });
    return thread;
}

