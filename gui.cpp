#include <iostream>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QFont>
#include <QWidget>
#include <QDialog>
#include <QList>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QRect>
#include "gui.h"
#include "ui_gui.h"
#include "image.h"
#include "game.h"

using namespace std;


Gui::Gui(QWidget *parent) :
    game(),
    QDialog(parent), 
    ui(new Ui::Gui),
    panel(new QWidget(this)),
    layout(new QGridLayout(panel)),
    startButton(new QPushButton("Start", panel)),
    quitWindowButton(new QPushButton("Quit", panel)),
    quitGameButton(new QPushButton("Quit",panel)),
    rearrangeButton(new QPushButton("Rearrange", panel)),
    endLabel(new QLabel("End of Game.", panel)),
    progressBar(new QProgressBar(panel)),
    timer(new QTimer(this)),
    timeAdded(false)
{
    ui->setupUi(this);
    QFile imageFiles(":/images/imageFiles.txt");
    QString qstring;
    QTextStream in(&imageFiles);

    if(!imageFiles.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout << "imageFiles.txt is missing." << endl;
        QApplication::quit();
    }

    while((qstring = in.readLine()) != ""){
        files.append(qstring);
    }

    panel -> setFixedSize(Board::NUM_COLS * 50 + 28, (Board::NUM_ROWS + 4) * 50);
    this -> setFixedSize(Board::NUM_COLS * 50 + 28, (Board::NUM_ROWS + 4) * 50);
    QRect *layoutRect = new QRect(0, 0, Board::NUM_COLS * 50, (Board::NUM_ROWS + 4) * 50);
    layout -> setGeometry(*layoutRect);
    layout -> setSpacing(1);

    timer->connect(timer, SIGNAL(timeout()), this, SLOT(slotOn_time_out()));

    slotWait();
}

Gui::~Gui()
{
    delete ui;
}

void Gui::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Gui::slotWait(){
    //int i, j;
    //cout << "Enter slotWait" << endl;

/*    if(game.hasPrevGame){
        for(i = 0; i < Board::NUM_ROWS; i++)
                   for(j = 0; j < Board::NUM_COLS; j++){
                        layout-> removeWidget(boardImages[i * Board::NUM_COLS + j]);
                        boardImages[i * Board::NUM_COLS + j]->close();
                    }
    }*/
    layout -> addWidget(startButton, 0, 0);
    layout -> addWidget(quitWindowButton, 1, 0);
    panel -> show();
    quitGameButton->hide();
    endLabel->hide();
    rearrangeButton->hide();
    progressBar->hide();
    startButton->show();
    quitWindowButton->show();
    connect(startButton, SIGNAL(clicked()), this, SLOT(slotCreateNew()));
    connect(quitWindowButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
}


void Gui::slotCreateNew(){
    startButton->hide();
    quitWindowButton->hide();
    layout-> removeWidget(startButton);
    layout-> removeWidget(quitWindowButton);
    connect(&game, SIGNAL(update(Game&, Location, Location)), this,
            SLOT(slotUpdate(Game&, Location, Location)));
    connect(&game, SIGNAL(updateTime()), this, SLOT(slotUpdateTimer()));
    connect(&game, SIGNAL(rearrange(Game&)), this, SLOT(slotRearrangeGui(Game&)));
    connect(&game, SIGNAL(gameEnd()), this, SLOT(slotGameEnd()));
    int difficulty = 30;
    game.setDifficulty(difficulty);
    game.setTimeFromDiff(difficulty);

    int i, j;
/*
    if(game.hasPrevGame){
        endLabel -> hide();
    }

    game.inGame = game.hasPrevGame = true;
*/
    game.inGame = true;
    boardImages.clear();
    for(i = 0; i < Board::NUM_ROWS; i++)
        for(j = 0; j < Board::NUM_COLS; j++){
            boardImages.append(new Image(files[game.getPicture(i, j)],
                    panel, i, j));
        }

    for(i = 0; i < Board::NUM_ROWS; i++)
        for(j = 0; j < Board::NUM_COLS; j++){
            layout->addWidget(boardImages[i * Board::NUM_COLS + j], i, j);
            boardImages[i * Board::NUM_COLS + j]->setFixedSize(50, 50);
            boardImages[i * Board::NUM_COLS + j]->setScaledContents(true);
        }

    layout -> addWidget(rearrangeButton, Board::NUM_ROWS + 2, 0, 1, 2);
    //connect(rearrangeButton, SIGNAL(clicked()), panel, SLOT(close()));

    layout -> addWidget(quitGameButton, Board::NUM_ROWS + 2, 2);
    //connect(quitGameButton, SIGNAL(clicked()), panel, SLOT(close()));

    layout -> addWidget(progressBar, Board::NUM_ROWS + 1, 0, 2, Board::NUM_COLS);
    progressBar->setRange(0,game.getTime());
    progressBar->setValue(game.getTime());
    progressBar->show();

    quitGameButton->show();
    rearrangeButton->show();
    panel->show();

    timer->start(1000); //updated every 1000ms

    connect(quitGameButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
    //connect(quitGameButton, SIGNAL(clicked()), this, SLOT(slotWait()));
    connect(rearrangeButton, SIGNAL(clicked()), &game, SLOT(slotRearrange()));

    for(i = 0; i < Board::NUM_ROWS; i++)
        for(j = 0; j < Board::NUM_COLS; j++){
            boardImages[i * Board::NUM_COLS + j]->
                    connect(boardImages[i * Board::NUM_COLS + j],
                    SIGNAL(onClick(Location)),&game,SLOT(slotImageClicked(Location)));
        }

    //connect(&game, SIGNAL(gameEnd()), panel, SLOT(close()));

    if(!game.hasSolution()) game.slotRearrange();
}

void Gui::slotUpdate(Game &game, Location loc1, Location loc2){
    int index1 = loc1.getRow() * Board::NUM_COLS + loc1.getCol();
    int index2 = loc2.getRow() * Board::NUM_COLS + loc2.getCol();
    connect(&game, SIGNAL(gameEnd()), this, SLOT(slotGameEnd()));
    layout->removeWidget(boardImages[index1]);
    boardImages[index1]->close();
    boardImages[index1]=new Image(files[0],panel, loc1.getRow(), loc1.getCol());
    layout->addWidget(boardImages[index1], loc1.getRow(), loc1.getCol());
    boardImages[index1]->setFixedSize(50, 50);
    boardImages[index1]->setScaledContents(true);

    layout->removeWidget(boardImages[index2]);
    boardImages[index2]->close();
    boardImages[index2]=new Image(files[0], panel, loc2.getRow(), loc2.getCol());
    layout->addWidget(boardImages[index2], loc2.getRow(), loc2.getCol());
    boardImages[index2]->setFixedSize(50, 50);
    boardImages[index2]->setScaledContents(true);

    connect(boardImages[index1],
    SIGNAL(onClick(Location)),&game,SLOT(slotImageClicked(Location)));
    connect(boardImages[index2],
    SIGNAL(onClick(Location)),&game,SLOT(slotImageClicked(Location)));

    game.checkEnd();

    if(!game.hasSolution()&& game.inGame) game.slotRearrange();
}

void Gui::slotRearrangeGui(Game &game){
    int i, j;

    for(i = 0; i < Board::NUM_ROWS; i++)
        for(j = 0; j < Board::NUM_COLS; j++){
            layout-> removeWidget(boardImages[i * Board::NUM_COLS + j]);
            boardImages[i * Board::NUM_COLS + j]->close();
        }

    boardImages.clear();
    for(i = 0; i < Board::NUM_ROWS; i++)
        for(j = 0; j < Board::NUM_COLS; j++){
            boardImages.append(new Image(files[game.getPicture(i, j)],
                    panel, i, j));
        }

    for(i = 0; i < Board::NUM_ROWS; i++)
        for(j = 0; j < Board::NUM_COLS; j++){
            layout->addWidget(boardImages[i * Board::NUM_COLS + j], i, j);
            boardImages[i * Board::NUM_COLS + j]->setFixedSize(50, 50);
            boardImages[i * Board::NUM_COLS + j]->setScaledContents(true);
        }
    for(i = 0; i < Board::NUM_ROWS; i++)
        for(j = 0; j < Board::NUM_COLS; j++){
            boardImages[i * Board::NUM_COLS + j]->
                    connect(boardImages[i * Board::NUM_COLS + j],
                    SIGNAL(onClick(Location)),&game,SLOT(slotImageClicked(Location)));
        }

}

void Gui::slotGameEnd(){
    //int i, j;

/*    game.inGame = false;
    game.hasPrevGame = true;
    for(i = 0; i < Board::NUM_ROWS; i++)
        for(j = 0; j < Board::NUM_COLS; j++){
            layout-> removeWidget(boardImages[i * Board::NUM_COLS + j]);
            boardImages[i * Board::NUM_COLS + j]->close();
        }
        */
    panel->close();

    QLabel *endOfGameLabel = new QLabel("End of Game", this);

    QFont f( "Arial", 20, QFont::Bold);
    endOfGameLabel->setFont(f);
    endOfGameLabel->setGeometry(150, 300, 200, 50);

    endOfGameLabel->show();
    //quitGameButton->hide();
    //rearrangeButton->hide();
    //layout -> addWidget(endLabel, 0, 0, 2, 1);
   /* layout -> addWidget(startButton, 1, 0);
    layout -> addWidget(quitWindowButton, 2, 0);*/
    //endLabel->show();
   /* startButton->show();
    quitWindowButton->show();*/
    //panel -> show();
    //connect(startButton, SIGNAL(clicked()), this, SLOT(slotCreateNew()));
    //connect(startButton, SIGNAL(clicked()), panel, SLOT(close()));
    //connect(quitWindowButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
}

void Gui::slotOn_time_out()
{
    if(timeAdded == false){
       game.setTime(game.getTime() - 1);
       progressBar->setValue(game.getTime());
    }
    else{
        this->updateTimer();
        timeAdded = false;
        game.setTime(game.getTime() - 1);
        progressBar->setValue(game.getTime());
    }

    if(game.getTime() == 0)
    {
        timer->stop();
        game.checkEnd();
    }
}

void Gui::slotUpdateTimer(){
    timeAdded = true;
}

void Gui::updateTimer(){
    int addtime = 6;
 /*   if(game.getDifficulty() > 10){
        addtime = 4;
        if(game.getDifficulty() > 20)
            addtime = 2;
    }*/
    addtime = 2;
    game.setTime(game.getTime() + addtime);
}
