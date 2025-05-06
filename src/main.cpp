#include "gui/mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Set application metadata
    a.setApplicationName("Advanced Tic Tac Toe");
    a.setApplicationVersion("1.0");
    a.setWindowIcon(QIcon(":/app-icon.png"));
    GameTypes::registerTypes();
    // Load stylesheet
    QFile styleFile(":/styles.qss");
    if (styleFile.open(QIODevice::ReadOnly)) {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    } else {
        qDebug() << "Using default styling (stylesheet not found)";
    }

    MainWindow w;
    w.show();
    
    return a.exec();
}