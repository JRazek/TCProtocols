//
// Created by user on 17.07.2021.
//

#ifndef OPENCV_TCPCLIENT_H
#define OPENCV_TCPCLIENT_H

#include <netinet/in.h>
#include <vector>

typedef unsigned short u_short;
typedef unsigned char byte;
class TCPClient {
private:
    int sock;
    sockaddr_in serv_addr;

    int sendDataRaw(const byte *data, u_int64_t dataSize);

public:
    TCPClient(const char *addr, u_short port);
    int connect();

    int sendPacketsMetaData(u_short packetsCount);
    int sendPacket(const std::vector<byte> &data);
    int sendPacket(const byte *bytes, u_int64_t size);
    void connClose();
};


#endif //OPENCV_TCPCLIENT_H
