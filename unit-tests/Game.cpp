#include "gtest/gtest.h"
#include "../src/common/game_state/Game.h"
#include "../src/common/serialization/json_utils.h"
#include "../src/common/exceptions/MonopolyException.hpp"

class GameTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        // game = new Game();
        // // add two dummy players
        // game->_players.push_back(new Player("1","player1",500,500,false,false,new std::vector<int>(),4,0,0,3,0));
        // game->_players.push_back(new Player("2","player2",1000,1000,false,false,new std::vector<int>(),6,0,0,5,0));
    }

    /* Any object and subroutine declared here can be accessed in the tests */

    Game *game;
    std::string err;
};

// 
TEST_F(GameTest, BuyPropertyTest)
{   
    
    EXPECT_EQ(1, 1);
}
