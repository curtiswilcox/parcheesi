//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>

#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    this->setWindowTitle("Parcheesi");
    this->resize(600, 600);
//    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.8);


    this->board = new Board;

    QPointer<QGridLayout> layout = createBoard();
    this->setLayout(layout);
}

MainWindow::~MainWindow() {
    delete this->board;
    this->board = nullptr;
}

QPointer<QGridLayout> MainWindow::createBoard() {
    auto layout = new QGridLayout(this);
    layout->setSpacing(Tile::TILE_SPACING);

    QPointer<StartTile> redStart = new StartTile({240, 240}, Qt::red);
    layout->addWidget(redStart, 0, 0, 8, 8);

    QPointer<StartTile> yellowStart = new StartTile({240, 240}, Qt::yellow);
    layout->addWidget(yellowStart, 0, 11, 8, 8);

    QPointer<StartTile> greenStart = new StartTile({240, 240}, Qt::darkGreen);
    layout->addWidget(greenStart, 11, 0, 8, 8);

    QPointer<StartTile> blueStart = new StartTile({240, 240}, Qt::darkBlue);
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
                                                    (i < 2 && j == 0) || (i >= 2 && j == 7) ? Qt::cyan : Qt::red);
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
                                          std::find(safeNums.begin(), safeNums.end(), tileCounter) != safeNums.end() ? Qt::cyan : Qt::white);

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

    return layout;
}

void MainWindow::move(Player activePlayer, int spaces) {

}
