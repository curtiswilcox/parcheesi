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

public:
    const QColor color;
    const int MAX_TILE;
    const int START_TILE_NUM;
    std::vector<QPointer<Pawn>> pawns;
    std::string colorString;
    const int id;
    explicit Player(const QColor &color, int id);

    std::string getColorString() const;

    void addPawn(const QPointer<Pawn> &pawn);

    int numPawnsStart() const;
    int numPawnsHome() const;
};


#endif //PARCHEESI_PLAYER_H
