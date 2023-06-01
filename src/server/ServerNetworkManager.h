//
// Created by Manuel on 12.02.2021.
//
// The ServerNetworkManager handles all incoming messages and offers functionality to broadcast messages
// to all connected players of a game.

#ifndef MONOPOLY_SERVER_NETWORK_MANAGER_H
#define MONOPOLY_SERVER_NETWORK_MANAGER_H

#include <thread>
#include <functional>
#include <unordered_map>
#include <shared_mutex>

#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"
#include "sockpp/tcp_acceptor.h"

#include "../common/network/requests/ClientRequest.h"
#include "../common/network/responses/ServerResponse.h"
#include "../common/game_state/player/Player.hpp"
#include "../common/game_state/Game.h"

class ServerNetworkManager {
private:

    inline static ServerNetworkManager* _instance;
    inline static std::shared_mutex _rwLock;
    inline static sockpp::tcp_acceptor _acc;

    inline static std::unordered_map<std::string, std::string> _playerIdToAddress;
    inline static std::unordered_map<std::string, sockpp::tcp_socket> __playerIdToSocket;

    void connect(const std::string& url, const uint16_t  port);

    static void listenerLoop();
    static void readMessage(sockpp::tcp_socket socket,
                            const std::function<void(const std::string&, const sockpp::tcp_socket::addr_t&)>& messageHandler);
    static void handle_incoming_message(const std::string& msg, const sockpp::tcp_socket::addr_t& peer_address);
    static ssize_t sendMessage(const std::string& msg, const std::string& address);
public:
    ServerNetworkManager();
    ~ServerNetworkManager();

    // Used to broadcast a ServerResponse (e.g. a FullStateResponse) to all 'players' except 'exclude'
    static void broadcast_message(ServerResponse& msg, const std::vector<Player*>& players, const Player* exclude);

    static void onPlayerLeft(std::string playerId);
};



#endif //MONOPOLY_SERVER_NETWORK_MANAGER_H
