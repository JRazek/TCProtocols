//
// Created by user on 23.07.2021.
//

#ifndef TCPROTOCOLS_LISTENER_H
#define TCPROTOCOLS_LISTENER_H


#include <netinet/in.h>
#include <mutex>
#include <thread>
#include "Socket.h"

class TCPServer;

class Listener{
public:
    const int id;
    const in_port_t port;


    /**
     * buffer for new sockets created
     */

    const size_t BUFFER_SIZE;

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
    Listener(int id, TCPServer *tcpServer, u_short port, u_short clientsPendingCount, size_t BUFFER_SIZE);
    void killListener();
    std::thread * run();
    ~Listener();
};


#endif //TCPROTOCOLS_LISTENER_H
