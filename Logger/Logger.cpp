//
// Created by user on 24.07.2021.
//

#include "Logger.h"
#include <mutex>
#include <iostream>

std::mutex Logger::mutex;

void Logger::log(std::string message, LEVEL level) {
    std::lock_guard guard(Logger::mutex);

    std::cout<<"["<<level<<"] ";

    if(level == LEVEL::ERROR){
        std::cout<<"\033[1;31m" + message + "\033[0m\n";
    } else if(level == LEVEL::WARNING){
        std::cout<<"\033[;33m" + message + "\033[0m\n";
    }else if(level == LEVEL::INFO){
        std::cout<<"\033[;37m" + message + "\033[0m\n";
    }else if(level == LEVEL::DEBUG){
        std::cout<<"\033[;34m" + message + "\033[0m\n";
    }
}
