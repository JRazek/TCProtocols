//
// Created by user on 23.07.2021.
//

#ifndef TCPROTOCOLS_LISTENER_H
#define TCPROTOCOLS_LISTENER_H


#include <netinet/in.h>
#include "Socket.h"

class TCPServer;

class Listener{
    sockaddr_in address;
    int listenerFileDescriptor;
    Listener(in_port_t port);
    int listen(u_short clientsPendingCount);
    Socket *acceptFirst(u_short BUFFER_SIZE);
    TCPServer *tcpServer;
};


#endif //TCPROTOCOLS_LISTENER_H
