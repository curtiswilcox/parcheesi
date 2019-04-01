//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>

#include "Tile.h"

using namespace std;
using Qt::GlobalColor;


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

Dimensions operator*(Dimensions &d, double multiplier) {
    return {(int) (d.height * multiplier), (int) (d.width * multiplier)};
}

Dimensions operator/(Dimensions &d, int multiplier) {
    return d * ((double) 1 / multiplier);
}

///////////////////////////////////////////////////////////////////////////
// Tile methods
///////////////////////////////////////////////////////////////////////////

Tile::Tile(const Dimensions &d, const QColor &c, QWidget *parent) : QWidget(parent), dimensions(d), color(c),
                                                                    isSafe(c == Qt::cyan) {}

///////////////////////////////////////////////////////////////////////////
// StartTile methods
///////////////////////////////////////////////////////////////////////////

StartTile::StartTile(const Dimensions &d, const QColor &c, QWidget *parent) : Tile(d, c, parent) {};

void StartTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, Qt::lightGray);

    painter.setBrush(QBrush(color));
    painter.drawRect(40, 40, dimensions.width - 80, dimensions.height - 80);

    // NOTE: all of these are completely magic numbers

    int angle = 30 * 16;
    int length = 120 * 16;

    painter.setBrush(QBrush(GlobalColor::yellow));
    painter.drawChord(27, 10, dimensions.width - 54, 120, angle, length);

    painter.setBrush(QBrush(QColor(0, 143, 229))); // blue
    painter.rotate(90);
    painter.drawChord(27, -dimensions.width + 10, dimensions.width - 54, 120, angle, length);

    painter.setBrush(QBrush(QColor(231, 0, 48))); // red
    painter.rotate(90);
    painter.drawChord(-213, -dimensions.width + 10, dimensions.width - 54, 120, angle, length);

    painter.setBrush(QBrush(GlobalColor::darkGreen));
    painter.rotate(90);
    painter.drawChord(-dimensions.height + 27, 10, dimensions.width - 54, 120, angle, length);
}

///////////////////////////////////////////////////////////////////////////
// HomeTile methods
///////////////////////////////////////////////////////////////////////////

HomeTile::HomeTile(const Dimensions &d, QWidget *parent, const GlobalColor &c) : Tile(d, c, parent) {};

void HomeTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, {148, 0, 211});
    painter.setFont(QFont("Arial", 30, 5));
    painter.drawText(5, 55, "Home");
}

///////////////////////////////////////////////////////////////////////////
// RectangleTile methods
///////////////////////////////////////////////////////////////////////////

RectangleTile::RectangleTile(const Dimensions &d, const QColor &c, QWidget *parent) : Tile(d, c, parent) {}

void RectangleTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, color);
}
