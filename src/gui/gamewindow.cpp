#include "gamewindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QFont>
#include <QDialog>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QComboBox>
#include <QHBoxLayout>
#include <QDir>
GameWindow::~GameWindow()
{
    // Clean up resources if needed
}
GameWindow::GameWindow(const QString &username, QWidget *parent)
    : QWidget(parent), username(username),
      boardLayout(new QGridLayout()),
      statusLabel(new QLabel("Current player: X", this)),
      newGameButton(new QPushButton("New Game", this)),
      menuButton(new QPushButton("Main Menu", this)),
      historyButton(new QPushButton("History", this)),
      gameModeCombo(new QComboBox(this)),
      aiDifficultyCombo(new QComboBox(this))
{
    setObjectName("gameWindow");
    game = new Game(this);
    game->setUsername(username);
    
    // Setup game mode combo
    gameModeCombo->addItem("Player vs AI", QVariant::fromValue(GameTypes::PlayerVsAI));
    gameModeCombo->addItem("Player vs Player", QVariant::fromValue(GameTypes::PlayerVsPlayer));
    gameModeCombo->setCurrentIndex(0);
    
    // Setup AI difficulty combo
    aiDifficultyCombo->addItem("Easy AI", QVariant::fromValue(GameTypes::Easy));
    aiDifficultyCombo->addItem("Medium AI", QVariant::fromValue(GameTypes::Medium));
    aiDifficultyCombo->addItem("Hard AI", QVariant::fromValue(GameTypes::Hard));
    aiDifficultyCombo->setCurrentIndex(2);
    
    // Connect combos
    connect(gameModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GameWindow::changeGameMode);
    connect(aiDifficultyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GameWindow::changeAIDifficulty);
    
    // Styling
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);
    
    newGameButton->setObjectName("newGameButton");
    menuButton->setObjectName("menuButton");
    historyButton->setObjectName("historyButton");
    
    // Layout setup
    QHBoxLayout *modeLayout = new QHBoxLayout();
    modeLayout->addWidget(new QLabel("Game Mode:", this));
    modeLayout->addWidget(gameModeCombo);
    modeLayout->addWidget(new QLabel("AI Difficulty:", this));
    modeLayout->addWidget(aiDifficultyCombo);
    modeLayout->addStretch();
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(newGameButton);
    buttonLayout->addWidget(historyButton);
    buttonLayout->addWidget(menuButton);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(modeLayout);
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(boardLayout);
    mainLayout->addLayout(buttonLayout);
    
    createBoard();
    
    connect(newGameButton, &QPushButton::clicked, this, &GameWindow::newGame);
    connect(menuButton, &QPushButton::clicked, this, &GameWindow::returnToMenu);
    connect(historyButton, &QPushButton::clicked, this, &GameWindow::showHistory);
    connect(game, &Game::gameUpdated, this, &GameWindow::updateUI);
    connect(game, &Game::gameOver, this, &GameWindow::handleGameOver);
    
    game->startNewGame(static_cast<GameTypes::GameMode>(GameTypes::PlayerVsAI), 
                      username, "AI", 
                      static_cast<GameTypes::AIDifficulty>(GameTypes::Hard));
}

void GameWindow::showHistory()
{
    QDialog historyDialog(this);
    historyDialog.setWindowTitle(QString("Game History - %1").arg(username));
    historyDialog.resize(800, 600);
    
    QTextEdit *historyText = new QTextEdit(&historyDialog);
    historyText->setReadOnly(true);
    historyText->setFont(QFont("Courier New", 12));
    
    QVBoxLayout *layout = new QVBoxLayout(&historyDialog);
    layout->addWidget(historyText);
    
    QFile file(QString("game_history/%1_history.txt").arg(username));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = "<html><body style='font-family: Arial; font-size: 14px;'>";
        content += "<h2 style='color: #3498db;'>Game History for " + username + "</h2>";
        content += "<table border='1' cellpadding='5' style='border-collapse: collapse; width: 100%;'>";
        content += "<tr style='background-color: #34495e; color: white;'>"
                   "<th>Date/Time</th><th>Players</th><th>Winner</th><th>Moves</th></tr>";
        
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 5) {
                content += "<tr style='border-bottom: 1px solid #34495e;'>";
                content += QString("<td>%1</td>").arg(parts[0]);
                content += QString("<td>%1 vs %2</td>").arg(parts[1]).arg(parts[2]);
                content += QString("<td style='color: %1;'>%2</td>")
                           .arg(parts[3] == "Draw" ? "orange" : 
                                (parts[3] == username ? "green" : "red"))
                           .arg(parts[3]);
                
                // Format moves as a Tic Tac Toe grid
                QString moves = parts[4];
                content += "<td><table style='border: none;'>";
                for (int i = 0; i < 3; ++i) {
                    content += "<tr>";
                    for (int j = 0; j < 3; ++j) {
                        QChar c = moves[i*3+j];
                        QString cellChar = (c == 'X') ? "X" : (c == 'O') ? "O" : " ";
                        QString color = (c == 'X') ? "red" : (c == 'O') ? "blue" : "gray";
                        content += QString("<td style='border: 1px solid #34495e; padding: 5px; "
                                         "text-align: center; color: %1; font-weight: bold;'>%2</td>")
                                  .arg(color).arg(cellChar);
                    }
                    content += "</tr>";
                }
                content += "</table></td></tr>";
            }
        }
        content += "</table></body></html>";
        historyText->setHtml(content);
        file.close();
    } else {
        historyText->setHtml("<p style='color: #e74c3c;'>No game history found for this user.</p>");
    }
    
    historyDialog.exec();
}

void GameWindow::changeGameMode(int index)
{
    GameTypes::GameMode mode = static_cast<GameTypes::GameMode>(gameModeCombo->currentData().toInt());
    aiDifficultyCombo->setEnabled(mode == GameTypes::PlayerVsAI);
    newGame();
}

void GameWindow::changeAIDifficulty(int index)
{
    newGame();
}

void GameWindow::newGame()
{
    GameTypes::GameMode mode = static_cast<GameTypes::GameMode>(gameModeCombo->currentData().toInt());
    GameTypes::AIDifficulty difficulty = static_cast<GameTypes::AIDifficulty>(aiDifficultyCombo->currentData().toInt());
    
    QString player2Name = mode == GameTypes::PlayerVsAI ? "AI" : "Player 2";
    clearBoard();
    createBoard();
    game->startNewGame(mode, username, player2Name, difficulty);
}

void GameWindow::createBoard()
{
    clearBoard();
    
    for(int i = 0; i < 9; ++i) {
        QPushButton *button = new QPushButton(this);
        button->setFixedSize(100, 100);
        button->setFont(QFont("Arial", 24));
        boardButtons.append(button);
        boardLayout->addWidget(button, i / 3, i % 3);
        connect(button, &QPushButton::clicked, this, &GameWindow::handleButtonClick);
    }
}

void GameWindow::clearBoard()
{
    qDeleteAll(boardButtons);
    boardButtons.clear();
}

void GameWindow::handleButtonClick()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int index = boardButtons.indexOf(button);
    if (index != -1) {
        int row = index / 3;
        int col = index % 3;
        game->makeMove(row, col);
    }
}

void GameWindow::updateUI()
{
    Board* board = game->getBoard();
    for (int i = 0; i < 9; ++i) {
        int row = i / 3;
        int col = i % 3;
        CellState state = board->getCell(row, col);
        QString text;
        switch (state) {
            case GameTypes::X: text = "X"; break;
            case GameTypes::O: text = "O"; break;
            default: text = ""; break;
        }
        boardButtons[i]->setText(text);
    }

    statusLabel->setText(QString("Current player: %1").arg(game->getCurrentPlayerName()));
}

void GameWindow::handleGameOver(CellState winner)
{
    QString message;
    if (winner == GameTypes::Empty) {
        message = "Game ended in a draw!";
    } else {
        message = QString("%1 wins!").arg(game->getCurrentPlayerName());
    }
    QMessageBox::information(this, "Game Over", message);
}