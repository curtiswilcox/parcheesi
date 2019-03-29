//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <numeric>

#include "Player.h"

using namespace std;

Pawn::Pawn(const Dimensions &d, const PawnColor &c, QWidget *parent) : dimensions(d), color(c), QWidget(parent) {}

Player::Player(QWidget *parent) : QWidget(parent) {}

int Player::numPawnsStart() {
    return accumulate(pawns.begin(), pawns.end(), 0, [&](int total, const Pawn &pawn) { return pawn.status == PawnStatus::START; });
}

int Player::numPawnsHome() {
    return accumulate(pawns.begin(), pawns.end(), 0, [&](int total, const Pawn &pawn) { return pawn.status == PawnStatus::HOME; });
}
