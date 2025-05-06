#include "board.h"

Board::Board(QObject *parent) : QObject(parent)
{
    reset();
}

bool Board::makeMove(int row, int col, CellState player)
{
    if (row < 0 || row >= 3 || col < 0 || col >= 3 || grid[row][col] != CellState::Empty) {
        return false;
    }
    
    grid[row][col] = player;
    emit boardChanged();
    return true;
}

CellState Board::checkWinner() const
{
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (grid[i][0] != CellState::Empty && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2]) {
            return grid[i][0];
        }
    }
    
    // Check columns
    for (int j = 0; j < 3; ++j) {
        if (grid[0][j] != CellState::Empty && grid[0][j] == grid[1][j] && grid[1][j] == grid[2][j]) {
            return grid[0][j];
        }
    }
    
    // Check diagonals
    if (grid[0][0] != CellState::Empty && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) {
        return grid[0][0];
    }
    if (grid[0][2] != CellState::Empty && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) {
        return grid[0][2];
    }
    
    return CellState::Empty;
}

bool Board::isDraw() const
{
    if (checkWinner() != CellState::Empty) return false;
    
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            if (cell == CellState::Empty) return false;
        }
    }
    return true;
}

bool Board::isGameOver() const
{
    return checkWinner() != CellState::Empty || isDraw();
}

void Board::reset()
{
    grid = std::vector<std::vector<CellState>>(3, std::vector<CellState>(3, CellState::Empty));
    emit boardChanged();
}

CellState Board::getCell(int row, int col) const
{
    if (row < 0 || row >= 3 || col < 0 || col >= 3) {
        return CellState::Empty;
    }
    return grid[row][col];
}

const std::vector<std::vector<CellState>>& Board::getGrid() const
{
    return grid;
}