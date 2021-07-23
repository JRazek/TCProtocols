//
// Created by user on 17.07.2021.
//

#ifndef OPENCV_TCPSERVER_H
#define OPENCV_TCPSERVER_H
#include <vector>
#include <netinet/in.h>
#include <unordered_map>
#include <thread>
#include <future>

struct Socket;
struct Listener;

typedef unsigned short u_short;
typedef unsigned char byte;

class TCPServer {
private:

    std::mutex mutex;

    std::unordered_map<int, Socket *> sockets;//id, val

    std::unordered_map<int, Listener *> listeners;//id, val

    std::condition_variable serverDoneCondition;

    void notifyAccept(Socket * socket);

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

    int listen(in_port_t port);

   /**
    * destructor, manages all the dynamic data
    */
   ~TCPServer();
};


#endif //OPENCV_TCPSERVER_H
