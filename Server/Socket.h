//
// Created by user on 23.07.2021.
//

#ifndef TCPROTOCOLS_SOCKET_H
#define TCPROTOCOLS_SOCKET_H

#include <vector>

typedef unsigned char byte;
class Socket {
private:
    const int id;
    const size_t BUFFER_SIZE;
    int socketFileDescriptor;
    int pendingPacketsCount;

    std::mutex mutex;


public:
    Socket(int id, int sockData, size_t BUFFER_SIZE);

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



    /**
     *
     * all the getters
    */
    const int getId() const;
    const size_t getBufferSize() const;
    int getSockData() const;

    /**
     *
     * @return packets that are pending from client
    */
    int getPendingPacketsCount() const;


    void shutdown();
};


#endif //TCPROTOCOLS_SOCKET_H
