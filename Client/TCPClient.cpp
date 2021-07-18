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

u_short TCPClient::connect() {
    if (::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed \n");
        return -1;
    }
    return 0;
}

u_short TCPClient::sendData(const std::vector<byte> &data) {
    byte *dataFormatted = TransferObjectData::encode(data.data(), data.size());
    u_int64_t formattedDataSize = data.size() + TransferObjectData::metaDataBytesSize;

    send(sock, dataFormatted, formattedDataSize, 0);
    delete [] dataFormatted;
    return 0;
}

void TCPClient::connClose() {
    close(sock);
}
