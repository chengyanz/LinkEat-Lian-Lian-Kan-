#ifndef GAME_H
#define GAME_H
#include <QWidget>
#include <QList>
#include "image.h"
#include "board.h"

struct Gui;

class Game: public QWidget
{
    Q_OBJECT

public:
    bool inGame;
    bool hasPrevGame;
    Game();
    // Return the integer representing a distinct picture at row r and column c,
    // where 0 represents a blank picture.
    int getPicture(int r, int c) const;
    bool isValid(Location &loc1, Location &loc2, QList<Location> &path) const;
    bool hasSolution();
    void setDifficulty(int diff);
    void setTimeFromDiff(int);
    void setTime(int tt);
    int getTime() const;
    int getDifficulty();
    void checkEnd();

signals:
    void update(Game &game, Location loc1, Location loc2);
    void rearrange(Game &game);
    void gameEnd();
    void updateTime();
public slots:
    void slotRearrange();
    void slotImageClicked(Location loc);

private:
    bool horizontalNoTurning(const int r, const int cs, const int cg,
                             QList<Location> &path) const;
    bool verticalNoTurning(const int c, const int rs, const int rg,
                           QList<Location> &path) const;
    bool horizontal1Turning(const int r1, const int r2, const int c1, const int c2,
                            QList<Location> &path)
    const;
    bool vertical1Turning(const int r1, const int r2, const int c1, const int c2,
                          QList<Location> &path)
    const;
    bool horizontal2turningsShort(const int r1, const int r2, const int c1,
                                  const int c2, int &r1f, int &r2f,
                                  QList<Location> &path) const;
    bool vertical2turningsShort(const int r1, const int r2, const int c1,
                                const int c2, int &c1f, int &c2f,
                                QList<Location> &path) const;
    bool horizontal2turningsLong(const int r1, const int r2, const int c1,
                                 const int c2, int &r1f, int &r2f,
                                 QList<Location> &path) const;
    bool vertical2turningsLong(const int r1, const int r2, const int c1,
                               const int c2, int &c1f, int &c2f,
                               QList<Location> &path) const;
    Board board;
    int clickTimes;
    int t; //time
    Location loc1;
    Location loc2;
};

#endif // GAME_H
