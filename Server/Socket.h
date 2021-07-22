//
// Created by user on 23.07.2021.
//

#ifndef TCPROTOCOLS_SOCKET_H
#define TCPROTOCOLS_SOCKET_H

#include "Connection.h"
#include <vector>


class Socket {
    const u_short port;
    int serverFileDescriptor;
    int socketFileDescriptor;
    sockaddr_in addressData;
    std::vector<Connection *> connections;
    Socket(u_short PORT);
};


#endif //TCPROTOCOLS_SOCKET_H
