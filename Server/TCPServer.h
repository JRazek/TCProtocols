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

    u_short packetsPendingCount;

public:
    /**
     * 
     * initializes data
     * port for listening and buffer size for receiving data in readData()
     * */
    TCPServer(u_short port, u_short BUFFER_SIZE);


    /**
     *
     * @return  0 if successful, otherwise error code
     * reserves the port specified in constructor, starts listening on that port
     */
    int listen(u_short clientsPendingCount = 5) const;


    /**
     *
     * @return 0 if successful, otherwise error code
     * waits for any connection request.
     */
    int accept();


    /**
     *
     * @return reads packets header from client. Runs automatically with @readPacket()
     */
    int readPacketsMetadata();


    /**
     * @name readPacket()
     * @return pair of status code and vector containing packet data.
     */
    std::pair<int, std::vector<byte>> readPacket();


    /**
     *
     * @return pair of status code, and vector of vectors that contain all the packets read @readPacket() for more
     */

    std::pair<int, std::vector<std::vector<byte>>> readPacketsAll();


    /**
     * 
     * @return packets that are pending from client
    */
   u_short getPacketsPendingCount();
    
    
};


#endif //OPENCV_TCPSERVER_H
