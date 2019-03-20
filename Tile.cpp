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

Tile::Tile(const Dimensions &d, const QColor &c) : dimensions(d), color(c), isSafe(c == Qt::cyan) {}

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
    painter.drawRect(40, 40, dimensions.width - 80, dimensions.height - 80);

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

HomeTile::HomeTile(const Dimensions &d, const GlobalColor &c) : Tile(d, c) {};

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

RectangleTile::RectangleTile(const Dimensions &d, const QColor &c, bool needsArrow) : needsArrow(needsArrow),
                                                                                      Tile(d, c) {}

void RectangleTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, color);

//    if (needsArrow) {
//        int bottomLeftX = 0;
//        int bottomLeftY = dimensions.height;
//
//        int bottomRightX = dimensions.width;
//        int bottomRightY = dimensions.height;
//
//        int topX = dimensions.width / 2;
//        int topY = 0;
//
//        QPainterPath path;
//        path.moveTo(bottomLeftX, bottomLeftY);
//        path.lineTo(bottomRightX, bottomRightY);
//        path.lineTo(topX, topY);
//        path.lineTo(bottomLeftX, bottomLeftY);
//        painter.fillPath(path, QBrush(GlobalColor::black));
//    }
}
