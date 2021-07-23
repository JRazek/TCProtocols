//
// Created by user on 23.07.2021.
//

#ifndef TCPROTOCOLS_LISTENER_H
#define TCPROTOCOLS_LISTENER_H


#include <netinet/in.h>
#include <mutex>
#include "Socket.h"

class TCPServer;

class Listener{
    const int id;
public:
    const int getId() const;

private:
    std::mutex mutex;
    sockaddr_in address;
    int listenerFileDescriptor;
    TCPServer *tcpServer;
public:
    Listener(int id, TCPServer* tcpServer, in_port_t port);
    int listen(u_short clientsPendingCount);
    //todo int killListener();
    Socket *acceptFirst(u_short BUFFER_SIZE);
    void killListener();
    ~Listener();
};


#endif //TCPROTOCOLS_LISTENER_H
