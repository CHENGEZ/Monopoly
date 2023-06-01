//
// Created by Manuel on 28.01.2021.
//

#include "SellPropertyRequest.h"

// Public constructor
SellPropertyRequest::SellPropertyRequest(std::string gameId, std::string player_id, std::string cellId)
        : ClientRequest(
        ClientRequest::createBaseClassProperties(RequestType::SELL_PROPERTY, uuid_generator::generate_uuid_v4(),
                                                 player_id, gameId) ),
          _cellId(cellId)
{ }

// private constructor for deserialization
SellPropertyRequest::SellPropertyRequest(ClientRequest::BaseClassProperties props, std::string cellId) :
        ClientRequest(props),
        _cellId(cellId)
{ }

SellPropertyRequest* SellPropertyRequest::fromJson(const rapidjson::Value &json) {
    BaseClassProperties props = ClientRequest::extractBaseClassProperties(json);
    if (json.HasMember("cellId")) {
        return new SellPropertyRequest(props, json["cellId"].GetString());
    } else {
        throw MonopolyException("Could not find 'cellId' or 'value' in SellPropertyRequest");
    }
}

void SellPropertyRequest::writeIntoJson(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<> &allocator) const {
    ClientRequest::writeIntoJson(json, allocator);
    rapidjson::Value cellIdVal(_cellId.c_str(), allocator);
    json.AddMember("cellId", cellIdVal,allocator);
}
