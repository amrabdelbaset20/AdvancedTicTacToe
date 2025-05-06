#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <vector>
#include "gametypes.h"

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);
    
    bool makeMove(int row, int col, CellState player);
    CellState checkWinner() const;
    bool isDraw() const;
    bool isGameOver() const;
    void reset();
    
    CellState getCell(int row, int col) const;
    const std::vector<std::vector<CellState>>& getGrid() const;

signals:
    void boardChanged();

private:
    std::vector<std::vector<CellState>> grid;
};

#endif // BOARD_H