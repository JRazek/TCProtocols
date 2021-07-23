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

TCPServer::TCPServer(){}

int TCPServer::killSocket(int socketID) {
    delete this->sockets[socketID];

    return 0;
}

int TCPServer::socketsCount() {
    std::lock_guard<std::mutex> lockGuard (this->mutex);
    return this->sockets.size();
}

TCPServer::~TCPServer() {
    for(int i = 0; i < this->sockets.size(); i ++){
        delete this->sockets[i];
    }
    for(int i = 0; i < this->listeners.size(); i ++){
        auto p = listeners[i];
        delete p.first; //releases the accept hold state ( kills socket )
        p.second->join();
        delete p.second;
    }
}

int TCPServer::listen(in_port_t port) {
    Listener *listener = new Listener(this->listeners.size(), this, port);

    std::thread * thread = new std::thread([](TCPServer *tcpServer, Listener *listener) mutable{
        listener->listen(3);
        Socket * socket1 = listener->acceptFirst(4096);
        if(socket1 != nullptr)
            tcpServer->notifyAccept(socket1);
        return 0;
    }, this, listener);

    listeners.push_back({listener, thread});
    return 0;
}

void TCPServer::notifyAccept(Socket * socket) {
    std::lock_guard<std::mutex> lockGuard (this->mutex);
    this->sockets.push_back(socket);
}
