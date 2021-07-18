//
// Created by user on 17.07.2021.
//

#ifndef OPENCV_TCPSERVER_H
#define OPENCV_TCPSERVER_H
#include <vector>
#include <netinet/in.h>

typedef unsigned short u_short;
typedef unsigned char byte;

class TCPServer {
private:
    const u_short port;
    bool socketReady;
    const u_short BUFFER_SIZE = 8192;
    const u_short transferObjectMETADATASize = 8;
    u_short constructorError;
    sockaddr_in address;
    int server_fd, new_socket;


public:
    /**
     * initializes data
     * port for listening and buffer size for receiving data in readData()
     * */
    TCPServer(u_short port, u_short BUFFER_SIZE);


    /**
     *
     * @return  0 if successful, otherwise error code
     * reserves the port specified in constructor, starts listening on that port
     */
    u_short listen();


    /**
     *
     * @return 0 if successful, otherwise error code
     * waits for any connection request.
     */
    u_short accept();


    /**
     *
     * @return pair of <0 if successful otherwise error code, vector of data received from single encoded packet>
     */
    std::pair<u_short , std::vector<byte>> readPacket();

};


#endif //OPENCV_TCPSERVER_H
