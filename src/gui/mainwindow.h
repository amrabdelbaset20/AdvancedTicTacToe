#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "loginwindow.h"
#include "gamewindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLoginSuccess(const QString &username);
    void returnToLogin();

private:
    QStackedWidget *stackedWidget;
    LoginWindow *loginWindow;
    GameWindow *gameWindow;

    void initializeLoginWindow();
    void cleanupGameWindow();
};

#endif // MAINWINDOW_H