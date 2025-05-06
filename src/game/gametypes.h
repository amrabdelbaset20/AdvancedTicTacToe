#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <QObject>

class GameTypes : public QObject
{
    Q_OBJECT
public:
    enum CellState { Empty, X, O };
    enum GameMode { PlayerVsPlayer, PlayerVsAI };
    enum AIDifficulty { Easy, Medium, Hard };
    
    Q_ENUM(CellState)
    Q_ENUM(GameMode)
    Q_ENUM(AIDifficulty)

    explicit GameTypes(QObject* parent = nullptr) : QObject(parent) {}

    // Add this static method if you need type registration
    static void registerTypes() {
        qRegisterMetaType<CellState>("CellState");
        qRegisterMetaType<GameMode>("GameMode");
        qRegisterMetaType<AIDifficulty>("AIDifficulty");
    }
};

typedef GameTypes::CellState CellState;
typedef GameTypes::GameMode GameMode;
typedef GameTypes::AIDifficulty AIDifficulty;

#endif // GAMETYPES_H