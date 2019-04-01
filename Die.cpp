//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <QPainter>
#include <random>

#include "Die.h"

using Qt::GlobalColor;

Die::Die(QWidget *parent) : QWidget(parent), value(0) {}

void Die::roll() {
    this->value = arc4random() % 6 + 1;
}

int Die::getValue() { return this->value; }

void Die::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(GlobalColor::white)));
    painter.drawRect(0, 0, 90, 90);
}

Die::operator int() { return this->value; }

bool operator==(const Die &lhs, const Die &rhs) {
    return lhs.value == rhs.value;
}