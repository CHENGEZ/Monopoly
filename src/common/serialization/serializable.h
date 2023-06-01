//
// Created by Manuel on 01.02.2021.
//
// abstract class for a serializable object.

#ifndef LAMA_SERIALIZABLE_H
#define LAMA_SERIALIZABLE_H

#include "../../../rapidjson/include/rapidjson/document.h"

class serializable {
public:

    virtual rapidjson::Document* toJson() const {
        rapidjson::Document* json = new rapidjson::Document();
        json->SetObject();
        this->writeIntoJson(*json, json->GetAllocator());
        return json;
    }

    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const = 0;
};

#endif //LAMA_SERIALIZABLE_H
