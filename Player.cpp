#include <utility>


//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <functional>
#include <numeric>

#include "Player.h"

using namespace std;
using Qt::GlobalColor;

Player::Player(const QColor &color) : color(color),
                                      MAX_TILE((color == QColor(231, 0, 48)) ? 59 : color == GlobalColor::yellow
                                                                                    ? 25 : color ==
                                                                                           GlobalColor::darkGreen
                                                                                           ? 42 : 8),
                                      START_TILE_NUM(
                                              MAX_TILE == 59 ? 95 : MAX_TILE == 25 ? 81 : MAX_TILE == 42 ? 88 : 74) {
    if (this->color == QColor(0, 0, 153)) this->colorString = "blue";
    else if (this->color == QColor(153, 0, 0)) this->colorString = "red";
    else if (this->color == QColor(153, 153, 0)) this->colorString = "yellow";
    else this->colorString = "green";
}


void Player::addPawn(const QPointer<Pawn> &pawn) {
    this->pawns.push_back(pawn);
}

string Player::getColorString() const {
    return this->colorString;
}

int Player::numPawnsStart() const {
    function<bool(int, QPointer<Pawn>)> lambda =
            [&](int total, const QPointer<Pawn> &pawn) { return pawn->getStatus() == PawnStatus::START; };
    return accumulate(pawns.begin(), pawns.end(), 0, lambda);
}

int Player::numPawnsHome() const {
    function<bool(int, QPointer<Pawn>)> lambda =
            [&](int total, const QPointer<Pawn> &pawn) { return pawn->getStatus() == PawnStatus::HOME; };
    return accumulate(pawns.begin(), pawns.end(), 0, lambda);
}
