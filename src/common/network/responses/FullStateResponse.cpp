//
// Created by Manuel on 15.02.2021.
//

#include "FullStateResponse.h"

#include "../../exceptions/MonopolyException.hpp"
#include "../../serialization/json_utils.h"

#ifdef MONOPOLY_CLIENT
//#include "../../../client/GameController.h"
#endif

FullStateResponse::FullStateResponse(ServerResponse::BaseClassProperties props, rapidjson::Value* stateJson) :
        ServerResponse(props),
        _stateJson(stateJson)
{ }

FullStateResponse::FullStateResponse(std::string gameId, const Game& state):
        ServerResponse(ServerResponse::createBaseClassProperties(ResponseType::FULL_STATE_MSG, gameId))
{
    this->_stateJson = state.toJson();
}


void FullStateResponse::writeIntoJson(rapidjson::Value &json,
                                       rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ServerResponse::writeIntoJson(json, allocator);
    json.AddMember("stateJson", *_stateJson, allocator);
}

FullStateResponse *FullStateResponse::fromJson(const rapidjson::Value& json) {
    if (json.HasMember("stateJson")) {
        return new FullStateResponse(ServerResponse::extractBaseClassProperties(json),
                                       json_utils::cloneValue(json["stateJson"].GetObject()));
    } else {
        throw MonopolyException("Could not parse FullStateResponse from json. state is missing.");
    }
}

FullStateResponse::~FullStateResponse() {
    if (_stateJson != nullptr) {
        delete _stateJson;
        _stateJson = nullptr;
    }
}

#ifdef MONOPOLY_CLIENT

Game * FullStateResponse::Process() const {
    try{
        Game* game = Game::fromJson(*_stateJson);
        return game;
    }catch(std::exception& e) {
        std::cerr << "Failed to extract game_state from full_state_response" << std::endl
                  << e.what() << std::endl;
    }
}

#endif
