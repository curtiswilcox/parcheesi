//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>
#include <QShortcut>

#include "MainWindow.h"

using namespace std;
using Qt::GlobalColor;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this, SLOT(close()));
    this->setWindowTitle("Parcheesi");
    this->resize(780, 600);

    this->board = new Board(parent);

    QPointer<QGridLayout> layout = createBoard();
    this->setLayout(layout);
}

QPointer<QGridLayout> MainWindow::createBoard() {
    QPointer<QGridLayout> layout = new QGridLayout(this);
    layout->setSpacing(Tile::TILE_SPACING);

    addStartTiles(layout);
    addHomeTiles(layout);
    addGeneralTiles(layout);
    addPlayers(layout);
    addDice(layout);

    return layout;
}

void MainWindow::addStartTiles(QPointer<QGridLayout> &layout) {
    QPointer<StartTile> blueStart = new StartTile({240, 240}, QColor(0, 143, 229), this);
    layout->addWidget(blueStart, 0, 0, 16, 16);

    QPointer<StartTile> redStart = new StartTile({240, 240}, QColor(231, 0, 48), this);
    layout->addWidget(redStart, 0, 22, 16, 16);

    QPointer<StartTile> yellowStart = new StartTile({240, 240}, GlobalColor::yellow, this);
    layout->addWidget(yellowStart, 22, 0, 16, 16);

    QPointer<StartTile> greenStart = new StartTile({240, 240}, GlobalColor::darkGreen, this);
    layout->addWidget(greenStart, 22, 22, 16, 16);
}

void MainWindow::addHomeTiles(QPointer<QGridLayout> &layout) {
    QPointer<HomeTile> home = new HomeTile({240, 240}, this);
    layout->addWidget(home, 16, 16, 6, 6);
}

void MainWindow::addGeneralTiles(QPointer<QGridLayout> &layout) {
    Dimensions horizontal = {30, 80};
    Dimensions vertical = {80, 30};

    horizontal *= 2;
    vertical *= 2;

    int tileCounter = 0;
    vector<int> safeNums = {4, 12, 20, 28, 35, 43, 51, 59};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) { // red "home" tiles and safe tiles at the edge
            QPointer<Tile> tile = new RectangleTile(i % 2 == 0 ? horizontal : vertical,
                                                    (i < 2 && j == 0) || (i >= 2 && j == 7) ? GlobalColor::cyan
                                                                                            : getPathColor(i),
                                                    this);

            switch (i) {
                case 0:
                    layout->addWidget(tile, j * 2, 18, 2, 2);
                    break;
                case 1:
                    layout->addWidget(tile, 18, j * 2, 2, 2);
                    break;
                case 2:
                    layout->addWidget(tile, (j + 11) * 2, 18, 2, 2);
                    break;
                case 3:
                    layout->addWidget(tile, 18, (j + 11) * 2, 2, 2);
                    break;
                default:
                    break;
            }
        }

        for (int j = 0; j < 2; ++j) { // other surrounding tiles
            for (int k = 0; k < 8; ++k) {
                QPointer<Tile> tile =
                        new RectangleTile(i % 2 == 0 ? horizontal : vertical,
                                          std::find(safeNums.begin(), safeNums.end(), tileCounter) != safeNums.end()
                                          ? GlobalColor::cyan
                                          : Qt::white,
                                          this);

                switch (i) {
                    case 0:
                        layout->addWidget(tile, k * 2, (j == 0 ? 8 : 10) * 2, 2, 2);
                        break;
                    case 1:
                        layout->addWidget(tile, (j == 0 ? 8 : 10) * 2, k * 2, 2, 2);
                        break;
                    case 2:
                        layout->addWidget(tile, (k + 11) * 2, (j == 0 ? 8 : 10) * 2, 2, 2);
                        break;
                    case 3:
                        layout->addWidget(tile, (j == 0 ? 8 : 10) * 2, (k + 11) * 2, 2, 2);
                        break;
                    default:
                        break;
                }
                ++tileCounter;
            }
        }
    }
}

void MainWindow::addPlayers(QPointer<QGridLayout> &layout) {
    // TODO assign each pawn set to a player
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> blueOne = new Pawn({10, 20}, QColor(0, 0, 153), this);
            layout->addWidget(blueOne, (i + 2) * 2, (j + 2) * 2, 8, 8);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> redOne = new Pawn({10, 20}, QColor(153, 0, 0), this);
            layout->addWidget(redOne, (i + 2) * 2, (j + 13) * 2, 12, 12);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> yellowOne = new Pawn({10, 20}, QColor(153, 153, 0), this);
            layout->addWidget(yellowOne, (i + 13) * 2, (j + 2) * 2, 8, 8);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> greenOneOne = new Pawn({10, 20}, QColor(0, 102, 0), this);
            layout->addWidget(greenOneOne, (i + 13) * 2, (j + 13) * 2, 8, 8);
        }
    }

    // pawns to test the placement on board pieces
    QPointer<Pawn> exPawn = new Pawn({10, 30}, QColor(0, 0, 153), this);
    layout->addWidget(exPawn, 16, 26, 1, 2);
//
//    QPointer<Pawn> exPawn2 = new Pawn({10, 20}, QColor(0, 0, 153), this);
//    layout->addWidget(exPawn2, 16, 27, 1, 1);

    QPointer<Pawn> exPawn3 = new Pawn({10, 30}, QColor(0, 0, 153), this);
    layout->addWidget(exPawn3, 17, 26, 1, 2);
}

void MainWindow::addDice(QPointer<QGridLayout> &layout) {
    QPointer<Die> die = new Die(this);
    layout->addWidget(die, 0, 38, 6, 6);
    QPointer<Die> secondDie = new Die(this);
    layout->addWidget(secondDie, 0, 44, 6, 6);
}

QColor MainWindow::getPathColor(int i) const {
    switch (i) {
        case 0:
            return {0, 143, 229}; // blue
        case 1:
            return GlobalColor::yellow;
        case 2:
            return GlobalColor::darkGreen;
        case 3:
            return {231, 0, 48}; // red
        default:
            return GlobalColor::white;
    }
}

void MainWindow::move(Player activePlayer, int spaces) {

}
