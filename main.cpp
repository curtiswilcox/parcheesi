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

    return app.exec();

}
