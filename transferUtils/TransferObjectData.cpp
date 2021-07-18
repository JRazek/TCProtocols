//
// Created by user on 17.07.2021.
//

#include "TransferObjectData.h"

byte *TransferObjectData::encode(const byte *dataRaw, const u_int64_t dataSize) {
    byte *data = new byte [dataSize + metaDataBytesSize];

    for(int i = 0; i < metaDataBytesSize; i ++){
        data[i] = (dataSize >> metaDataBytesSize * (7 - i));
    }

    std::copy(dataRaw, dataRaw + dataSize, data + metaDataBytesSize);
//todo remove
    std::vector<byte> tmp;
    for(int i = 0; i < dataSize + metaDataBytesSize; i ++){
        tmp.push_back(data[i]);
    }
    return data;
}

std::pair<u_int64_t, byte *> TransferObjectData::decode(const byte *objectEncoded){
    u_int64_t dataSize = decodeDataLength(objectEncoded);
    byte *decodedData = new byte [dataSize];
    std::copy(objectEncoded + metaDataBytesSize, objectEncoded + metaDataBytesSize + dataSize, decodedData);
    return {dataSize, decodedData};
}

void TransferObjectData::init(u_short newMetaDataLengthSize) {
    TransferObjectData::metaDataBytesSize = newMetaDataLengthSize;
}

u_int64_t TransferObjectData::decodeDataLength(const byte *metaData) {
    u_int64_t dataSize = 0;
    for(int i = 0; i < metaDataBytesSize; i ++){
        dataSize += ((u_int64_t)metaData[i] << metaDataBytesSize * (7 - i));
    }
    return dataSize;
}


