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
    cout << "After roll:" << this->value << endl;
}

int Die::getValue() { return this->value; }

void Die::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(GlobalColor::white)));
    painter.drawRect(0, 0, 90, 90);

    stringstream valueAsString;
    valueAsString << this->value;
    painter.setFont(QFont("Arial", 20));
    painter.setPen(QPen(GlobalColor::black));
//    painter.drawText(10, 12, QString(valueAsString.str().c_str()));
    painter.drawText(10, 12, QString::fromStdString(test()));
    cerr << "value: " << this->value << ", vAS" << valueAsString.str() << endl;
}

string Die::test() {
    switch (value) {
        case 1: return "one"; break;
        case 2: return "two"; break;
        case 3: return "three"; break;
        case 4: return "four"; break;
        case 5: return "five"; break;
        case 6: return "six"; break;
        default: return "problem";
    }
}

Die::operator int() { return this->value; }

bool operator==(const Die &lhs, const Die &rhs) {
    return lhs.value == rhs.value;
}

bool operator!=(const Die &lhs, const Die &rhs) {
    return !(lhs == rhs);
}

bool operator<=(const Die &lhs, const Die &rhs) {
    return lhs < rhs || lhs == rhs;
}

bool operator<(const Die &lhs, const Die &rhs) {
    return lhs.value < rhs.value;
}

bool operator>(const Die &lhs, const Die &rhs) {
    return !(lhs <= rhs);
}

bool operator>=(const Die &lhs, const Die &rhs) {
    return !(lhs < rhs);
}