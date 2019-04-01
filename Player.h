//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_PLAYER_H
#define PARCHEESI_PLAYER_H

#include <memory>
#include <vector>

#include "Tile.h"

enum PawnStatus {
    START, PLAYING, HOME
};

class Pawn : public QWidget {
Q_OBJECT
private:
    Dimensions dimensions = {0, 0};
    const QColor color;
    PawnStatus status = START;

    friend class Player; // allow a player to directly access pawn information

public:
    explicit Pawn(const Dimensions &d, QColor c = Qt::GlobalColor::white, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
};


class Player : public QWidget {
Q_OBJECT
private:
    std::vector<Pawn> pawns;
public:
    explicit Player(QWidget *parent = nullptr);

    int numPawnsStart();
    int numPawnsHome();
};


#endif //PARCHEESI_PLAYER_H
