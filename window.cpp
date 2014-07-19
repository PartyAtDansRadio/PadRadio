/*
                Copyright (C) 2014 PartyAtDansRadio

PadRadio is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 3.

PadRadio is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License Version 3 for more details.

You should have received a copy of the GNU General Public License
along with PadRadio. If not, see <http://www.gnu.org/licenses/>.

See project home page at: <https://github.com/PartyAtDansRadio/PadRadio>
*/

#include "window.h"
#include "ui_window.h"

//#include <QDebug> //for debug output using [qDebug() << "text";]

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window)
{
    //Setup UI
    ui->setupUi(this);
    setImage(ui->graphicsView);
    setFixedSize(width()+10, height()+10);

    //Setup media player
    mediaPlayer = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    mediaPlayer->setMedia(QUrl("http://sc3.spacialnet.com:31560"));
    mediaPlayer->setVolume(50);

    //Setup events
    connect(ui->playButton, SIGNAL(clicked()), mediaPlayer, SLOT(play()));
    connect(ui->pauseButton, SIGNAL(clicked()), mediaPlayer, SLOT(pause()));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), mediaPlayer, SLOT(setVolume(int)));
}

void Window::setImage(QGraphicsView *view)
{
    //Set player logo
    QGraphicsScene *scene = new QGraphicsScene();
    view->setScene(scene);
    QGraphicsPixmapItem *pixmapItem = scene->addPixmap(QPixmap(":/PadImg"));
    view->fitInView(pixmapItem);
    view->scale(3,10);
}

Window::~Window()
{
    delete ui;
}
