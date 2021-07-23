//
// Created by user on 23.07.2021.
//

#ifndef TCPROTOCOLS_SOCKET_H
#define TCPROTOCOLS_SOCKET_H

#include <vector>


class Socket {
    const int id;
    const size_t BUFFER_SIZE;
    int sockData;

    int pendingPacketsCount;

public:
    Socket(int id, int sockData, size_t BUFFER_SIZE);
    int readPacketsHeader();
};


#endif //TCPROTOCOLS_SOCKET_H
