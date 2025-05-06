#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.h"

class HumanPlayer : public Player
{
public:
    HumanPlayer(const QString& name, CellState symbol);
    
    bool isHuman() const override;
    void makeMove(Board& board) override;
};

#endif // HUMANPLAYER_H