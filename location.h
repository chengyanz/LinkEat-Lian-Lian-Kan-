#ifndef LOCATION_H
#define LOCATION_H
class Location{
public:
    Location(int r, int c);
    void setLocation(int r, int c);
    int getRow() const;
    int getCol() const;
    bool operator==(const Location loc) const;
    Location& operator=(const Location loc);
    static Location getLocation(int r, int c);
private:
    int row;
    int col;
};
#endif // LOCATION_H
