//
// Created by Manuel on 28.01.2021.
//

#include "BuyPropertyRequest.h"

// Public constructor
BuyPropertyRequest::BuyPropertyRequest(std::string gameId, std::string player_id, std::string cellId)
        : ClientRequest(
        ClientRequest::createBaseClassProperties(RequestType::BUY_PROPERTY, uuid_generator::generate_uuid_v4(),
                                                 player_id, gameId) ),
        _cellId(cellId)
{ }

// private constructor for deserialization
BuyPropertyRequest::BuyPropertyRequest(ClientRequest::BaseClassProperties props, std::string cellId) :
        ClientRequest(props),
        _cellId(cellId)
{ }

BuyPropertyRequest* BuyPropertyRequest::fromJson(const rapidjson::Value &json) {
    BaseClassProperties props = ClientRequest::extractBaseClassProperties(json);
    if (json.HasMember("cellId")) {
        return new BuyPropertyRequest(props, json["cellId"].GetString());
    } else {
        throw MonopolyException("Could not find 'cellId' or 'value' in BuyPropertyRequest");
    }
}

void BuyPropertyRequest::writeIntoJson(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<> &allocator) const {
    ClientRequest::writeIntoJson(json, allocator);
    rapidjson::Value cellIdVal(_cellId.c_str(), allocator);
    json.AddMember("cellId", cellIdVal,allocator);
}
