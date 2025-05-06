#include "ai.h"
#include "board.h"
#include <algorithm>
#include <limits>
#include <random>
#include <chrono>

AI::AI(CellState symbol, AIDifficulty difficulty) :
    Player("AI", symbol), difficulty(difficulty) {
}

bool AI::isHuman() const {
    return false;
}

void AI::makeMove(Board& board) {
    switch (difficulty) {
    case AIDifficulty::Easy:
        if (rand() % 2 == 0) {
            makeRandomMove(board);
            return;
        }
        // fall through
    case AIDifficulty::Medium:
        if (rand() % 4 == 0) {
            makeRandomMove(board);
            return;
        }
        // fall through
    case AIDifficulty::Hard:
        int bestScore = std::numeric_limits<int>::min();
        int bestRow = -1, bestCol = -1;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board.getCell(i, j) == CellState::Empty) {
                    Board tempBoard;
                    tempBoard.reset();
                    for (int x = 0; x < 3; x++) {
                        for (int y = 0; y < 3; y++) {
                            tempBoard.makeMove(x, y, board.getCell(x, y));
                        }
                    }

                    tempBoard.makeMove(i, j, this->getSymbol());
                    int score = minimax(tempBoard, 0, false,
                        std::numeric_limits<int>::min(),
                        std::numeric_limits<int>::max());

                    if (score > bestScore) {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }

        if (bestRow != -1 && bestCol != -1) {
            board.makeMove(bestRow, bestCol, this->getSymbol());
        }
        break;
    }
}

void AI::makeRandomMove(Board& board) {
    std::vector<std::pair<int, int>> availableMoves;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board.getCell(i, j) == CellState::Empty) {
                availableMoves.emplace_back(i, j);
            }
        }
    }

    if (!availableMoves.empty()) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(availableMoves.begin(), availableMoves.end(), std::default_random_engine(seed));
        auto move = availableMoves.front();
        board.makeMove(move.first, move.second, this->getSymbol());
    }
}

int AI::evaluate(const Board& board) const {
    CellState winner = board.checkWinner();
    if (winner == this->getSymbol()) return 10;
    if (winner != CellState::Empty) return -10;
    return 0;
}

int AI::minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta) {
    int score = evaluate(board);
    if (score == 10 || score == -10 || board.isDraw()) {
        return score;
    }

    if (isMaximizing) {
        int best = std::numeric_limits<int>::min();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board.getCell(i, j) == CellState::Empty) {
                    Board tempBoard;
                    tempBoard.reset();
                    for (int x = 0; x < 3; x++) {
                        for (int y = 0; y < 3; y++) {
                            tempBoard.makeMove(x, y, board.getCell(x, y));
                        }
                    }
                    tempBoard.makeMove(i, j, this->getSymbol());
                    best = std::max(best, minimax(tempBoard, depth + 1, !isMaximizing, alpha, beta));
                    alpha = std::max(alpha, best);
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    }
    else {
        int best = std::numeric_limits<int>::max();
        CellState opponent = (this->getSymbol() == CellState::X) ? CellState::O : CellState::X;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board.getCell(i, j) == CellState::Empty) {
                    Board tempBoard;
                    tempBoard.reset();
                    for (int x = 0; x < 3; x++) {
                        for (int y = 0; y < 3; y++) {
                            tempBoard.makeMove(x, y, board.getCell(x, y));
                        }
                    }
                    tempBoard.makeMove(i, j, opponent);
                    best = std::min(best, minimax(tempBoard, depth + 1, !isMaximizing, alpha, beta));
                    beta = std::min(beta, best);
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    }
}