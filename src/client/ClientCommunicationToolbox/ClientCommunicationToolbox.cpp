//
// Created by Ricky on 2023/5/6.
//

#include "ClientCommunicationToolbox.h"
#include "../HelperFunctions.cpp"
#include "ClientNetworkManager.h"

#include "../../common/game_state/player/Player.hpp"
#include "../../common/network/requests/JoinLobbyRequest.h"
#include "../../common/network/requests/StartGameRequest.h"
#include "../../common/network/requests/RollDiceRequest.h"
#include "../../common/network/requests/BuyBuildingRequest.h"
#include "../../common/network/requests/BuyPropertyRequest.h"
#include "../../common/network/requests/EndTurnRequest.h"
#include "../../common/network/requests/SellBuildingRequest.h"
#include "../../common/network/requests/SellPropertyRequest.h"
#include "../../common/network/requests/UseGetOutOfJailFreeRequest.h"

using namespace std;

Player * ClientCommunicationToolbox::connectToServer(const std::string& iP, const int port, const std::string& Name) {
    if(port > 65535){
        showError("Invalid port");
        return nullptr;
    }
    ClientNetworkManager::init(iP, port);
    Player* currPlayer = new Player(Name);
    JoinLobbyRequest request = JoinLobbyRequest(currPlayer->getId(), currPlayer->getName());
    ClientNetworkManager::sendRequest(request);
    return currPlayer;
}

bool ClientCommunicationToolbox::startGame(Game *currGame, Player* currPlayer) {
    if(currGame == nullptr || currPlayer == nullptr){
        showError("Error in backend, one of game or player passed to the connection is nullptr");
        return false;
    }else{
        StartGameRequest request = StartGameRequest(currGame->getId(), currPlayer->getId());
        ClientNetworkManager::sendRequest(request);
        return true;
    }
}

bool ClientCommunicationToolbox::rollDiceRequest(const string gameID, const string playerID) {
    if (gameID == "" || playerID == ""){
        showError("backend error, the game and player id passed has at least one empty");
        return false;
    }else{
        RollDiceRequest request = RollDiceRequest(gameID, playerID);
        ClientNetworkManager::sendRequest(request);
        return true;
    }
}

bool ClientCommunicationToolbox::buyBuilding(string gameID, string playerID, string cellID) {
    if (gameID == "" || playerID == "" || cellID == ""){
        showError("backend error, the game and player id passed has at least one empty");
        return false;
    }else{
        BuyBuildingRequest request = BuyBuildingRequest(gameID, playerID, cellID);
        ClientNetworkManager::sendRequest(request);
        return true;
    }
    return false;
}

bool ClientCommunicationToolbox::buyProperty(std::string gameID, std::string playerID, std::string cellID) {
    if (gameID == "" || playerID == "" || cellID == ""){
        showError("backend error, the game and player id passed has at least one empty");
        return false;
    }else{
        BuyPropertyRequest request = BuyPropertyRequest(gameID, playerID, cellID);
        ClientNetworkManager::sendRequest(request);
        return true;
    }
    return false;
}

bool ClientCommunicationToolbox::endTurn(std::string gameID, std::string playerID) {
    if (gameID == "" || playerID == ""){
        showError("backend error, the game and player id passed has at least one empty");
        return false;
    }else{
        EndTurnRequest request = EndTurnRequest(gameID, playerID);
        ClientNetworkManager::sendRequest(request);
        return true;
    }
}

bool ClientCommunicationToolbox::sellBuilding(std::string gameID, std::string playerID, std::string cellID) {
    if (gameID == "" || playerID == "" || cellID == ""){
        showError("backend error, the game and player id passed has at least one empty");
        return false;
    }else{
        SellBuildingRequest request = SellBuildingRequest(gameID, playerID, cellID);
        ClientNetworkManager::sendRequest(request);
        return true;
    }
    return false;
}

bool ClientCommunicationToolbox::sellProperty(std::string gameID, std::string playerID, std::string cellID) {
    if (gameID == "" || playerID == "" || cellID == ""){
        showError("backend error, the game and player id passed has at least one empty");
        return false;
    }else{
        SellPropertyRequest request = SellPropertyRequest(gameID, playerID, cellID);
        ClientNetworkManager::sendRequest(request);
        return true;
    }
    return false;
}

bool ClientCommunicationToolbox::useGetOutOfJailFree(std::string gameID, std::string playerID) {
    if (gameID == "" || playerID == ""){
        showError("backend error, the game and player id passed has at least one empty");
        return false;
    }else{
        UseGetOutOfJailFreeRequest request = UseGetOutOfJailFreeRequest(gameID, playerID);
        ClientNetworkManager::sendRequest(request);
        return true;
    }
}

