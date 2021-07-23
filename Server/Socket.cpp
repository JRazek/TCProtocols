//
// Created by user on 23.07.2021.
//

#include <cstdio>
#include <sys/socket.h>
#include "Socket.h"
#include "../transferUtils/TransferObjectData.h"

Socket::Socket(int id, int sockData, size_t BUFFER_SIZE) :id(id), BUFFER_SIZE(BUFFER_SIZE) {
    this->socketFileDescriptor = sockData;
    this->pendingPacketsCount = 0;
}

int Socket::readPacketsHeader() {
    if(!this->pendingPacketsCount){
        byte expectedPacketsMetaData[TransferObjectData::metaDataBytesSize];
        int status = read(socketFileDescriptor, expectedPacketsMetaData, sizeof(expectedPacketsMetaData));
        this->pendingPacketsCount = TransferObjectData::decodeDataLength(expectedPacketsMetaData);

        if(status < 0){
            return status;
        }
    } else{
        return -2;
    }
    return 0;
}

std::pair<int, std::vector<byte>> Socket::readPacket() {
    if(this->pendingPacketsCount) {

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

int Socket::getPendingPacketsCount() const {
    return pendingPacketsCount;
}

const int Socket::getId() const {
    return id;
}

const size_t Socket::getBufferSize() const {
    return BUFFER_SIZE;
}

int Socket::getSockData() const {
    return socketFileDescriptor;
}

Socket::~Socket() {
    close(this->socketFileDescriptor);
}

void Socket::shutdown() {
    ::shutdown(this->socketFileDescriptor, SHUT_RDWR);
}
