//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_DIE_H
#define PARCHEESI_DIE_H


class Die {
private:
    int value;
public:
    Die() = default;

    void roll();

    int getValue();

    explicit operator int();

    friend bool operator==(const Die &lhs, const Die &rhs);
};


#endif //PARCHEESI_DIE_H
