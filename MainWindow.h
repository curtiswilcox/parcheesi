//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_MAINWINDOW_H
#define PARCHEESI_MAINWINDOW_H

#include <memory>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QPointer>
#include <QWidget>

#include "Board.h"

class MainWindow : public QWidget {
Q_OBJECT

private:
    QPointer<Board> board;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    QPointer<QGridLayout> createBoard();
    void move(Player activePlayer, int spaces);
};


#endif //PARCHEESI_MAINWINDOW_H
