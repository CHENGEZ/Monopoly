#include "Cell.hpp"
#include "Avenue.hpp"
#include "EventTriggerer.hpp"
#include "Go2Jail.hpp"
#include "Jail.hpp"
#include "GoAndFreeParking.hpp"
#include "Railway.hpp"
#include "Tax.hpp"
#include "Utility.hpp"

void Cell::playerArrive(Player *player)
{
    std::string idOfPlayer = player->getId();
    this->playerIDsOnCell.push_back(idOfPlayer);
}

void Cell::playerLeave(Player *player)
{
    std::string idOfPlayer = player->getId();
    this->playerIDsOnCell.erase(std::remove(this->playerIDsOnCell.begin(), this->playerIDsOnCell.end(), idOfPlayer), this->playerIDsOnCell.end());
}

Cell::Cell(const int ID, const string NAME, std::vector<std::string> playerIDsOnCell) : playerIDsOnCell(playerIDsOnCell),
                                                                                        ID(ID),
                                                                                        NAME(NAME)
{
}

const int Cell::getId() const
{
    return this->ID;
}

vector<std::string> Cell::getPlayerOnCell() const
{
    return this->playerIDsOnCell;
}

const string Cell::getName() const
{
    return this->NAME;
}

const int Cell::getPrice() const
{
    return 0;
}

const int Cell::getBuildingPrice() const
{
    return 0;
}

const int Cell::getMortgage() const
{
    return 0;
}

const int Cell::getBaseRent() const
{
    return 0;
}

int Cell::getActualRent() const
{
    return 0;
}

const int Cell::getMaxBuildings() const
{
    return 0;
}

const int Cell::getRentPerBuilding() const
{
    return 0;
}

int Cell::getCurrentNumBuildings() const
{
    return 0;
}

std::string Cell::getOwnerId() const
{
    return "";
}

vector<Event *> *Cell::getEventDatabase() const
{
    return nullptr;
}

const int Cell::getRoundToGo() const
{
    return 0;
}

const int Cell::getTaxAmount() const
{
    return 0;
}

void Cell::setOwnerId(std::string newOwnerID)
{
}

void Cell::setNumBuildings(int newNumBuildings)
{
}

Cell::~Cell() {}

Cell *Cell::fromJson(const rapidjson::Value &json)
{
    std::string name = json["name"]["value"].GetString();
    int id = json["id"]["value"].GetInt();
    std::vector<std::string> playerIdsOnCell;
    const auto& playerIdsJsonArray = json["playerIDsOnCell"].GetArray();
    for (const auto& playerIdJson : playerIdsJsonArray)
    {
        playerIdsOnCell.push_back(playerIdJson["value"].GetString());
    }
    Cell::CellType cellType = static_cast<Cell::CellType>(json["CellType"]["value"].GetInt());
    switch (cellType) {
    case Cell::AVENUE:
        return Avenue::fromJson(json);
    case Cell::RAILWAY:
        return Railway::fromJson(json);
    case Cell::TAX:
        return Tax::fromJson(json);
    case Cell::UTILITY:
        return Utility::fromJson(json);
    case Cell::GO_2_JAIL:
        return new Go2Jail(id, name, playerIdsOnCell);
    case Cell::JAIL:
        return new Jail(id, name, playerIdsOnCell);
    case Cell::GO_AND_FREEPARKING:
        return new GoAndFreeParking(id, name, playerIdsOnCell);
    case Cell::CHANCE:
        return new Chance(id, name, playerIdsOnCell);
    case Cell::COMMUNITY_CHEST:
        return new CommunityChest(id, name, playerIdsOnCell);
    default:
        return nullptr;
    }
}

void Cell::writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const
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

}
