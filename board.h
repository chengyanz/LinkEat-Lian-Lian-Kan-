#ifndef BOARD_H
#define BOARD_H
#include "location.h"
#include "picture.h"
#include <iostream>
#include <QList>
using namespace std;


class Board {
public:
    static const int NUM_ROWS = 10;         // The number of rows on any boar1d.
    static const int NUM_COLS = 10;         // The number of columns on any board.
    static Board EMPTY;           // The empty board.

    enum State{WIN, LOSE, NOT_OVER, DEAD};
    static QList<Location> LOCATIONS;
    Board();
    void setBoard(int diff);
    void regenerate();
    void setBoardSize(int NUM_ROW, int NUM_COL);
    void setDifficulty(int d);
    int getDifficulty() const;
    Board &update(Location loc1, Location loc2);
    int getScore();
    int getRemainGrid();
    Picture getPicture(int row, int col) const;
    Picture getPicture(Location loc) const;

private:
    int numOfGrids;
    int RemainGrid;
    int score;
    int difficulty;
    Picture board[NUM_ROWS][NUM_COLS];  // contains NUM_ROWS x NUM_COLS
    bool ifWin;
    State state;

};

#endif // BOARD_H
