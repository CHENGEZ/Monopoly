//
// Created by Manuel on 29.01.2021.
//

#ifndef MONOPOLY_START_GAME_REQUEST_H
#define MONOPOLY_START_GAME_REQUEST_H


#include <string>
#include "ClientRequest.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class StartGameRequest : public ClientRequest{

private:

    /*
     * Private constructor for deserialization
     */
    explicit StartGameRequest(BaseClassProperties);

public:
    StartGameRequest(std::string game_id, std::string player_id);
    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static StartGameRequest* fromJson(const rapidjson::Value& json);
};

#endif //MONOPOLY_START_GAME_REQUEST_H