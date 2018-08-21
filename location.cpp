#include "location.h"

Location::Location(int r, int c): row(r), col(c) {}

void Location::setLocation(int r, int c) {
    row = r, col = c;
}

int Location::getRow() const{
    return row;
}

int Location::getCol() const{
    return col;
}

bool Location::operator==(const Location loc) const{
    return row == loc.row && col == loc.col;
}

Location& Location::operator=(const Location loc){
    row = loc.getRow();
    col = loc.getCol();
    return *this;
}

Location Location::getLocation(int r, int c){
	Location l(r, c);
	return l;
}
