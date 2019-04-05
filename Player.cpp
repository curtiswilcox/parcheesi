#include <utility>

//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <numeric>

#include "Player.h"

using namespace std;
using Qt::GlobalColor;

Pawn::Pawn(const Dimensions &d, QColor c, QWidget *parent) : dimensions(d), color(std::move(c)), QWidget(parent) {}

void Pawn::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), 1));
    painter.drawRect(rect);
    painter.fillRect(rect, color);
}

Player::Player(QWidget *parent) : QWidget(parent) {}

int Player::numPawnsStart() {
    auto lambda = [&](int total, const Pawn &pawn) { return pawn.status == PawnStatus::START; };
    return accumulate(pawns.begin(), pawns.end(), 0, lambda);
}

int Player::numPawnsHome() {
    auto lambda = [&](int total, const Pawn &pawn) { return pawn.status == PawnStatus::HOME; };
    return accumulate(pawns.begin(), pawns.end(), 0, lambda);
}
