//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_PLAYER_H
#define PARCHEESI_PLAYER_H

#include <memory>
#include <vector>

#include "Pawn.h"
#include "Tile.h"

class Player {
private:
    std::vector<Pawn> pawns;
public:
    Player(std::vector<Pawn> pawns);

    int numPawnsStart();
    int numPawnsHome();
};


#endif //PARCHEESI_PLAYER_H
