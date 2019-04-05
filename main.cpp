//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>
#include <QApplication>

#include "MainWindow.h"

using namespace std;

int main(int argc, char **argv) {
    int exitCode = 0;

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();
    exitCode = app.exec();

    return exitCode;
};
