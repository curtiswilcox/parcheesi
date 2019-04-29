//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QAction>
#include <QLabel>
#include <QGroupBox>
#include <QButtonGroup>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QShortcut>
#include <QScrollArea>
#include <QSettings>
#include <QShortcut>
#include <QSizePolicy>

#include "MainWindow.h"

using namespace std;
using Qt::GlobalColor;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), settings("CS205", "Parcheesi") {
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
    QPointer<QAction> gameMenu = fileMenu->addAction("&New Game");
    gameMenu->setShortcut(Qt::CTRL + Qt::Key_N);
    gameMenu->setShortcut(Qt::Key_Space);


    auto newGame = [&, this]() {
        startWindow = new QWidget(this, Qt::Window);
        this->hide();
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), startWindow, SLOT(close()));
        startWindow->resize(780, 600);
        startWindow->setWindowTitle("New Game of Parcheesi");


//        QPointer<QGroupBox> menuBox = new QGroupBox(startWindow);

        QPointer<QButtonGroup> colorChoice = new QButtonGroup(startWindow);
        QPointer<QRadioButton> colorBlue = new QRadioButton("Blue", startWindow);
        QPointer<QRadioButton> colorRed = new QRadioButton("Red", startWindow);
        QPointer<QRadioButton> colorGreen = new QRadioButton("Green", startWindow);
        QPointer<QRadioButton> colorYellow = new QRadioButton("Yellow", startWindow);
        colorChoice->addButton(colorBlue, 0);
        colorChoice->addButton(colorRed, 1);
        colorChoice->addButton(colorGreen, 2);
        colorChoice->addButton(colorYellow, 3);

        QPointer<QSpinBox> numPlayers = new QSpinBox(startWindow);
        numPlayers->setStyleSheet("background-color: white; color: black;");
        numPlayers->setMinimum(2);
        numPlayers->setMaximum(4);
        numPlayers->setSingleStep(1);
        numPlayers->setValue(4);


        QPointer<QPushButton> startButton = new QPushButton("Start Game", startWindow);
        startButton->setStyleSheet("background-color: white; color: black;");

        QPointer<QButtonGroup> startGroup = new QButtonGroup(startWindow);

        QPointer<QVBoxLayout> vbox = new QVBoxLayout(startWindow);
        QPointer<QLabel> colorLabel = new QLabel(startWindow);
        colorLabel->setText("Choose your color:");

        QPointer<QLabel> playersLabel = new QLabel(startWindow);
        playersLabel->setText("Choose your number of players:");

        vbox->addWidget(colorLabel);
        vbox->addWidget(colorRed);
        vbox->addWidget(colorBlue);
        vbox->addWidget(colorGreen);
        vbox->addWidget(colorYellow);

        vbox->addWidget(playersLabel);
        vbox->addWidget(numPlayers);

        vbox->addWidget(startButton);
        vbox->addStretch(1);
        vbox->setAlignment(Qt::AlignCenter);
//        menuBox->setLayout(vbox);
        startWindow->setLayout(vbox);

        connect(startButton, &QPushButton::released, this, [colorChoice, this]() {
            int val;
            int colorChoiceId = colorChoice->checkedId();
            QSpinBox ignoreThis;
            function<void(QSpinBox *)> lambda = [&](QSpinBox *box) {
                val = box->value();
                cout << val << endl;
                cout << colorChoiceId << endl;
            };

            iterateThroughLayout(lambda);
            settings.setValue("playerColor", colorChoiceId);
            settings.setValue("numPlayers", val);
            this->players = this->resetBoard();

            if (colorChoiceId == 0) { // blue
                settings.setValue("currentPlayer", QColor(0, 0, 153));
            } else if (colorChoiceId == 1) { // red
                settings.setValue("currentPlayer", QColor(153, 0, 0));
            } else if (colorChoiceId == 2) { // yellow
                settings.setValue("currentPlayer", QColor(153, 153, 0));
            } else {
                settings.setValue("currentPlayer", QColor(0, 102, 0));
            }

            this->show();
            startWindow->hide();
        });

        // create start menu window (use code from rules window) - call in constructor
        // hide MainWindow (this->hide())
        // add the options
        // when user selects and starts game, "save" preferences to MW
        // close start menu and show MW

        startWindow->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored); // disable user resizing
        startWindow->setFixedSize(startWindow->width(), startWindow->height());
        startWindow->show();

    };

    connect(new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this), &QShortcut::activated, newGame);
    connect(gameMenu, &QAction::triggered, this, newGame);

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
    layout->setSpacing(Tile::TILE_SPACING);
    QPointer<QLabel> startLabel = new QLabel(this);
    startLabel->setText("Press CMD+N to start");
    layout->addWidget(startLabel);
    this->setLayout(layout);
    settings.setValue("currentPlayer", players[0].color);
}


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

//    addStartMenu(layout);
    addStartTiles(layout);
    addHomeTile(layout);
    addGeneralTiles(layout);
    vector<Player> players = addPawns(layout);
    addDice(layout);
    addNextButton(layout);
    addDialogueBox(layout);

    this->hide();

    return players;
}

vector<Player> MainWindow::resetBoard() {

    QLayoutItem *item;
    QLayout *sublayout;
    QWidget *widget;
    while ((item = this->layout()->takeAt(0))) {
        if ((sublayout = item->layout()) != 0) {/* do the same for sublayout*/}
        else if ((widget = item->widget()) != 0) {
            widget->hide();
            delete widget;
        }
        else { delete item; }
    }

    QPointer<QGridLayout> layout = new QGridLayout(this);
    vector<Player> players = createBoard(layout);
    delete this->layout();
    this->setLayout(layout);

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
    Player yellowPlayer(QColor(153, 153, 0), 3); // yellow
    Player greenPlayer(QColor(0, 102, 0), 2); // green

    function<void(QPointer<Pawn>)> movePawnLambda = [&, this](QPointer<Pawn> pawn) {
//        this->movePawn(pawn, 1, pawn->MAX_TILE);

        if (pawn->getStatus() == PawnStatus::PLAYING) {
            if (this->canMove(pawn, 1)) {
                this->movePawnTest(pawn, 1);
            }
        } else if (pawn->getStatus() == PawnStatus::START) {
            if (this->canMove(pawn, 5)) {
                this->movePawnTest(pawn, 1);
            }
        }

    };

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int row = (i + 3) * 2;
            int column = (j + 3) * 2;

            QPointer<Pawn> blueOne = new Pawn({10, 10}, StartTile::BLUE_START_NUM, (2 * i) + j, "Blue", movePawnLambda,
                                              QColor(0, 0, 153),
                                              this);
//            blueOne->setPassedZeroTile(true);
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
            redOne->setPassedZeroTile(true);
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
            yellowOne->setPassedZeroTile(true);
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
            greenOne->setPassedZeroTile(true);
            this->pawnLocations["GreenStart" + to_string(greenOne->id)] = make_tuple(row, column);
            tuple<int, int> location = this->pawnLocations["GreenStart" + to_string(greenOne->id)];
            settings.setValue(QString::fromStdString("green" + to_string(greenOne->id)), StartTile::GREEN_START_NUM);
            layout->addWidget(greenOne, row, column, 1, 1);
            greenPlayer.addPawn(greenOne);
        }
    }

    return {bluePlayer, redPlayer, greenPlayer, yellowPlayer};
}

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

        gameOutput.push_back(QString::fromStdString("You rolled ") +
                             QString::fromStdString(to_string(settings.value("firstRoll").toInt())) +
                             QString::fromStdString(", ") +
                             QString::fromStdString(to_string(settings.value("secondRoll").toInt())));
        updateScroll();
    });

    layout->addWidget(rollButton, 7, 39, 1, 10);
}

void MainWindow::addNextButton(QPointer<QGridLayout> &layout) {
    QPointer<QPushButton> nextButton = new QPushButton("Next Turn", this);
    nextButton->setStyleSheet("background-color: white; color: black;");
    settings.setValue("doubleCount", 0);
    settings.setValue("rollWasDoubles", false);

    connect(nextButton, &QPushButton::released, [&, this]() {
        int currId;

        for (const Player &player : this->players) {
            if (player.color == settings.value("currentPlayer").value<QColor>()) {
                this->play(player);
                currId = player.id;
            }
        }
        if (!settings.value("rollWasDoubles").toBool() || settings.value("doubleCount").toInt() >= 3) {
            settings.setValue("currentPlayer", players[(currId + 1) % 4].color);
            settings.setValue("doubleCount", 0);
            settings.setValue("rollWasDoubles", false);
        }
    });

    layout->addWidget(nextButton, 9, 39, 1, 10);
}

void MainWindow::addDialogueBox(QPointer<QGridLayout> &layout) {

    QPointer<QLabel> label = new QLabel(this);

    label->setWordWrap(true);
    label->setContentsMargins(5, label->contentsMargins().top(), 5, label->contentsMargins().bottom());
    label->textInteractionFlags().setFlag(Qt::TextInteractionFlag::TextEditable, false);
    label->textInteractionFlags().setFlag(Qt::TextInteractionFlag::TextSelectableByMouse, true);
    gameOutput.push_back("Welcome!");
    label->setText("Welcome!");
    label->setStyleSheet("background-color: white; color: black;");
    layout->addWidget(label, 12, 39, 26, 10);
}


//bool MainWindow::movePawn(const QPointer<Pawn> &pawn, int spaces, int pawnMax, bool backToStart) {
//    if (backToStart) {
//        function<void(StartTile *)> lambda = [&, this](StartTile *start) {
//            if (tolower(start->getColorString()) == tolower(pawn->team)) {
//                if (tolower(pawn->team) == "blue") {
//                    pawn->currentTileNum = StartTile::BLUE_START_NUM;
//                } else if (tolower(pawn->team) == "red") {
//                    pawn->currentTileNum = StartTile::RED_START_NUM;
//                } else if (tolower(pawn->team) == "yellow") {
//                    pawn->currentTileNum = StartTile::YELLOW_START_NUM;
//                } else {
//                    pawn->currentTileNum = StartTile::GREEN_START_NUM;
//                }
//
//                string capitalizedTeam = (char) std::toupper(pawn->team[0]) + (string(pawn->team).erase(0, 1));
//                tuple<int, int> initialStart = this->pawnLocations[capitalizedTeam + "Start" + to_string(pawn->id)];
//                this->layout()->removeWidget(pawn);
//                pawn->setPassedZeroTile(false);
//                dynamic_cast<QGridLayout *>(this->layout())->addWidget(pawn, get<0>(initialStart), get<1>(initialStart),
//                                                                       1, 1);
//            }
//        };
//        iterateThroughLayout(lambda);
//
//        return true;
//    }
//    // else...
//    int tileToMoveTo = (pawn->currentTileNum + spaces);// % 68;
//    if (pawn->currentTileNum + spaces > 67) pawn->setPassedZeroTile(true);
//
//    string tileType;
//
//    if (pawn->hasPassedZeroTile() && pawn->currentTileNum <= pawnMax && tileToMoveTo > pawnMax) {
//        int used = pawnMax - pawn->currentTileNum;
//        tileToMoveTo = used + jump(pawn) + (spaces - used) + pawn->currentTileNum - 1;
//
//        tileType = pawn->team + "Home" + to_string(tileToMoveTo - jump(pawn) - pawnMax);
//
//        cout << tileType << endl;
////        cout << "first" << endl;
//    } else if (pawn->hasPassedZeroTile() && pawn->currentTileNum >= pawnMax + jump(pawn) &&
//               pawn->currentTileNum + spaces < pawnMax + jump(pawn) + 7) { // seven home stretch tiles
//
//        tileToMoveTo = (pawn->currentTileNum) + spaces;
//        tileType = pawn->team + "Home" + to_string(tileToMoveTo - jump(pawn) - pawnMax);
//
//        cout << tileType << endl;
////        cout << "second" << endl;
//    } else if (pawn->hasPassedZeroTile() && pawn->currentTileNum >= pawnMax + jump(pawn) &&
//               pawn->currentTileNum + spaces == pawnMax + jump(pawn) + 7) { // home tile
//
//        tileToMoveTo = -5; // because moving to the home tile
////        cout << "third" << endl;
//        tileType = "HomeTile" + pawn->team + to_string(pawn->id);
//    } else if (pawn->currentTileNum + spaces > pawnMax + jump(pawn) + 7) { // past the home tile
////        cout << "fourth" << endl;
//        return false; // can't move up the home stretch
//    } else {
//        tileToMoveTo %= 68;
//        tileType = "NormalTile" + to_string(tileToMoveTo);
//        cout << "fifth" << endl;
//    }
//
//    RectangleTile *prevTile = nullptr;
//    RectangleTile *nextTile = nullptr;
//    HomeTile *home = nullptr;
//    function<void(Tile *)> lambda = [&, this](Tile *t) {
//        if (auto recTile = dynamic_cast<RectangleTile *>(t)) {
//            if (recTile->getNumber() == pawn->currentTileNum) {
//                prevTile = recTile;
//            } else if (recTile->getNumber() == tileToMoveTo) {
//                nextTile = recTile;
//            }
//        } else if (auto homeTile = dynamic_cast<HomeTile *>(t)) {
//            if (pawn->currentTileNum + spaces == pawnMax + jump(pawn) + 7) { // to get to the home tile
//                home = homeTile;
//            }
//        }
//    };
//    iterateThroughLayout(lambda);
//
//    bool bumpPlayerOffStart = nextTile ? (nextTile->isOccupied() && !nextTile->isBlockaded() &&
//                                          // determine if an incoming pawn can
//                                          (*nextTile->getOccupyingPawn())->team != pawn->team &&
//                                          // knock an enemy pawn off of its
//                                          ((nextTile->getNumber() - 1 == StartTile::BLUE_START_NUM && // first tile
//                                            tolower(pawn->team) == "blue") ||
//                                           (nextTile->getNumber() - 1 == StartTile::RED_START_NUM &&
//                                            tolower(pawn->team) == "red") ||
//                                           (nextTile->getNumber() - 1 == StartTile::YELLOW_START_NUM &&
//                                            tolower(pawn->team) == "yellow") ||
//                                           (nextTile->getNumber() - 1 == StartTile::GREEN_START_NUM &&
//                                            tolower(pawn->team) == "green")))
//
//                                       : false;
//    bool moveSuccessful = false;
//    if ((nextTile && (!nextTile->isOccupied() ||
//                      (nextTile->isOccupied() &&
//                       (*nextTile->getOccupyingPawn())->team == pawn->team &&
//                       !nextTile->isBlockaded()) ||
//                      bumpPlayerOffStart)) || home) {
//
//        this->layout()->removeWidget(pawn);
//
//        cout << "Tile to move from: " << pawn->currentTileNum << endl;
//        cout << "Tile to move to: " << tileToMoveTo << endl;
//        tuple<int, int> pawnLocationToMove;
//
//        string aOrBPosition = (nextTile && nextTile->isOccupied() ? "b" : "a");
//        // TODO move pawn from b to a if it's now alone
//
//        if (((pawn->getStatus() != PawnStatus::HOME && tileToMoveTo >= pawnMax + jump(pawn) && // ends on home stretch
//              tileToMoveTo < pawnMax + jump(pawn) + 7) ||
//             (pawn->currentTileNum <= pawnMax &&
//              tileToMoveTo >= pawnMax + jump(pawn)))) {
//
//            pawnLocationToMove = this->pawnLocations[tileType + aOrBPosition];
//            cout << tileType + aOrBPosition << endl;
////            cout << "first move" << endl;
////            moveSuccessful = true;
//        } else if (home) { // get to home tile
//            pawnLocationToMove = this->pawnLocations[tileType];
//            pawn->setStatus(HOME);
//            cout << tileType << endl;
////            moveSuccessful = true;
////            cout << "second move" << endl;
//        } else {//} if (tileToMoveTo <= pawnMax || tileToMoveTo > pawnMax) {
//            pawnLocationToMove = this->pawnLocations[tileType + aOrBPosition];
//            cout << tileType + aOrBPosition << endl;
////            moveSuccessful = true;
////            cout << "third move" << endl;
//        }
//
//        pawn->currentTileNum = tileToMoveTo;
//        auto gridLayout = dynamic_cast<QGridLayout *>(this->layout());
//        gridLayout->addWidget(pawn, get<0>(pawnLocationToMove), get<1>(pawnLocationToMove), 1, 1);
//
//        if (bumpPlayerOffStart) {
//            QPointer<Pawn> occupyingPawn = *(nextTile->getOccupyingPawn());
//            movePawn(occupyingPawn, 0, pawnMax, true);
//            nextTile->removePawn(); // guaranteed to not be nullptr
//        }
//
//        prevTile->removePawn();
//
//        if (nextTile && !home) {
//            nextTile->addPawn(pawn);
//            pawn->setStatus(PLAYING);
//        }
//        moveSuccessful = true;
//    } else if ((nextTile && nextTile->isOccupied() && !nextTile->isBlockaded() && !nextTile->isSafe) ||
//               (nextTile && !nextTile->isBlockaded() && pawn->getStatus() == PawnStatus::START && bumpPlayerOffStart)) {
//
//        this->layout()->removeWidget(pawn);
//
//        pawn->currentTileNum = tileToMoveTo;
//        tuple<int, int> pawnLocationToMove = this->pawnLocations[tileType + to_string(pawn->currentTileNum) + "a"];
//        auto gridLayout = dynamic_cast<QGridLayout *>(this->layout());
//        gridLayout->addWidget(pawn, get<0>(pawnLocationToMove), get<1>(pawnLocationToMove), 1, 1);
//
//        QPointer<Pawn> occupyingPawn = *(nextTile->getOccupyingPawn());
//        movePawn(occupyingPawn, 0, pawnMax, true);
//        prevTile->removePawn();
//        nextTile->removePawn();
//        nextTile->addPawn(pawn);
//        moveSuccessful = true;
//    }
//    return moveSuccessful;
//}


bool MainWindow::movePawnTest(const QPointer<Pawn> &pawn, int spaces) {
    bool moveSuccessful = false;

    tuple<int, string> nextTileInfo = getTileInformation(pawn, spaces);
    int tileToMoveTo = get<0>(nextTileInfo);
    string tileInfo = get<1>(nextTileInfo);

    RectangleTile *prevTile = nullptr;
    RectangleTile *nextTile = nullptr;
    HomeTile *homeTile = nullptr;

    function<void(Tile *)> findTilesLambda = [&](Tile *tile) {
        if (auto recTile = dynamic_cast<RectangleTile *>(tile)) {
            if (recTile->getNumber() == pawn->currentTileNum) prevTile = recTile;
            else if (recTile->getNumber() == tileToMoveTo) nextTile = recTile;
        } else if (auto home = dynamic_cast<HomeTile *>(tile)) {
            if (tileToMoveTo == -5) homeTile = home;
        }
    };
    iterateThroughLayout(findTilesLambda);

    bool bumpPlayerOffStart = nextTile && (nextTile->isOccupied() && !nextTile->isBlockaded() &&
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
                                             tolower(pawn->team) == "green")));

    string aOrBPosition;
    if (nextTile && !nextTile->isOccupied()) {
        aOrBPosition = "a";
    } else if (nextTile && nextTile->isOccupied() && !nextTile->isBlockaded() &&
               (*nextTile->getOccupyingPawn())->team != pawn->team) {
        aOrBPosition = "a";
    } else if (nextTile && nextTile->isOccupied() && !nextTile->isBlockaded() && !nextTile->occupiedSpaceA()) {
        aOrBPosition = "a";
    } else if (nextTile && nextTile->isOccupied() && !nextTile->isBlockaded() && !nextTile->occupiedSpaceB()) {
        aOrBPosition = "b";
    }

    if (homeTile) {
        this->layout()->removeWidget(pawn);
        tuple<int, int> locationToMove = this->pawnLocations[tileInfo];
        dynamic_cast<QGridLayout *>(this->layout())->addWidget(pawn, get<0>(locationToMove), get<1>(locationToMove));
        pawn->currentTileNum = tileToMoveTo;
        pawn->setStatus(PawnStatus::HOME);
        prevTile->removePawn();
        moveSuccessful = true;
    } else if (nextTile) {

        if ((nextTile->isOccupied() && !nextTile->isBlockaded() && !nextTile->isSafe &&
             (*nextTile->getOccupyingPawn())->team != pawn->team) ||
            (!nextTile->isBlockaded() && pawn->getStatus() == PawnStatus::START && bumpPlayerOffStart)) {
            QPointer<Pawn> occupyingPawn = *nextTile->removePawn();

            function<void(StartTile *)> lambda = [&, this](StartTile *start) {
                if (tolower(start->getColorString()) == tolower(occupyingPawn->team)) {
                    if (tolower(occupyingPawn->team) == "blue") {
                        occupyingPawn->currentTileNum = StartTile::BLUE_START_NUM;
                    } else if (tolower(occupyingPawn->team) == "red") {
                        occupyingPawn->currentTileNum = StartTile::RED_START_NUM;
                    } else if (tolower(occupyingPawn->team) == "yellow") {
                        occupyingPawn->currentTileNum = StartTile::YELLOW_START_NUM;
                    } else {
                        occupyingPawn->currentTileNum = StartTile::GREEN_START_NUM;
                    }

                    string capitalizedTeam =
                            (char) std::toupper(occupyingPawn->team[0]) + (string(occupyingPawn->team).erase(0, 1));

                    tuple<int, int> initialStart = this->pawnLocations[capitalizedTeam + "Start" +
                                                                       to_string(occupyingPawn->id)];
                    this->layout()->removeWidget(occupyingPawn);
                    dynamic_cast<QGridLayout *>(this->layout())->addWidget(occupyingPawn, get<0>(initialStart),
                                                                           get<1>(initialStart), 1, 1);
                    occupyingPawn->setStatus(PawnStatus::START);
                }
            };
            iterateThroughLayout(lambda);
        }

        this->layout()->removeWidget(pawn);
        tuple<int, int> locationToMove = this->pawnLocations[tileInfo + aOrBPosition];
        dynamic_cast<QGridLayout *>(this->layout())->addWidget(pawn, get<0>(locationToMove),
                                                               get<1>(locationToMove));
        pawn->currentTileNum = tileToMoveTo;

        if (prevTile) {
//            QPointer<Pawn> removed = *prevTile->removePawn();
//            if (removed != pawn) {
            prevTile->removePawn(); // should be correct pawn
//                prevTile->addPawn(removed);
//            nextTile->addPawn(pawn);
//            }
        }
        nextTile->addPawn(pawn);
        cout << nextTile->occupiedSpaceA() << ", " << nextTile->occupiedSpaceB() << ", " << nextTile->isBlockaded()
             << endl;
        moveSuccessful = true;

        if (pawn->getStatus() == PawnStatus::START) {
            pawn->setStatus(PawnStatus::PLAYING);
        }
    }

    return moveSuccessful;
}

tuple<int, string> MainWindow::getTileInformation(const QPointer<Pawn> &pawn, int spaces) const {
    int tileToMoveTo = pawn->currentTileNum + spaces;
    string tileInformation;

    if (pawn->getStatus() == PawnStatus::PLAYING &&
        tileToMoveTo == pawn->MAX_TILE + jump(pawn) + 7) { // move to the home tile
        tileToMoveTo = -5;
        tileInformation = "HomeTile" + pawn->team + to_string(pawn->id);
    } else if (pawn->getStatus() == PawnStatus::PLAYING && pawn->currentTileNum >= pawn->MAX_TILE + jump(pawn) &&
               tileToMoveTo >= pawn->MAX_TILE + jump(pawn) &&
               tileToMoveTo < pawn->MAX_TILE + jump(pawn) + 7) {
        // pawn is on home stretch and is moving to home stretch

        tileInformation = pawn->team + "Home" + to_string(tileToMoveTo - jump(pawn) - pawn->MAX_TILE);
        if (tileInformation == pawn->team + "Home7") {
            tileInformation = "HomeTile" + pawn->team + to_string(pawn->id);
        }

    } else if (pawn->hasPassedZeroTile() && pawn->getStatus() == PawnStatus::PLAYING &&
               pawn->currentTileNum <= pawn->MAX_TILE &&
               tileToMoveTo > pawn->MAX_TILE) {
        // pawn passed/is on 0, move to home stretch

        int leftover = (pawn->currentTileNum + spaces) - pawn->MAX_TILE;
        tileToMoveTo = pawn->MAX_TILE + jump(pawn) + leftover - 1;
        tileInformation = pawn->team + "Home" + to_string(tileToMoveTo - jump(pawn) - pawn->MAX_TILE);
    } else if (!pawn->hasPassedZeroTile() && tolower(pawn->team) == "blue" &&
               pawn->getStatus() == PawnStatus::PLAYING &&
               pawn->currentTileNum > pawn->MAX_TILE &&
               pawn->currentTileNum < pawn->MAX_TILE + jump(pawn) &&
               tileToMoveTo > pawn->MAX_TILE &&
               tileToMoveTo - pawn->currentTileNum < 0) {
        // pawn hasn't passed 0, moving to home stretch (probably just for blue)

        int distToZeroTile = 68 - pawn->currentTileNum;
        spaces -= distToZeroTile;
        spaces -= pawn->MAX_TILE;
        tileToMoveTo = pawn->MAX_TILE + jump(pawn) + spaces - 1;
        tileInformation = pawn->team + "Home" + to_string(tileToMoveTo - jump(pawn) - pawn->MAX_TILE);
        pawn->setPassedZeroTile(true);
    } else {
        // pawn's movement has nothing to do with the home stretch

        if (pawn->currentTileNum + spaces > 67) pawn->setPassedZeroTile(true);
        tileToMoveTo %= 68;
        tileInformation = "NormalTile" + to_string(tileToMoveTo);
    }

    cout << pawn->team << ": " << pawn->currentTileNum << " -> " << tileToMoveTo << " @" << tileInformation << endl;
    return make_pair(tileToMoveTo, tileInformation);
}


void MainWindow::updateLabelText(const QString &text) {
    function<void(QLabel *)> lambda = [&](QLabel *info) {

        info->setText(info->text() + "\n" + text);
        info->repaint();
    };
    iterateThroughLayout(lambda);
}

void MainWindow::updateScroll() {
    function<void(QLabel *)> lambda = [&](QLabel *info) {
        info->setText("");
        int max = 20;
        if (gameOutput.size() >= max) {
            for (int i = gameOutput.size() - max; i < gameOutput.size(); i++) {

                info->setText(info->text() + "\n" + gameOutput[i]);
                info->repaint();
            }
        } else {
            for (QString text : gameOutput) {

                info->setText(info->text() + "\n" + text);
                info->repaint();
            }
        }
    };
    iterateThroughLayout(lambda);
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
    QSettings settings("CS205", "Parcheesi");

    function<void(Die *)> rollDice = [&](Die *die) {
        die->roll();
        die->repaint();
        settings.setValue(QString::fromStdString(die->name) + "Roll", /*5*/die->getValue());
    };
    iterateThroughLayout(rollDice);

//    cout << settings.value("firstRoll").toInt() << settings.value("secondRoll").toInt() << endl;

    if (settings.value("firstRoll").toInt() == settings.value("secondRoll").toInt()) {
        settings.setValue("rollWasDoubles", true);
        settings.setValue("doubleCount", settings.value("doubleCount").toInt() + 1);
    } else {
        settings.setValue("rollWasDoubles", false);
    }

    if (player.color == players[settings.value("playerColor").toInt()].color) {
        gameOutput.push_back("You rolled " +
                             QString::fromStdString(to_string(settings.value("firstRoll").toInt())) +
                             QString::fromStdString(", ") +
                             QString::fromStdString(to_string(settings.value("secondRoll").toInt())));
        if (settings.value("doubleCount").toInt() < 3) {
            this->gameOutput.push_back("Please move " + QString(std::toupper(player.getColorString()[0])) +
                                       QString::fromStdString(player.getColorString().erase(0, 1)));
            updateScroll();
//        playerTurn(player);
        } else {
            this->gameOutput.push_back("Too many doubles");
            updateScroll();
            moveFarthestToStart(player);
        }
    } else {
        this->gameOutput.push_back("It is " + QString(std::toupper(player.getColorString()[0])) +
                                   QString::fromStdString(player.getColorString().erase(0, 1)) + "'s turn!");
        gameOutput.push_back(QString(std::toupper(player.getColorString()[0])) +
                             QString::fromStdString(player.getColorString().erase(0, 1)) +
                             QString::fromStdString(" rolled ") +
                             QString::fromStdString(to_string(settings.value("firstRoll").toInt())) +
                             QString::fromStdString(", ") +
                             QString::fromStdString(to_string(settings.value("secondRoll").toInt())));
        updateScroll();
        if (settings.value("doubleCount").toInt() == 3) {
            this->gameOutput.push_back("Too many doubles");
            updateScroll();
            moveFarthestToStart(player);
            return;
        } else {
            cpuTurn(player);
        }
    }
}


bool MainWindow::canMove(const QPointer<Pawn> &pawn, int spaces) const {
    bool good = true;

    int current = pawn->currentTileNum;
    int max = pawn->MAX_TILE;
    int wantsToMoveTo = get<0>(getTileInformation(pawn, spaces));
    string homeStart = toupper(pawn->team + "_START_NUM");
    if (pawn->team == "Blue" && spaces == 5 && pawn->getStatus() == START) {
        wantsToMoveTo = StartTile::BLUE_START_NUM + 1;
    } else if (pawn->team == "Red" && spaces == 5 && pawn->getStatus() == START) {
        wantsToMoveTo = StartTile::RED_START_NUM + 1;
    } else if (pawn->team == "Green" && spaces == 5 && pawn->getStatus() == START) {
        wantsToMoveTo = StartTile::GREEN_START_NUM + 1;
    } else if (spaces == 5 && pawn->getStatus() == START) {
        wantsToMoveTo = StartTile::YELLOW_START_NUM + 1;
    }

    function<void(RectangleTile *)> blockadeAndSafeLambda = [&](RectangleTile *tile) { // FIXME this doesn't quite work
        int tileNum = tile->getNumber();

        if (tile->isBlockaded()) {
            cout << "Blockaded: " << tile->getNumber() << endl;
            cout << "Aiming to get to tile " << wantsToMoveTo << endl;
            if (wantsToMoveTo != -5) { // magic number for home tile
                if (wantsToMoveTo < 68 && current > max && current < tileNum && tileNum <= wantsToMoveTo) {
                    good = false;
                } else if (current <= max && wantsToMoveTo <= max && current < tileNum && tileNum <= wantsToMoveTo) {
                    good = false;
                } else if (wantsToMoveTo >= max + jump(pawn) && wantsToMoveTo < max + jump(pawn) + 7) {
                    if (current < tileNum && tileNum <= max) {
                        good = false;
                    } else if (current < tileNum && tileNum >= max + jump(pawn) && tileNum <= wantsToMoveTo) {
                        good = false;
                    }
                }
            } else {
                if (current <= max) {
                    if (current < tileNum && tileNum <= max) {
                        good = false;
                    }
                } else if (current >= max + jump(pawn)) {
                    if (current < tileNum && tileNum > max + jump(pawn) && tileNum < max + jump(pawn) + 7) {
                        good = false;
                    }
                }
//                good = true;
            }
        }
        if (tileNum == wantsToMoveTo && tile->isSafe &&
            tile->isOccupied() && (*tile->getOccupyingPawn())->team != pawn->team) {
            good = false;
        }
    };
    iterateThroughLayout(blockadeAndSafeLambda);

    if (pawn->getStatus() == PawnStatus::START && spaces != 5) good = false;
    else if (pawn->getStatus() == PawnStatus::HOME) good = false;
    else if (pawn->getStatus() == PawnStatus::PLAYING && current + spaces > max + jump(pawn) + 7) good = false;

    cout << "Returning " << (good ? "true" : "false") << endl;
    return good;
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

//int MainWindow::jump(int startNum, int spaces, const Player &player) const {
//    if (startNum + spaces > player.MAX_TILE) {
//        if (tolower(player.getColorString()) == "blue") {
//            return startNum + spaces + 60;
//        } else if (tolower(player.getColorString()) == "red") {
//            return startNum + spaces + 30;
//        } else if (tolower(player.getColorString()) == "yellow") {
//            return startNum + spaces + 50;
//        } else { // green
//            return startNum + spaces + 40;
//        }
//    } else {
//        return startNum + spaces;
//    }
//}

void MainWindow::cpuTurn(const Player &player) {

    int bigger = max(settings.value("firstRoll").toInt(), settings.value("secondRoll").toInt());
    int smaller = min(settings.value("firstRoll").toInt(), settings.value("secondRoll").toInt());
//    bigger = 5;
//    smaller = 5;

    bool biggerUsed = false;
    bool smallerUsed = false;

//    cout << player.numPawnsStart() << endl;

    for (const QPointer<Pawn> &pawn : player.pawns) {
        if (pawn->getStatus() == PawnStatus::START) {
            if (!biggerUsed && bigger == 5 && canMove(pawn, 5)) {
//                if (this->movePawn(pawn, 1, pawn->MAX_TILE)) {
                if (this->movePawnTest(pawn, 1)) { // just moving off start tile
                    biggerUsed = true;
                }
            } else if (!smallerUsed && smaller == 5 && canMove(pawn, 5)) {
//                if (this->movePawn(pawn, 1, pawn->MAX_TILE)) {
                if (this->movePawnTest(pawn, 1)) { // just moving off start tile
                    smallerUsed = true;
                }
            } else if (!biggerUsed && !smallerUsed && bigger + smaller == 5 && canMove(pawn, 5)) {
//                if (this->movePawn(pawn, 1, pawn->MAX_TILE)) {
                if (this->movePawnTest(pawn, 1)) { // just moving off start tile
                    biggerUsed = true;
                    smallerUsed = true;
                }
            }
        }
    }
    for (const QPointer<Pawn> &pawn : player.pawns) {
        if (pawn->getStatus() == PawnStatus::PLAYING) {
            if (!biggerUsed && !smallerUsed && canMove(pawn, bigger + smaller)) {
//                if (this->movePawn(pawn, bigger + smaller, pawn->MAX_TILE)) {
                if (this->movePawnTest(pawn, bigger + smaller)) {
                    biggerUsed = true;
                    smallerUsed = true;
                }
            } else {
                if (!biggerUsed && canMove(pawn, bigger)) {
//                    if (this->movePawn(pawn, bigger, pawn->MAX_TILE)) {
                    if (this->movePawnTest(pawn, bigger)) {
                        biggerUsed = true;
                    }
                }
                if (!smallerUsed && canMove(pawn, smaller)) {
//                    if (this->movePawn(pawn, smaller, pawn->MAX_TILE)) {
                    if (this->movePawnTest(pawn, smaller)) {
                        smallerUsed = true;
                    }
                }
            }
        }
    }
    cout << endl << endl;
}

void MainWindow::moveFarthestToStart(const Player &player) {
    QPointer<Pawn> farthestPawn = nullptr;
    for (const QPointer<Pawn> pawn : player.pawns) {
        if (farthestPawn) {
            int farLocation;
            int currLocation;

            if (farthestPawn->hasPassedZeroTile()) {
                farLocation = farthestPawn->currentTileNum + 68;
            } else {
                farLocation = farthestPawn->currentTileNum;
            }

            if (pawn->hasPassedZeroTile()) {
                currLocation = pawn->currentTileNum + 68;
            } else {
                currLocation = pawn->currentTileNum;
            }
            if (currLocation > farLocation) {
                farthestPawn = pawn;
            }
        } else {
            farthestPawn = pawn;
        }
    }

    function<void(StartTile *)> lambda = [&, this](StartTile *start) {
        if (tolower(start->getColorString()) == tolower(farthestPawn->team)) {
            if (tolower(farthestPawn->team) == "blue") {
                farthestPawn->currentTileNum = StartTile::BLUE_START_NUM;
            } else if (tolower(farthestPawn->team) == "red") {
                farthestPawn->currentTileNum = StartTile::RED_START_NUM;
            } else if (tolower(farthestPawn->team) == "yellow") {
                farthestPawn->currentTileNum = StartTile::YELLOW_START_NUM;
            } else {
                farthestPawn->currentTileNum = StartTile::GREEN_START_NUM;
            }

            string capitalizedTeam =
                    (char) std::toupper(farthestPawn->team[0]) + (string(farthestPawn->team).erase(0, 1));

            tuple<int, int> initialStart = this->pawnLocations[capitalizedTeam + "Start" +
                                                               to_string(farthestPawn->id)];
            this->layout()->removeWidget(farthestPawn);
            dynamic_cast<QGridLayout *>(this->layout())->addWidget(farthestPawn, get<0>(initialStart),
                                                                   get<1>(initialStart), 1, 1);
            farthestPawn->setStatus(PawnStatus::START);
        }
    };
    iterateThroughLayout(lambda);
}

string MainWindow::tolower(const string &s) const {
    string lowered;
    for (char c : s) {
        lowered += (char) std::tolower(c);
    }
    return lowered;
}

string MainWindow::toupper(const string &s) const {
    string uppered;
    for (char c : s) {
        uppered += (char) std::toupper(c);
    }
    return uppered;
}
