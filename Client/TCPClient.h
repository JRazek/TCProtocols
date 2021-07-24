//
// Created by user on 17.07.2021.
//

#ifndef TCPROTOCOLS_TCPCLIENT_H
#define TCPROTOCOLS_TCPCLIENT_H

#include <netinet/in.h>
#include <vector>

typedef unsigned short u_short;
typedef unsigned char byte;
class TCPClient {
private:
    int sock;
    sockaddr_in serv_addr;

    /**
     * sends raw data to server
     * for use only by internal methods as it may corrupt the connection
     * @return status code - 0 if success
     */
    int sendDataRaw(const byte *data, u_int64_t dataSize);

public:
    TCPClient(const char *addr, u_short port);


    /**
     * connects to specified in the constructor ip and port
     * @return status code - 0 if success
     */
    int connect();


    /**
     * sends header containing expected packets
     * @return status code - 0 if success
     */
    int sendPacketsMetaData(u_short packetsCount);

    /**
     * sends encoded packet to server
     * @return status code - 0 if success
     */
    int sendPacket(const std::string data);

    /**
     * sends encoded packet to server
     * @return status code - 0 if success
     */
    int sendPacket(const std::vector<byte> &data);

    /**
     * sends encoded packet to server
     * @return status code - 0 if success
     */
    int sendPacket(const byte *bytes, u_int64_t size);


    /**
     * closes the connection with server
     */
    void connClose();
};


#endif //OPENCV_TCPCLIENT_H
