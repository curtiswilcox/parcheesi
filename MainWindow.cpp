//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <iostream>
#include <QAction>
#include <QLabel>
#include <QShortcut>
#include <QScrollArea>
#include <QSizePolicy>

#include "MainWindow.h"

using namespace std;
using Qt::GlobalColor;


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this, SLOT(close()));
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowTitle("Parcheesi");
    this->resize(780, 600);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored); // disable user resizing
    this->setFixedSize(this->width(), this->height());
    this->setStyleSheet("background-color: black;");
    this->board = new Board(parent);

    QPointer<QMenuBar> menuBar = new QMenuBar(this);
    QPointer<QMenu> fileMenu = menuBar->addMenu("&Help");
    menuBar->addMenu(fileMenu);
    QPointer<QAction> gameplayInstructions = fileMenu->addAction("&Rules");
    gameplayInstructions->setShortcut(Qt::CTRL + Qt::Key_R);

    auto showRules = [&]() {
        rulesWindow = new QWidget(this, Qt::Window);
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), rulesWindow, SLOT(close()));
        rulesWindow->resize(780, 600);
        rulesWindow->setWindowTitle("Parcheesi Rules");

        QPointer<QScrollArea> scroll = new QScrollArea(rulesWindow);
        scroll->resize(rulesWindow->size());

        QPointer<QLabel> rulesText = new QLabel(scroll);
        rulesText->setWordWrap(true);
        rulesText->setContentsMargins(
                5, rulesText->contentsMargins().top(), 5, rulesText->contentsMargins().bottom()
        );
        rulesText->textInteractionFlags().setFlag(Qt::TextInteractionFlag::TextEditable, false);
        rulesText->textInteractionFlags().setFlag(Qt::TextInteractionFlag::TextSelectableByMouse, true);
        rulesText->setText(
                "<h1>Gameplay/Rules:</h1>\n\n"
                "<ul>"
                "<li>A player rolls the dice and must use the top die values shown to move their pieces around the board in one of the following ways:</li>\n"
                "<li>Only pieces not in the nest may move forward on the board.</li>\n"
                "<li>Pieces may only leave the nest with a roll of a five on a single die or the sum of the dice. A double five can be used to move two pieces from the nest simultaneously.</li>\n"
                "<li>In the case of a non-doubles roll, a player may move one or two pieces, either one piece by each of the numbers on the two dice or one piece by the total. If no move is possible, the turn is forfeited.</li>\n"
                "<li>When moving a single piece the total of two dice the turn is taken in increments, allowing pieces to be captured along the way. For example, if a double two is rolled and an opponent's piece lies on a cream space two spaces in front of the piece you wish to move the full four, you would move the piece two, and then two again, allowing the opponent's piece to be captured.</li>\n"
                "<li>All die rolls must be taken and may not be voluntarily forfeited by a player.</li>\n"
                "<li>If either of two rolls must be forfeited, the player must forfeit the lower number.</li>\n"
                "<li>All die moves must be taken before the application of any extra rewards for sending an opponent to their nest or moving a piece to its home position.</li>\n"
                "<li>With a roll of doubles, the player makes four moves, one for each of the numbers on top of the two dice and one for each of the numbers on the bottoms. The player may distribute these four moves among one, two, three, or four pieces. Note that the sum of numbers on the opposite sides of a die is always seven, so with doubles, there are a total of fourteen spaces to move. This can only be done if all four pieces are out of the nest.</li>\n"
                "<li>When the player rolls doubles, the player rolls again after moving.</li>\n"
                "<li>When a piece ends its move on the same space as an opponent's piece, the opponent's piece is sent back to its nest.</li>\n"
                "<li>A piece may not be placed on a safe space (generally colored light blue) if it is occupied by an opponent's piece. The exception is the safe space used when a piece leaves its nest — a single piece occupying such a safe space is sent back to its nest when an opponent's piece leaves the nest and occupies the space.</li>\n"
                "<li>A blockade is formed when two pieces of a single player occupy the same space. No piece of any player may move through a blockade, including pieces of the blockade owner. Blockade pieces may not be moved forward together with the roll of a double. Another player's piece cannot land in a space occupied by a blockade, even to leave its nest.</li>\n"
                "<li>A piece is not required to enter the home row and can pass the row and start another circuit of the board voluntarily or as the result of requirement of the use of the total die roll.</li>\n"
                "<li>A turn ends when the next player rolls the dice with the consent of the current player. Any rewards not taken are lost.</li>\n\n"
                "</ul>"

                "<h1>Rewards:</h1>\n"
                "<ul>"
                "<li>The reward for sending an opponent's piece to the nest is a free move of twenty spaces that may not be split between pieces.</li>\n"
                "<li>The reward for landing a piece in the home space is a free move of ten spaces that may not be split between pieces.</li>\n\n"
                "</ul>"

                "<h1>Winning:</h1>\n"
                "<ul>"
                "<li>Moving all four pieces to the home position wins the game.</li>\n"
                "<li>Pieces may only be moved to the home position with an exact application of the total roll, the value on a single die, or the complete application of a reward.</li>"
                "</ul>"
                "\n\n\n"
                "Rules courtesy of \"https://en.wikipedia.org/wiki/Parcheesi#Rules\".");
        rulesText->adjustSize();
        scroll->setWidget(rulesText);
        rulesWindow->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored); // disable user resizing
        rulesWindow->setFixedSize(rulesWindow->width(), rulesWindow->height());
        rulesWindow->show();
    };

    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this), &QShortcut::activated, showRules);
    connect(gameplayInstructions, &QAction::triggered, this, showRules);

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
            for (int k = 0; k < 8; ++k, ++tileCounter) {
                QPointer<Tile> tile =
                        new RectangleTile(i % 2 == 0 ? horizontal : vertical,
                                          std::find(safeNums.begin(), safeNums.end(), tileCounter) != safeNums.end()
                                          ? GlobalColor::cyan
                                          : GlobalColor::white,
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
//    QPointer<Pawn> exPawn = new Pawn({10, 30}, QColor(0, 0, 153), this);
//    layout->addWidget(exPawn, 16, 26, 1, 2);
//
//    QPointer<Pawn> exPawn2 = new Pawn({10, 20}, QColor(0, 0, 153), this);
//    layout->addWidget(exPawn2, 16, 27, 1, 1);

//    QPointer<Pawn> exPawn3 = new Pawn({10, 30}, QColor(0, 0, 153), this);
//    layout->addWidget(exPawn3, 17, 26, 1, 2);
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
