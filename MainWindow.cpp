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

//MainWindow::~MainWindow() {
//    delete this->board;
//    this->board = nullptr;
//}

QPointer<QGridLayout> MainWindow::createBoard() {
    QPointer<QGridLayout> layout = new QGridLayout(this);
    layout->setSpacing(Tile::TILE_SPACING);

    QPointer<StartTile> redStart = new StartTile({240, 240}, GlobalColor::red);
    layout->addWidget(redStart, 0, 0, 8, 8);

    QPointer<StartTile> yellowStart = new StartTile({240, 240}, GlobalColor::yellow);
    layout->addWidget(yellowStart, 0, 11, 8, 8);

    QPointer<StartTile> greenStart = new StartTile({240, 240}, GlobalColor::darkGreen);
    layout->addWidget(greenStart, 11, 0, 8, 8);

    QPointer<StartTile> blueStart = new StartTile({240, 240}, GlobalColor::darkBlue);
    layout->addWidget(blueStart, 11, 11, 8, 8);

    QPointer<HomeTile> home = new HomeTile({240, 240});
    layout->addWidget(home, 8, 8, 3, 3);

    Dimensions horizontal = {30, 80};
    Dimensions vertical = {80, 30};

    int tileCounter = 0;
    vector<int> safeNums = {4, 12, 20, 28, 35, 43, 51, 59};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) { // red "home" tiles and safe tiles at the edge
            QPointer<Tile> tile = new RectangleTile(i % 2 == 0 ? horizontal : vertical,
                                                    (i < 2 && j == 0) || (i >= 2 && j == 7) ? GlobalColor::cyan
                                                                                            : getPathColor(i));

            switch (i) {
                case 0:
                    layout->addWidget(tile, j, 9);
                    break;
                case 1:
                    layout->addWidget(tile, 9, j);
                    break;
                case 2:
                    layout->addWidget(tile, j + 11, 9);
                    break;
                case 3:
                    layout->addWidget(tile, 9, j + 11);
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
                                          : Qt::white);

                switch (i) {
                    case 0:
                        layout->addWidget(tile, k, j == 0 ? 8 : 10);
                        break;
                    case 1:
                        layout->addWidget(tile, j == 0 ? 8 : 10, k);
                        break;
                    case 2:
                        layout->addWidget(tile, k + 11, j == 0 ? 8 : 10);
                        break;
                    case 3:
                        layout->addWidget(tile, j == 0 ? 8 : 10, k + 11);
                        break;
                    default:
                        break;
                }
                ++tileCounter;
            }
        }
    }

    QPointer<Die> die = new Die();
    layout->addWidget(die, 0, 19, 3, 3);
    QPointer<Die> secondDie = new Die();
    layout->addWidget(secondDie, 0, 22, 3, 3);

    return layout;
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
