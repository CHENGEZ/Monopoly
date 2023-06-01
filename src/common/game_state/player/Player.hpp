#include <string>
#include <vector>
#include <algorithm>

#ifndef MONOPOLY_PLAYER_H
#define MONOPOLY_PLAYER_H


#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../serialization/unique_serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../serialization/uuid_generator.h"

const int PLAYER_INIT_BALANCE = 1000;
#define DEVELOPER_TESTING 0
class Player : public unique_serializable
{
#if DEVELOPER_TESTING
public:
    std::string _playerName;
    bool _outOfGame;
    bool _isBankrupt;
    int _balance;
    int _netWorth;
    int _turnsLeftInJail;
    std::vector<int> *_propertiesOwnedIds;
    int _position;
    int _numOwnedBuildings = 0;
    int _numGetOutOfJailFreeCardOwned = 0;
    int _lastRolledDice = 0;
    bool _isAlreadyRolled = false;
    bool _ownChanceGetOutJailFree = false;
    bool _ownCommunityChestGetOutJailFree = false;

    /*
     * Deserialization constructor
     */
    Player(std::string id,
           std::string playerName,
           int balance,
           int netWorth,
           bool outOfGame,
           bool isBankrupt,
           std::vector<int> *propertiesOwnedIds, // use vector_utils for serialization, refer to hand.h/cpp for example
           int position,
           int numOwnedBuildings,
           int numGetOutOfJailFreeCardOwned,
           int lastRolledDice,
           int turnsLeftInJail);
#else
private:
    std::string _playerName;
    bool _outOfGame;
    bool _isBankrupt;
    int _balance;
    int _netWorth;
    int _turnsLeftInJail;
    std::vector<int> *_propertiesOwnedIds;
    int _position;
    int _numOwnedBuildings = 0;
    int _numGetOutOfJailFreeCardOwned = 0;
    int _lastRolledDice = 0;
    bool _isAlreadyRolled = false;
    bool _ownChanceGetOutJailFree = false;
    bool _ownCommunityChestGetOutJailFree = false;

    /*
     * Deserialization constructor
     */
    Player(std::string id,
           std::string playerName,
           int balance,
           int netWorth,
           bool outOfGame,
           bool isBankrupt,
           std::vector<int> *propertiesOwnedIds, // use vector_utils for serialization, refer to hand.h/cpp for example
           int position,
           int numOwnedBuildings,
           int numGetOutOfJailFreeCardOwned,
           int lastRolledDice,
           int turnsLeftInJail);
#endif

public:
    // constructors
    explicit Player(std::string name); // for client
    ~Player();

#ifdef MONOPOLY_SERVER
    std::string _gameId;
    Player(std::string id, std::string name); // for server

    std::string getGameId() const;
    void setGameId(std::string gameId);

    // mutators
    void setBalance(int newBalance);
    void addOwnedProperties(int newCellID);
    void reduceOwnedProperties(int cellID);
    void setOutOfGame(bool value);
    void setBankrupt(bool value);
    void setPosition(int targetCellID);
    void setTurnsLeftInJail(int value);
    void setNumGetOutOfJailFreeCard(int value);
    void setNumOwnedBuildings(int value);
    void setNetWorth(int value);
    void setLastRolledDice(int value);
    void setAlreadyRolled(bool value);
    void setOwnChanceGetOutOfJail(bool value);
    void setOwnCommunityChestGetOutOfJail(bool value);
#endif

    // accessors
    bool getOwnChanceGetOutOfJail() const noexcept;
    bool getOwnCommunityChestGetOutOfJail() const noexcept;
    bool isAlreadyRolled() const noexcept;
    int getPosition() const noexcept;
    std::string getName() const noexcept;
    int getBalance() const noexcept;
    int getNetWorth() const noexcept;
    int getTurnsLeftInJail() const noexcept;
    bool isOutOfGame() const noexcept;
    bool isBankrupt() const noexcept;
    int getNumGetOutOfJailFreeCard() const noexcept;
    std::vector<int> *getPropertiesOwnedIds() const noexcept;
    int getNumOwnedBuildings() const noexcept;
    int getLastRolledDice() const noexcept;

    // serialization
    static Player *fromJson(const rapidjson::Value &json);
    virtual void writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const override;
};

#endif
