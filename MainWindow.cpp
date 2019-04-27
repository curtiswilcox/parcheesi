//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QAction>
#include <QLabel>
#include <QPushButton>
#include <QTime>
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
    this->players = createBoard(layout);
    this->setLayout(layout);

    settings.setValue("currentPlayer", players[0].color);
//    for (int i = 0; i < 20; i++) {
//        this->play(players[0]);
//        this->play(players[1]);
//        this->play(players[2]);
//        this->play(players[3]);
//    }
}

//void MainWindow::mainLoop(QPointer<QGridLayout> &layout, vector<Player> players) {
//    settings.setValue("currentPlayer", players.at(0).color);
//    for (int i = 0; i < 50; i++) {
//        this->play(players[0]);
//        this->play(players[1]);
//        this->play(players[2]);
//        this->play(players[3]);
//    }
//}



#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"

QString MainWindow::readRules() {
    ifstream infile;
    infile.open("../rules.txt");
    if (!infile.is_open()) {
        infile.open("rules.txt");
    }
    string rules;

    while (infile && !infile.eof()) {
        string line;
        getline(infile, line);
        rules += line;
    }
    infile.close();
    return QString::fromStdString(rules);
}

#pragma clang diagnostic pop


vector<Player> MainWindow::createBoard(QPointer<QGridLayout> &layout) {
    layout->setSpacing(Tile::TILE_SPACING);

    addStartTiles(layout);
    addHomeTile(layout);
    addGeneralTiles(layout);
    vector<Player> players = addPawns(layout);
    addDice(layout);
    addNextButton(layout);
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

void MainWindow::addHomeTile(QPointer<QGridLayout> &layout) {
    QPointer<HomeTile> home = new HomeTile({240, 240}, this);
    layout->addWidget(home, 16, 16, 6, 6);
    for (int i = 0; i < 4; ++i) {
        this->pawnLocations["HomeTileBlue" + to_string(i)] = make_tuple(16, 17 + i);
    }

    for (int i = 0; i < 4; ++i) {
        this->pawnLocations["HomeTileRed" + to_string(i)] = make_tuple(17 + i, 21);
    }

    for (int i = 0; i < 4; ++i) {
        this->pawnLocations["HomeTileYellow" + to_string(i)] = make_tuple(17 + i, 16);
    }

    for (int i = 0; i < 4; ++i) {
        this->pawnLocations["HomeTileGreen" + to_string(i)] = make_tuple(21, 17 + i);
    }
//    this->pawnLocations["HomeTileBlue0"] = make_tuple(16, 17);
//    this->pawnLocations["HomeTileBlue1"] = make_tuple(16, 18);
//    this->pawnLocations["HomeTileBlue2"] = make_tuple(16, 19);
//    this->pawnLocations["HomeTileBlue3"] = make_tuple(16, 20);

//    this->pawnLocations["HomeTileRed0"] = make_tuple(17, 21);
//    this->pawnLocations["HomeTileRed1"] = make_tuple(18, 21);
//    this->pawnLocations["HomeTileRed2"] = make_tuple(19, 21);
//    this->pawnLocations["HomeTileRed3"] = make_tuple(20, 21);

//    this->pawnLocations["HomeTileYellow0"] = make_tuple(17, 16);
//    this->pawnLocations["HomeTileYellow1"] = make_tuple(18, 16);
//    this->pawnLocations["HomeTileYellow2"] = make_tuple(19, 16);
//    this->pawnLocations["HomeTileYellow3"] = make_tuple(20, 16);

//    this->pawnLocations["HomeTileGreen0"] = make_tuple(21, 17);
//    this->pawnLocations["HomeTileGreen1"] = make_tuple(21, 18);
//    this->pawnLocations["HomeTileGreen2"] = make_tuple(21, 19);
//    this->pawnLocations["HomeTileGreen3"] = make_tuple(21, 20);
}

void MainWindow::addGeneralTiles(QPointer<QGridLayout> &layout) {
    Dimensions horizontal = {30, 80};
    Dimensions vertical = {80, 30};

    horizontal *= 2;
    vertical *= 2;

    int tileCounter = 0;
    vector<int> safeNums = {3, 13, 20, 30, 37, 47, 54, 64}; // starts top right of home, goes widdershins

    int lastRecTileNum = 67;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) { // "home stretch" tiles

            if ((i < 2 && j == 0) || (i >= 2 && j == 7)) continue; // skip safe spaces at end of home tiles

            QPointer<Tile> tile = new RectangleTile(++lastRecTileNum, i % 2 == 0 ? horizontal : vertical,
                                                    getPathColor(i), this);

            switch (i) {
                case 0: {
                    int row = j * 2;
                    int column = 18;
                    layout->addWidget(tile, row, column, 2, 2); // blue
                    this->pawnLocations["BlueHome" + to_string(j - 1) + "a"] = make_tuple(row, column);
                    this->pawnLocations["BlueHome" + to_string(j - 1) + "b"] = make_tuple(row, column + 1);
//                    cout << "BlueHome" + to_string(j - 1) + "a" << endl;
                    break;
                }
                case 1: {
                    int row = 18;
                    int column = j * 2;
                    layout->addWidget(tile, row, column, 2, 2); // yellow
                    this->pawnLocations["YellowHome" + to_string(j - 1) + "a"] = make_tuple(row, column);
                    this->pawnLocations["YellowHome" + to_string(j - 1) + "b"] = make_tuple(row + 1, column);
//                    cout << "YellowHome" + to_string(j - 1) + "a" << endl;
                    break;
                }
                case 2: {
                    int use = 6 - j;
                    int row = (use + 11) * 2;
                    int column = 18;

                    layout->addWidget(tile, row, column, 2, 2); // green
                    this->pawnLocations["GreenHome" + to_string(j) + "a"] = make_tuple(row, column);
//                    cout << "GreenHome" + to_string(j) + "a" << endl;
                    this->pawnLocations["GreenHome" + to_string(j) + "b"] = make_tuple(row, column);
                    break;
                }
                case 3: {
                    int use = 6 - j;
                    int row = 18;
                    int column = (use + 11) * 2;

                    layout->addWidget(tile, row, column, 2, 2); // red
                    this->pawnLocations["RedHome" + to_string(j) + "a"] = make_tuple(row, column);
                    this->pawnLocations["RedHome" + to_string(j) + "b"] = make_tuple(row + 1, column);
                    break;
                }
                default:
                    break;
            }

        }

        for (int j = 0; j < 2; ++j) { // other surrounding tiles
            if (j == 1) {
                QPointer<Tile> tile = new RectangleTile(tileCounter++, i % 2 == 0 ? horizontal : vertical,
                                                        GlobalColor::cyan, this);
                if (i == 0) { // these are safe tiles around the edge (at the bottom of each "home stretch"
                    layout->addWidget(tile, 0, 18, 2, 2); // 0, 18
                    this->pawnLocations["NormalTile" + to_string(tileCounter - 1) + "a"] = make_tuple(0, 18);
                    this->pawnLocations["NormalTile" + to_string(tileCounter - 1) + "b"] = make_tuple(0, 18 + 1);
                } else if (i == 1) {
                    layout->addWidget(tile, 18, 0, 2, 2); // 18, 0
                    this->pawnLocations["NormalTile" + to_string(tileCounter - 1) + "a"] = make_tuple(18, 0);
                    this->pawnLocations["NormalTile" + to_string(tileCounter - 1) + "b"] = make_tuple(18 + 1, 0);
                } else if (i == 2) {
                    layout->addWidget(tile, 36, 18, 2, 2); // 36, 18
                    this->pawnLocations["NormalTile" + to_string(tileCounter - 1) + "a"] = make_tuple(36, 18);
                    this->pawnLocations["NormalTile" + to_string(tileCounter - 1) + "b"] = make_tuple(36, 18 + 1);
                } else if (i == 3) {
                    layout->addWidget(tile, 18, 36, 2, 2); // 18, 36
                    this->pawnLocations["NormalTile" + to_string(tileCounter - 1) + "a"] = make_tuple(18, 36);
                    this->pawnLocations["NormalTile" + to_string(tileCounter - 1) + "b"] = make_tuple(18 + 1, 36);
                }
            }

            for (int k = 0; k < 8; ++k, ++tileCounter) { // all of the other tiles (could be safe, could be regular)
                QPointer<Tile> tile =
                        new RectangleTile(tileCounter, i % 2 == 0 ? horizontal : vertical,
                                          std::find(safeNums.begin(), safeNums.end(), tileCounter) != safeNums.end()
                                          ? GlobalColor::cyan : GlobalColor::white,
                                          this);
                if (i == 0) {
                    if (j == 0) {
                        int row = (14 - (2 * k));
                        int column = 10 * 2;
                        layout->addWidget(tile, row, column, 2, 2);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "a"] = make_tuple(row, column);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "b"] = make_tuple(row, column + 1);
                    } else {
                        int row = k * 2;
                        int column = 8 * 2;
                        layout->addWidget(tile, row, column, 2, 2);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "a"] = make_tuple(row, column);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "b"] = make_tuple(row, column + 1);
                    }

                } else if (i == 1) {
                    if (j == 0) {
                        int row = 8 * 2;
                        int column = (14 - (k * 2));
                        layout->addWidget(tile, row, column, 2, 2);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "a"] = make_tuple(row, column);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "b"] = make_tuple(row + 1, column);
                    } else {
                        int row = 10 * 2;
                        int column = k * 2;
                        layout->addWidget(tile, row, column, 2, 2);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "a"] = make_tuple(row, column);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "b"] = make_tuple(row + 1, column);
                    }

                } else if (i == 2) {
                    if (j == 0) {
                        int row = (k + 11) * 2;
                        int column = 8 * 2;
                        layout->addWidget(tile, row, column, 2, 2);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "a"] = make_tuple(row, column);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "b"] = make_tuple(row, column + 1);
                    } else {
                        int row = (14 + 4 - k) * 2;
                        int column = 10 * 2;
                        layout->addWidget(tile, row, column, 2, 2);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "a"] = make_tuple(row, column);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "b"] = make_tuple(row, column + 1);
                    }

                } else if (i == 3) {
                    if (j == 0) {
                        int row = 10 * 2;
                        int column = (k + 11) * 2;
                        layout->addWidget(tile, row, column, 2, 2);

                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "a"] = make_tuple(row, column);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "b"] = make_tuple(row + 1, column);
                    } else {
                        int row = 8 * 2;
                        int column = ((int) (14.5 * 2 - (k + 11)) * 2);
                        layout->addWidget(tile, row, column, 2, 2);

                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "a"] = make_tuple(row, column);
                        this->pawnLocations["NormalTile" + to_string(tileCounter) + "b"] = make_tuple(row + 1, column);
                    }
                }
            }
        }
    }
}

vector<Player> MainWindow::addPawns(QPointer<QGridLayout> &layout) {
    Player bluePlayer(QColor(0, 0, 153), 0); // blue
    Player redPlayer(QColor(153, 0, 0), 1); // red
    Player yellowPlayer(QColor(153, 153, 0), 2); // yellow
    Player greenPlayer(QColor(0, 102, 0), 3); // green

    function<void(QPointer<Pawn>)> movePawnLambda = [&, this](QPointer<Pawn> pawn) {
        this->movePawn(pawn, 1, pawn->MAX_TILE);
    };

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int row = (i + 3) * 2;
            int column = (j + 3) * 2;

            QPointer<Pawn> blueOne = new Pawn({10, 10}, StartTile::BLUE_START_NUM, (2 * i) + j, "Blue", movePawnLambda,
                                              QColor(0, 0, 153),
                                              this);
            this->pawnLocations["BlueStart" + to_string(blueOne->id)] = make_tuple(row, column);
            tuple<int, int> location = this->pawnLocations["BlueStart" + to_string(blueOne->id)];
            settings.setValue(QString::fromStdString("blue" + to_string(blueOne->id)), StartTile::BLUE_START_NUM);
            layout->addWidget(blueOne, row, column, 1, 1);
            bluePlayer.addPawn(blueOne);
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int row = (i + 3) * 2;
            int column = (j + 14) * 2;

            QPointer<Pawn> redOne = new Pawn({10, 10}, StartTile::RED_START_NUM, (2 * i) + j, "Red", movePawnLambda,
                                             QColor(153, 0, 0),
                                             this);
            this->pawnLocations["RedStart" + to_string(redOne->id)] = make_tuple(row, column);
            tuple<int, int> location = this->pawnLocations["RedStart" + to_string(redOne->id)];
            settings.setValue(QString::fromStdString("red" + to_string(redOne->id)), StartTile::RED_START_NUM);
            layout->addWidget(redOne, row, column, 1, 1);
            redPlayer.addPawn(redOne);
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int row = (i + 14) * 2;
            int column = (j + 3) * 2;

            QPointer<Pawn> yellowOne = new Pawn({10, 10}, StartTile::YELLOW_START_NUM, (2 * i) + j, "Yellow",
                                                movePawnLambda, QColor(153, 153, 0),
                                                this);
            this->pawnLocations["YellowStart" + to_string(yellowOne->id)] = make_tuple(row, column);
            tuple<int, int> location = this->pawnLocations["YellowStart" + to_string(yellowOne->id)];
            settings.setValue(QString::fromStdString("yellow" + to_string(yellowOne->id)),
                              StartTile::YELLOW_START_NUM);
            layout->addWidget(yellowOne, row, column, 1, 1);
            yellowPlayer.addPawn(yellowOne);
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int row = (i + 14) * 2;
            int column = (j + 14) * 2;

            QPointer<Pawn> greenOne = new Pawn({10, 10}, StartTile::GREEN_START_NUM, (2 * i) + j, "Green",
                                               movePawnLambda, QColor(0, 102, 0),
                                               this);
            this->pawnLocations["GreenStart" + to_string(greenOne->id)] = make_tuple(row, column);
            tuple<int, int> location = this->pawnLocations["GreenStart" + to_string(greenOne->id)];
            settings.setValue(QString::fromStdString("green" + to_string(greenOne->id)), StartTile::GREEN_START_NUM);
            layout->addWidget(greenOne, row, column, 1, 1);
            greenPlayer.addPawn(greenOne);
        }
    }

    return {bluePlayer, redPlayer, yellowPlayer, greenPlayer};
}


//void MainWindow::cpuTest() {
//    QPointer<QPushButton> cpuTurnButton = new QPushButton("Test CPU turn", this);
//    cpuTurnButton->setStyleSheet("background-color: white; color: black;");
//
//    Player bluePlayer(QColor(0, 0, 153)); // blue
//
//    function<void(QPointer<Pawn>)> movePawnLambda = [&, this](QPointer<Pawn> pawn) {
//        // cout << "moving" << endl;
//        if (tolower(pawn->team) == "blue") {
//            this->movePawn(pawn, 1,
//                           8); // accessing the constants from the Player instances isn't working for some reason
//        } else if (tolower(pawn->team) == "red") {
//            this->movePawn(pawn, 1, 59);
//        } else if (tolower(pawn->team) == "yellow") {
//            this->movePawn(pawn, 1, 25);
//        } else { // green
//            this->movePawn(pawn, 1, 42);
//        }
//    };
//
//
//
//    for (int i = 0; i < 2; i++) {
//        for (int j = 0; j < 2; j++) {
//            int row = (i + 3) * 2;
//            int column = (j + 3) * 2;
//
//            QPointer<Pawn> blueOne = new Pawn({10, 10}, StartTile::BLUE_START_NUM, (2 * i) + j, "Blue", movePawnLambda,
//                                              QColor(0, 0, 153),
//                                              this);
//            this->pawnLocations["BlueStart" + to_string(blueOne->id)] = make_tuple(row, column);
//            tuple<int, int> location = this->pawnLocations["BlueStart" + to_string(blueOne->id)];
//            settings.setValue(QString::fromStdString("blue" + to_string(blueOne->id)), StartTile::BLUE_START_NUM);
//            dynamic_cast<QGridLayout*>(this->layout())->addWidget(blueOne, row, column, 1, 1);
//            bluePlayer.addPawn(blueOne);
//        }
//    }
//
////    for (const QPointer<Pawn> &pawn : bluePlayer.pawns) {
////        cout << *pawn << endl;
////    }
//
//    connect(cpuTurnButton, &QPushButton::released, [&]() {
//        // create new player to test
//        cpuTurn(bluePlayer);
//    });
//
//    dynamic_cast<QGridLayout*>(this->layout())->addWidget(cpuTurnButton, 9, 39, 1, 10);
//}

void MainWindow::addDice(QPointer<QGridLayout> &layout) {
    QPointer<Die> die = new Die("first", this);
    layout->addWidget(die, 0, 38, 6, 6);

    QPointer<Die> secondDie = new Die("second", this);
    layout->addWidget(secondDie, 0, 44, 6, 6);

    QPointer<QPushButton> rollButton = new QPushButton("Roll Dice", this);
    rollButton->setStyleSheet("background-color: white; color: black;");

    connect(rollButton, &QPushButton::released, [&]() {
        function<void(Die *)> rollDice = [&](Die *die) {
            die->roll();
            die->repaint();
            settings.setValue(QString::fromStdString(die->name) + "Roll", die->getValue());
        };
        iterateThroughLayout(rollDice);
    });

    layout->addWidget(rollButton, 7, 39, 1, 10);
}

void MainWindow::addNextButton(QPointer<QGridLayout> &layout) {
    QPointer<QPushButton> nextButton = new QPushButton("Next turn", this);
    nextButton->setStyleSheet("background-color: white; color: black;");

    connect(nextButton, &QPushButton::released, [&, this]() {
        int currId;
        for (const Player &player : this->players) {
            if (player.color == settings.value("currentPlayer").value<QColor>()) {
                this->play(player);
                currId = player.id;
            }
        }
        settings.setValue("currentPlayer", players[(currId + 1) % 4].color);

    });

    layout->addWidget(nextButton, 9, 39, 1, 10);
}

void MainWindow::addDialogueBox(QPointer<QGridLayout> &layout) {
    QPointer<QScrollArea> scroll = new QScrollArea(this);
    QPointer<QLabel> gameOutput = new QLabel(scroll);

    gameOutput->setWordWrap(true);
    gameOutput->setContentsMargins(5, gameOutput->contentsMargins().top(), 5, gameOutput->contentsMargins().bottom());
    gameOutput->textInteractionFlags().setFlag(Qt::TextInteractionFlag::TextEditable, false);
    gameOutput->textInteractionFlags().setFlag(Qt::TextInteractionFlag::TextSelectableByMouse, true);

    gameOutput->setText("Welcome!");
    gameOutput->setStyleSheet("background-color: white; color: black;");
    scroll->setStyleSheet("background-color: white; color: black;");
    scroll->setWidget(gameOutput);
    layout->addWidget(scroll, 12, 39, 26, 10);
}


void MainWindow::movePawn(const QPointer<Pawn> &pawn, int spaces, int pawnMax, bool backToStart) {
    if (backToStart) {
        function<void(Tile *)> lambda = [&, this](Tile *tile) {
            if (auto start = dynamic_cast<StartTile *>(tile)) { // find the pawn's start tile
                if (tolower(start->getColorString()) == tolower(pawn->team)) {
                    if (tolower(pawn->team) == "blue") {
                        pawn->currentTileNum = StartTile::BLUE_START_NUM;
                    } else if (tolower(pawn->team) == "red") {
                        pawn->currentTileNum = StartTile::RED_START_NUM;
                    } else if (tolower(pawn->team) == "yellow") {
                        pawn->currentTileNum = StartTile::YELLOW_START_NUM;
                    } else {
                        pawn->currentTileNum = StartTile::GREEN_START_NUM;
                    }

                    string capitalizedTeam = (char) std::toupper(pawn->team[0]) + (string(pawn->team).erase(0, 1));
                    tuple<int, int> initialStart = this->pawnLocations[capitalizedTeam + "Start" + to_string(pawn->id)];
                    this->layout()->removeWidget(pawn);
                    pawn->setPassedZeroTile(false);
                    dynamic_cast<QGridLayout *>(this->layout())->addWidget(pawn, get<0>(initialStart),
                                                                           get<1>(initialStart), 1, 1);
                }
            }
        };
        iterateThroughLayout(lambda);

        return;
    }
    // else...
    int tileToMoveTo = (pawn->currentTileNum + spaces) % 68;
    if (pawn->currentTileNum + spaces > 67) pawn->setPassedZeroTile(true);

    string tileType;
    cout << "Moving " << *pawn << " from " << pawn->currentTileNum << " to " << tileToMoveTo << endl;
    if (pawn->hasPassedZeroTile() && pawn->currentTileNum <= pawnMax && tileToMoveTo > pawnMax) {
        tileToMoveTo = (pawn->currentTileNum) + jump(pawn) + (spaces - (pawnMax - pawn->currentTileNum)) - 1;
        tileType = pawn->team + "Home";
      
//        cout << tileType << endl;
//        cerr << "first" << endl;
    } else if (pawn->hasPassedZeroTile() && pawn->currentTileNum >= pawnMax + jump(pawn) &&
               pawn->currentTileNum + spaces < pawnMax + jump(pawn) + 7) { // seven home stretch tiles

        tileToMoveTo = (pawn->currentTileNum) + spaces;
        tileType = pawn->team + "Home";

//        cerr << "second" << endl;
    } else if (pawn->hasPassedZeroTile() && pawn->currentTileNum >= pawnMax + jump(pawn) &&
               pawn->currentTileNum + spaces == pawnMax + jump(pawn) + 7) { // home tile

//        tileToMoveTo = (pawn->currentTileNum) + spaces;
        tileToMoveTo = -5; // because moving to the home tile
        tileType = "HomeTile";
    } else if (pawn->currentTileNum + spaces > pawnMax + jump(pawn) + 7) { // past the home tile
        return; // can't move up the home stretch
    } else {
        tileType = "NormalTile";
    }

    RectangleTile *prevTile = nullptr;
    RectangleTile *nextTile = nullptr;
    HomeTile *home = nullptr;
    function<void(Tile *)> lambda = [&, this](Tile *t) {
        if (auto recTile = dynamic_cast<RectangleTile *>(t)) {
            if (recTile->getNumber() == pawn->currentTileNum) {
                prevTile = recTile;
            } else if (recTile->getNumber() == tileToMoveTo) {
                nextTile = recTile;
            }
        } else if (auto homeTile = dynamic_cast<HomeTile *>(t)) {
            if (pawn->currentTileNum + spaces == pawnMax + jump(pawn) + 7) { // to get to the home tile
                home = homeTile;
            }
        }
    };
    iterateThroughLayout(lambda);

    bool bumpPlayerOffStart = nextTile ? (nextTile->isOccupied() && !nextTile->isBlockaded() &&
                                          // determine if an incoming pawn can
                                          (*nextTile->getOccupyingPawn())->team != pawn->team &&
                                          // knock an enemy pawn off of its
                                          ((nextTile->getNumber() - 1 == StartTile::BLUE_START_NUM && // first tile
                                            tolower(pawn->team) == "blue") ||
                                           (nextTile->getNumber() - 1 == StartTile::RED_START_NUM &&
                                            tolower(pawn->team) == "red") ||
                                           (nextTile->getNumber() - 1 == StartTile::YELLOW_START_NUM &&
                                            tolower(pawn->team) == "yellow") ||
                                           (nextTile->getNumber() - 1 == StartTile::GREEN_START_NUM &&
                                            tolower(pawn->team) == "green")))

                                       : false;

    if ((nextTile && (!nextTile->isOccupied() ||

                      (nextTile->isOccupied() &&
                       (*nextTile->getOccupyingPawn())->team == pawn->team &&
                       !nextTile->isBlockaded()) ||
                      bumpPlayerOffStart)) || (home)) {

        this->layout()->removeWidget(pawn);

        pawn->currentTileNum = tileToMoveTo;
//        cerr << "New current tile " << pawn->currentTileNum << endl;
        tuple<int, int> pawnLocationToMove;

        string aOrBPosition = (nextTile && nextTile->isOccupied() ? "b" : "a");
        // TODO move pawn from b to a if it's now alone

        if (pawn->hasPassedZeroTile() && pawn->currentTileNum > pawnMax &&
            pawn->currentTileNum != pawnMax + jump(pawn) + 7) { // not the home tile
//            cout << tileType + to_string(nextTile->getNumber() - pawnMax - jump(pawn)) + aOrBPosition << endl;
            pawnLocationToMove = this->pawnLocations[
                    tileType + to_string(nextTile->getNumber() - pawnMax - jump(pawn)) + aOrBPosition
            ];
        } else if (home) { // get to home tile
            pawnLocationToMove = this->pawnLocations[tileType + pawn->team + to_string(pawn->id)];
        } else {
            pawnLocationToMove = this->pawnLocations[tileType + to_string(pawn->currentTileNum) + aOrBPosition];
        }

        auto gridLayout = dynamic_cast<QGridLayout *>(this->layout());
        gridLayout->addWidget(pawn, get<0>(pawnLocationToMove), get<1>(pawnLocationToMove), 1, 1);
        if (bumpPlayerOffStart) {
            QPointer<Pawn> occupyingPawn = *(nextTile->getOccupyingPawn());
            movePawn(occupyingPawn, 0, pawnMax, true);
            nextTile->removePawn(); // guaranteed to not be nullptr
        }

        prevTile->removePawn();
        pawn->setStatus(PLAYING);

        if (nextTile && !home) {
            nextTile->addPawn(pawn);
        }
    } else if ((nextTile && nextTile->isOccupied() && !nextTile->isBlockaded() && !nextTile->isSafe) ||
               (nextTile && !nextTile->isBlockaded() && pawn->getStatus() == PawnStatus::START && bumpPlayerOffStart)) {

        this->layout()->removeWidget(pawn);

        pawn->currentTileNum = tileToMoveTo;
        tuple<int, int> pawnLocationToMove = this->pawnLocations[tileType + to_string(pawn->currentTileNum) + "a"];
        auto gridLayout = dynamic_cast<QGridLayout *>(this->layout());
        gridLayout->addWidget(pawn, get<0>(pawnLocationToMove), get<1>(pawnLocationToMove), 1, 1);

        QPointer<Pawn> occupyingPawn = *(nextTile->getOccupyingPawn());
        movePawn(occupyingPawn, 0, pawnMax, true);
        prevTile->removePawn();
        nextTile->removePawn();
        nextTile->addPawn(pawn);
    }
}


void MainWindow::updateLabelText(const QString &text) {
    function<void(QLabel *)> lambda = [&](QLabel *info) {
        info->setText(info->text() + "\n" + text);
        info->repaint();
    };
    iterateThroughLayout(lambda);
}

void MainWindow::updateScroll() {

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

void MainWindow::play(const Player &player) {
    this->updateLabelText("It is " + QString(std::toupper(player.getColorString()[0])) +
                          QString::fromStdString(player.getColorString().erase(0, 1)) + "'s turn!");
//    this->updateScroll();
    cpuTurn(player);
}


bool MainWindow::canMove(const Player &activePlayer, const QPointer<Tile> &tile, int spaces) const {
    int currentTileNum = qobject_cast<RectangleTile *>(tile)->getNumber();

    if (qobject_cast<HomeTile *>(tile)) {
        return !jump(currentTileNum, spaces, activePlayer) - 1 == activePlayer.MAX_TILE;
    }
    if (qobject_cast<StartTile *>(tile)) {
        return spaces == 5;
    }

    bool moveIsPossible = false;

    // 67 is the max number of tiles that go around (not counting "home stretch" tiles)
    function<void(const RectangleTile *)> findMatchingTile = [&](const RectangleTile *rectangleTile) {
        int endTileNum = rectangleTile->getNumber();
        if (((endTileNum <= 67 && endTileNum == currentTileNum + spaces) ||
             ((endTileNum > 67 &&
               endTileNum == jump(currentTileNum, spaces, activePlayer)) ||
              jump(currentTileNum, spaces, activePlayer) == activePlayer.MAX_TILE - 1)) &&
            // TODO if the home button is clicked, its number is -1
            !rectangleTile->isBlockaded() &&
            (!(rectangleTile->isSafe && rectangleTile->isOccupied()))) { // end tile can be moved to

            bool blockadePresent = false;
            function<void(const RectangleTile *)> findBlockades = [&](const RectangleTile *recTile) {
//                if (recTile->getNumber() > currentTileNum && endTileNum < currentTileNum + spaces) {
//                    if (recTile->isBlockaded()) { // TODO don't think this is tested for blockades on home stretch, the lower one might fix it
//                        blockadePresent = true;
//                    }
//                }
                if ((endTileNum <= 67 && recTile->getNumber() > currentTileNum &&
                     endTileNum > currentTileNum + spaces) ||
                    (endTileNum > 67 && recTile->getNumber() > jump(currentTileNum, spaces, activePlayer) &&
                     endTileNum > jump(currentTileNum, spaces, activePlayer))) {
                    if (recTile->isBlockaded()) blockadePresent = true;
                }
            };
            iterateThroughLayout(findBlockades); // make sure tiles in between aren't blockaded

            if (!blockadePresent) {
//                cout << "Success!" << endl; // TODO not tested in the slightest
                moveIsPossible = true;
            }
        }
    };

    iterateThroughLayout(findMatchingTile);

    return moveIsPossible;
}

int MainWindow::jump(const QPointer<Pawn> &pawn) const {
    if (tolower(pawn->team) == "blue") {
        return 60;
    }
    if (tolower(pawn->team) == "red") {
        return 30;
    }
    if (tolower(pawn->team) == "yellow") {
        return 50;
    }
    return 40; // green
}

int MainWindow::jump(int startNum, int spaces, const Player &player) const {
    if (startNum + spaces > player.MAX_TILE) {
        if (tolower(player.getColorString()) == "blue") {
            return startNum + spaces + 60;
        } else if (tolower(player.getColorString()) == "red") {
            return startNum + spaces + 30;
        } else if (tolower(player.getColorString()) == "yellow") {
            return startNum + spaces + 50;
        } else { // green
            return startNum + spaces + 40;
        }
    } else {
        return startNum + spaces;
    }
}

void MainWindow::cpuTurn(const Player &player) {

    function<void(Die *)> rollDice = [&](Die *die) {
        die->roll();
        die->repaint();
        settings.setValue(QString::fromStdString(die->name) + "Roll", die->getValue());
    };
    iterateThroughLayout(rollDice);

    int dieOneValue = settings.value("firstRoll").toInt();
    bool dieOneUsed = false;
    int dieTwoValue = settings.value("secondRoll").toInt();
    bool dieTwoUsed = false;

    string playerTeam = player.colorString;
    vector<QPointer<Pawn>> playerPawns = player.pawns;
    int firstTileNum;
    int moveHereNum;
    int pawnMax;

    // declare initial tiles to move to from start
    if (tolower(playerTeam) == "blue") {
        firstTileNum = StartTile::BLUE_START_NUM + 1;
        pawnMax = 8;
    } else if (tolower(playerTeam) == "red") {
        firstTileNum = StartTile::RED_START_NUM + 1;
        pawnMax = 59;
    } else if (tolower(playerTeam) == "green") {
        firstTileNum = StartTile::GREEN_START_NUM + 1;
        pawnMax = 42;
    } else { // yellow
        firstTileNum = StartTile::YELLOW_START_NUM + 1;
        pawnMax = 25;
    }

    RectangleTile *firstFromStart;
    RectangleTile *moveHere;

    function<void(RectangleTile *)> lambda = [&](RectangleTile *tile) {
        if (tile->getNumber() == firstTileNum) {
            firstFromStart = tile;
        }
        if (tile->getNumber() == moveHereNum) {
            moveHere = tile;
        }
    };

    iterateThroughLayout(lambda);

    cout << playerTeam <<  " rolled " << dieOneValue << ", " << dieTwoValue << endl;

    for (const QPointer<Pawn> &pawn : playerPawns) {
        if (pawn->getStatus() == START) {
            if (dieOneValue == 5) {
                if (canMove(player, firstFromStart, 1) && !dieOneUsed) {
                    this->movePawn(pawn, 1, pawnMax);
                    dieOneUsed = true;
                }
            } else if (dieTwoValue == 5) {
                if (canMove(player, firstFromStart, 1) && !dieTwoUsed) {
                    this->movePawn(pawn, 1, pawnMax);
                    dieTwoUsed = true;
                }
            } else if (dieOneValue + dieTwoValue == 5 && !dieOneUsed && !dieTwoUsed) {
                if (canMove(player, firstFromStart, 1)) {
                    this->movePawn(pawn, 1, pawnMax);
                    dieOneUsed = true;
                    dieTwoUsed = true;
                }
            }
        } else if (pawn->getStatus() == PLAYING) {
            moveHereNum = (pawn->currentTileNum + dieOneValue + dieTwoValue) % 68;
            iterateThroughLayout(lambda);
            if (canMove(player, moveHere, dieOneValue + dieTwoValue) && !dieOneUsed && !dieTwoUsed) {
                this->movePawn(pawn, dieOneValue + dieTwoValue, pawnMax);
                dieOneUsed = true;
                dieTwoUsed = true;
            }
            moveHereNum = (pawn->currentTileNum + dieOneValue) % 68;
            iterateThroughLayout(lambda);
            if (canMove(player, moveHere, dieOneValue) && !dieOneUsed) {
                this->movePawn(pawn, dieOneValue, pawnMax);
                dieOneUsed = true;
            }
            moveHereNum = (pawn->currentTileNum + dieTwoValue) % 68;
            iterateThroughLayout(lambda);
            if (canMove(player, moveHere, dieTwoValue) && !dieTwoUsed) {
                this->movePawn(pawn, dieTwoValue, pawnMax);
                dieTwoUsed = true;
            }

        } else {
            cout << " we home safe and sound " << endl;
        }
    }
}

string MainWindow::tolower(const std::string &s) const {
    string lowered;
    for (char c : s) {
        lowered += (char) std::tolower(c);
    }
    return lowered;
}

string MainWindow::toupper(const std::string &s) const {
    string uppered;
    for (char c : s) {
        uppered += (char) std::toupper(c);
    }
    return uppered;
}
