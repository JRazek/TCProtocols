//
// Created by user on 17.07.2021.
//

#ifndef OPENCV_TRANSFEROBJECTDATA_H
#define OPENCV_TRANSFEROBJECTDATA_H
#include <zconf.h>
#include <algorithm>


typedef unsigned short u_short;
typedef unsigned char byte;
struct TransferObjectData {
    inline static u_short metaDataBytesSize = 8;
    static void init(u_short newMetaDataLengthSize);

    static std::pair<u_int64_t, byte *> decode(const byte *objectEncoded);
    static byte *encode(const byte *dataRaw, const u_int64_t dataSize);
    static u_int64_t decodeDataLength(const byte *metaData);
};


#endif //OPENCV_TRANSFEROBJECTDATA_H
