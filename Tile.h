//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_TILE_H
#define PARCHEESI_TILE_H

#include <QEvent>
#include <QPainter>
#include <QWidget>

struct Dimensions {
    int height;
    int width;
};

//struct Point {
//    int x;
//    int y;
//};

class Tile : public QWidget {
protected:
    bool hasBlockade = false;

    Dimensions dimensions = {0, 0};
//    Point point = {0, 0};
public:
    static const int TILE_SPACING = 1;

    const Qt::GlobalColor color;

    explicit Tile(const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);
//    Tile(const Point &p, const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);

    void paintEvent(QPaintEvent *event) override = 0;
};

class StartTile : public Tile {
public:
    explicit StartTile(const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);
//    StartTile(const Point &p, const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);

    void paintEvent(QPaintEvent *event) override;
};

class HomeTile : public Tile {
public:
    explicit HomeTile(const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);

//    HomeTile(const Point &p, const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);
//
    void paintEvent(QPaintEvent *event) override;
};

class RectangleTile : public Tile {
public:
    explicit RectangleTile(const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);
//    RectangleTile(const Point &p, const Dimensions &d, const Qt::GlobalColor &c = Qt::GlobalColor::white);

    void paintEvent(QPaintEvent *event) override;
};


#endif //PARCHEESI_TILE_H
