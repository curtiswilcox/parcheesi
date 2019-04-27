//
// Created by Curtis Wilcox on 2019-04-05.
//

#ifndef PARCHEESI_PAWN_H
#define PARCHEESI_PAWN_H

#include <QPainter>
#include <QPen>
#include <QPointer>
#include <QWidget>


///////////////////////////////////////////////////////////////////////////
// Dimensions struct declaration
///////////////////////////////////////////////////////////////////////////

struct Dimensions {
    int height;
    int width;

    void operator*=(double multiplier);
    void operator/=(int multiplier);
};

///////////////////////////////////////////////////////////////////////////
// PawnStatus enum declaration
///////////////////////////////////////////////////////////////////////////

enum PawnStatus {
    START, PLAYING, HOME
};

///////////////////////////////////////////////////////////////////////////
// Pawn class declaration
///////////////////////////////////////////////////////////////////////////

class Pawn : public QWidget {
Q_OBJECT
private:
    Dimensions dimensions = {0, 0};
    const QColor color;
    PawnStatus status = START;
    bool passedZeroTile = false; // TODO false
public:
    const std::string team;
    int id;
    int currentTileNum;
    const std::function<void(QPointer<Pawn>)> lambda;
    const int MAX_TILE;

    explicit Pawn(const Dimensions &d, QColor c = Qt::GlobalColor::white, QWidget *parent = nullptr);
    explicit Pawn(const Dimensions &d, int currentTileNum, int id, const std::string &team,
                  const std::function<void(QPointer<Pawn>)> &lambda, QColor c = Qt::GlobalColor::white,
                  QWidget *parent = nullptr);

    inline const Dimensions &getDimensions() const { return this->dimensions; }

    inline const PawnStatus &getStatus() const { return this->status; }

    inline const QColor &getColor() const { return this->color; }

    inline bool hasPassedZeroTile() const { return this->passedZeroTile; }

    inline void setStatus(PawnStatus status) { this->status = status; }

    inline void setPassedZeroTile(bool pass) { this->passedZeroTile = pass; }

    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    inline friend bool operator==(const Pawn &lhs, const Pawn &rhs);

    inline friend bool operator!=(const Pawn &lhs, const Pawn &rhs);

    friend std::ostream &operator <<(std::ostream &outs, const Pawn &pawn);
};

#endif //PARCHEESI_PAWN_H
