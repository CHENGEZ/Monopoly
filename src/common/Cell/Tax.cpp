#include "Tax.hpp"

Tax::Tax(const int ID, const string NAME, vector<std::string> playerIDsOnCell, const int TAX_AMOUNT)
    : Cell(ID, NAME, playerIDsOnCell), TAX_AMOUNT(TAX_AMOUNT)
{
    cellType = TAX;
}

CellActivations Tax::activate()
{
    return P2C_MONEY_TAX;
}

const int Tax::getTaxAmount() const { return this->TAX_AMOUNT; }

void Tax::writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const
{
    // NAME
    rapidjson::Value nameValue(rapidjson::kObjectType);
    serializable_value<std::string> serializableNameValue = serializable_value<std::string>(this->NAME);
    serializableNameValue.writeIntoJson(nameValue, allocator);
    json.AddMember("name", nameValue, allocator);

    // id
    rapidjson::Value idValue(rapidjson::kObjectType);
    serializable_value<int> serializableIDValue = serializable_value<int>(this->ID);
    serializableIDValue.writeIntoJson(idValue, allocator);
    json.AddMember("id", idValue, allocator);

    // playerIDsOnCell
    rapidjson::Value arrVal(rapidjson::kArrayType);
    for (int i = 0; i < playerIDsOnCell.size(); i++)
    {
        rapidjson::Value elem(rapidjson::kObjectType);
        serializable_value<std::string> serializableElem = serializable_value<std::string>(this->playerIDsOnCell.at(i));
        serializableElem.writeIntoJson(elem, allocator);
        arrVal.PushBack(elem, allocator);
    }
    json.AddMember("playerIDsOnCell", arrVal, allocator);

    // CellType
    rapidjson::Value cellTypeValue(rapidjson::kObjectType);
    int ct = this->cellType;
    serializable_value<int> serializableCellTypeValue = serializable_value<int>(ct);
    serializableCellTypeValue.writeIntoJson(cellTypeValue, allocator);
    json.AddMember("CellType", cellTypeValue, allocator);

    // TAX_AMOUNT
    rapidjson::Value taxAmountValue(rapidjson::kObjectType);
    serializable_value<int> serializabletaxAmountValue = serializable_value<int>(this->TAX_AMOUNT);
    serializabletaxAmountValue.writeIntoJson(taxAmountValue, allocator);
    json.AddMember("taxAmount", taxAmountValue, allocator);
}

Tax *Tax::fromJson(const rapidjson::Value &json)
{
    // Deserialize the base Cell data.
    std::string name = json["name"]["value"].GetString();
    int id = json["id"]["value"].GetInt();
    std::vector<std::string> playerIdsOnCell;
    const auto& playerIdsJsonArray = json["playerIDsOnCell"].GetArray();
    for (const auto& playerIdJson : playerIdsJsonArray)
    {
        playerIdsOnCell.push_back(playerIdJson["value"].GetString());
    }

    // Deserialize the Tax-specific data.
    int taxAmount = json["taxAmount"]["value"].GetInt();

    // Construct a new Tax with the deserialized data and return a pointer to it.
    return new Tax(id, name, playerIdsOnCell, taxAmount);
}

