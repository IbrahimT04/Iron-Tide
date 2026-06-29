//
// Created by itari on 2025-11-22.
//

#include "ClientCommunicator.h"

#include <cstdio>
#include <iostream>
#include <cstring>
#include <SDL3/SDL.h>

#include "enet.h"

int ClientCommunicator::connect_to_server(const std::stop_token& st) {
    SDL_Log("Client: connect_to_server START");
    if (enet_initialize() != 0) {
        SDL_Log("An error occurred while initializing ENet.");
        return EXIT_FAILURE;
    }

    ENetHost *client = NULL;
    client = enet_host_create(NULL /* create a client host */,
                              1 /* only allow 1 outgoing connection */,
                              2 /* allow up 2 channels to be used, 0 and 1 */,
                              0 /* assume any amount of incoming bandwidth */,
                              0 /* assume any amount of outgoing bandwidth */);
    if (client == NULL) {
        SDL_Log("An error occurred while trying to create an ENet client host.");
        exit(EXIT_FAILURE);
    }

    ENetAddress address{};
    ENetEvent event{};
    ENetPeer *peer = NULL;

    /* Connect to some.server.net:1234. */
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 7777;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == NULL) {
        SDL_Log("No available peers for initiating an ENet connection.");
        exit(EXIT_FAILURE);
    }

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        SDL_Log("Connection to some.server.net:1234 succeeded.");
        } else {
            /* Either the 5 seconds are up or a disconnect event was
             * received. Reset the peer in the event the 5 seconds
             * had run out without any significant event.
             */
            enet_peer_reset(peer);
            SDL_Log("Connection to some.server.net:1234 failed.");

            enet_host_destroy(client);
            enet_deinitialize();
            return EXIT_FAILURE;
        }

    int i = 0;
    while (!st.stop_requested()) {
        while (enet_host_service(client, &event, 100) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    SDL_Log("Client received packet: %s",
                            reinterpret_cast<char*>(event.packet->data));
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    SDL_Log("Client: server disconnected.");
                    break;

                case ENET_EVENT_TYPE_CONNECT:
                    SDL_Log("Client: got CONNECT event in loop.");
                    break;

                default:
                    break;
            }
        }

        SDL_Log("Client loop iteration %d", i);

        if (i % 30 == 0) {
            const char* msg = "Hello from client!";
            ENetPacket* packet = enet_packet_create(
                msg,
                std::strlen(msg) + 1,
                ENET_PACKET_FLAG_RELIABLE
            );
            enet_peer_send(peer, 0, packet);
            enet_host_flush(client);
            SDL_Log("Client: sent packet to server.");
        }
        ++i;
    }

    // Disconnect
    enet_peer_disconnect(peer, 0);

    uint8_t disconnected = false;
    /* Allow up to 3 seconds for the disconnect to succeed
     * and drop any packets received packets.
     */
    while (enet_host_service(client, &event, 3000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                SDL_Log("Disconnection succeeded.");
                disconnected = true;
                break;
            default: ;
        }
    }

    // Drop connection, since disconnection didn't successed
    if (!disconnected) {
        enet_peer_reset(peer);
    }

    enet_host_destroy(client);
    enet_deinitialize();

    return 0;
}
