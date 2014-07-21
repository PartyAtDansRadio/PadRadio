/*
                Copyright (C) 2014 PartyAtDansRadio

PadRadio is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation.

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
    updateImage();

    //Init meta data
    metaData = QList<QString>();
    timer = new QTimer(this);
    updateMetaData();

    //Setup media player
    mediaPlayer = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    mediaPlayer->setMedia(QUrl("http://sc3.spacialnet.com:31560"));
    mediaPlayer->setVolume(50);

    //Setup events
    connect(ui->playButton, SIGNAL(clicked()), mediaPlayer, SLOT(play()));
    connect(ui->stopButton, SIGNAL(clicked()), mediaPlayer, SLOT(stop()));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), mediaPlayer, SLOT(setVolume(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(mainLoop()));
    connect(this, SIGNAL(metaUpdate()), SLOT(metaDidUpdate()));
}

void Window::mainLoop()
{
    //Run all timed tasks
    QTime start = QTime::fromString(metaData[2], "hh:mm:ss ap");
    QTime end = QTime::fromString(metaData[6], "m:ss");
    QTime now = QTime::currentTime();
    int toEnd = start.msecsTo(now);
    int atEnd = QTime(0, 0, 0).msecsTo(end);
    QString secs = QString::number((int)(toEnd/1000)%60);
    if(secs.length() == 1)
        secs = '0' + secs;
    if(((float)toEnd/(float)atEnd)*100 > 100) {
        updateMetaData();
    }
    else {
        ui->musicTimeLeft->setText(QString::number((int)((toEnd/(1000*60))%60)) + ":" + secs);
        ui->progressBar->setValue(((float)toEnd/(float)atEnd)*1000);
    }
}

void Window::updateImage()
{
    //Update current album art image
    if(metaData.length() == 0 || metaData[8] == "http://www.mcttelecom.com/~d_libby/pictures/") {
        QGraphicsScene *scene = new QGraphicsScene();
        ui->graphicsView->setScene(scene);
        QGraphicsPixmapItem *pixmapItem;
        pixmapItem = scene->addPixmap(QPixmap(":/PadImg"));
        ui->graphicsView->fitInView(pixmapItem);
        ui->graphicsView->scale(3, 9);
    }
    else {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateImageReply(QNetworkReply*)));
        manager->get(QNetworkRequest(QUrl(metaData[9])));
    }
}

void Window::updateImageReply(QNetworkReply* reply)
{
    //Set current album art image
    QGraphicsScene *scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QGraphicsPixmapItem *pixmapItem;
    QPixmap img;
    img.loadFromData(reply->readAll());
    pixmapItem = scene->addPixmap(img);
    ui->graphicsView->fitInView(pixmapItem);
}

void Window::updateMetaData()
{
    //Update music info
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(metaDataReply(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://www.mcttelecom.com/~d_libby/meta.txt")));
}

void Window::metaDataReply(QNetworkReply* reply)
{
    //Try to set music info
    QList<QString> rawData = QList<QString>(((QString)reply->readAll()).split("\r\n"));
    if(metaData.length() == 0 || rawData[2] != metaData[2]) {
        metaData.clear();
        metaData.append(rawData);
        for(int i=0; i < metaData.length(); i++)
            if(metaData[i].length() == 0)
                metaData.removeAt(i);
        emit metaUpdate();
    }
}

void Window::metaDidUpdate()
{
    //Update the UI parts
    updateImage();
    ui->musicListeners->setText(metaData[0]);
    ui->musicMaxListeners->setText(metaData[1]);
    ui->musicArtist->setText(metaData[4]);
    ui->musicAlbum->setText(metaData[5]);
    ui->musicTitle->setText(metaData[3]);
    ui->musicYear->setText(metaData[7]);
    ui->musicType->setText(metaData[8]);
    ui->musicTimeEnd->setText(metaData[6]);
    if(!timer->isActive())
        mediaPlayer->play();
        timer->start(250);
}

Window::~Window()
{
    delete ui;
}
