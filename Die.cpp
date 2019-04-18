//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>
#include <QPainter>
#include <random>
#include <sstream>

#include "Die.h"

using Qt::GlobalColor;
using namespace std;

Die::Die(QWidget *parent) : QWidget(parent), value(0) {}

void Die::roll() {
    this->value = arc4random() % 6 + 1;
}

int Die::getValue() { return this->value; }

void Die::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(GlobalColor::white)));
    painter.drawRect(0, 0, 90, 90);

    stringstream valueAsString;
    valueAsString << this->value;
    painter.setFont(QFont("Arial", 40));
    painter.setPen(QPen(GlobalColor::black));
    painter.drawText(35, 55, QString(value == 0 ? "-" : valueAsString.str().c_str()));
}

int operator+(const Die &lhs, const Die &rhs) {
    return lhs.value + rhs.value;
}

bool operator==(const Die &lhs, const Die &rhs) {
    return lhs.value == rhs.value;
}

bool operator!=(const Die &lhs, const Die &rhs) {
    return !(lhs == rhs);
}

bool operator<(const Die &lhs, const Die &rhs) {
    return lhs.value < rhs.value;
}

bool operator<=(const Die &lhs, const Die &rhs) {
    return lhs < rhs || lhs == rhs;
}

bool operator>(const Die &lhs, const Die &rhs) {
    return !(lhs <= rhs);
}

bool operator>=(const Die &lhs, const Die &rhs) {
    return !(lhs < rhs);
}