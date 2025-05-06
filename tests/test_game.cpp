#include "gtest/gtest.h"
#include "../src/game/game.h"

class GameTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
    }

    std::unique_ptr<Game> game;
};

TEST_F(GameTest, Initialization) {
    EXPECT_FALSE(game->isGameOver());
    EXPECT_EQ(game->getWinner(), CellState::Empty);
    EXPECT_TRUE(game->getBoard() != nullptr);
}

TEST_F(GameTest, PlayerVsPlayerGame) {
    game->startNewGame(GameMode::PlayerVsPlayer, "Player1", "Player2");
    
    // Player 1 (X) moves
    EXPECT_TRUE(game->makeMove(0, 0));
    EXPECT_EQ(game->getBoard()->getCell(0, 0), CellState::X);
    
    // Player 2 (O) moves
    EXPECT_TRUE(game->makeMove(1, 1));
    EXPECT_EQ(game->getBoard()->getCell(1, 1), CellState::O);
    
    // Invalid move
    EXPECT_FALSE(game->makeMove(0, 0)); // Cell already occupied
    EXPECT_FALSE(game->makeMove(-1, 0)); // Invalid position
}

TEST_F(GameTest, GameOverDetection) {
    game->startNewGame(GameMode::PlayerVsPlayer, "Player1", "Player2");
    
    // X wins
    game->makeMove(0, 0); // X
    game->makeMove(1, 0); // O
    game->makeMove(0, 1); // X
    game->makeMove(1, 1); // O
    game->makeMove(0, 2); // X
    
    EXPECT_TRUE(game->isGameOver());
    EXPECT_EQ(game->getWinner(), CellState::X);
}