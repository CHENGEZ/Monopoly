//
// Created by Manuel on 15.02.2021.
//

#ifndef MONOPOLY_REQUEST_RESPONSE_H
#define MONOPOLY_REQUEST_RESPONSE_H

#include <string>
#include "ServerResponse.h"
#include "../../game_state/Game.h"


class RequestResponse : public ServerResponse {
private:
    bool _success;
    std::string _err;
    std::string _reqId;
    rapidjson::Value* _stateJson = nullptr;

    RequestResponse(BaseClassProperties props, std::string reqId, bool success, rapidjson::Value* stateJson, std::string& err);

public:
    /**
     * @brief Creates a new RequestResponse with the given parameters.
     *
     * @param gameId The id of the game.
     * @param reqId The id of the request.
     * @param success The success state of the request.
     * @param stateJson The serialised game state.
     * @param err Any error message.
     */
    RequestResponse(std::string gameId, std::string reqId, bool success, rapidjson::Value* stateJson, std::string err);
    ~RequestResponse();

    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static RequestResponse* fromJson(const rapidjson::Value& json);

#ifdef MONOPOLY_CLIENT
    virtual Game * Process() const override;
#endif
};


#endif //MONOPOLY_REQUEST_RESPONSE_H
