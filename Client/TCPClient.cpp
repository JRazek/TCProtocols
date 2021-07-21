//
// Created by user on 17.07.2021.
//

#include <arpa/inet.h>
#include <cstdio>
#include <sys/socket.h>
#include <stdexcept>
#include "TCPClient.h"
#include "../transferUtils/TransferObjectData.h"

TCPClient::TCPClient(const char *addr, u_short port) {

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    this->serv_addr = {};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if(inet_pton(AF_INET, addr, &serv_addr.sin_addr)<=0) {
        throw std::invalid_argument("\nInvalid address/ Address not supported \n");
    }

}

int TCPClient::connect() {
    if (::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed \n");
        return -1;
    }
    return 0;
}
int TCPClient::sendPacket(const byte *bytes, u_int64_t size) {
    byte *dataFormatted = TransferObjectData::encode(bytes, size);
    u_int64_t formattedDataSize = size + TransferObjectData::metaDataBytesSize;

    if(sendDataRaw(dataFormatted, formattedDataSize) == -1){
        delete [] dataFormatted;
        return -1;
    }
    delete [] dataFormatted;
    return 0;
}

int TCPClient::sendPacket(const std::vector<byte> &data) {
    return this->sendPacket(data.data(), data.size());
}

void TCPClient::connClose() {
    close(sock);
}


int TCPClient::sendPacketsMetaData(u_short packetsCount){
    byte *metaData = TransferObjectData::encodeDataLength(packetsCount);
    u_short result = sendDataRaw(metaData, TransferObjectData::metaDataBytesSize);

    delete [] metaData;
    return result;
}

int TCPClient::sendDataRaw(const byte *data, u_int64_t dataSize) {
    return send(sock, data, dataSize, 0);
}
