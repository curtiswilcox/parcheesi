//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_MAINWINDOW_H
#define PARCHEESI_MAINWINDOW_H

#include <QCoreApplication>
#include <QDesktopWidget>
#include <QMainWindow>

#include "Board.h"


class MainWindow : public QMainWindow {
Q_OBJECT

private:
    Board board;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void createBoard();
    void move(Player activePlayer, int spaces);
};


#endif //PARCHEESI_MAINWINDOW_H
