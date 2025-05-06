#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "gametypes.h"

// Forward declaration
class Board;

class Player
{
public:
    Player(const QString& name, CellState symbol);
    virtual ~Player() = default;
    
    QString getName() const;
    CellState getSymbol() const;
    
    virtual bool isHuman() const = 0;
    virtual void makeMove(Board& board) = 0;

protected:
    QString name;
    CellState symbol;
};

#endif // PLAYER_H