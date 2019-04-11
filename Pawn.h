//
// Created by Curtis Wilcox on 2019-04-05.
//

#ifndef PARCHEESI_PAWN_H
#define PARCHEESI_PAWN_H

#include <QPainter>
#include <QPen>
#include <QPointer>
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
// PawnStatus enum declaration
///////////////////////////////////////////////////////////////////////////

enum PawnStatus {
    START, PLAYING, HOME
};

///////////////////////////////////////////////////////////////////////////
// Pawn class declaration
///////////////////////////////////////////////////////////////////////////

class Pawn : public QWidget {
Q_OBJECT
private:
    Dimensions dimensions = {0, 0};
    const QColor color;
    PawnStatus status = START;

public:
    const int MAX_TILE;

    explicit Pawn(const Dimensions &d, QColor c = Qt::GlobalColor::white, QWidget *parent = nullptr);

    inline const Dimensions &getDimensions() const { return this->dimensions; }
    inline const PawnStatus &getStatus() const { return this->status; }
    inline const QColor &getColor() const { return this->color; }

    inline void setStatus(PawnStatus status) { this->status = status; }

    void paintEvent(QPaintEvent *event) override;
};

#endif //PARCHEESI_PAWN_H
