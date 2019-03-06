//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <random>
#include "Die.h"

void Die::roll() {
    this->value = arc4random() % 6 + 1;
}

int Die::getValue() { return this->value; }

Die::operator int() { return this->value; }

bool operator==(const Die &lhs, const Die &rhs) {
    return lhs.value == rhs.value;
}