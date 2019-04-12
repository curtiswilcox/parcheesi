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
//    cout << "After roll:" << this->value << endl;
}

int Die::getValue() { return this->value; }

void Die::paintEvent(QPaintEvent *event) {
//    cout << "Die paintEvent!" << endl;
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(GlobalColor::white)));
    painter.drawRect(0, 0, 90, 90);

    stringstream valueAsString;
    valueAsString << this->value;
    painter.setFont(QFont("Arial", 20));
    painter.setPen(QPen(GlobalColor::black));
    painter.drawText(40, 50, QString(value == 0 ? "-" : valueAsString.str().c_str()));
//    painter.drawText(10, 12, QString::fromStdString(test()));
//    cerr << "value: " << this->value << ", test(): " << test() << endl;
}

//string Die::test() {
//    cout << "test function value: " << value << endl;
//    switch (value) {
//        case 1:
//            return "one";
//        case 2:
//            return "two";
//        case 3:
//            return "three";
//        case 4:
//            return "four";
//        case 5:
//            return "five";
//        case 6:
//            return "six";
//        default:
//            return "problem";
//    }
//}

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