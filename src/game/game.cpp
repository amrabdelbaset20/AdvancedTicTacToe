#include "game.h"
#include <QTimer>

Game::Game(QObject* parent) : QObject(parent),
    board(new Board(this)),
    currentPlayer(nullptr),
    gameActive(false)
{
    connect(board, &Board::boardChanged, this, &Game::gameUpdated);
}

Game::~Game()
{
    disconnect(board, nullptr, this, nullptr);
}

void Game::startNewGame(GameMode mode, const QString& player1Name, 
                       const QString& player2Name, AIDifficulty difficulty)
{
    board->reset();
    currentMode = mode;
    currentDifficulty = difficulty;
    gameActive = true;

    player1 = std::make_unique<HumanPlayer>(player1Name, CellState::X);
    
    if (mode == GameMode::PlayerVsPlayer) {
        player2 = std::make_unique<HumanPlayer>(player2Name, CellState::O);
    } else {
        player2 = std::make_unique<AI>(CellState::O, difficulty);
    }

    currentPlayer = player1.get();
    emit gameUpdated();
}

bool Game::makeMove(int row, int col)
{
    if (!gameActive || row < 0 || row >= 3 || col < 0 || col >= 3) {
        return false;
    }

    if (!board->makeMove(row, col, currentPlayer->getSymbol())) {
        return false;
    }

    checkGameState();
    return true;
}

void Game::checkGameState()
{
    if (board->isGameOver()) {
        gameActive = false;
        saveGameHistory(username);
        emit gameOver(board->checkWinner());
    } else {
        switchPlayer();
        emit gameUpdated();

        if (!currentPlayer->isHuman() && gameActive) {
            QTimer::singleShot(500, this, &Game::handleAIMove);
        }
    }
}

void Game::handleAIMove()
{
    if (gameActive && !currentPlayer->isHuman()) {
        aiMove();
    }
}

void Game::aiMove()
{
    dynamic_cast<AI*>(currentPlayer)->makeMove(*board);
    checkGameState();
}

void Game::switchPlayer()
{
    currentPlayer = (currentPlayer == player1.get()) ? player2.get() : player1.get();
}

QString Game::serializeBoard() const
{
    QString result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            switch(board->getCell(i, j)) {
                case CellState::X: result += "X"; break;
                case CellState::O: result += "O"; break;
                default: result += "-"; break;
            }
        }
    }
    return result;
}

void Game::saveGameHistory(const QString& username)
{
    GameRecord record;
    record.timestamp = QDateTime::currentDateTime();
    record.player1 = player1->getName();
    record.player2 = player2->getName();
    record.moves = serializeBoard();
    record.username = username;
    
    CellState winner = board->checkWinner();
    if (winner == CellState::X) {
        record.winner = player1->getName();
    } else if (winner == CellState::O) {
        record.winner = player2->getName();
    } else {
        record.winner = "Draw";
    }
    
    gameHistory.append(record);
    
    QDir dir;
    if (!dir.exists("game_history")) {
        dir.mkdir("game_history");
    }
    
    QFile file(QString("game_history/%1_history.txt").arg(username));
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << record.timestamp.toString(Qt::ISODate) << ","
               << record.player1 << ","
               << record.player2 << ","
               << record.winner << ","
               << record.moves << "\n";
        file.close();
    }
}

const QList<Game::GameRecord>& Game::getGameHistory(const QString& username) const
{
    static QList<GameRecord> userHistory;
    userHistory.clear();
    
    for (const auto& record : gameHistory) {
        if (record.username == username) {
            userHistory.append(record);
        }
    }
    
    return userHistory;
}

QString Game::getCurrentPlayerName() const {
    return currentPlayer ? currentPlayer->getName() : QString();
}

bool Game::isGameOver() const {
    return board->isGameOver();
}

CellState Game::getWinner() const {
    return board->checkWinner();
}

Board* Game::getBoard() const {
    return board;
}

void Game::setUsername(const QString& username) {
    this->username = username;
}