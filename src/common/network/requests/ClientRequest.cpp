//
// Created by Manuel on 28.01.2021.
//

#include "ClientRequest.h"
#include "BuyBuildingRequest.h"
#include "SellBuildingRequest.h"
#include "BuyPropertyRequest.h"
#include "SellPropertyRequest.h"
#include "JoinLobbyRequest.h"
#include "RollDiceRequest.h"
#include "EndTurnRequest.h"
#include "StartGameRequest.h"
#include "UseGetOutOfJailFreeRequest.h"

#include <iostream>

// for deserialization
const std::unordered_map<std::string, RequestType> ClientRequest::StringToRequestType = {
        {"JOIN_LOBBY", RequestType::JOIN_LOBBY },
        {"START_GAME", RequestType::START_GAME},
        {"BUY_PROPERTY", RequestType::BUY_PROPERTY},
        {"SELL_PROPERTY", RequestType::SELL_PROPERTY},
        {"BUY_BUILDING", RequestType::BUY_BUILDING},
        {"SELL_BUILDING", RequestType::SELL_BUILDING},
        {"ROLL_DICE", RequestType::ROLL_DICE},
        {"END_TURN", RequestType::END_TURN},
        {"USE_GET_OUT_OF_JAIL_FREE", RequestType::USE_GET_OUT_OF_JAIL_FREE},
};
// for serialization
const std::unordered_map<RequestType, std::string> ClientRequest::RequestTypeToString = {
        {RequestType::JOIN_LOBBY, "JOIN_LOBBY"},
        {RequestType::START_GAME, "START_GAME"},
        {RequestType::BUY_PROPERTY, "BUY_PROPERTY"},
        {RequestType::SELL_PROPERTY, "SELL_PROPERTY"},
        {RequestType::BUY_BUILDING, "BUY_BUILDING"},
        {RequestType::SELL_BUILDING, "SELL_BUILDING"},
        {RequestType::ROLL_DICE, "ROLL_DICE"},
        {RequestType::END_TURN, "END_TURN"},
        {RequestType::USE_GET_OUT_OF_JAIL_FREE, "USE_GET_OUT_OF_JAIL_FREE"}
};

// protected constructor. only used by subclasses
ClientRequest::ClientRequest(ClientRequest::BaseClassProperties props) :
        _type(props._type),
        _reqId(props._reqId),
        _playerId(props._playerId),
        _gameId(props._gameId)
{ }


// used by subclasses to retrieve information from the json stored by this superclass
ClientRequest::BaseClassProperties ClientRequest::extractBaseClassProperties(const rapidjson::Value& json) {
    if (json.HasMember("playerId") && json.HasMember("gameId") && json.HasMember("reqId")) {
        std::string playerId = json["playerId"].GetString();
        std::string gameId = json["gameId"].GetString();
        std::string reqId = json["reqId"].GetString();
        return createBaseClassProperties(
                ClientRequest::StringToRequestType.at(json["type"].GetString()),
                reqId,
                playerId,
                gameId
        );
    }
    else
    {
        throw MonopolyException("Client Request did not contain playerId or gameId");
    }
}

ClientRequest::BaseClassProperties ClientRequest::createBaseClassProperties(
        RequestType type,
        std::string reqId,
        std::string& playerId,
        std::string& gameId)
{
    ClientRequest::BaseClassProperties res;
    res._playerId = playerId;
    res._gameId = gameId;
    res._reqId = reqId;
    res._type = type;
    return res;
}


void ClientRequest::writeIntoJson(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<> &allocator) const {
    rapidjson::Value type_val(RequestTypeToString.at(this->_type).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    rapidjson::Value playerIdVal(_playerId.c_str(), allocator);
    json.AddMember("playerId", playerIdVal, allocator);

    rapidjson::Value gameIdVal(_gameId.c_str(), allocator);
    json.AddMember("gameId", gameIdVal, allocator);

    rapidjson::Value reqIdVal(_reqId.c_str(), allocator);
    json.AddMember("reqId", reqIdVal, allocator);
}

ClientRequest* ClientRequest::fromJson(const rapidjson::Value &json) {
    if (json.HasMember("type") && json["type"].IsString()) {
        const std::string type = json["type"].GetString();
        const RequestType request_type = ClientRequest::StringToRequestType.at(type);

        // Check which type of request it is and call the respective fromJson constructor
        if (request_type == RequestType::BUY_BUILDING) {
            return BuyBuildingRequest::fromJson(json);
        }
        else if (request_type == RequestType::SELL_BUILDING) {
            return SellBuildingRequest::fromJson(json);
        }
        else if (request_type == RequestType::BUY_PROPERTY) {
            return BuyPropertyRequest::fromJson(json);
        }
        else if (request_type == RequestType::SELL_PROPERTY) {
            return SellPropertyRequest::fromJson(json);
        }
        else if (request_type == RequestType::USE_GET_OUT_OF_JAIL_FREE) {
            return UseGetOutOfJailFreeRequest::fromJson(json);
        }
        else if (request_type == RequestType::END_TURN) {
            return EndTurnRequest::fromJson(json);
        }
        else if (request_type == RequestType::JOIN_LOBBY) {
            return JoinLobbyRequest::fromJson(json);
        }
        else if (request_type == RequestType::START_GAME) {
            return StartGameRequest::fromJson(json);
        }
        else if (request_type == RequestType::ROLL_DICE) {
            return RollDiceRequest::fromJson(json);
        }
        else {
            throw MonopolyException("Encountered unknown ClientRequest type " + type);
        }
    }
    throw MonopolyException("Could not determine type of ClientRequest. JSON was:\n" + json_utils::toString(&json));
}


std::string ClientRequest::toString() const {
    return "ClientRequest of type " + ClientRequest::RequestTypeToString.at(_type) + " for playerId " + _playerId + " and gameId " + _gameId;
}






