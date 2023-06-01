#include "Player.hpp"

// Desirilization Constructor
Player::Player(std::string id, std::string playerName, int balance,
               int netWorth, bool outOfGame, bool isBankrupt, std::vector<int> *propertiesOwnedIds,
               int position, int numOwnedBuildings, int numGetOutOfJailFreeCardOwned, int lastRolledDice, int turnsLeftInJail)
    : unique_serializable(id),
      _playerName(playerName),
      _balance(balance),
      _netWorth(netWorth),
      _outOfGame(outOfGame),
      _isBankrupt(isBankrupt),
      _position(position),
      _numOwnedBuildings(numOwnedBuildings),
      _numGetOutOfJailFreeCardOwned(numGetOutOfJailFreeCardOwned),
      _propertiesOwnedIds(new std::vector<int>(*propertiesOwnedIds)),
      _lastRolledDice(lastRolledDice),
      _turnsLeftInJail(turnsLeftInJail)
{
}

Player::Player(std::string name) : unique_serializable(),
                                   _playerName(name),
                                   _balance(PLAYER_INIT_BALANCE),
                                   _netWorth(PLAYER_INIT_BALANCE),
                                   _outOfGame(false),
                                   _isBankrupt(false),
                                   _position(0),
                                   _numOwnedBuildings(0),
                                   _turnsLeftInJail(0),
                                   _numGetOutOfJailFreeCardOwned(0),
                                   _propertiesOwnedIds(new std::vector<int>()),
                                   _lastRolledDice(0)
{
}

Player::~Player()
{
    delete _propertiesOwnedIds;
}

int Player::getPosition() const noexcept { return this->_position; }

#ifdef MONOPOLY_SERVER
Player::Player(std::string id, std::string name)
    : unique_serializable(id),
      _playerName(name),
      _balance(PLAYER_INIT_BALANCE),
      _netWorth(PLAYER_INIT_BALANCE),
      _outOfGame(false),
      _isBankrupt(false),
      _position(0),
      _turnsLeftInJail(0),
      _numGetOutOfJailFreeCardOwned(0),
      _numOwnedBuildings(0),
      _propertiesOwnedIds(new std::vector<int>()),
      _lastRolledDice(0)
{
}

std::string Player::getGameId() const { return this->_gameId; }
void Player::setGameId(std::string gameId) { this->_gameId = gameId; }

bool Player::isAlreadyRolled() const noexcept { return this->_isAlreadyRolled; }



void Player::setBalance(int newBalance) { this->_balance = newBalance; }
void Player::addOwnedProperties(int newCellID)
{
    this->_propertiesOwnedIds->push_back(newCellID);
    sort(this->_propertiesOwnedIds->begin(), this->_propertiesOwnedIds->end());
}
void Player::reduceOwnedProperties(int cellID)
{
    this->_propertiesOwnedIds->erase(std::remove(this->_propertiesOwnedIds->begin(), this->_propertiesOwnedIds->end(), cellID), this->_propertiesOwnedIds->end());
    sort(this->_propertiesOwnedIds->begin(), this->_propertiesOwnedIds->end());
}

void Player::setOutOfGame(bool value) { this->_outOfGame = value; }
void Player::setBankrupt(bool value) { this->_isBankrupt = value; }
void Player::setPosition(int targetCellID) { this->_position = targetCellID; }
void Player::setTurnsLeftInJail(int value) { this->_turnsLeftInJail = value; }
void Player::setNumGetOutOfJailFreeCard(int value) { this->_numGetOutOfJailFreeCardOwned = value; }
void Player::setNumOwnedBuildings(int value) { this->_numOwnedBuildings = value; }
void Player::setNetWorth(int value) { this->_netWorth = value; }
void Player::setLastRolledDice(int value) { this->_lastRolledDice = value; }
void Player::setAlreadyRolled(bool value) { this->_isAlreadyRolled = value; }
void Player::setOwnChanceGetOutOfJail(bool value) { this->_ownChanceGetOutJailFree = value; }
void Player::setOwnCommunityChestGetOutOfJail(bool value) { this->_ownCommunityChestGetOutJailFree = value; }
#endif

int Player::getBalance() const noexcept
{
    return this->_balance;
}
std::string Player::getName() const noexcept { return this->_playerName; }
int Player::getNetWorth() const noexcept { return this->_netWorth; }
int Player::getTurnsLeftInJail() const noexcept { return this->_turnsLeftInJail; }
bool Player::isOutOfGame() const noexcept { return this->_outOfGame; }
bool Player::isBankrupt() const noexcept { return this->_isBankrupt; }
int Player::getNumGetOutOfJailFreeCard() const noexcept { return this->_numGetOutOfJailFreeCardOwned; }
int Player::getLastRolledDice() const noexcept { return this->_lastRolledDice; }

std::vector<int> *Player::getPropertiesOwnedIds() const noexcept { return this->_propertiesOwnedIds; }

int Player::getNumOwnedBuildings() const noexcept { return this->_numOwnedBuildings; }

Player *Player::fromJson(const rapidjson::Value &json)
{
    std::string id = json["id"].GetString();
    std::string _playerName = json["playerName"]["value"].GetString();
    bool _outOfGame = json["outOfGame"]["value"].GetBool();
    bool _isBankrupt = json["isBankrupt"]["value"].GetBool();
    int _balance = json["balance"]["value"].GetInt();
    int _netWorth = json["netWorth"]["value"].GetInt();
    int _turnsLeftInJail = json["turnsLeftInJail"]["value"].GetInt();

    std::vector<int> *_propertiesOwnedIds = new std::vector<int>();
    const rapidjson::Value &propertiesArray = json["propertiesOwnedIds"];
    for (rapidjson::SizeType i = 0; i < propertiesArray.Size(); i++)
    {
        _propertiesOwnedIds->push_back(propertiesArray[i]["value"].GetInt());
    }

    int _position = json["position"]["value"].GetInt();
    int _numOwnedBuildings = json["numOwnedBuildings"]["value"].GetInt();
    int _numGetOutOfJailFreeCardOwned = json["numGetOutOfJailFreeCardOwned"]["value"].GetInt();
    int _lastRolledDice = json["lastRolledDice"]["value"].GetInt();

    return new Player(id, _playerName, _balance, _netWorth, _outOfGame, _isBankrupt, _propertiesOwnedIds, _position, _numOwnedBuildings, _numGetOutOfJailFreeCardOwned, _lastRolledDice, _turnsLeftInJail);
}

void Player::writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const
{
    unique_serializable::writeIntoJson(json, allocator);

    // _playerName
    rapidjson::Value playerNameValue(rapidjson::kObjectType);
    serializable_value<std::string> serializablePlayerNameValue = serializable_value<std::string>(this->_playerName);
    serializablePlayerNameValue.writeIntoJson(playerNameValue, allocator);
    json.AddMember("playerName", playerNameValue, allocator);

    // _outOfGame
    rapidjson::Value outOfGameValue(rapidjson::kObjectType);
    serializable_value<bool> serializableOutOfGameValue = serializable_value<bool>(this->_outOfGame);
    serializableOutOfGameValue.writeIntoJson(outOfGameValue, allocator);
    json.AddMember("outOfGame", outOfGameValue, allocator);

    // _isBankrupt
    rapidjson::Value isBankruptValue(rapidjson::kObjectType);
    serializable_value<bool> serializableIsBankruptValue = serializable_value<bool>(this->_isBankrupt);
    serializableIsBankruptValue.writeIntoJson(isBankruptValue, allocator);
    json.AddMember("isBankrupt", isBankruptValue, allocator);

    // _balance
    rapidjson::Value balanceValue(rapidjson::kObjectType);
    serializable_value<int> serializableBalanceValue = serializable_value<int>(this->_balance);
    serializableBalanceValue.writeIntoJson(balanceValue, allocator);
    json.AddMember("balance", balanceValue, allocator);

    // _netWorth
    rapidjson::Value netWorthValue(rapidjson::kObjectType);
    serializable_value<int> serializableNetWorthValue = serializable_value<int>(this->_netWorth);
    serializableNetWorthValue.writeIntoJson(netWorthValue, allocator);
    json.AddMember("netWorth", netWorthValue, allocator);

    // _turnsLeftInJail
    rapidjson::Value turnsLeftInJailValue(rapidjson::kObjectType);
    serializable_value<int> serializableTurnsLeftInJailValue = serializable_value<int>(this->_turnsLeftInJail);
    serializableTurnsLeftInJailValue.writeIntoJson(turnsLeftInJailValue, allocator);
    json.AddMember("turnsLeftInJail", turnsLeftInJailValue, allocator);

    // _propertiesOwnedIds
    rapidjson::Value arrVal(rapidjson::kArrayType);
    for (int i = 0; i < _propertiesOwnedIds->size(); i++)
    {
        rapidjson::Value elem(rapidjson::kObjectType);
        serializable_value<int> serializableElem = serializable_value<int>(this->_propertiesOwnedIds->at(i));
        serializableElem.writeIntoJson(elem, allocator);
        arrVal.PushBack(elem, allocator);
    }
    json.AddMember("propertiesOwnedIds", arrVal, allocator);

    // _position
    rapidjson::Value positionValue(rapidjson::kObjectType);
    serializable_value<int> serializablePositionValue = serializable_value<int>(this->_position);
    serializablePositionValue.writeIntoJson(positionValue, allocator);
    json.AddMember("position", positionValue, allocator);

    // _numOwnedBuildings
    rapidjson::Value numOwnedBuildingsValue(rapidjson::kObjectType);
    serializable_value<int> serializableNumOwnedBuildingsValue = serializable_value<int>(this->_numOwnedBuildings);
    serializableNumOwnedBuildingsValue.writeIntoJson(numOwnedBuildingsValue, allocator);
    json.AddMember("numOwnedBuildings", numOwnedBuildingsValue, allocator);

    // _numGetOutOfJailFreeCardOwned
    rapidjson::Value numGetOutOfJailFreeCardOwnedValue(rapidjson::kObjectType);
    serializable_value<int> serializableNumGetOutOfJailFreeCardOwnedValue = serializable_value<int>(this->_numGetOutOfJailFreeCardOwned);
    serializableNumGetOutOfJailFreeCardOwnedValue.writeIntoJson(numGetOutOfJailFreeCardOwnedValue, allocator);
    json.AddMember("numGetOutOfJailFreeCardOwned", numGetOutOfJailFreeCardOwnedValue, allocator);


    // _lastRollDice
    rapidjson::Value lastRolledDiceValue(rapidjson::kObjectType);
    serializable_value<int> serializablelastRolledDiceValue = serializable_value<int>(this->_lastRolledDice);
    serializablelastRolledDiceValue.writeIntoJson(lastRolledDiceValue, allocator);
    json.AddMember("lastRolledDice", lastRolledDiceValue, allocator);
}


bool Player::getOwnChanceGetOutOfJail() const noexcept { return this->_ownChanceGetOutJailFree; }
bool Player::getOwnCommunityChestGetOutOfJail() const noexcept { return this->_ownCommunityChestGetOutJailFree; }


