//
// Created by user on 17.07.2021.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <zconf.h>
#include "TCPServer.h"
#include "../transferUtils/TransferObjectData.h"
//todo rsa crypt
TCPServer::TCPServer(u_short port, size_t BUFFER_SIZE): port(port), BUFFER_SIZE(BUFFER_SIZE) {
    this->socketReady = false;
    this->packetsPendingCount = 0;
    int opt = 1;

    this->address = {};
    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        this->constructorError = 301;
        return;
    }


    if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
        perror("setsockopt");
        this->constructorError = 302;
        return;
    }
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons( port );

    if (bind(this->server_fd, (struct sockaddr *)&this->address, sizeof(this->address))<0){
        perror("bind failed");
        this->constructorError = 303;
        return;
    }
}

int TCPServer::listen(u_short clientsPendingCount) const {
    if (::listen(server_fd, clientsPendingCount) < 0){
        perror("listen");
        return -1;
    }
    return 0;
}

int TCPServer::accept() {
    int addrLen = sizeof(address);
    if ((new_socket = ::accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrLen))<0){
        perror("accept");
        return -1;
    }
    this->socketReady = true;
    return 0;
}


std::pair<int , std::vector<byte>> TCPServer::readPacket() {
    this->readPacketsMetadata();
    if(this->packetsPendingCount) {

        byte packetMetaData[TransferObjectData::metaDataBytesSize];

        int status = read(new_socket, &packetMetaData, sizeof(packetMetaData));
        if (status < 0)
            return {status, {}};

        u_int64_t expectedDataSize = TransferObjectData::decodeDataLength(packetMetaData);

        std::vector<byte> bytesVector;

        u_int64_t dataReceived = 0;
        u_int64_t requestedData = BUFFER_SIZE < expectedDataSize - dataReceived ? BUFFER_SIZE : expectedDataSize;

        byte buffer[this->BUFFER_SIZE];
        while (int packet = recv(new_socket, &buffer, requestedData, MSG_DONTWAIT | 0)) {
            dataReceived += packet;
            requestedData = BUFFER_SIZE < (expectedDataSize - dataReceived) ? BUFFER_SIZE : expectedDataSize - dataReceived;
            if (packet < 1)
                break;
            bytesVector.insert(bytesVector.end(), buffer, buffer + packet);
            if (!requestedData)
                break;
        }
        this->packetsPendingCount --;
        return {0, bytesVector};
    }
    return {-1, {}};
}

u_short TCPServer::getPacketsPendingCount(){
    return this->packetsPendingCount;
}

std::pair<int, std::vector<std::vector<byte>>> TCPServer::readPacketsAll() {
    this->readPacketsMetadata();
    std::pair<int, std::vector<std::vector<byte>>> result;
    result.second.reserve(this->packetsPendingCount);
    while (this->packetsPendingCount){
        auto tmpRes = this->readPacket();
        if(tmpRes.first < 0){
            result.first = tmpRes.first;
            return result;
        }
        result.second.push_back(tmpRes.second);
    }
    return result;
}

int TCPServer::readPacketsMetadata() {
    if(!this->packetsPendingCount){
        byte expectedPacketsMetaData[TransferObjectData::metaDataBytesSize];
        int status = read(new_socket, expectedPacketsMetaData, sizeof(expectedPacketsMetaData));
        this->packetsPendingCount = TransferObjectData::decodeDataLength(expectedPacketsMetaData);

        if(status < 0){
            return status;
        }
    } else{
        return -2;
    }
    return 0;
}
