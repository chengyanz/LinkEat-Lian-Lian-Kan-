#include <cassert>
#include <iostream>

#include "image.h"

#include <QBitmap>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>

using namespace std;

Image::Image(const QString& filename, QWidget *parent, int r, int c):
    QLabel(parent),loc(r, c)
{
  //MouseTracking must be set to true, to make Image clickable
  this->setMouseTracking(true);
  //Set the image
  QPixmap pixmap(filename);
  assert(!pixmap.isNull());
  //Set transparency (white, in this example)
  const QBitmap mask = pixmap.createMaskFromColor(QColor(255,255,255,0).rgb());
  pixmap.setMask(mask);
  this->setPixmap(pixmap);
}


void Image::mousePressEvent( QMouseEvent *)
{
  emit onClick(loc);
}

void Image::setLocation(int r, int c)
{
    loc.setLocation(r, c);
}

Location Image::getLocation() const{
    return loc;
}
