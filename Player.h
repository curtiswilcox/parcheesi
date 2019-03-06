//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_PLAYER_H
#define PARCHEESI_PLAYER_H

#include <vector>

#include "Tile.h"

enum PawnColor {
    RED, YELLOW, GREEN, BLUE
};

enum PawnStatus {
    START, PLAYING, HOME
};

struct Pawn {
    PawnColor color;
    PawnStatus status = START;
    Tile currentTile;

    explicit Pawn(PawnColor color);
};


class Player {
private:
    std::vector<Pawn> pawns;
public:
    Player() = default;
};


#endif //PARCHEESI_PLAYER_H
