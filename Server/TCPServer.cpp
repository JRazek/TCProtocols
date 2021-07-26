//
// Created by user on 17.07.2021.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <zconf.h>
#include <iostream>
#include "TCPServer.h"
#include "../transferUtils/TransferObjectData.h"
#include "Socket.h"
#include "Listener.h"
#include "../Logger/Logger.h"

TCPServer::TCPServer(){
    this->currListenerID = 0;
    this->currSocketID = 0;
}

int TCPServer::killSocket(int socketID) {
    delete this->sockets[socketID];
    return 0;
}

int TCPServer::socketsCount() {
    return this->sockets.size();
}

TCPServer::~TCPServer() {
    std::unique_lock<std::mutex> uniqueLock (this->mutex);
    for(int i = 0; i < this->listeners.size(); i ++){
        auto p = listeners[i];
        delete p; //releases the accept hold state ( kills socket )
    }
    for(int i = 0; i < this->sockets.size(); i ++){
        delete this->sockets[i];
    }
}

void TCPServer::notifyAccept(int listenerID, int socketFileDescriptor) {
    std::lock_guard guard(this->mutex);
    Socket * socket = new Socket(this->currSocketID, socketFileDescriptor, this, listeners[listenerID]->BUFFER_SIZE);
    this->sockets[socket->id] = socket;
    currSocketID ++;
    socket->run();
}

std::thread * TCPServer::run() {
    auto * run = new std::thread([this]() {
        Logger::log("Starting tcp server... ", LEVEL::INFO);
        std::vector<std::thread *> threads;
        for(auto l : listeners){
            threads.push_back(l.second->run());
        }
        for(auto t : threads){
            t->join();
        }
    });
    return run;
}

void TCPServer::addListener(in_port_t port, size_t BUFFER_SIZE) {
    std::lock_guard lockGuard(this->mutex);

    auto * listener = new Listener(this->currListenerID, this, port, 4, BUFFER_SIZE);
    this->listeners[listener->id] = listener;
    currListenerID ++;
}

void TCPServer::notifyNewPacket(int socketID, std::vector<byte> &data) {
    //override it
    Logger::log("received new packet from socket " + std::to_string(socketID), LEVEL::INFO);
    std::string  dataString = "";
    for(auto c : data){
        dataString += c;
    }
    Logger::log("content of message " + dataString, LEVEL::INFO);
}

void TCPServer::notifyDataIncomplete(int socketID, std::vector<byte> &incompleteData) {
    Logger::log("INCOMPLETE DATA SENT IN SOCKET " + std::to_string(socketID), LEVEL::WARNING);
}

void TCPServer::notifySocketDone(int socketID) {
    Logger::log("Socket with id " + std::to_string(socketID) + " has died", LEVEL::DEBUG);
    delete this->sockets[socketID];
    this->sockets.erase(socketID);
}
