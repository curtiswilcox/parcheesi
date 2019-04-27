//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>
#include <QApplication>
#include <QSettings>
#include "MainWindow.h"

using namespace std;

int main(int argc, char **argv) {
    QSettings settings("CS205", "Parcheesi");
    int exitCode = 0;

    QApplication app(argc, argv);

    MainWindow mainWindow;

    mainWindow.show();

    MainWindow::mainLoop(mainWindow->)
//    settings.setValue("currentPlayer", players.at(0).color);
//    for (int i = 0; i < 50; i++) {
//        this->play(players[0]);
//        this->play(players[1]);
//        this->play(players[2]);
//        this->play(players[3]);
//    }
    exitCode = app.exec();

    return exitCode;
};
