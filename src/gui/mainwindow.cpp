#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    stackedWidget(new QStackedWidget(this)),
    loginWindow(nullptr),
    gameWindow(nullptr)
{
    setWindowTitle("Advanced Tic Tac Toe");
    resize(400, 300);
    setCentralWidget(stackedWidget);
    
    initializeLoginWindow();
}

MainWindow::~MainWindow()
{
    cleanupGameWindow();
    if(loginWindow) {
        loginWindow->deleteLater();
    }
}

void MainWindow::initializeLoginWindow()
{
    if(loginWindow) {
        stackedWidget->removeWidget(loginWindow);
        loginWindow->deleteLater();
    }
    
    loginWindow = new LoginWindow();
    stackedWidget->addWidget(loginWindow);
    connect(loginWindow, &LoginWindow::loginSuccess,
            this, &MainWindow::handleLoginSuccess);
}

void MainWindow::cleanupGameWindow()
{
    if(gameWindow) {
        disconnect(gameWindow, &GameWindow::returnToMenu, 
                 this, &MainWindow::returnToLogin);
        stackedWidget->removeWidget(gameWindow);
        gameWindow->deleteLater();
        gameWindow = nullptr;
    }
}

void MainWindow::handleLoginSuccess(const QString &username)
{
    qDebug() << "Transitioning to game for user:" << username;
    
    cleanupGameWindow();
    
    gameWindow = new GameWindow(username);
    stackedWidget->addWidget(gameWindow);
    connect(gameWindow, &GameWindow::returnToMenu,
            this, &MainWindow::returnToLogin);
    
    stackedWidget->setCurrentWidget(gameWindow);
    resize(600, 600);
}

void MainWindow::returnToLogin()
{
    qDebug() << "Returning to login screen";
    cleanupGameWindow();
    initializeLoginWindow();
    stackedWidget->setCurrentWidget(loginWindow);
    resize(400, 300);
}