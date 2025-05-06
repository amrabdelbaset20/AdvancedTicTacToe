#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include "../game/game.h"
#include "../game/gametypes.h"

class QGridLayout;
class QPushButton;
class QLabel;
class QTextEdit;
class QComboBox;

class GameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameWindow(const QString& username, QWidget* parent = nullptr);
    ~GameWindow();
    void setUsername(const QString& username);

signals:
    void returnToMenu();

private slots:
    void handleButtonClick();
    void updateUI();
    void handleGameOver(CellState winner);
    void newGame();
    void showHistory();
    void changeGameMode(int index);
    void changeAIDifficulty(int index);

private:
    void createBoard();
    void clearBoard();

    Game* game;
    QString username;
    QGridLayout* boardLayout;
    QVector<QPushButton*> boardButtons;
    QLabel* statusLabel;
    QPushButton* newGameButton;
    QPushButton* menuButton;
    QPushButton* historyButton;
    QComboBox* gameModeCombo;
    QComboBox* aiDifficultyCombo;
};

#endif // GAMEWINDOW_H