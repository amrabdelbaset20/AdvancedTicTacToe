#include "humanplayer.h"

HumanPlayer::HumanPlayer(const QString& name, CellState symbol) : 
    Player(name, symbol) {}

bool HumanPlayer::isHuman() const {
    return true;
}

void HumanPlayer::makeMove(Board& board) {
    // Handled through UI
}