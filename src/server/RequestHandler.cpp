//
// Created by Manuel on 08.02.2022.
//

#ifndef MONOPOLY_REQUEST_HANDLER_CPP
#define MONOPOLY_REQUEST_HANDLER_CPP

#include "RequestHandler.h"

#include "PlayerManager.h"
#include "GameInstanceManager.h"
#include "GameInstance.h"
#include <iostream>

#include "../common/network/requests/JoinLobbyRequest.h"
#include "../common/network/requests/BuyPropertyRequest.h"
#include "../common/network/requests/SellPropertyRequest.h"
#include "../common/network/requests/BuyBuildingRequest.h"
#include "../common/network/requests/SellBuildingRequest.h"
// Requests that does not need cellID do not need to be included (Refer to lama)

RequestResponse* request_handler::handle_request(const ClientRequest* const req) {

    // Prepare variables that are used by every request type
    Player* player;
    std::string err;
    GameInstance* gameInstancePtr = nullptr;

    // Get common properties of requests
    RequestType type = req->getType();
    std::string reqId = req->getReqId();
    std::string gameId = req->getGameId();
    std::string playerId = req->getPlayerId();
//    cerr << "this player id: " << playerId << endl;


    // Switch behavior according to request type
    switch(type) {

        // ##################### JOIN GAME #####################  //
        case RequestType::JOIN_LOBBY: {
            std::string playerName = ((JoinLobbyRequest *) req)->getPlayerName();

            // Create new player or get existing one with that name
            PlayerManager::addOrGetPlayer(playerName, playerId, player);

            if (gameId.empty()) {
                // join any game
                if (GameInstanceManager::tryAddPlayerToAnyGame(player, gameInstancePtr, err)) {
                    // gameInstancePtr got updated to the joined game

                    // return response with full game_state attached
                    return new RequestResponse(gameInstancePtr->getId(), reqId, true,
                                                gameInstancePtr->getGameState()->toJson(), err);
                } else {
                    // failed to find game to join
                    return new RequestResponse("", reqId, false, nullptr, err);
                }
            } else {
                // join a specific game denoted by req->getGameId()
                if (GameInstanceManager::tryGetGameInstance(gameId, gameInstancePtr)) {
                    if (GameInstanceManager::tryAddPlayer(player, gameInstancePtr, err)) {
                        // return response with full game_state attached
                        return new RequestResponse(gameId, reqId, true,
                                                    gameInstancePtr->getGameState()->toJson(), err);
                    } else {
                        // failed to join requested game
                        return new RequestResponse("", reqId, false, nullptr, err);
                    }
                } else {
                    // failed to find requested game
                    return new RequestResponse("", reqId, false, nullptr, "Requested game could not be found.");
                }
            }
        }


        // ##################### START GAME ##################### //
        case RequestType::START_GAME: {
            if (GameInstanceManager::tryGetPlayerAndGameInstance(playerId, player, gameInstancePtr, err)) {
                if (gameInstancePtr->startGame(player, err)) {
                    return new RequestResponse(gameInstancePtr->getId(), reqId, true,
                                                gameInstancePtr->getGameState()->toJson(), err);
                }
            }
            return new RequestResponse("", reqId, false, nullptr, err);
        }


        // ##################### BUY BUILDING ##################### //
        case RequestType::BUY_BUILDING: {
            if (GameInstanceManager::tryGetPlayerAndGameInstance(playerId, player, gameInstancePtr, err)) {
                int cellId = stoi(((BuyBuildingRequest *) req)->getCellId());
                if (gameInstancePtr->buyBuilding(player, cellId, err)) {
                    return new RequestResponse(gameInstancePtr->getId(), reqId, true,
                                                gameInstancePtr->getGameState()->toJson(), err);
                }
            }
            return new RequestResponse("", reqId, false, nullptr, err);
        }

        // ##################### SELL BUILDING ##################### //
        case RequestType::SELL_BUILDING: {
            if (GameInstanceManager::tryGetPlayerAndGameInstance(playerId, player, gameInstancePtr, err)) {
                int cellId = stoi(((SellBuildingRequest *) req)->getCellId());
                if (gameInstancePtr->sellBuilding(player, cellId, err)) {
                    return new RequestResponse(gameInstancePtr->getId(), reqId, true,
                                               gameInstancePtr->getGameState()->toJson(), err);
                }
            }
            return new RequestResponse("", reqId, false, nullptr, err);
        }

        // ##################### BUY PROPERTY ##################### //
        case RequestType::BUY_PROPERTY: {
            if (GameInstanceManager::tryGetPlayerAndGameInstance(playerId, player, gameInstancePtr, err)) {
                int cellId = stoi(((BuyPropertyRequest *) req)->getCellId());
                if (gameInstancePtr->buyProperty(player, cellId, err)) {
                    return new RequestResponse(gameInstancePtr->getId(), reqId, true,
                                               gameInstancePtr->getGameState()->toJson(), err);
                }
            }
            return new RequestResponse("", reqId, false, nullptr, err);
        }

        // ##################### SELL PROPERTY ##################### //
        case RequestType::SELL_PROPERTY: {
            if (GameInstanceManager::tryGetPlayerAndGameInstance(playerId, player, gameInstancePtr, err)) {
                int cellId = stoi(((SellPropertyRequest *) req)->getCellId());
                if (gameInstancePtr->sellProperty(player, cellId, err)) {
                    return new RequestResponse(gameInstancePtr->getId(), reqId, true,
                                               gameInstancePtr->getGameState()->toJson(), err);
                }
            }
            return new RequestResponse("", reqId, false, nullptr, err);
        }

        // ##################### END TURN ##################### //
        case RequestType::END_TURN: {
            if (GameInstanceManager::tryGetPlayerAndGameInstance(playerId, player, gameInstancePtr, err)) {
                if (gameInstancePtr->endTurn(player, err)) {
                    return new RequestResponse(gameInstancePtr->getId(), reqId, true,
                                               gameInstancePtr->getGameState()->toJson(), err);
                }
            }
            return new RequestResponse("", reqId, false, nullptr, err);
        }

        // ##################### ROLL DICE ##################### //
        case RequestType::ROLL_DICE: {
            if (GameInstanceManager::tryGetPlayerAndGameInstance(playerId, player, gameInstancePtr, err)) {
                if (gameInstancePtr->rollDice(player, err)) {
                    return new RequestResponse(gameInstancePtr->getId(), reqId, true,
                                               gameInstancePtr->getGameState()->toJson(), err);
                }
            }
            return new RequestResponse("", reqId, false, nullptr, err);
        }

        // ##################### USE GET OUT OF JAIL FREE ##################### //
        case RequestType::USE_GET_OUT_OF_JAIL_FREE: {
            if (GameInstanceManager::tryGetPlayerAndGameInstance(playerId, player, gameInstancePtr, err)) {
                if (gameInstancePtr->useGetOutOfJailCard(player, err)) {
                    return new RequestResponse(gameInstancePtr->getId(), reqId, true,
                                               gameInstancePtr->getGameState()->toJson(), err);
                }
            }
            return new RequestResponse("", reqId, false, nullptr, err);
        }
        // ##################### UNKNOWN REQUEST ##################### //
        default:
            return new RequestResponse("", reqId, false, nullptr, "Unknown RequestType " + ClientRequest::RequestTypeToString.at(type));
    }
}

#endif //MONOPOLY_REQUEST_HANDLER_CPP
