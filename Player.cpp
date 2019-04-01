//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <numeric>

#include "Player.h"

using namespace std;
using Qt::GlobalColor;

Pawn::Pawn(const Dimensions &d, const QColor &c, QWidget *parent) : dimensions(d), color(c), QWidget(parent) {}

void Pawn::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), 2));
    painter.drawRect(rect);
    painter.fillRect(rect, Qt::lightGray);

    painter.setBrush(QBrush(color));
    painter.drawRect(30, 30, dimensions.width - 60, dimensions.height - 60);

    // NOTE: all of these are completely magic numbers
}

Player::Player(QWidget *parent) : QWidget(parent) {}

int Player::numPawnsStart() {
    return accumulate(pawns.begin(), pawns.end(), 0, [&](int total, const Pawn &pawn) { return pawn.status == PawnStatus::START; });
}

int Player::numPawnsHome() {
    return accumulate(pawns.begin(), pawns.end(), 0, [&](int total, const Pawn &pawn) { return pawn.status == PawnStatus::HOME; });
}
