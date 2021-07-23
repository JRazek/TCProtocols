//
// Created by user on 23.07.2021.
//

#include <cstdio>
#include "Socket.h"
#include "../transferUtils/TransferObjectData.h"

Socket::Socket(int id, int sockData, size_t BUFFER_SIZE) :id(id), BUFFER_SIZE(BUFFER_SIZE) {
    this->sockData = sockData;
    this->pendingPacketsCount = 0;
}

int Socket::readPacketsHeader() {
    if(!this->pendingPacketsCount){
        byte expectedPacketsMetaData[TransferObjectData::metaDataBytesSize];
        int status = read(sockData, expectedPacketsMetaData, sizeof(expectedPacketsMetaData));
        this->pendingPacketsCount = TransferObjectData::decodeDataLength(expectedPacketsMetaData);

        if(status < 0){
            return status;
        }
    } else{
        return -2;
    }
    return 0;
}
