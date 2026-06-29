//
// Created by itari on 2025-11-22.
//

#include <enet.h>
#include <cstdio>
#include <string>
#include <cstring>

#include "ServerCommunicator.h"

int ServerCommunicator::connect_to_client() {
    if (enet_initialize () != 0) {
        printf("An error occurred while initializing ENet.\n");
        return 1;
    }

    ENetAddress address = {0};

    address.host = ENET_HOST_ANY; /* Bind the server to the default localhost.     */
    address.port = 7777; /* Bind the server to port 7777. */

    #define MAX_CLIENTS 32

    /* create a server */
    ENetHost * server = enet_host_create(&address, MAX_CLIENTS, 2, 0, 0);

    if (server == NULL) {
        printf("An error occurred while trying to create an ENet server host.\n");
        return 1;
    }

    printf("Started a server...\n");

    ENetEvent event;

    bool running = true;
    /* Wait up to 1000 milliseconds for an event. (WARNING: blocking) */
    while (running) {
        if (enet_host_service(server, &event, 100) > 0) {
            switch (event.type) {
                struct ClientInfo {
                    std::string name;
                };

                case ENET_EVENT_TYPE_CONNECT:
                {
                    printf("A new client connected from %x:%u.\n",
                        event.peer->address.host, event.peer->address.port);

                    auto* info = new ClientInfo{ "Client information" };
                    event.peer->data = static_cast<void*>(info);

                    const char* msg = "Hello from server!";
                    ENetPacket* packet = enet_packet_create(
                        msg,
                        std::strlen(msg) + 1,
                        ENET_PACKET_FLAG_RELIABLE
                    );
                    enet_peer_send(event.peer, 0, packet);
                    enet_host_flush(server);
                    break;
                }

                case ENET_EVENT_TYPE_RECEIVE:
                {
                    auto* info = static_cast<ClientInfo*>(event.peer->data);
                    printf("A packet of length %lu containing %s was received from %s on channel %u.\n",
                           event.packet->dataLength,
                           event.packet->data,
                           info->name.c_str(),
                           event.channelID);

                    const auto reply = "Server: got your message!";
                    ENetPacket* replyPacket = enet_packet_create(
                        reply,
                        std::strlen(reply) + 1,
                        ENET_PACKET_FLAG_RELIABLE
                    );
                    enet_peer_send(event.peer, 0, replyPacket);
                    enet_host_flush(server);


                    enet_packet_destroy(event.packet);
                    break;
                }

                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    auto* info = static_cast<ClientInfo*>(event.peer->data);
                    printf("%s disconnected.\n", info->name.c_str());
                    delete info;
                    event.peer->data = nullptr;
                    running = false;
                    break;
                }


                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                    printf("%s disconnected due to timeout.\n", static_cast<const char *>(event.peer->data));
                    event.peer->data = NULL;
                    break;

                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    }
    enet_host_destroy(server);
    enet_deinitialize();
    return 0;
}
