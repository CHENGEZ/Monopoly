//
// Created by Manuel on 15.02.2021.
//
// Base class for all messages sent from the server to the client.
// It offers a function to deserialize a ServerResponse subclass from a valid json.

#ifndef MONOPOLY_SERVER_RESPONSE_H
#define MONOPOLY_SERVER_RESPONSE_H

#include <string>
#include <unordered_map>

#include "../../serialization/serializable.h"
#include "../../game_state/Game.h"

// Identifier for the different response types.
// The ResponseType is sent with every ServerResponse to identify the type of ServerResponse
// during deserialization on the client side.
enum ResponseType {
    REQ_RESPONSE,
    STATE_DIFF_MSG,
    FULL_STATE_MSG
};

class ServerResponse : public serializable {
private:

    // for deserialization
    static const std::unordered_map<std::string, ResponseType> _stringToResponseType;
    // for serialization
    static const std::unordered_map<ResponseType, std::string> _responseTypeToString;

protected:
    std::string _gameId;
    ResponseType _type;

    struct BaseClassProperties {
        std::string gameId;
        ResponseType type;
    };

    explicit ServerResponse(BaseClassProperties); // base constructor
    static BaseClassProperties createBaseClassProperties(ResponseType type, const std::string& gameId);
    static BaseClassProperties extractBaseClassProperties(const rapidjson::Value& json);

public:
    ResponseType getType() const;
    std::string getGameId() const;

    // Tries to create the specific ServerResponse from the provided json.
    // Throws exception if parsing fails -> Use only inside "try{ }catch()" block
    static ServerResponse* fromJson(const rapidjson::Value& json);

    // Serializes the ServerResponse into a json object that can be sent over the network
    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

#ifdef MONOPOLY_CLIENT
    virtual Game * Process() const = 0;
#endif
};


#endif //MONOPOLY_SERVER_RESPONSE_H
