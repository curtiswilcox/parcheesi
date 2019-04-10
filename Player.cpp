
//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <numeric>

#include "Player.h"

using namespace std;
using Qt::GlobalColor;

Player::Player(vector<QPointer<Pawn>> pawns) : pawns(std::move(pawns)) {}

void Player::addPawn(const QPointer<Pawn> &pawn) {
    this->pawns.push_back(pawn);
}

int Player::numPawnsStart() {
    auto lambda = [&](int total, const QPointer<Pawn> &pawn) { return pawn->getStatus() == PawnStatus::START; };
    return accumulate(pawns.begin(), pawns.end(), 0, lambda);
}

int Player::numPawnsHome() {
    auto lambda = [&](int total, const QPointer<Pawn> &pawn) { return pawn->getStatus() == PawnStatus::HOME; };
    return accumulate(pawns.begin(), pawns.end(), 0, lambda);
}
