#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <memory>
#include <QDateTime>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "board.h"
#include "player.h"
#include "humanplayer.h"
#include "ai.h"
#include "gametypes.h"

class Game : public QObject
{
    Q_OBJECT
public:
    struct GameRecord {
        QDateTime timestamp;
        QString player1;
        QString player2;
        QString winner;
        QString moves;
        QString username;
    };

    explicit Game(QObject* parent = nullptr);
    ~Game();

    void startNewGame(GameMode mode, const QString& player1Name, 
                     const QString& player2Name = "AI",
                     AIDifficulty difficulty = AIDifficulty::Hard);
    bool makeMove(int row, int col);
    bool isGameOver() const;
    CellState getWinner() const;
    Board* getBoard() const;
    QString getCurrentPlayerName() const;
    const QList<GameRecord>& getGameHistory(const QString& username) const;
    void saveGameHistory(const QString& username);
    void setUsername(const QString& username);

signals:
    void gameUpdated();
    void gameOver(CellState winner);

private slots:
    void handleAIMove();

private:
    QString serializeBoard() const;
    void checkGameState();
    void switchPlayer();
    void aiMove();

    Board* board;
    GameMode currentMode;
    AIDifficulty currentDifficulty;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    Player* currentPlayer;
    bool gameActive;
    QList<GameRecord> gameHistory;
    QString username;
};

#endif // GAME_H