//
// Created by itari on 2025-11-21.
//

#include <enet.h>      // from your external/enet
#include <iostream>
#include <thread>

#include "ServerCommunicator.h"
// include shared ECS and game logic

int main(int argc, char** argv) {
    // ENet init, create ENetHost for server, run server loop
    // no SDL at all
    std::cout << "Hello World!\n";

    std::thread connection(ServerCommunicator::connect_to_client);

    connection.join();

    return 0;
}
