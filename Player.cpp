//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <numeric>

#include "Player.h"

using namespace std;

Pawn::Pawn(PawnColor color) : color(color) {}

Player::Player(QWidget *parent) : QWidget(parent) {}

int Player::numPawnsStart() {
    auto lambda = [&](int total, const Pawn &pawn) { return pawn.status == PawnStatus::START; };
    return accumulate(pawns.begin(), pawns.end(), 0, lambda);
}

int Player::numPawnsHome() {
    auto lambda = [&](int total, const Pawn &pawn) { return pawn.status == PawnStatus::HOME; };
    return accumulate(pawns.begin(), pawns.end(), 0, lambda);
}
