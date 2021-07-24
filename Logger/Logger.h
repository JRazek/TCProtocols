//
// Created by user on 24.07.2021.
//

#ifndef TCPROTOCOLS_LOGGER_H
#define TCPROTOCOLS_LOGGER_H

#include <mutex>
#include "LEVEL.h"

class Logger {
public:
    static std::mutex mutex;
    static void log(std::string message, LEVEL level);
};


#endif //TCPROTOCOLS_LOGGER_H
