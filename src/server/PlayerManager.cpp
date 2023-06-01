//
// Created by Manuel on 29.01.2021.
//
// The PlayerManager only exists on the server side. It stores all connected users since starting the server. It offers
// functionality to retrieve players by id or adding players when they first connect to the server.
//

#include "PlayerManager.h"

// Initialize static map
std::unordered_map<std::string, Player*> PlayerManager::_playersLut = {};

bool PlayerManager::tryGetPlayer(const std::string& playerId, Player *&playerPtr) {
    playerPtr = nullptr;
    _rwLock.lock_shared();
    auto it = PlayerManager::_playersLut.find(playerId);
    if (it != _playersLut.end()) {
        playerPtr = it->second;
    }
    _rwLock.unlock_shared();
    return playerPtr;
}

bool PlayerManager::addOrGetPlayer(std::string name, const std::string& playerId, Player *&playerPtr) {
    if (tryGetPlayer(playerId, playerPtr)) {
        return true;
    }
    playerPtr = new Player(playerId, name);
    _rwLock.lock();    // exclusive
    PlayerManager::_playersLut.insert({playerId, playerPtr});
    _rwLock.unlock();
    return true;
}

bool PlayerManager::removePlayer(const std::string& playerId, Player *&player) {
    if (tryGetPlayer(playerId, player)) {
        _rwLock.lock();    // exclusive
        int nof_removals = PlayerManager::_playersLut.erase(playerId);
        _rwLock.unlock();
        return true;
    }
    return false;
}

