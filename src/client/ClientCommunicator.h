//
// Created by itari on 2025-11-22.
//

#ifndef IRON_TIDE_CLIENTCOMMUNICATOR_H
#define IRON_TIDE_CLIENTCOMMUNICATOR_H

#include <thread>

class ClientCommunicator {
public:
    static int connect_to_server(const std::stop_token& st);
};


#endif //IRON_TIDE_CLIENTCOMMUNICATOR_H