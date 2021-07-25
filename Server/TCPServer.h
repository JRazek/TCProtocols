//
// Created by user on 17.07.2021.
//

#ifndef TCPROTOCOLS_TCPSERVER_H
#define TCPROTOCOLS_TCPSERVER_H
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
    friend class Listener;
    friend class Socket;
    std::mutex mutex;

    std::unordered_map<int, Socket *> sockets;//id, val

    std::unordered_map<int, Listener *> listeners;//id, val

    std::condition_variable serverDoneCondition;

    /**
     *
     * @param socketID
     * kills the socket.
     * @return 0 on success
     */

    int killSocket(int socketID);


    /**
     * accessed from other threads. Must be secure.
     * @param socketFileDescriptor
     */

    void notifyAccept(int listenerID, int socketFileDescriptor);


    /**
     * this is the function that is called whenever the new packet from any socket comes. By default its empty.
     * Can be overridden as needed
     * @param socketID socket which sent the data
     * @param data - byte array incoming from connection
     */

    virtual void notifyNewPacket(int socketID, std::vector<byte> &data);


public:

    /**
     * init
     * */

    TCPServer();

    /**
     * inits listeners to run,
     * @param port - port for listening
     * @param new sockets created on accept will have that buffer size.
     */
    void addListener(in_port_t port, size_t BUFFER_SIZE);


    /**
     * runs the server
     */

    std::thread * run();


   /**
    * destructor, manages all the dynamic data
    */

   ~TCPServer();

    /**
     *
     * @return sockets count
     */

    int socketsCount();
};


#endif //OPENCV_TCPSERVER_H
