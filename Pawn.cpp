//
// Created by Curtis Wilcox on 2019-04-05.
//

#include "Pawn.h"

using namespace std;

Pawn::Pawn(const Dimensions &d, QColor c, QWidget *parent) : dimensions(d), color(std::move(c)), QWidget(parent) {}

void Pawn::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), 1));
    painter.drawRect(rect);
    painter.fillRect(rect, color);
}