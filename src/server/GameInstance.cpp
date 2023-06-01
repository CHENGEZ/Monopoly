//
// Created by Manuel on 25.01.2021.
//
// The GameInstance class is a wrapper around the game_state of an active instance of the game.
// This class contains functions to modify the contained game_state.

#include "GameInstance.h"
#include "ServerNetworkManager.h"
#include "../common/network/responses/FullStateResponse.h"



GameInstance::GameInstance() {
    _gameState = new Game();
}

GameInstance::~GameInstance() {
    if (_gameState != nullptr) {
        delete _gameState;
    }
    _gameState = nullptr;
}

Game *GameInstance::getGameState() {
    return _gameState;
}

std::string GameInstance::getId() {
    return _gameState->getId();
}

bool GameInstance::isPlayerAllowedToPlay(Player *player) {
    return _gameState->isAllowedToPlayNow(player);
}

bool GameInstance::isFull() {
    return _gameState->isFull();
}

bool GameInstance::isStarted() {
    return _gameState->isStarted();
}

bool GameInstance::isFinished() {
    return _gameState->isFinished();
}

bool GameInstance::buyProperty(Player *player, int cellId, std::string &err) {
    modificationLock.lock();
    if (_gameState->buyProperty(player, cellId, err)) {
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), player);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}

bool GameInstance::sellProperty(Player *player, int cellId, std::string &err) {
    modificationLock.lock();
    if (_gameState->sellProperty(player, cellId, err)) {
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), player);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}

bool GameInstance::buyBuilding(Player *player, int cellId, std::string &err) {
    modificationLock.lock();
    if (_gameState->buyBuilding(player, cellId, err)) {
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), player);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}

bool GameInstance::sellBuilding(Player *player, int cellId, std::string &err) {
    modificationLock.lock();
    if (_gameState->sellBuilding(player, cellId, err)) {
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), player);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}

bool GameInstance::rollDice(Player *player, std::string& err) {
    modificationLock.lock();
    cerr << "Curr player id: " << player->getId() << endl;
    cerr << "theorical curr player id: " << _gameState->getCurrentPlayer()->getId() << endl;
    for (int i = 0; i < _gameState->getPlayers().size();i++)
    {
        cerr << "this player id in list is: " << _gameState->getPlayers()[i]->getId() << endl;
    }
    if (_gameState->rollDice(player, err)) {
        // send state update to all other players
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), player);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}

bool GameInstance::endTurn(Player* player, std::string& err) {
    modificationLock.lock();
    if (_gameState->endTurn(player, err)) {
        // send state update to all other players
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), player);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}

bool GameInstance::useGetOutOfJailCard(Player *player, std::string &err) {
    modificationLock.lock();
    if (_gameState->useGetOutOfJailCard(player, err)) {
        // send state update to all other players
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), player);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}



bool GameInstance::startGame(Player* player, std::string &err) {
    modificationLock.lock();
    if (_gameState->startGame(err)) {
        // send state update to all other players
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), player);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}

bool GameInstance::tryRemovePlayer(Player *player, std::string &err) {
    modificationLock.lock();
    if (_gameState->removePlayer(player, err)) {
        player->setGameId("");
        // send state update to all other players
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), player);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}

bool GameInstance::tryAddPlayer(Player *newPlayer, std::string &err) {
    modificationLock.lock();
    if (_gameState->addPlayer(newPlayer, err)) {
        newPlayer->setGameId(getId());
        // send state update to all other players
        FullStateResponse state_update_msg = FullStateResponse(this->getId(), *_gameState);
        ServerNetworkManager::broadcast_message(state_update_msg, _gameState->getPlayers(), newPlayer);
        modificationLock.unlock();
        return true;
    }
    modificationLock.unlock();
    return false;
}
