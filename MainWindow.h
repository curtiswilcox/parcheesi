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
#include "Die.h"

class MainWindow : public QWidget {
Q_OBJECT

private:
    QPointer<Board> board;

    void addStartTiles(QPointer<QGridLayout> &layout);
    void addHomeTiles(QPointer<QGridLayout> &layout);
    void addGeneralTiles(QPointer<QGridLayout> &layout);
    void addPlayers(QPointer<QGridLayout> &layout);
    void addDice(QPointer<QGridLayout> &layout);
    QColor getPathColor(int i) const;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QPointer<QGridLayout> createBoard();
    void move(Player activePlayer, int spaces);
};


#endif //PARCHEESI_MAINWINDOW_H
