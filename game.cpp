#include <QList>
#include "game.h"
#include "location.h"

Game::Game(): board(), clickTimes(0),loc1(0, 0), loc2(0, 0), inGame(false),
hasPrevGame(false), t(60) // t will be overwritten
{}

int Game::getPicture(int r, int c) const{
    return (int)board.getPicture(r, c);
}

void Game::setDifficulty(int diff){
    board.setBoard(diff);
}

int Game::getDifficulty(){
    return board.getDifficulty();
}

void Game::setTimeFromDiff(int diff){
    int tentative = static_cast<int>(diff * 3 - 0.01 * diff * diff);
    t = tentative > 20? tentative: 20; // At least 20 seconds
}

void Game::setTime(int tt){
    t = tt;
}

int Game::getTime() const{
    return t;
}

/*------------------------------------Game::isValid()----------------------------*/
/* Judge if two grids can be linked with recursion.
 * Return true if the two grids can be linked and false otherwise.
 * loc1     = location of one of the grid
 * loc2     = location of the other grid
 * path     = valid path, including loc1, turning location(s), and loc2, if the two
 *            grids can be linked
 */
bool Game::isValid(Location &loc1, Location &loc2, QList<Location> &path) const{
    int r1 = loc1.getRow(), r2 = loc2.getRow();
    int c1 = loc1.getCol(), c2 = loc2.getCol();
    int rs, rg, cs, cg; // rs is the smaller one within r1 and r2, rg the greater

    // For horizontal2turningsShort, r1f records how far the path can go starting
    // from loc1 towards loc2 in the row of loc1. For horizontal2turningsLong, the
    // r1f assigned by horizontal2turningsShort is used to judge if a long 2-turning
    // path is feasible, and then used to record how far the path can go starting
    // from loc1 deviating from loc2 both in the row of loc1 and the row of loc2.
    int r1f, r2f, c1f, c2f;

    QList<Location> tmp;
    bool flag;
    int pathLength = 0;

    // loc1 and loc2 cannot be the same location
    if(r1 == r2 && c1 == c2) return false;

    // loc1 and loc2 must have the same pictures
    if(board.getPicture(r1, c1) != board.getPicture(r2, c2)) return false;

    // loc1 must have a picture
    if(board.getPicture(r1,c1) == Picture::A) return false;

    cs = c1 < c2 ? c1 : c2;
    cg = c1 + c2 - cs;

    rs = r1 < r2 ? r1 : r2;
    rg = r1 + r2 - rs;

    // None of these functions called relies on the content of path

    // A none-turning path is only possible for two locations either in the same
    // row or in the same column.
    if(r1 == r2)
        if(horizontalNoTurning(r1, cs, cg, path))
            return true;

    if(c1 == c2)
        if(verticalNoTurning(c1, rs, rg, path))
            return true;

    // Check 1-turning paths and then 2-turning paths, first shorter paths and
    // then longer paths. For two locations in the same row or column, if they
    // cannot be linked by a none-turning path, they can only be linked by a
    // two turning path and the two parallel segments must be on the same side
    // of the remaining segment.
    if(r1 != r2 && c1 != c2){
        if(horizontal1Turning(r1, r2, c1, c2, path))
            return true;
        if(vertical1Turning(r1, r2, c1, c2, path))
            return true;
        if(horizontal2turningsShort(r1, r2, c1, c2, r1f, r2f, path))
            return true;

        if(vertical2turningsShort(r1, r2, c1, c2, c1f, c2f, path))
            return true;

        flag = horizontal2turningsLong(r1, r2, c1, c2, r1f, r2f, path);
        if(flag) pathLength = abs(path[0].getCol() - path[1].getCol()) +
                abs(path[1].getRow() - path[2].getRow()) +
                abs(path[2].getCol() - path[3]. getCol());
        if(vertical2turningsLong(r1, r2, c1, c2, c1f, c2f, tmp)) {
            // Overwrite path if a better path is found.
            if (pathLength > abs(tmp[0].getRow() - tmp[1].getRow()) +
                    abs(tmp[1].getCol() - tmp[2].getCol()) +
                    abs(tmp[2].getRow() - tmp[3].getRow()) ) {
                path[1] = tmp[1];
                path[2] = tmp[2]; // 0th and 3rd elements are already the same.
            }
            if(!flag){
                path.clear();
                path = tmp;
            }
            return true;
        }
        return flag;
    }
    else{
        if(c1 == c2)
            return horizontal2turningsLong(r1, r2, c1, c2, r1f, r2f, path);
        if(r1 == r2)
            return vertical2turningsLong(r1, r2, c1, c2, c1f, c2f, path);
    }

    return false;
}

// Check if loc1 and loc2 can be linked horizontally without any turning
bool Game::horizontalNoTurning(const int r, const int cs, const int cg,
                               QList<Location> &path) const {
    int i;

    path.clear();
    for (i = cs + 1; i < cg; i++)
        if (board.getPicture(r, i) != Picture::A)
            return false;

    path.push_back(Location::getLocation(r, cs));
    path.push_back(Location::getLocation(r, cg));
    return true;
}

// Check if loc1 and loc2 can be linked vertically without any turning
bool Game::verticalNoTurning(const int c, const int rs, const int rg,
                             QList<Location> &path) const {
    int i;

    path.clear();
    for (i = rs + 1; i < rg; i++)
        if (board.getPicture(i, c) != Picture::A)
            return false;

    path.push_back(Location::getLocation(rs, c));
    path.push_back(Location::getLocation(rg, c));
    return true;
}

// Check if loc1 and loc2 can be linked first horizontally (viewed by loc1) with 1
// turning
bool Game::horizontal1Turning(const int r1, const int r2, const int c1, const int c2,
                              QList<Location> &path)
const{
    int i;
    int inc;

    path.clear();
    inc = (c2 - c1) / abs(c2 - c1);
    // Whatever the sign of inc, i will be incremented to c2 if the return sentence
    // is not executed.
    for(i = c1 + inc; i != c2; i += inc)
        if(board.getPicture(r1, i) != Picture::A)
            return false;

    inc = (r2 - r1) / abs(r2 - r1);
    for(i = r1; i != r2; i += inc)
        if(board.getPicture(i, c2) != Picture::A)
            return false;

    path.push_back(Location::getLocation(r1, c1));
    path.push_back(Location::getLocation(r1, c2));
    path.push_back(Location::getLocation(r2, c2));
    return true;
}

// Check if loc1 and loc2 can be linked first vertically (viewed by loc1) with 1
// turning
bool Game::vertical1Turning(const int r1, const int r2, const int c1, const int c2,
                            QList<Location> &path)
const{
    int i;
    int inc;

    path.clear();
    inc = (r2 - r1) / abs(r2 - r1);
    for(i = r1 + inc; i != r2; i += inc)
        if(board.getPicture(i, c1) != Picture::A)
            return false;

    inc = (c2 - c1) / abs(c2 - c1);
    for(i = c1; i != c2; i += inc)
        if(board.getPicture(r2, i) != Picture::A)
            return false;

    path.push_back(Location::getLocation(r1, c1));
    path.push_back(Location::getLocation(r2, c1));
    path.push_back(Location::getLocation(r2, c2));
    return true;
}

// Check if loc1 and loc2 can be linked with two horizontal segments and one
// vertical segment (2 turnings) and the two horizontal segments are on differect
// sides of the veritcal segment
bool Game::horizontal2turningsShort(const int r1, const int r2, const int c1,
                                    const int c2, int &r1f, int &r2f,
                                    QList<Location> &path) const {
    int i, j;
    int incC, incR;

    path.clear();
    incC = (c2 - c1) / abs(c2 - c1);
    r1f = c1 + incC, r2f = c2 - incC;

    for (; r1f != c2; r1f += incC, r2f -= incC){
        // If a location with a picture is found in the row of loc1, continue
        // the scan of the row of loc2 to find the nearest location with a
        // picture.
        if(board.getPicture(r1, r1f) != Picture::A){
            for(;r2f != c1; r2f -= incC){
                if(board.getPicture(r2, r2f) != Picture::A)
                    break;
            }
            break;
        }
        if(board.getPicture(r2, r2f) != Picture::A){
            for(;r1f != c2; r1f += incC){
                if(board.getPicture(r1, r1f) != Picture::A)
                    break;
            }
            break;
        }
    }

    incR = (r2 - r1) / abs(r2 - r1);
    if(incC > 0) {
        // Scan the common columns bounded by r1f and r2f
        for (i = r2f + incC; i < r1f; i += incC){
            for(j = r1; j != r2; j += incR){
                if(board.getPicture(j, i) != Picture::A)
                    break;
            }
            if(j == r2){
                path.push_back(Location::getLocation(r1, c1));
                path.push_back(Location::getLocation(r1, i));
                path.push_back(Location::getLocation(r2, i));
                path.push_back(Location::getLocation(r2, c2));
                return true;
            }
        }
    }
    else{
        for(i = r2f + incC; i > r1f; i += incC){
            for(j = r1; j != r2; j += incR){
                if(board.getPicture(j, i) != Picture::A)
                    break;
            }
            if(j == r2){
                path.push_back(Location::getLocation(r1, c1));
                path.push_back(Location::getLocation(r1, i));
                path.push_back(Location::getLocation(r2, i));
                path.push_back(Location::getLocation(r2, c2));
                return true;
            }
        }
    }
    return false;
}

// Check if loc1 and loc2 can be linked with two vertical segments and one
// horizontal segment (2 turnings) and the two vertical segments are on different
// sides of the horizontal segment
bool Game::vertical2turningsShort(const int r1, const int r2, const int c1,
                                  const int c2, int &c1f, int &c2f,
                                  QList<Location> &path) const {
    int i, j;
    int incC, incR;

    path.clear();
    incR = (r2 - r1) / abs(r2 - r1);
    c1f = r1 + incR, c2f = r2 - incR;
    for (; c1f != r2; c1f += incR, c2f -= incR){
        if(board.getPicture(c1f, c1) != Picture::A){
            for(;c2f != r1; c2f -= incR){
                if(board.getPicture(c2f, c2) != Picture::A)
                    break;
            }
            break;
        }
        if(board.getPicture(c2f, c2) != Picture::A){
            for(;c1f != r2; c1f += incR){
                if(board.getPicture(c1f, c1) != Picture::A)
                    break;
            }
            break;
        }
    }

    incC = (c2 - c1) / abs(c2 - c1);
    if(incR > 0) {
        for (i = c2f + incR; i < c1f; i += incR){
            for(j = c1; j != c2; j += incC){
                if(board.getPicture(i, j) != Picture::A)
                    break;
            }
            if(j == c2){
                path.push_back(Location::getLocation(r1, c1));
                path.push_back(Location::getLocation(i, c1));
                path.push_back(Location::getLocation(i, c2));
                path.push_back(Location::getLocation(r2, c2));
                return true;
            }
        }
    }
    else{
        for(i = c2f + incR; i > c1f; i += incR){
            for(j = c1; j != c2; j += incC){
                if(board.getPicture(i, j) != Picture::A)
                    break;
            }
            if(j == c2){
                path.push_back(Location::getLocation(r1, c1));
                path.push_back(Location::getLocation(i, c1));
                path.push_back(Location::getLocation(i, c2));
                path.push_back(Location::getLocation(r2, c2));
                return true;
            }
        }
    }

    return false;
}

// Check if loc1 and loc2 can be linked with two horizontal segments and one
// vertical segment (2 turnings) and the two horizontal segments are on the same
// side of the vertical segment
bool Game::horizontal2turningsLong(const int r1, const int r2, const int c1,
                                   const int c2, int &r1f, int &r2f,
                                   QList<Location> &path) const{
    int i, j;
    int incC, incR;
    int r1f0 = r1f, r2f0 = r2f, i0 = 0;
    bool flag = false;

    path.clear();
    incR = (r2 - r1) / abs(r2 - r1);
    if(c1 != c2) {
        incC = (c2 - c1) / abs(c2 - c1);
        if(r1f != c2 && r2f != c1) return false;
    }
    else {
        incC = 1;
        r1f0 = r1f = c2, r2f0 = r2f = c1;
    }

    // Scan from shorter paths to longer paths
    if((r1f == c2 && board.getPicture(r1, c2) == Picture::A) || c1 == c2){
        for(r1f += incC;r1f >= 0 && r1f < board.NUM_COLS; r1f += incC){
            if(board.getPicture(r1, r1f) != Picture::A ||
                    board.getPicture(r2, r1f) != Picture::A)
                break;
        }

        for(i = c2 + incC; i != r1f; i += incC){
            for(j = r1 + incR; j != r2; j += incR){
                if(board.getPicture(j, i) != Picture::A)
                    break;
            }
            if(j == r2){
                path.push_back(Location::getLocation(r1, c1));
                path.push_back(Location::getLocation(r1, i));
                path.push_back(Location::getLocation(r2, i));
                path.push_back(Location::getLocation(r2, c2));
                i0 = i;
                flag = true;
                break;
            }
        }
    }

    r1f = r1f0, r2f = r2f0;
    if((r2f == c1 && board.getPicture(r2, c1) == Picture::A) || c1 == c2){
        for(r2f -= incC;r2f >= 0 && r2f < board.NUM_COLS; r2f -= incC){
            if(board.getPicture(r1, r2f) != Picture::A ||
                    board.getPicture(r2, r2f) != Picture::A)
                break;
        }

        for(i = c1 - incC; i != r2f; i -= incC){
            for(j = r1 + incR; j != r2; j += incR){
                if(board.getPicture(j, i) != Picture::A)
                    break;
            }
            if(j == r2){
                if(flag && abs(c1 - i) > abs(c2 - i0)) return true;
                path.clear();
                path.push_back(Location::getLocation(r1, c1));
                path.push_back(Location::getLocation(r1, i));
                path.push_back(Location::getLocation(r2, i));
                path.push_back(Location::getLocation(r2, c2));
                return true;
            }
        }
    }

    return flag;
}

// Check if loc1 and loc2 can be linked with two vertical segments and one horizontal
// segment (2 turnings) and the two vertical segments are on the same side of the
// horizontal segment
bool Game::vertical2turningsLong(const int r1, const int r2, const int c1,
                                 const int c2, int &c1f, int &c2f,
                                 QList<Location> &path) const{
    int i, j;
    int incR, incC;
    int c1f0 = c1f, c2f0 = c2f, i0 = 0;
    bool flag = false;

    path.clear();
    incC = (c2 - c1) / abs(c2 - c1);
    if(r1 != r2) {
        incR = (r2 - r1) / abs(r2 - r1);
        if(c1f != r2 && c2f != r1) return false;
    }
    else {
        incR = 1;
        c1f0 = c1f = r2, c2f0 = c2f = r1;
    }

    if((c1f == r2 && board.getPicture(r2, c1) == Picture::A) || r1 == r2){
        for(c1f += incR;c1f >= 0 && c1f < board.NUM_COLS; c1f += incR){
            if(board.getPicture(c1f, c1) != Picture::A ||
                    board.getPicture(c1f, c2) != Picture::A)
                break;
        }

        for(i = r2 + incR; i != c1f; i += incR){
            for(j = c1 + incC; j != c2; j += incC){
                if(board.getPicture(i, j) != Picture::A)
                    break;
            }
            if(j == c2){
                path.push_back(Location::getLocation(r1, c1));
                path.push_back(Location::getLocation(i, c1));
                path.push_back(Location::getLocation(i, c2));
                path.push_back(Location::getLocation(r2, c2));
                i0 = i;
                flag = true;
                break;
            }
        }
    }

    c1f = c1f0, c2f = c2f0;
    if((c2f == r1 && board.getPicture(r1, c2) == Picture::A) || r1 == r2){
        for(c2f -= incR;c2f >= 0 && c2f < board.NUM_COLS; c2f -= incR){
            if(board.getPicture(c2f, c1) != Picture::A ||
                    board.getPicture(c2f, c2) != Picture::A)
                break;
        }

        for(i = r1 - incR; i != c2f; i -= incR){
            for(j = c1 + incC; j != c2; j += incC){
                if(board.getPicture(i, j) != Picture::A)
                    break;
            }
            if(j == c2){
                if(flag && abs(r1 - i) > abs(r2 - i0)) return true;
                path.clear();
                path.push_back(Location::getLocation(r1, c1));
                path.push_back(Location::getLocation(i, c1));
                path.push_back(Location::getLocation(i, c2));
                path.push_back(Location::getLocation(r2, c2));
                return true;
            }
        }
    }

    return flag;
}

bool Game::hasSolution(){
   // For every grid on board, if it's not blank, search all the grids with the same picture
   // and see if there's a legal path between the two grid.
   QList<Location> path;
   for (int i = 0; i<board.NUM_ROWS ; i++) {
       for (int j = 0; j<board.NUM_COLS; j++){
           if (board.getPicture(i,j)!=Picture::A){
               Picture ptemp = board.getPicture(i,j);
               Location loc1(i,j);


               for(int m=i; m<board.NUM_ROWS; m++){
                   for(int n=0; n<board.NUM_COLS; n++){
                   if(board.getPicture(m,n) == ptemp){
                       Location loc2(m,n);
                       if(!isValid(loc1,loc2,path)){ continue;}
                       else{ return true; }
                   }
                   }
                   }
               }
           }
           }
   return false;


}

void Game::slotImageClicked(Location loc){
    cout << loc.getRow() << " " << loc.getCol() << " is clicked.\n";
    cout << "time = " << t << endl;
    QList<Location> path;
    clickTimes++;
    if(clickTimes == 1){
        loc1 = loc;
    }
    else{
        if(clickTimes == 2){
            loc2 = loc;
            if(this->isValid(loc1, loc2, path)){
                board.update(loc1, loc2);
                clickTimes = 0;
                emit updateTime();
                emit update(*this, loc1, loc2);
            }
        }
        else{
            loc1 = loc2;
            loc2 = loc;
            if(this->isValid(loc1, loc2, path)){
                board.update(loc1, loc2);
                clickTimes = 0;
                emit updateTime();
                emit update(*this, loc1, loc2);
            }
        }
    }
}

void Game::checkEnd(){
    if(board.getRemainGrid() == 0 || t == 0){
        inGame = false;
        emit gameEnd();
    }
}

void Game::slotRearrange(){
    if(board.getRemainGrid() != 0){
        do{
            board.regenerate();
        }while (!this->hasSolution());
    }

    emit rearrange(*this);
}


