//
// Created by user on 23.07.2021.
//

#include <cstdio>
#include <sys/socket.h>
#include <mutex>
#include <thread>
#include "Socket.h"
#include "../transferUtils/TransferObjectData.h"
#include "TCPServer.h"
#include "../Logger/Logger.h"

Socket::Socket(int id, int fileDescriptor, TCPServer *tcpServer, size_t BUFFER_SIZE) : id(id), BUFFER_SIZE(BUFFER_SIZE) {
    this->socketFileDescriptor = fileDescriptor;
    this->pendingPacketsCount = 0;
    this->tcpServer = tcpServer;
}

int Socket::readPacketsHeader() {
    if(!this->pendingPacketsCount){
        byte expectedPacketsMetaData[TransferObjectData::metaDataBytesSize];
        int status = read(socketFileDescriptor, expectedPacketsMetaData, sizeof(expectedPacketsMetaData));
        this->pendingPacketsCount = TransferObjectData::decodeDataLength(expectedPacketsMetaData);

        if(status < 0){
            perror("header");
        }
        return status;
    }
    return -1;
}

std::pair<int, std::vector<byte>> Socket::readPacket() {
    if(this->pendingPacketsCount) {
        std::lock_guard lockGuard(this->mutex);
        byte packetMetaData[TransferObjectData::metaDataBytesSize];
        int status = read(socketFileDescriptor, &packetMetaData, sizeof(packetMetaData));
        if (status < 0)
            return {status, {}};

        u_int64_t expectedDataSize = TransferObjectData::decodeDataLength(packetMetaData);

        std::vector<byte> bytesVector;

        u_int64_t dataReceived = 0;
        u_int64_t requestedData = BUFFER_SIZE < expectedDataSize - dataReceived ? BUFFER_SIZE : expectedDataSize;

        byte buffer[this->BUFFER_SIZE];
        while (int packet = recv(socketFileDescriptor, &buffer, requestedData, MSG_WAITALL)) {
            dataReceived += packet;
            requestedData = BUFFER_SIZE < (expectedDataSize - dataReceived) ? BUFFER_SIZE : expectedDataSize - dataReceived;
            if (packet < 1) {
                return {packet, {}};
            }
            bytesVector.insert(bytesVector.end(), buffer, buffer + packet);

            if (!requestedData) {
                //E of data wanted. ok
                break;
            }
        }
        this->pendingPacketsCount --;
        if(expectedDataSize == dataReceived)
            return {0, bytesVector};
        else{
            //client sending uncomplete data
            return {-5, bytesVector};
        }

    }

    return {-1, {}};
}

Socket::~Socket() {
    std::lock_guard lockGuard(this->mutex);
    close(this->socketFileDescriptor);
}

void Socket::shutdown() {
    ::shutdown(this->socketFileDescriptor, SHUT_RDWR);
}

void Socket::run() {
    std::thread receiveThread([this] (){
        while(readPacketsHeader() > 0) {
            while (this->pendingPacketsCount) {
                auto res = this->readPacket();
                if(errno == EWOULDBLOCK || errno == EAGAIN){
                    Logger::log("error occurred while reading a packet!" + std::to_string(errno), LEVEL::ERROR);
                    throw std::system_error();
                }

                if(res.first == 0){
                    this->tcpServer->notifyNewPacket(this->id, res.second);
                }else if(res.first == -5){
                    this->tcpServer->notifyDataIncomplete(this->id, res.second);
                }else{
                    Logger::log("here4 " + std::to_string(res.first), LEVEL::WARNING);
                }

            }
        }
        this->tcpServer->notifySocketDone(this->id);
    });
    receiveThread.detach();
}
