#ifndef AI_H
#define AI_H

#include "player.h"
#include "gametypes.h"

// Forward declaration
class Board;

class AI : public Player
{
public:
    AI(CellState symbol, AIDifficulty difficulty = AIDifficulty::Hard);

    bool isHuman() const override;
    void makeMove(Board& board) override;

protected:
    int evaluate(const Board& board) const;
    int minimax(Board& board, int depth, bool isMaximizing, int alpha, int beta);
    void makeRandomMove(Board& board);

private:
    AIDifficulty difficulty;
};

#endif // AI_H