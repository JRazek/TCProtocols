//
// Created by user on 23.07.2021.
//

#ifndef TCPROTOCOLS_CONNECTION_H
#define TCPROTOCOLS_CONNECTION_H
#include <netinet/in.h>


class Connection {
    sockaddr_in address;
    int new_socket;
    const size_t BUFFER_SIZE = 600000;
    u_short packetsPendingCount;


};


#endif //TCPROTOCOLS_CONNECTION_H
