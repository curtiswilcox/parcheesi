//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_BOARD_H
#define PARCHEESI_BOARD_H

#include <vector>

#include "Player.h"
#include "Tile.h"

class Board {
private:
    std::vector<Tile> tiles;
    std::vector<Player> players;

};


#endif //PARCHEESI_BOARD_H
