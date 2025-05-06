#include "gtest/gtest.h"
#include "../src/game/board.h"

TEST(BoardTest, InitialState) {
    Board board;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_EQ(board.getCell(i, j), CellState::Empty);
        }
    }
    EXPECT_FALSE(board.isGameOver());
    EXPECT_FALSE(board.isDraw());
    EXPECT_EQ(board.checkWinner(), CellState::Empty);
}

TEST(BoardTest, MakeMove) {
    Board board;
    EXPECT_TRUE(board.makeMove(0, 0, CellState::X));
    EXPECT_EQ(board.getCell(0, 0), CellState::X);
    EXPECT_FALSE(board.makeMove(0, 0, CellState::O)); // Cell already occupied
    EXPECT_FALSE(board.makeMove(-1, 0, CellState::O)); // Invalid row
    EXPECT_FALSE(board.makeMove(0, 3, CellState::O)); // Invalid column
}

TEST(BoardTest, CheckWinner) {
    Board board;
    // Test row win
    board.makeMove(0, 0, CellState::X);
    board.makeMove(0, 1, CellState::X);
    board.makeMove(0, 2, CellState::X);
    EXPECT_EQ(board.checkWinner(), CellState::X);
    EXPECT_TRUE(board.isGameOver());
    
    board.reset();
    // Test column win
    board.makeMove(0, 0, CellState::O);
    board.makeMove(1, 0, CellState::O);
    board.makeMove(2, 0, CellState::O);
    EXPECT_EQ(board.checkWinner(), CellState::O);
    
    board.reset();
    // Test diagonal win
    board.makeMove(0, 0, CellState::X);
    board.makeMove(1, 1, CellState::X);
    board.makeMove(2, 2, CellState::X);
    EXPECT_EQ(board.checkWinner(), CellState::X);
    
    board.reset();
    // Test anti-diagonal win
    board.makeMove(0, 2, CellState::O);
    board.makeMove(1, 1, CellState::O);
    board.makeMove(2, 0, CellState::O);
    EXPECT_EQ(board.checkWinner(), CellState::O);
}

TEST(BoardTest, IsDraw) {
    Board board;
    // Fill board without a winner
    board.makeMove(0, 0, CellState::X);
    board.makeMove(0, 1, CellState::O);
    board.makeMove(0, 2, CellState::X);
    board.makeMove(1, 0, CellState::X);
    board.makeMove(1, 1, CellState::X);
    board.makeMove(1, 2, CellState::O);
    board.makeMove(2, 0, CellState::O);
    board.makeMove(2, 1, CellState::X);
    board.makeMove(2, 2, CellState::O);
    
    EXPECT_TRUE(board.isDraw());
    EXPECT_TRUE(board.isGameOver());
    EXPECT_EQ(board.checkWinner(), CellState::Empty);
}