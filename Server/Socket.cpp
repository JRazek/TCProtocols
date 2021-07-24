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
            perror("");
            return status;
        }
    } else{
        return -2;
    }
    return 0;
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
        while (int packet = recv(socketFileDescriptor, &buffer, requestedData, 0)) {
            dataReceived += packet;
            requestedData = BUFFER_SIZE < (expectedDataSize - dataReceived) ? BUFFER_SIZE : expectedDataSize - dataReceived;
            if (packet < 1)
                break;
            bytesVector.insert(bytesVector.end(), buffer, buffer + packet);
            if (!requestedData)
                break;
        }
        this->pendingPacketsCount --;
        return {0, bytesVector};
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
        while(readPacketsHeader() >= 0) {
            while (this->pendingPacketsCount) {
                auto res = this->readPacket();
                if (res.first < 0) {
                    throw std::runtime_error(std::string("Failed: ") + std::to_string(res.first));
                }
                this->tcpServer->notifyNewPacket(this->id, res.second);
            }
        }
        perror("read");

    });
    receiveThread.detach();
}
