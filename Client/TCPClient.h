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
public:
    TCPClient(const char *addr, u_short port);
    u_short connect();
    u_short sendData(const std::vector<byte> &data);
    void connClose();
};


#endif //OPENCV_TCPCLIENT_H
