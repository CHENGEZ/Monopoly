#include "Avenue.hpp"

Avenue::Avenue(const int ID, const std::string NAME, vector<std::string> playerIDsOnCell, const int PRICE, const int BASE_RENT, const int MAX_BUILDINGS, const int RENT_PER_BUILDING, const int BUILDING_PRICE, const int MORTGAGE, std::string ownerID, int numBuildings)
    : Cell(ID, NAME, playerIDsOnCell), PRICE(PRICE), BASE_RENT(BASE_RENT), MAX_BUILDINGS(MAX_BUILDINGS), RENT_PER_BUILDING(RENT_PER_BUILDING), BUILDING_PRICE(BUILDING_PRICE), MORTGAGE(MORTGAGE)
{
    this->numBuildings = numBuildings;
    this->ownerID = ownerID;
    cellType = AVENUE;
}

const int Avenue::getPrice() const { return this->PRICE; }
const int Avenue::getBuildingPrice() const { return this->BUILDING_PRICE; }
const int Avenue::getMortgage() const { return this->MORTGAGE; }
const int Avenue::getBaseRent() const { return this->BASE_RENT; }
const int Avenue::getMaxBuildings() const { return this->MAX_BUILDINGS; }
const int Avenue::getRentPerBuilding() const { return this->RENT_PER_BUILDING; }
int Avenue::getCurrentNumBuildings() const { return this->numBuildings; }
std::string Avenue::getOwnerId() const { return this->ownerID; }

int Avenue::getActualRent() const { return BASE_RENT + numBuildings * RENT_PER_BUILDING; }

void Avenue::setOwnerId(std::string newOwnerID) { this->ownerID = newOwnerID; }
void Avenue::setNumBuildings(int newNumBuildings) { this->numBuildings = newNumBuildings; }

void Avenue::writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const
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

    // MAX_BUILDINGS
    rapidjson::Value maxBuildingValue(rapidjson::kObjectType);
    serializable_value<int> serializablemaxBuildingValue = serializable_value<int>(this->MAX_BUILDINGS);
    serializablemaxBuildingValue.writeIntoJson(maxBuildingValue, allocator);
    json.AddMember("maxBuilding", maxBuildingValue, allocator);

    // RENT_PER_BUILDING
    rapidjson::Value rentPerBuildingValue(rapidjson::kObjectType);
    serializable_value<int> serializablerentPerBuildingValue = serializable_value<int>(this->RENT_PER_BUILDING);
    serializablerentPerBuildingValue.writeIntoJson(rentPerBuildingValue, allocator);
    json.AddMember("rentPerBuilding", rentPerBuildingValue, allocator);

    // BUILDING_PRICE
    rapidjson::Value buildingPriceValue(rapidjson::kObjectType);
    serializable_value<int> serializablebuildingPriceValue = serializable_value<int>(this->BUILDING_PRICE);
    serializablebuildingPriceValue.writeIntoJson(buildingPriceValue, allocator);
    json.AddMember("buildingPrice", buildingPriceValue, allocator);

    // MORTGAGE
    rapidjson::Value mortageValue(rapidjson::kObjectType);
    serializable_value<int> serializablemortageValue = serializable_value<int>(this->MORTGAGE);
    serializablemortageValue.writeIntoJson(mortageValue, allocator);
    json.AddMember("mortage", mortageValue, allocator);

    // numBuildings;
    rapidjson::Value numBuildingsValue(rapidjson::kObjectType);
    serializable_value<int> serializablenumBuildingsValue = serializable_value<int>(this->numBuildings);
    serializablenumBuildingsValue.writeIntoJson(numBuildingsValue, allocator);
    json.AddMember("numBuildings", numBuildingsValue, allocator);

    // ownerID;
    rapidjson::Value ownerIDValue(rapidjson::kObjectType);
    serializable_value<std::string> serializableownerIDValue = serializable_value<std::string>(this->ownerID);
    serializableownerIDValue.writeIntoJson(ownerIDValue, allocator);
    json.AddMember("ownerID", ownerIDValue, allocator);
}

Avenue *Avenue::fromJson(const rapidjson::Value &json)
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
    
    // Deserialize the Avenue-specific data.
    int price = json["price"]["value"].GetInt();
    int baseRent = json["baseRent"]["value"].GetInt();
    int maxBuildings = json["maxBuilding"]["value"].GetInt();
    int rentPerBuilding = json["rentPerBuilding"]["value"].GetInt();
    int buildingPrice = json["buildingPrice"]["value"].GetInt();
    int mortgage = json["mortage"]["value"].GetInt();
    int numBuildings = json["numBuildings"]["value"].GetInt();
    std::string ownerID = json["ownerID"]["value"].GetString();

    // Construct a new Avenue with the deserialized data and return a pointer to it.
    return new Avenue(id, name, playerIdsOnCell, price, baseRent, maxBuildings, rentPerBuilding, buildingPrice, mortgage, ownerID, numBuildings);
}


CellActivations Avenue::activate()
{
    std::string playerJustArrived = this->getPlayerOnCell().back();
    if (playerJustArrived == this->ownerID || this->ownerID == "")
        return DO_NOTHING;
    else
        return P2P_MONEY_AVENUE;
}
