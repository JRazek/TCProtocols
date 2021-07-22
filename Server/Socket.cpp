//
// Created by user on 23.07.2021.
//

#include <cstdio>
#include "Socket.h"

Socket::Socket(u_short PORT) : port(PORT) {
    int opt = 1;
    if ((this->serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        return;
    }


    if (setsockopt(this->serverFileDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
        perror("setsockopt");
        return;
    }
    this->addressData = {};
    this->addressData.sin_family = AF_INET;
    this->addressData.sin_addr.s_addr = INADDR_ANY;
    this->addressData.sin_port = htons( port );

    if (bind(this->serverFileDescriptor, (struct sockaddr *)&this->addressData, sizeof(this->addressData))<0){
        perror("bind failed");
        return;
    }
}
