//
// Created by Manuel on 15.02.2021.
//

#include "ServerResponse.h"
#include "RequestResponse.h"
#include "FullStateResponse.h"

#include "../../exceptions/MonopolyException.hpp"

// for deserialization
const std::unordered_map<std::string, ResponseType> ServerResponse::_stringToResponseType = {
        {"REQ_RESPONSE", ResponseType::REQ_RESPONSE },
        {"STATE_DIFF_MSG", ResponseType::STATE_DIFF_MSG},
        {"FULL_STATE_MSG", ResponseType::FULL_STATE_MSG}
};
// for serialization
const std::unordered_map<ResponseType, std::string> ServerResponse::_responseTypeToString = {
        { ResponseType::REQ_RESPONSE,   "REQ_RESPONSE" },
        { ResponseType::STATE_DIFF_MSG, "STATE_DIFF_MSG"},
        { ResponseType::FULL_STATE_MSG, "FULL_STATE_MSG"}
};

ServerResponse::ServerResponse(ServerResponse::BaseClassProperties params):
        _type(params.type),
        _gameId(params.gameId)
{ }

ResponseType ServerResponse::getType() const {
    return this->_type;
}

std::string ServerResponse::getGameId() const {
    return this->_gameId;
}


ServerResponse::BaseClassProperties
ServerResponse::createBaseClassProperties(ResponseType type, const std::string &gameId) {
    ServerResponse::BaseClassProperties params;
    params.type = type;
    params.gameId = gameId;
    return params;
}

ServerResponse::BaseClassProperties ServerResponse::extractBaseClassProperties(const rapidjson::Value& json) {
    if (json.HasMember("type") && json.HasMember("gameId")) {
        std::string gameId = json["gameId"].GetString();
        return createBaseClassProperties(
                ServerResponse::_stringToResponseType.at(json["type"].GetString()),
                gameId
        );
    }
    else
    {
        throw MonopolyException("Server Response did not contain gameId");
    }
}


ServerResponse *ServerResponse::fromJson(const rapidjson::Value& json) {

    if (json.HasMember("type") && json["type"].IsString()) {
        std::string type = json["type"].GetString();
        ResponseType responseType = ServerResponse::_stringToResponseType.at(type);

        if (responseType == ResponseType::REQ_RESPONSE) {
            return RequestResponse::fromJson(json);
        }
        else if (responseType == ResponseType::FULL_STATE_MSG) {
            return FullStateResponse::fromJson(json);
        } else {
            throw MonopolyException("Encountered unknown ServerResponse type " + type);
        }
    }
    throw MonopolyException("Could not determine type of ClientRequest");
}

void ServerResponse::writeIntoJson(rapidjson::Value &json,
                                      rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    rapidjson::Value typeVal(_responseTypeToString.at(this->_type).c_str(), allocator);
    json.AddMember("type", typeVal, allocator);

    rapidjson::Value gameIdVal(_gameId.c_str(), allocator);
    json.AddMember("gameId", gameIdVal, allocator);
}



