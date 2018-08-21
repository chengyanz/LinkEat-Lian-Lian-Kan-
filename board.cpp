#include <random>
#include "picture.h"
#include "board.h"
#include "location.h"

Board::Board(){
    setBoard(5);
}

void Board::setBoard(int diff){
    difficulty = diff;
    this-> numOfGrids = difficulty * 2;
    RemainGrid = numOfGrids;


    for ( int i=0; i<NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            board[i][j] = Picture::A;
        }
    }


    this->state = NOT_OVER;
    this->ifWin = false;
    for ( int r=0; r<NUM_ROWS; r++){
        for (int c=0; c<NUM_COLS; c++){
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dist(0, NUM_PIC - 1);
            int random = dist(gen);
            if(board[r][c]== Picture::A){
                board[r][c] = static_cast<Picture>(random);


                int r1 = r;
                int c1 = c;
                while(board[r1][c1]!=Picture::A){
                    do{
                        random_device rd1;
                        mt19937 gen1(rd1());
                        uniform_int_distribution<> dist1(0, NUM_ROWS - 1);
                        r1 = dist1(gen1);


                        random_device rd2;
                        mt19937 gen2(rd2());
                        uniform_int_distribution<> dist2(0, NUM_COLS - 1);
                        c1 = dist2(gen2);
                    }while(r1 == r && c1 == c);
                }
                board[r1][c1] = static_cast<Picture>(random);
                if(board[r][c] != Picture::A)
                    numOfGrids = numOfGrids - 2;
                if(numOfGrids == 0)
                    break;
            }
        }
        if(numOfGrids == 0)
            break;
    }

}

int Board::getDifficulty() const{
    return difficulty;
}


void Board::setDifficulty(int d){
    this->difficulty = d;
}




Board& Board::update(Location loc1, Location loc2){
    this->board[loc1.getRow()][loc1.getCol()] = Picture::A;
    this->board[loc2.getRow()][loc2.getCol()] = Picture::A;


    RemainGrid = RemainGrid -2 ;


    if(RemainGrid ==0){
        ifWin = true;
    }


    if (ifWin)
        this->state = WIN;
    else
        this->state = NOT_OVER;
    return *this;
}

int Board::getRemainGrid(){
    return RemainGrid;
}


Picture Board::getPicture(int row, int col) const{
    return board[row][col];
}


Picture Board::getPicture(Location loc) const{
    return board[loc.getRow()][loc.getCol()];
}


void Board::regenerate(){


    vector<Picture*> vec;
    for (int i = 0; i<NUM_ROWS ; i++) {
        for (int j = 0; j<NUM_COLS; j++){
            if (board[i][j]!=Picture::A)
                vec.push_back(&board[i][j]);
        }
    }


    for(int k = 0; k < vec.size(); k++){


        random_device rd3;
        mt19937 gen(rd3());
        uniform_int_distribution<> dist(0, vec.size()-1);
        int r3 = dist(gen);


        Picture temp = *vec[k];
        *vec[k] = *vec[r3];
        *vec[r3] = temp;
    }
}
