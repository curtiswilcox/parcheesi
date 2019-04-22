//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_MAINWINDOW_H
#define PARCHEESI_MAINWINDOW_H

#include <functional>
#include <iostream>
#include <memory>
#include <list>
#include <tuple>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPointer>
#include <QWidget>
#include <QWindow>

#include "Die.h"
#include "Pawn.h"
#include "Player.h"
#include "Tile.h"


class MainWindow : public QWidget {
Q_OBJECT

private:
    std::map<std::string, std::tuple<int, int>> pawnLocations;
    QWidget *rulesWindow;
    void addStartTiles(QPointer<QGridLayout> &layout);
    void addHomeTiles(QPointer<QGridLayout> &layout);
    void addGeneralTiles(QPointer<QGridLayout> &layout);
    std::vector<Player> addPawns(QPointer<QGridLayout> &layout);
    void addDice(QPointer<QGridLayout> &layout);
    void addDialogueBox(QPointer<QGridLayout> &layout);
    QColor getPathColor(int i) const;

public:
    void movePawn(QPointer<QGridLayout> &layout, QPointer<Pawn> &pawn);
    explicit MainWindow(QWidget *parent = nullptr);
//    ~MainWindow();
    QString readRules();
    std::vector<Player> createBoard(QPointer<QGridLayout> &layout);
    void play(const std::vector<Player> &players);
    bool canMove(const Player &activePlayer, const QPointer<Tile> &tile, int spaces);

    template<typename T>
    void iterateThroughLayout(const T &toCast, const std::function<void(T *)> &func) {
        for (int i = 0; i < this->layout()->count(); ++i) {
            auto item = this->layout()->itemAt(i);
            if (auto widItem = dynamic_cast<QWidgetItem *>(item)) {
                if (auto t = dynamic_cast<T *>(item->widget())) {
                    func(t);
                }
            }
        }
    }

};


#endif //PARCHEESI_MAINWINDOW_H
