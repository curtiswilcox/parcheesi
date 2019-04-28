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
#include <QSettings>
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
    QSettings settings;
    std::vector<QString> gameOutput;

    void addStartTiles(QPointer<QGridLayout> &layout);
    void addHomeTile(QPointer<QGridLayout> &layout);
    void addGeneralTiles(QPointer<QGridLayout> &layout);
    std::vector<Player> addPawns(QPointer<QGridLayout> &layout);
    void addDice(QPointer<QGridLayout> &layout);
    void addNextButton(QPointer<QGridLayout> &layout);
    void addDialogueBox(QPointer<QGridLayout> &layout);

    bool movePawn(const QPointer<Pawn> &pawn, int spaces);
    std::tuple<int, std::string> getTileInformation(const QPointer<Pawn> &pawn, int spaces) const;

    inline void updateLabelText(const std::string &text) { this->updateLabelText(QString::fromStdString(text)); }

    void updateLabelText(const QString &text);
    void updateScroll();
    QColor getPathColor(int i) const;
    int jump(const QPointer<Pawn> &pawn) const;
    void cpuTurn(const Player &player);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QString readRules();
    std::vector<Player> createBoard(QPointer<QGridLayout> &layout);
    std::vector<Player> resetBoard();
    void play(const Player &player);
    bool canMove(const QPointer<Pawn> &pawn, int spaces) const;
    std::string tolower(const std::string &s) const;
    std::string toupper(const std::string &s) const;

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
