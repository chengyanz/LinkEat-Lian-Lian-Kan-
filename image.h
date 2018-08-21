#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <QWidget>
#include <QColor>
#include <QLabel>
#include <QObject>
#include "location.h"

struct QColor;
struct QMouseEvent;

using namespace std;

class Image : public QLabel
{
  Q_OBJECT

public:
    Image(const QString &filename, QWidget *parent, int r = 0, int c = 0);
    Image(const Image&);
    void mousePressEvent( QMouseEvent * e);
    void setLocation(int r, int c);
    Location getLocation() const;
private:
    Location loc;
 signals:
    void onClick(Location);
};

#endif // IMAGE_H
