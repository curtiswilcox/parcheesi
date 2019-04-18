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
    std::vector<QPointer<Pawn>> pawns;
public:
    const QColor color;

    Player() = default;
    explicit Player(QColor color);
    explicit Player(std::vector<QPointer<Pawn>> pawns);

    void addPawn(const QPointer<Pawn> &pawn);

    int numPawnsStart() const;
    int numPawnsHome() const;
};


#endif //PARCHEESI_PLAYER_H
