#include <utility>

//
// Created by Curtis Wilcox on 2019-04-05.
//

#include <iostream>
#include <QSettings>

#include "Pawn.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Dimensions operators
///////////////////////////////////////////////////////////////////////////

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-narrowing-conversions" // suppress warnings

void Dimensions::operator*=(double multiplier) {
    this->height *= multiplier;
    this->width *= multiplier;
}

void Dimensions::operator/=(int multiplier) {
    this->height *= ((double) 1 / multiplier);
    this->width *= ((double) 1 / multiplier);
}


#pragma clang diagnostic pop



///////////////////////////////////////////////////////////////////////////
// Pawn methods
///////////////////////////////////////////////////////////////////////////

Pawn::Pawn(const Dimensions &d, QColor c, QWidget *parent) : dimensions(d), color(std::move(c)),
            MAX_TILE((c == QColor(231, 0, 48)) ? 59 : c == Qt::GlobalColor::yellow
                                               ? 25 : c == Qt::GlobalColor::darkGreen
                                               ? 42 : 8),
                                                             QWidget(parent) {}

Pawn::Pawn(const Dimensions &d, int currentTileNum, int id, string team,
           function<void(QPointer<Pawn>)> lambda, QColor c, QWidget *parent) : dimensions(d), color(std::move(c)),
            MAX_TILE((c == QColor(153, 0, 0)) ? 59 : c == QColor(153, 153, 0)
                                               ? 25 : c == QColor(0, 102, 0)
                                               ? 42 : 8),
                                               currentTileNum(currentTileNum),
                                               id(id),
                                               team(std::move(team)),
                                               lambda(std::move(lambda)),
                                               QWidget(parent) {

}

void Pawn::mouseReleaseEvent(QMouseEvent *event) {
    this->lambda(this);
}

void Pawn::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), 1));
    painter.drawRect(rect);
    painter.fillRect(rect, color);
}

bool operator<(const Pawn &lhs, const Pawn &rhs) {
    if (lhs.getStatus() == PawnStatus::START) return true;
    if (rhs.getStatus() == PawnStatus::HOME) return true;
    return lhs.getStatus() == rhs.getStatus() == PawnStatus::PLAYING;
}

bool operator==(const Pawn &lhs, const Pawn &rhs) {
    return lhs.color == rhs.color && lhs.currentTileNum == rhs.currentTileNum;
}

bool operator!=(const Pawn &lhs, const Pawn &rhs) {
    return !(lhs == rhs);
}

ostream &operator <<(ostream &outs, const Pawn &pawn) {
    outs << pawn.team << " " << pawn.id;
    return outs;
}