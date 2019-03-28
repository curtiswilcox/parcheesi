//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_MAINWINDOW_H
#define PARCHEESI_MAINWINDOW_H

#include <functional>
#include <memory>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPointer>
#include <QWidget>
#include <QWindow>

#include "Board.h"
#include "Die.h"


class MainWindow : public QWidget {
Q_OBJECT

private:
    QPointer<Board> board;
    QWidget *rulesWindow;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    QPointer<QGridLayout> createBoard();
    QColor getPathColor(int i) const;
    void move(Player activePlayer, int spaces);

//public slots:
//    std::unique_ptr<Window> showRules();
};


#endif //PARCHEESI_MAINWINDOW_H
