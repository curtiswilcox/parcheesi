//
// Created by Curtis Wilcox on 2019-04-05.
//

#ifndef PARCHEESI_PAWN_H
#define PARCHEESI_PAWN_H

#include <QPointer>
#include <QWidget>

#include "Tile.h"

enum PawnStatus {
    START, PLAYING, HOME
};

class Pawn : public QWidget {
    Q_OBJECT
private:
    Dimensions dimensions = {0, 0};
    const QColor color;
    PawnStatus status = START;

    friend class Player; // allow a player to directly access pawn information

public:
    explicit Pawn(const Dimensions &d, QColor c = Qt::GlobalColor::white, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
};

#endif //PARCHEESI_PAWN_H
