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

struct Pawn {
    PawnColor color;
    PawnStatus status = START;
    std::unique_ptr<Tile> currentTile;

    explicit Pawn(PawnColor color);
};


class Player : public QWidget {
private:
    std::vector<Pawn> pawns;
public:
    Player(QWidget *parent = nullptr);
};


#endif //PARCHEESI_PLAYER_H
