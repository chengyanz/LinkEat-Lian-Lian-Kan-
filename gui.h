#ifndef GUI_H
#define GUI_H

#include <QList>
#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QProgressBar>
#include <QComboBox>
#include <QTimer>
#include "game.h"

struct Image;

using namespace std;

namespace Ui {
  class Gui;
}

class Gui : public QDialog
{
  Q_OBJECT

public:
  explicit Gui(QWidget *parent = 0);
  ~Gui();
  void updateTimer();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::Gui *ui;
  //Images on the screen
  QList<Image *> boardImages;
  QList<QString> files;
  Game game;
  QWidget *panel;
  QGridLayout * layout;
  QPushButton *startButton;
  QPushButton *quitWindowButton;
  QPushButton *quitGameButton;
  QPushButton *rearrangeButton;
  QLabel *endLabel;
  QProgressBar *progressBar;
  QTimer *timer;
  bool timeAdded;

private slots:
  void slotWait();
  void slotCreateNew();
  void slotUpdate(Game &game, Location loc1, Location loc2);
  void slotRearrangeGui(Game &game);
  void slotGameEnd();
  void slotOn_time_out();
  void slotUpdateTimer();

};

#endif // GUI_H
