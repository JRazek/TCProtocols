//
// Created by user on 23.07.2021.
//

#ifndef TCPROTOCOLS_SOCKET_H
#define TCPROTOCOLS_SOCKET_H

#include <vector>

class TCPServer;

typedef unsigned char byte;
class Socket {
private:
    int socketFileDescriptor;
    int pendingPacketsCount;
    TCPServer* tcpServer;

    std::mutex mutex;

public:
    const int id;
    const size_t BUFFER_SIZE;


    Socket(int id, int fileDescriptor, TCPServer *tcpServer, size_t BUFFER_SIZE);

    /**
     *
     * @return reads and decodes packets header from client.
     */
    int readPacketsHeader();

    /**
     * @name readPacket()
     * @return pair of status code and vector containing packet data.
     */
    std::pair<int, std::vector<byte>> readPacket();

    /**
     * unbinds socket, kills all the streams and prepare for deletion
     * @return
     */
    ~Socket();


    void run();

    void shutdown();
};


#endif //TCPROTOCOLS_SOCKET_H
