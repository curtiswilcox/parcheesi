//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_TILE_H
#define PARCHEESI_TILE_H


class Tile {
private:
    bool hasBlockade = false;
public:
    const bool isSafe;

    Tile();

    explicit Tile(bool safe);

};


#endif //PARCHEESI_TILE_H
