//
// Created by Curtis Wilcox on 2019-03-04.
//

#include <fstream>
#include <iostream>
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


    auto newGame = [&, this]() {
        startWindow = new QWidget(this, Qt::Window);
        this->hide();
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), startWindow, SLOT(close()));
        startWindow->resize(780, 600);
        startWindow->setWindowTitle("New Game of Parcheesi");


//        QPointer<QGroupBox> menuBox = new QGroupBox(startWindow);

        QPointer<QButtonGroup> colorChoice = new QButtonGroup(startWindow);
        QPointer<QRadioButton> colorRed = new QRadioButton("Red", startWindow);
        QPointer<QRadioButton> colorBlue = new QRadioButton("Blue", startWindow);
        QPointer<QRadioButton> colorGreen = new QRadioButton("Green", startWindow);
        QPointer<QRadioButton> colorYellow = new QRadioButton("Yellow", startWindow);
        colorChoice->addButton(colorRed, 1);
        colorChoice->addButton(colorBlue, 2);
        colorChoice->addButton(colorGreen, 3);
        colorChoice->addButton(colorYellow, 4);

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

            iterateThroughLayout(startWindow->layout(), ignoreThis, lambda);
            settings.setValue("playerColor", colorChoiceId);
            settings.setValue("numPlayers", val);
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
    vector<Player> players = createBoard(layout);
    this->setLayout(layout);
//    this->hide();
//    newGame();

//    this->play(players);
}


//MainWindow::~MainWindow() {
//    for (int i = 0; i < this->layout()->count(); ++i) {
//        auto item = this->layout()->itemAt(i);
//        if (auto widItem = dynamic_cast<QWidgetItem *>(item)) {
//            if (auto t = dynamic_cast<Die *>(widItem->widget())) {
//                this->layout()->removeWidget(t);
//                delete t;
//                t = nullptr;
//            }
//        }
//    }
//}

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

//    addStartMenu(layout);
    addStartTiles(layout);
    addHomeTiles(layout);
    addGeneralTiles(layout);
    vector<Player> players = addPawns(layout);
    addDice(layout);
    addDialogueBox(layout);

    this->hide();

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
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j) { // red "home" tiles

            if ((i < 2 && j == 0) || (i >= 2 && j == 7)) continue; // skip safe spaces at end of home tiles

            QPointer<Tile> tile = new RectangleTile(-1, i % 2 == 0 ? horizontal : vertical, getPathColor(i), this);

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
    Player bluePlayer;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> blueOne = new Pawn({10, 20}, QColor(0, 0, 153), this);
            layout->addWidget(blueOne, (i + 2) * 2, (j + 2) * 2, 1, 2);
            bluePlayer.addPawn(blueOne);
        }
    }

    Player redPlayer;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> redOne = new Pawn({10, 20}, QColor(153, 0, 0), this);
            layout->addWidget(redOne, (i + 2) * 2, (j + 13) * 2, 1, 2);
            redPlayer.addPawn(redOne);
        }
    }

    Player yellowPlayer;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            QPointer<Pawn> yellowOne = new Pawn({10, 20}, QColor(153, 153, 0), this);
            layout->addWidget(yellowOne, (i + 13) * 2, (j + 2) * 2, 1, 2);
            yellowPlayer.addPawn(yellowOne);
        }
    }

    Player greenPlayer;
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
//        QPointer<Die> ignoreThis = new Die(this);
        Die ignoreThis;

        function<void(Die *)> lambda = [&](Die *die) {
            die->roll();
            this->repaint();
        };
        iterateThroughLayout(this->layout(), ignoreThis, lambda);

//        iterateThroughLayout(ignoreThis, [this](QWidgetItem *item) {
//            if (auto d = qobject_cast<Die *>(item->widget())) {
//                d->roll();
//                this->repaint();
//            }
//        });
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


bool MainWindow::canMove(const Player &activePlayer, const QPointer<Tile> &tile, int spaces) {
    if (qobject_cast<HomeTile *>(tile)) return false;
    if (qobject_cast<StartTile *>(tile)) return spaces == 5;

//    QPointer<RectangleTile> ignoreThis = new RectangleTile(this);
    RectangleTile ignoreThis;

    bool found = false;
//    iterateThroughLayout(ignoreThis, [&](QWidgetItem *item) {
//        if (auto t = dynamic_cast<RectangleTile *>(item->widget())) {
//            if ((t->getNumber() == qobject_cast<RectangleTile *>(tile)->getNumber() + spaces) &&
//                !t->isBlockaded() &&
//                (!(t->isSafe && t->isOccupied()))) {
//                cout << "Found!" << endl; // TODO not tested in the slightest
//                found = true;
//            }
//        }
//    });

    function<void(RectangleTile *)> lambda = [&](RectangleTile *rectangleTile) {
        if ((rectangleTile->getNumber() == qobject_cast<RectangleTile *>(tile)->getNumber() + spaces) &&
            !rectangleTile->isBlockaded() &&
            (!(rectangleTile->isSafe && rectangleTile->isOccupied()))) {

            cout << "Found!" << endl; // TODO not tested in the slightest
            found = true;
        }
    };

    iterateThroughLayout(this->layout(), ignoreThis, lambda);

//    for (int i = 0; i < this->layout()->count(); ++i) {
//        auto item = this->layout()->itemAt(i);
//        if (auto widItem = dynamic_cast<QWidgetItem *>(item)) {
//            if (auto t = dynamic_cast<RectangleTile *>(widItem->widget())) {
//                if ((t->getNumber() == qobject_cast<RectangleTile *>(tile)->getNumber() + spaces) &&
//                    !t->isBlockaded() &&
//                    (!(t->isSafe && t->isOccupied()))) {
//                    cout << "Found!" << endl; // TODO not tested in the slightest
//                    return true;
//                }
//            }
//        }
//    }
    return found;
}
