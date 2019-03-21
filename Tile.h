//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_TILE_H
#define PARCHEESI_TILE_H

#include <QEvent>
#include <QPainter>
#include <QWidget>

///////////////////////////////////////////////////////////////////////////
// Dimensions struct declaration
///////////////////////////////////////////////////////////////////////////

struct Dimensions {
    const int height;
    const int width;
};

///////////////////////////////////////////////////////////////////////////
// Tile class declaration
///////////////////////////////////////////////////////////////////////////

class Tile : public QWidget {
protected:
    bool hasBlockade = false;
    const bool isSafe;

    Dimensions dimensions = {0, 0};
public:
    static const int TILE_SPACING = 1;

    const QColor color;

    explicit Tile(const Dimensions &d, const QColor &c = Qt::GlobalColor::white);

    void paintEvent(QPaintEvent *event) override = 0;
};

///////////////////////////////////////////////////////////////////////////
// StartTile class declaration
///////////////////////////////////////////////////////////////////////////

class StartTile : public Tile {
public:
    explicit StartTile(const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);

    void paintEvent(QPaintEvent *event) override;
};

///////////////////////////////////////////////////////////////////////////
// HomeTile class declaration
///////////////////////////////////////////////////////////////////////////

class HomeTile : public Tile {
public:
    explicit HomeTile(const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);

    void paintEvent(QPaintEvent *event) override;
};

///////////////////////////////////////////////////////////////////////////
// RectangleTile class declaration
///////////////////////////////////////////////////////////////////////////

class RectangleTile : public Tile {
public:
    explicit RectangleTile(const Dimensions &d, const QColor &c = Qt::GlobalColor::white);

    void paintEvent(QPaintEvent *event) override;
};


#endif //PARCHEESI_TILE_H
