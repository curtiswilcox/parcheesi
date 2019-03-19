//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>

#include "Tile.h"

using namespace std;
using Qt::GlobalColor;

///////////////////////////////////////////////////////////////////////////
// Tile methods
///////////////////////////////////////////////////////////////////////////

Tile::Tile(const Dimensions &d, const GlobalColor &c) : dimensions(d), color(c), isSafe(c == Qt::cyan) {}

///////////////////////////////////////////////////////////////////////////
// StartTile methods
///////////////////////////////////////////////////////////////////////////

StartTile::StartTile(const Dimensions &d, const GlobalColor &c) : Tile(d, c) {};

void StartTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, Qt::lightGray);

    painter.setBrush(QBrush(GlobalColor::cyan));
    painter.drawRect(40, 30, dimensions.width - 80, dimensions.height - 80);

    int angle = 30 * 16;
    int length = 120 * 16;

    painter.setBrush(QBrush(GlobalColor::yellow));
    painter.drawChord(27, 0, dimensions.width - 54, 120, angle, length);

    painter.setBrush(QBrush(QColor(0, 143, 229)));
    painter.rotate(90);
    painter.drawChord(17, -dimensions.width + 10, dimensions.width - 54, 120, angle, length);

    painter.setBrush(QBrush(QColor(231, 0, 48)));
    painter.rotate(90);
    painter.drawChord(-213, -dimensions.width + 20, dimensions.width - 54, 120, angle, length);

    painter.setBrush(QBrush(GlobalColor::darkGreen));
    painter.rotate(90);
    painter.drawChord(-dimensions.height + 37, 10, dimensions.width - 54, 120, angle, length);
}

///////////////////////////////////////////////////////////////////////////
// HomeTile methods
///////////////////////////////////////////////////////////////////////////

HomeTile::HomeTile(const Dimensions &d, const GlobalColor &c) : Tile(d, c) {};

void HomeTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, color);
}

///////////////////////////////////////////////////////////////////////////
// RectangleTile methods
///////////////////////////////////////////////////////////////////////////

RectangleTile::RectangleTile(const Dimensions &d, const GlobalColor &c) : Tile(d, c) {}

void RectangleTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, color);
}
