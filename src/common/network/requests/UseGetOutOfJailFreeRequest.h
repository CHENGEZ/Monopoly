//
// Created by Manuel on 29.01.2021.
//

#ifndef MONOPOLY_USE_GET_OUT_OF_JAIL_FREE_REQUEST_H
#define MONOPOLY_USE_GET_OUT_OF_JAIL_FREE_REQUEST_H


#include <string>
#include "ClientRequest.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class UseGetOutOfJailFreeRequest : public ClientRequest{

private:

    /*
     * Private constructor for deserialization
     */
    explicit UseGetOutOfJailFreeRequest(BaseClassProperties);

public:
    UseGetOutOfJailFreeRequest(std::string game_id, std::string player_id);
    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static UseGetOutOfJailFreeRequest* fromJson(const rapidjson::Value& json);
};

#endif //MONOPOLY_USE_GET_OUT_OF_JAIL_FREE_REQUEST_H