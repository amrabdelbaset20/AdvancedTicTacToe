#include "player.h"

Player::Player(const QString& name, CellState symbol) : 
    name(name), symbol(symbol) {}

QString Player::getName() const {
    return name;
}

CellState Player::getSymbol() const {
    return symbol;
}