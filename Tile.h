//
// Created by Curtis Wilcox on 2019-03-04.
//

#ifndef PARCHEESI_TILE_H
#define PARCHEESI_TILE_H

#include <optional>
#include <QEvent>
#include <QPainter>
#include <QWidget>
#include "Pawn.h"

///////////////////////////////////////////////////////////////////////////
// PlayerAddition enum declaration
///////////////////////////////////////////////////////////////////////////

enum PlayerAddition {
    SUCCESS, FAILURE, CAPTURE
};

///////////////////////////////////////////////////////////////////////////
// Tile class declaration
///////////////////////////////////////////////////////////////////////////

class Tile : public QWidget {
Q_OBJECT
protected:
    bool hasBlockade = false;

    Dimensions dimensions = {0, 0};
public:
    static const int TILE_SPACING = 1;

    const bool isSafe;

    const QColor color;

    explicit Tile(const Dimensions &d, const QColor &c = Qt::GlobalColor::white, QWidget *parent = nullptr);

    virtual std::optional<QPointer<Pawn>> getOccupyingPawn() const = 0;

    inline bool isBlockaded() const { return hasBlockade; }

    void paintEvent(QPaintEvent *event) override = 0;
};

///////////////////////////////////////////////////////////////////////////
// StartTile class declaration
///////////////////////////////////////////////////////////////////////////

class StartTile : public Tile {
Q_OBJECT
private:
    std::vector<std::optional<QPointer<Pawn>>> pawns;
public:
    explicit StartTile(const Dimensions &d, const QColor &c = Qt::GlobalColor::white, QWidget *parent = nullptr);

    std::optional<QPointer<Pawn>> getOccupyingPawn() const override;

    void paintEvent(QPaintEvent *event) override;
};

///////////////////////////////////////////////////////////////////////////
// HomeTile class declaration
///////////////////////////////////////////////////////////////////////////

class HomeTile : public Tile {
Q_OBJECT
private:
    std::vector<std::optional<QPointer<Pawn>>> pawns;
public:
    explicit HomeTile(const Dimensions &d, QWidget *parent = nullptr,
                      const Qt::GlobalColor &c = Qt::GlobalColor::white);

    std::optional<QPointer<Pawn>> getOccupyingPawn() const override;

    void paintEvent(QPaintEvent *event) override;
};

///////////////////////////////////////////////////////////////////////////
// RectangleTile class declaration
///////////////////////////////////////////////////////////////////////////

class RectangleTile : public Tile {
Q_OBJECT
private:
    const int number;
    std::optional<QPointer<Pawn>> occupyingPawn = std::nullopt;
    std::optional<QPointer<Pawn>> secondPawn = std::nullopt;
public:
    explicit RectangleTile(int counter, const Dimensions &d, const QColor &c = Qt::GlobalColor::white,
                           QWidget *parent = nullptr);

    PlayerAddition addPawn(const QPointer<Pawn> &pawn);
    std::optional<QPointer<Pawn>> removePawn();
    std::optional<QPointer<Pawn>> getOccupyingPawn() const override;

    inline bool isOccupied() const { return this->getOccupyingPawn() != std::nullopt; }

    inline int getNumber() { return this->number; }

    void paintEvent(QPaintEvent *event) override;
};


#endif //PARCHEESI_TILE_H
