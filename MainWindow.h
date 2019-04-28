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
    std::vector<Player> players;
    QWidget *rulesWindow;
    QWidget *startWindow;
    void addStartTiles(QPointer<QGridLayout> &layout);
    void addHomeTile(QPointer<QGridLayout> &layout);
    void addGeneralTiles(QPointer<QGridLayout> &layout);
    std::vector<Player> addPawns(QPointer<QGridLayout> &layout);
    void cpuTest();
    void addDice(QPointer<QGridLayout> &layout);
    void addNextButton(QPointer<QGridLayout> &layout);
    void addDialogueBox(QPointer<QGridLayout> &layout);
//    void movePawn(QPointer<QGridLayout> &layout, QPointer<Pawn> &pawn, int tileNum, char position);
//    void movePawn(QPointer<QGridLayout> &layout, QPointer<Pawn> &pawn);
    bool movePawn(const QPointer<Pawn> &pawn, int spaces, int pawnMax, bool backToStart = false);
    inline void updateLabelText(const std::string &text) {this->updateLabelText(QString::fromStdString(text)); }
    void updateLabelText(const QString &text);
    void updateScroll();
    QColor getPathColor(int i) const;
    int jump(const QPointer<Pawn> &pawn) const;
    int jump(int startNum, int spaces, const Player &player) const;
    void cpuTurn(const Player &player);

public:
    explicit MainWindow(QWidget *parent = nullptr);
//    void mainLoop(QPointer<QGridLayout> &layout, vector<Player> players);
    std::vector<Player> getPlayers();
    QString readRules();
    std::vector<Player> createBoard(QPointer<QGridLayout> &layout);
    std::vector<Player> resetBoard();
    void play(const Player &player);
    bool canMove(const QPointer<Pawn> &pawn, int spaces) const;
    std::string tolower(const std::string &s) const;
    std::string toupper(const std::string &s) const;
    bool movePawnTest(QPointer<Pawn> &, int);
    template<typename T>
    void iterateThroughLayout(const std::function<void(T *)> &func) const {
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
