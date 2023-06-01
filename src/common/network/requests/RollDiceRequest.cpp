//
// Created by Manuel on 29.01.2021.
//

#include "RollDiceRequest.h"

// Public constructor
RollDiceRequest::RollDiceRequest(std::string gameId, std::string playerId)
        : ClientRequest(
        ClientRequest::createBaseClassProperties(RequestType::ROLL_DICE, uuid_generator::generate_uuid_v4(), playerId,
                                                 gameId) )
{ }

// private constructor for deserialization
RollDiceRequest::RollDiceRequest(ClientRequest::BaseClassProperties props) :
        ClientRequest(props)
{ }

RollDiceRequest* RollDiceRequest::fromJson(const rapidjson::Value &json) {
    return new RollDiceRequest(ClientRequest::extractBaseClassProperties((json)));
}

void RollDiceRequest::writeIntoJson(rapidjson::Value &json,
                                   rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::writeIntoJson(json, allocator);
}