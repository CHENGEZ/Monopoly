#include "Railway.hpp"

// Railway::Railway(const std::string NAME, const int id, const int PRICE, const int MORTGAGE, const int BASE_RENT) : Cell(NAME, id), PRICE(PRICE), MORTGAGE(MORTGAGE), BASE_RENT(BASE_RENT) {}

Railway::Railway(const int ID, const std::string NAME, vector<std::string> playerIDsOnCell, const int PRICE, const int MORTGAGE, const int BASE_RENT, string ownerID)
    : Cell(ID, NAME, playerIDsOnCell), PRICE(PRICE), MORTGAGE(MORTGAGE), BASE_RENT(BASE_RENT)
{
    this->ownerID = ownerID;
    cellType = RAILWAY;
}

CellActivations Railway::activate()
{
    std::string playerJustArrived = this->getPlayerOnCell().back();
    if (playerJustArrived == this->ownerID || this->ownerID == "")
        return DO_NOTHING;
    else
        return P2P_MONEY_RAILWAY;
}

const int Railway::getPrice() const { return this->PRICE; }
const int Railway::getMortgage() const { return this->MORTGAGE; }
const int Railway::getBaseRent() const { return this->BASE_RENT; }
std::string Railway::getOwnerId() const { return this->ownerID; }

void Railway::setOwnerId(std::string newOwnerID) { this->ownerID = newOwnerID; }

void Railway::writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const
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

    // BASE_RENT
    rapidjson::Value baseRentValue(rapidjson::kObjectType);
    serializable_value<int> serializablebaseRentValue = serializable_value<int>(this->BASE_RENT);
    serializablebaseRentValue.writeIntoJson(baseRentValue, allocator);
    json.AddMember("baseRent", baseRentValue, allocator);

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

Railway *Railway::fromJson(const rapidjson::Value &json)
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

    // Deserialize the Railway-specific data.
    int price = json["price"]["value"].GetInt();
    int baseRent = json["baseRent"]["value"].GetInt();
    int mortgage = json["mortage"]["value"].GetInt();
    std::string ownerID = json["ownerID"]["value"].GetString();

    // Construct a new Railway with the deserialized data and return a pointer to it.
    return new Railway(id, name, playerIdsOnCell, price, baseRent, mortgage, ownerID);
}

