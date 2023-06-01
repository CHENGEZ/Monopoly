#include "Utility.hpp"

Utility::Utility(const int ID, const std::string NAME, vector<std::string> playerIDsOnCell, 
                const int PRICE, const int MORTGAGE, string ownerID)
    : Cell(ID, NAME, playerIDsOnCell), PRICE(PRICE), MORTGAGE(MORTGAGE)
{
    this->ownerID = ownerID;
    cellType = UTILITY;
}

CellActivations Utility::activate()
{
    std::string playerJustArrived = this->getPlayerOnCell().back();
    if (playerJustArrived == this->ownerID || this->ownerID == "")
        return DO_NOTHING;
    else
        return P2P_MONEY_UTILITY;
}

const int Utility::getPrice() const { return this->PRICE; }
const int Utility::getMortgage() const { return this->MORTGAGE; }
std::string Utility::getOwnerId() const { return this->ownerID; }

void Utility::setOwnerId(std::string newOwnerID) { this->ownerID = newOwnerID; }

void Utility::writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const
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

    // PRICE
    rapidjson::Value priceValue(rapidjson::kObjectType);
    serializable_value<int> serializablepriceValue = serializable_value<int>(this->PRICE);
    serializablepriceValue.writeIntoJson(priceValue, allocator);
    json.AddMember("price", priceValue, allocator);

    // MORTGAGE
    rapidjson::Value mortageValue(rapidjson::kObjectType);
    serializable_value<int> serializablemortageValue = serializable_value<int>(this->MORTGAGE);
    serializablemortageValue.writeIntoJson(mortageValue, allocator);
    json.AddMember("mortage", mortageValue, allocator);

    // ownerID;
    rapidjson::Value ownerIDValue(rapidjson::kObjectType);
    serializable_value<std::string> serializableownerIDValue = serializable_value<std::string>(this->ownerID);
    serializableownerIDValue.writeIntoJson(ownerIDValue, allocator);
    json.AddMember("ownerID", ownerIDValue, allocator);
}

Utility *Utility::fromJson(const rapidjson::Value &json)
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

    // Deserialize the Utility-specific data.
    int price = json["price"]["value"].GetInt();
    int mortgage = json["mortage"]["value"].GetInt();
    std::string ownerId = json["ownerID"]["value"].GetString();

    // Construct a new Utility with the deserialized data and return a pointer to it.
    return new Utility(id, name, playerIdsOnCell, price, mortgage, ownerId);
}

