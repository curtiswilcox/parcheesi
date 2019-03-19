//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>

#include "Tile.h"

using namespace std;
using Qt::GlobalColor;

Tile::Tile(const Dimensions &d, const GlobalColor &c) : dimensions(d), color(c) {}
//Tile::Tile(const Point &p, const Dimensions &d, const GlobalColor &c) : point(p), dimensions(d), color(c) {}

StartTile::StartTile(const Dimensions &d, const GlobalColor &c) : Tile(d, c) {};
//StartTile::StartTile(const Point &p, const Dimensions &d, const GlobalColor &c) : Tile(p, d, c) {};

void StartTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
//    rect.setRect(point.x, point.y, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, Qt::lightGray);

    painter.drawEllipse(0, 0, dimensions.width - 5, dimensions.height - 5);
//    painter.drawArc(0, 0, dimensions.width - 5, dimensions.height - 5, 30 * 16, 180 * 16);
}

HomeTile::HomeTile(const Dimensions &d, const GlobalColor &c) : Tile(d, c) {};
//HomeTile::HomeTile(const Point &p, const Dimensions &d, const GlobalColor &c) : Tile(p, d, c) {};

void HomeTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
//    rect.setRect(point.x, point.y, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, color);
//    painter.drawText(0, 0, 80, 80, 0, "Home");
}

RectangleTile::RectangleTile(const Dimensions &d, const GlobalColor &c) : Tile(d, c) {}
//RectangleTile::RectangleTile(const Point &p, const Dimensions &d, const GlobalColor &c) : Tile(p, d, c) {}

void RectangleTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
//    rect.setRect(point.x, point.y, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, color);
}
