//
// Created by user on 17.07.2021.
//

#ifndef OPENCV_TCPSERVER_H
#define OPENCV_TCPSERVER_H
#include <vector>
#include <netinet/in.h>
#include <unordered_map>

struct Socket;

typedef unsigned short u_short;
typedef unsigned char byte;

class TCPServer {
private:
    std::vector<Socket *> sockets;

public:

    /**
     *
     * @return sockets count
     */
    int socketsCount();

    /**
     * init
     * */
    TCPServer();


    /**
     *
     * @param socketID
     * kills the socket.
     * @return 0 on success
     */
    int killSocket(int socketID);

   /**
    * destructor, manages all the dynamic data
    */
   ~TCPServer();
};


#endif //OPENCV_TCPSERVER_H
