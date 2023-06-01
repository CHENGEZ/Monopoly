//
// Created by Manuel on 15.02.2021.
//

#ifndef MONOPOLY_FULL_STATE_RESPONSE_H
#define MONOPOLY_FULL_STATE_RESPONSE_H
#include "../../game_state/Game.h"
#include "ServerResponse.h"

class FullStateResponse : public ServerResponse {
private:
    rapidjson::Value* _stateJson;

    /*
     * Private constructor for deserialization
     */
    FullStateResponse(BaseClassProperties props, rapidjson::Value* stateJson);
    rapidjson::Document* _stateToJson(Game state);

public:

    FullStateResponse(std::string gameId, const Game& state);
    ~FullStateResponse();

    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static FullStateResponse* fromJson(const rapidjson::Value& json);

#ifdef MONOPOLY_CLIENT
    virtual Game * Process() const override;
#endif
};


#endif //MONOPOLY_FULL_STATE_RESPONSE_H
