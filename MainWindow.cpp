//
// Created by Curtis Wilcox on 2019-03-04.
//

#include "MainWindow.h"

#include <QtCore/qpropertyanimation.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setWindowTitle("Parcheesi");
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.8);
}

void MainWindow::createBoard() {

}

void MainWindow::move(Player activePlayer, int spaces) {

}
