//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <fstream>
#include <iostream>
#include <QAction>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QShortcut>
#include <QSizePolicy>

#include "MainWindow.h"

using namespace std;
using Qt::GlobalColor;

QSettings settings("CS205", "Parcheesi"); // NOLINT(cert-err58-cpp)

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this, SLOT(close()));
    this->setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowTitle("Parcheesi");
    this->resize(780, 600);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored); // disable user resizing
    this->setFixedSize(this->width(), this->height());
    this->setStyleSheet("background-color: black; color: white;");

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
        rulesText->setText(readRules());
        rulesText->adjustSize();
        scroll->setWidget(rulesText);
        rulesWindow->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored); // disable user resizing
        rulesWindow->setFixedSize(rulesWindow->width(), rulesWindow->height());
        rulesWindow->show();
    };

    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this), &QShortcut::activated, showRules);
    connect(gameplayInstructions, &QAction::triggered, this, showRules);

    QPointer<QGridLayout> layout = new QGridLayout(this);
    vector<Player> players = createBoard(layout);
    this->setLayout(layout);

    this->play(players);
}

QString MainWindow::readRules() {
    ifstream infile;
    infile.open("../rules.txt");
    string rules;

    while (infile && !infile.eof()) {
        string line;
        getline(infile, line);
        rules += line;
    }
    infile.close();
    return QString::fromStdString(rules);
}


vector<Player> MainWindow::createBoard(QPointer<QGridLayout> &layout) {
    layout->setSpacing(Tile::TILE_SPACING);

    addStartTiles(layout);
    addHomeTiles(layout);
    addGeneralTiles(layout);
    vector<Player> players = addPawns(layout);
    addDice(layout);
    addDialogueBox(layout);

    return players;
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
    vector<int> safeNums = {3, 13, 20, 30, 37, 47, 54, 64}; // top right of home, widdershins

    int lastRecTileNum = 67;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) { // "home stretch" tiles

            if ((i < 2 && j == 0) || (i >= 2 && j == 7)) continue; // skip safe spaces at end of home tiles

            QPointer<Tile> tile = new RectangleTile(++lastRecTileNum, i % 2 == 0 ? horizontal : vertical, getPathColor(i), this);

            switch (i) {
                case 0:
                    layout->addWidget(tile, j * 2, 18, 2, 2); // blue
                    break;
                case 1:
                    layout->addWidget(tile, 18, j * 2, 2, 2); // yellow
                    break;
                case 2:
                    int use;
                    switch (j) {
                        case 0:
                            use = 6;
                            break;
                        case 1:
                            use = 5;
                            break;
                        case 2:
                            use = 4;
                            break;
                        case 3:
                            use = 3;
                            break;
                        case 4:
                            use = 2;
                            break;
                        case 5:
                            use = 1;
                            break;
                        case 6:
                            use = 0;
                            break;
                        default: use = 0;
                    }
                    layout->addWidget(tile, (use + 11) * 2, 18, 2, 2); // green
                    break;
                case 3:
                    switch (j) {
                        case 0:
                            use = 6;
                            break;
                        case 1:
                            use = 5;
                            break;
                        case 2:
                            use = 4;
                            break;
                        case 3:
                            use = 3;
                            break;
                        case 4:
                            use = 2;
                            break;
                        case 5:
                            use = 1;
                            break;
                        case 6:
                            use = 0;
                            break;
                        default: use = 0;
                    }
                    layout->addWidget(tile, 18, (use + 11) * 2, 2, 2); // red
                    break;
                default:
                    break;
            }

        }

        for (int j = 0; j < 2; ++j) { // other surrounding tiles
            if (j == 1) {
                QPointer<Tile> tile = new RectangleTile(tileCounter++, i % 2 == 0 ? horizontal : vertical,
                                                        GlobalColor::cyan, this);
                switch (i) {
                    case 0:
                        layout->addWidget(tile, 0, 18, 2, 2); // 0, 18
                        break;
                    case 1:
                        layout->addWidget(tile, 18, 0, 2, 2); // 18, 0
                        break;
                    case 2:
                        layout->addWidget(tile, 36, 18, 2, 2); // 36, 18
                        break;
                    case 3:
                        layout->addWidget(tile, 18, 36, 2, 2); // 18, 36
                        break;
                    default:
                        break;
                }
            }

            for (int k = 0; k < 8; ++k, ++tileCounter) {
                QPointer<Tile> tile =
                        new RectangleTile(tileCounter, i % 2 == 0 ? horizontal : vertical,
                                          std::find(safeNums.begin(), safeNums.end(), tileCounter) != safeNums.end()
                                          ? GlobalColor::cyan : GlobalColor::white,
                                          this);

                switch (i) {
                    case 0:
                        j == 0 ?
                        layout->addWidget(tile, (14 - (2 * k)), 10 * 2, 2, 2) :
                        layout->addWidget(tile, k * 2, 8 * 2, 2, 2);
                        break;
                    case 1:
                        j == 0 ?
                        layout->addWidget(tile, 8 * 2, (14 - (k * 2)), 2, 2) :
                        layout->addWidget(tile, 10 * 2, k * 2, 2, 2);
                        break;
                    case 2:
                        j == 0 ?
                        layout->addWidget(tile, (k + 11) * 2, 8 * 2, 2, 2) :
                        layout->addWidget(tile, (14 + 4 - k) * 2, 10 * 2, 2, 2);
                        break;
                    case 3:
                        j == 0 ?
                        layout->addWidget(tile, 10 * 2, (k + 11) * 2, 2, 2) :
                        layout->addWidget(tile, 8 * 2, ((int) (14.5 * 2 - (k + 11)) * 2), 2, 2);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

vector<Player> MainWindow::addPawns(QPointer<QGridLayout> &layout) {
    Player bluePlayer(QColor(0, 0, 153));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> blueOne = new Pawn({10, 20}, QColor(0, 0, 153), this);
            layout->addWidget(blueOne, (i + 2) * 2, (j + 2) * 2, 1, 2);
            bluePlayer.addPawn(blueOne);
        }
    }

    Player redPlayer(QColor(153, 0, 0));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> redOne = new Pawn({10, 20}, QColor(153, 0, 0), this);
            layout->addWidget(redOne, (i + 2) * 2, (j + 13) * 2, 1, 2);
            redPlayer.addPawn(redOne);
        }
    }

    Player yellowPlayer(QColor(153, 153, 0));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> yellowOne = new Pawn({10, 20}, QColor(153, 153, 0), this);
            layout->addWidget(yellowOne, (i + 13) * 2, (j + 2) * 2, 1, 2);
            yellowPlayer.addPawn(yellowOne);
        }
    }

    Player greenPlayer(QColor(0, 102, 0));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> greenOne = new Pawn({10, 20}, QColor(0, 102, 0), this);
            layout->addWidget(greenOne, (i + 13) * 2, (j + 13) * 2, 1, 2);
            greenPlayer.addPawn(greenOne);
        }
    }

    return {bluePlayer, redPlayer, yellowPlayer, greenPlayer};
}

void MainWindow::addDice(QPointer<QGridLayout> &layout) {
    QPointer<Die> die = new Die(this);
    layout->addWidget(die, 0, 38, 6, 6);

    QPointer<Die> secondDie = new Die(this);
    layout->addWidget(secondDie, 0, 44, 6, 6);

    QPointer<QPushButton> rollButton = new QPushButton("Roll Dice", this);
    rollButton->setStyleSheet("background-color: white; color: black;");

    connect(rollButton, &QPushButton::released, [&, this]() {
        Die ignoreThis;

        function<void(Die *)> lambda = [&](Die *die) {
            die->roll();
            this->repaint();
        };
        iterateThroughLayout(ignoreThis, lambda);
    });

    layout->addWidget(rollButton, 7, 39, 1, 10);
}

void MainWindow::addDialogueBox(QPointer<QGridLayout> &layout) {
    QPointer<QLabel> label = new QLabel(this);

    label->setWordWrap(true);
    label->setContentsMargins(5, label->contentsMargins().top(), 5, label->contentsMargins().bottom());
    label->textInteractionFlags().setFlag(Qt::TextInteractionFlag::TextEditable, false);
    label->textInteractionFlags().setFlag(Qt::TextInteractionFlag::TextSelectableByMouse, true);

    label->setText("Welcome!");
    label->setStyleSheet("background-color: white; color: black;");
    layout->addWidget(label, 10, 39, 28, 10);
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

void MainWindow::play(const vector<Player> &players) {

}


bool MainWindow::canMove(bool firstClick, const Player &activePlayer, const QPointer<Tile> &tile, int spaces) {
    if (qobject_cast<HomeTile *>(tile)) return !firstClick;
    if (qobject_cast<StartTile *>(tile)) return firstClick && spaces == 5;

    RectangleTile ignoreThis;

    bool moveIsPossible = false;

    // TODO still must add the getting to home row
    // 67 is the max number of tiles that go around (not counting "home stretch" tiles)
    function<void(RectangleTile *)> findMatchingTile = [&](RectangleTile *rectangleTile) {
        int currentTileNum = qobject_cast<RectangleTile *>(tile)->getNumber();
        if ((rectangleTile->getNumber() <= 67 && rectangleTile->getNumber() == currentTileNum + spaces) &&
            !rectangleTile->isBlockaded() &&
            (!(rectangleTile->isSafe && rectangleTile->isOccupied()))) { // end tile can be moved to

            RectangleTile ignoreThis;
            bool blockadePresent = false;
            function<void(RectangleTile *)> findBlockades = [&](RectangleTile *recTile) {
                if (recTile->getNumber() > currentTileNum && rectangleTile->getNumber() < currentTileNum + spaces) {
                    if (!recTile->isBlockaded()) {
                        blockadePresent = true;
                    }
                }
            };
            iterateThroughLayout(ignoreThis, findBlockades); // make sure tiles in between aren't blockaded

            if (!blockadePresent) {
                cout << "Found!" << endl; // TODO not tested in the slightest
                moveIsPossible = true;
            }
        }
        if (rectangleTile->getNumber() > 67) {

        }
    };

    iterateThroughLayout(ignoreThis, findMatchingTile);

    return moveIsPossible;
}
