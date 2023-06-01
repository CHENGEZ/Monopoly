//
// Created by Manuel on 29.01.2021.
//
// The PlayerManager only exists on the server side. It stores all connected users since starting the server. It offers
// functionality to retrieve players by id or adding players when they first connect to the server.
//

#ifndef MONOPOLY_PLAYER_MANAGER_H
#define MONOPOLY_PLAYER_MANAGER_H

#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "../common/game_state/player/Player.hpp"

class PlayerManager {

private:

    inline static std::shared_mutex _rwLock;
    static std::unordered_map<std::string, Player*> _playersLut;

public:
    static bool tryGetPlayer(const std::string& playerId, Player*& playerPtr);
    static bool addOrGetPlayer(std::string name, const std::string& playerId, Player*& playerPtr);
    static bool removePlayer(const std::string& playerId, Player*& player);  // not implemented
};


#endif //MONOPOLY_PLAYER_MANAGER_H
