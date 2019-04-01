//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_PLAYER_H
#define PARCHEESI_PLAYER_H

#include <memory>
#include <vector>

#include "Tile.h"

enum PawnColor {
    RED = Qt::red, YELLOW = Qt::yellow, GREEN = Qt::green, BLUE = Qt::blue
};

enum PawnStatus {
    START, PLAYING, HOME
};


class Pawn : public QWidget {
private:
    Dimensions dimensions = {0, 0};

public:
    const QColor color;
    PawnStatus status = START;

    explicit Pawn(const Dimensions &d, const QColor &c = Qt::GlobalColor::white, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);

};


class Player : public QWidget {
private:
    std::vector<Pawn> pawns;
public:
    explicit Player(QWidget *parent = nullptr);

    int numPawnsStart();
    int numPawnsHome();
};


#endif //PARCHEESI_PLAYER_H
