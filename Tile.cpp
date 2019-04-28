//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>
#include <QSettings>
#include <sstream>

#include "Tile.h"

using namespace std;
using Qt::GlobalColor;


///////////////////////////////////////////////////////////////////////////
// Tile methods
///////////////////////////////////////////////////////////////////////////

Tile::Tile(QWidget *parent) : QWidget(parent), isSafe(false) {}

Tile::Tile(const Dimensions &d, const QColor &c, QWidget *parent) : QWidget(parent), dimensions(d), color(c),
                                                                    isSafe(c == Qt::cyan) {}

///////////////////////////////////////////////////////////////////////////
// StartTile methods
///////////////////////////////////////////////////////////////////////////

StartTile::StartTile(const Dimensions &d, const QColor &c, QWidget *parent) : Tile(d, c, parent) {
    for (int i = 0; i < 4; ++i) {
        pawns.emplace_back(nullopt);
    }

    if (c == QColor(0, 143, 229)) this->colorString = "Blue";
    else if (c == QColor(231, 0, 48)) colorString = "Red";
    else if (c == GlobalColor::yellow) colorString = "Yellow";
    else colorString = "Green";
}

optional<QPointer<Pawn>> StartTile::getOccupyingPawn() const {
    for (int i = pawns.size() - 1; i >=0; --i) {
        if (pawns[i]) return *pawns[i]; // return the last pawn in the vector that isn't nullopt
    }
    return nullopt;
}

void StartTile::mouseReleaseEvent(QMouseEvent *event) {
    cout << "Clicked Start Tile color (" << color.red() << ", " << color.green() << ", " << color.blue() << ")!" << endl;
}

void StartTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, Qt::lightGray);

    painter.setBrush(QBrush(color));
    painter.drawRect(40, 40, dimensions.width - 80, dimensions.height - 80);

    // NOTE: all of these are completely magic numbers

    int angle = 30 * 16;
    int length = 120 * 16;

    painter.setBrush(QBrush(GlobalColor::yellow));
    painter.drawChord(27, 10, dimensions.width - 54, 120, angle, length);

    painter.setBrush(QBrush(QColor(0, 143, 229))); // blue
    painter.rotate(90);
    painter.drawChord(27, -dimensions.width + 10, dimensions.width - 54, 120, angle, length);

    painter.setBrush(QBrush(QColor(231, 0, 48))); // red
    painter.rotate(90);
    painter.drawChord(-213, -dimensions.width + 10, dimensions.width - 54, 120, angle, length);

    painter.setBrush(QBrush(GlobalColor::darkGreen));
    painter.rotate(90);
    painter.drawChord(-dimensions.height + 27, 10, dimensions.width - 54, 120, angle, length);
}

///////////////////////////////////////////////////////////////////////////
// HomeTile methods
///////////////////////////////////////////////////////////////////////////

HomeTile::HomeTile(const Dimensions &d, QWidget *parent, const GlobalColor &c) : Tile(d, c, parent) {}

optional<QPointer<Pawn>> HomeTile::getOccupyingPawn() const {
    for (int i = pawns.size(); i >= 0; --i) {
        if (pawns[i]) return *pawns[i];
    }
    return nullopt;
}

void HomeTile::mouseReleaseEvent(QMouseEvent *event) {
    cout << "Clicked Home Tile!" << endl;
}

void HomeTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, {148, 0, 211});
    painter.setFont(QFont("Arial", 25, 5));
    painter.drawText(10, 55, "Home");
}

///////////////////////////////////////////////////////////////////////////
// RectangleTile methods
///////////////////////////////////////////////////////////////////////////

RectangleTile::RectangleTile(QWidget *parent) : number(-1), Tile(parent) {}

RectangleTile::RectangleTile(int counter, const Dimensions &d, const QColor &c, QWidget *parent) : number(counter), Tile(d, c, parent) {}

PlayerAddition RectangleTile::addPawn(const QPointer<Pawn> &pawn) {
    if (!this->occupyingPawn) { // tile is empty
        this->occupyingPawn = pawn;
        return SUCCESS;
    }

    if (!this->secondPawn && (*occupyingPawn)->getColor() == pawn->getColor()) { // make a blockade
        this->secondPawn = pawn;
        this->hasBlockade = true;
        return SUCCESS;
    }

    if (this->occupyingPawn && (*occupyingPawn)->getColor() != pawn->getColor()) { // capture current pawn
        (*occupyingPawn)->setStatus(PawnStatus::START);
        // TODO move the occupyingPawn back to its Start
        occupyingPawn = pawn;
        return CAPTURE;
    }

    return FAILURE;
}

optional<QPointer<Pawn>> RectangleTile::removePawn() {
    if (this->secondPawn) {
        QPointer<Pawn> toReturn = *secondPawn;
        this->secondPawn = nullopt;
        this->hasBlockade = false;
        return toReturn;
    }
    if (this->occupyingPawn) {
        QPointer<Pawn> toReturn = *occupyingPawn;
        this->occupyingPawn = nullopt;
        return toReturn;
    }
    return nullopt;
}

optional<QPointer<Pawn>> RectangleTile::getOccupyingPawn() const {
    if (this->secondPawn) return *secondPawn;
    if (this->occupyingPawn) return *occupyingPawn;
    return nullopt;
}


void RectangleTile::mouseReleaseEvent(QMouseEvent *event) {
    QSettings settings("CS205", "Parcheesi");
//    cout << "Previous Rectangle Tile: " << settings.value("currentTileNumber", -2).toInt() << endl;
    cout << "Clicked Rectangle Tile #" << number << "! " << (this->isOccupied() ? "O" : "Not o") << "ccupied." << endl;
    settings.setValue("currentTileNumber", number);
}

void RectangleTile::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect;
    rect.setRect(0, 0, dimensions.width, dimensions.height);
    painter.setPen(QPen(QBrush(Qt::black), TILE_SPACING));
    painter.drawRect(rect);
    painter.fillRect(rect, color);

    stringstream numAsString;
    numAsString << this->number;
    painter.setFont(QFont("Arial", 10));
    painter.drawText(10, 12, QString(numAsString.str().c_str()));

}
