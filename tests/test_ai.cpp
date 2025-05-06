#include "gtest/gtest.h"
#include "../src/game/ai.h"
#include "../src/game/board.h"

TEST(AITest, EasyDifficultyMakesMove) {
    Board board;
    AI ai(CellState::O, AIDifficulty::Easy);
    
    ai.makeMove(board);
    bool moveMade = false;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board.getCell(i, j) == CellState::O) {
                moveMade = true;
                break;
            }
        }
    }
    EXPECT_TRUE(moveMade);
}

TEST(AITest, HardAICanWin) {
    Board board;
    AI ai(CellState::O, AIDifficulty::Hard);
    
    // Setup a board where O can win in one move
    board.makeMove(0, 0, CellState::O);
    board.makeMove(0, 1, CellState::O);
    board.makeMove(1, 0, CellState::X);
    board.makeMove(1, 1, CellState::X);
    
    ai.makeMove(board);
    EXPECT_EQ(board.getCell(0, 2), CellState::O);
    EXPECT_EQ(board.checkWinner(), CellState::O);
}

TEST(AITest, HardAICanBlock) {
    Board board;
    AI ai(CellState::O, AIDifficulty::Hard);
    
    // Setup a board where X can win in one move
    board.makeMove(0, 0, CellState::X);
    board.makeMove(0, 1, CellState::X);
    board.makeMove(1, 0, CellState::O);
    board.makeMove(1, 1, CellState::O);
    
    ai.makeMove(board);
    EXPECT_EQ(board.getCell(0, 2), CellState::O);
    EXPECT_EQ(board.checkWinner(), CellState::Empty);
}