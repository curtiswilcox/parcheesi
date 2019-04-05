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
    int height;
    int width;

    void operator*=(double multiplier);
    void operator/=(int multiplier);
};

///////////////////////////////////////////////////////////////////////////
// Tile class declaration
///////////////////////////////////////////////////////////////////////////

class Tile : public QWidget {
Q_OBJECT
protected:
    bool hasBlockade = false;
    const bool isSafe;

    Dimensions dimensions = {0, 0};
public:
    static const int TILE_SPACING = 1;

    const QColor color;

    explicit Tile(const Dimensions &d, const QColor &c = Qt::GlobalColor::white, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override = 0;
};

///////////////////////////////////////////////////////////////////////////
// StartTile class declaration
///////////////////////////////////////////////////////////////////////////

class StartTile : public Tile {
Q_OBJECT
public:
    explicit StartTile(const Dimensions &d, const QColor &c = Qt::GlobalColor::white, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
};

///////////////////////////////////////////////////////////////////////////
// HomeTile class declaration
///////////////////////////////////////////////////////////////////////////

class HomeTile : public Tile {
Q_OBJECT
public:
    explicit HomeTile(const Dimensions &d, QWidget *parent = nullptr,
                      const Qt::GlobalColor &c = Qt::GlobalColor::white);

    void paintEvent(QPaintEvent *event) override;
};

///////////////////////////////////////////////////////////////////////////
// RectangleTile class declaration
///////////////////////////////////////////////////////////////////////////

class RectangleTile : public Tile {
Q_OBJECT
public:
    explicit RectangleTile(const Dimensions &d, const QColor &c = Qt::GlobalColor::white, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
};


#endif //PARCHEESI_TILE_H
