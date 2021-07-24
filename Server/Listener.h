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
    u_short clientsPendingCount;


    int listen();
    /**
     * returns file descriptor.
     * */
    int acceptFirst();
public:
    Listener(int id, TCPServer* tcpServer, in_port_t port, u_short clientsPendingCount);
    void killListener();
    void run();
    ~Listener();
};


#endif //TCPROTOCOLS_LISTENER_H
