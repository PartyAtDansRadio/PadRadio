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

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window)
{
    //Setup UI
    ui->setupUi(this);

    //Init main loop
    timer = new QTimer(this);

    //Setup media player
    mediaPlayer = new SamMedia(QUrl("http://www.mcttelecom.com/~d_libby/metaData.txt"), this);
    mediaPlayer->setMedia(QUrl("http://sc3.spacialnet.com:31560"));
    mediaPlayer->setVolume(50);

    //Setup events
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), mediaPlayer, SLOT(setVolume(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(mainLoop()));
    connect(mediaPlayer, SIGNAL(samMetaDataChanged()), this, SLOT(samDidMetaUpdate()));
    connect(ui->graphicsView, SIGNAL(clickedOn()), this, SLOT(fullscrean()));
}

void Window::resizeEvent(QResizeEvent* event)
{
    //Resize widgets for better display
    ui->progressBar->setMinimumWidth(event->size().width()*0.90*0.75);
    ui->volumeSlider->setMinimumWidth(event->size().width()*0.3333*0.75);
    ui->playButton->setMinimumWidth(event->size().width()*0.3333*0.75);
    ui->stopButton->setMinimumWidth(event->size().width()*0.3333*0.75);
}

void Window::keyPressEvent(QKeyEvent *event)
{
    //Get key input events
    if((isMaximized() || isFullScreen()) && event->key() == 16777216) { //ESC
        showNormal();
    }
    else if(event->key()==32 || event->key()==16777220) { //Space & Enter
        if(playing) {
            playing = false;
            mediaPlayer->stop();
        }
        else {
            playing = true;
            mediaPlayer->play();
        }
    }
    else {
        event->ignore();
    }
}

void Window::mainLoop()
{
    //Try to un all timed tasks
    QTime start = mediaPlayer->metaData(SamMetaType::MetaUpdateTime).toTime();
    QTime end = mediaPlayer->metaData(SamMetaType::Duration).toTime();
    QTime now = QTime::currentTime();
    int toEnd = start.msecsTo(now);
    int atEnd = QTime(0, 0, 0).msecsTo(end);
    QString secs = QString::number((int)(toEnd/1000)%60);
    if(secs.length() == 1)
        secs = '0' + secs;
    ui->musicTimeLeft->setText(QString::number((int)((toEnd/(1000*60))%60)) + ":" + secs +" / " + end.toString("m:ss"));
    ui->progressBar->setValue(((float)toEnd/(float)atEnd)*1000);
}

void Window::updateImage(QUrl albumArt)
{
    //Try to update current album art image by resource
    if(albumArt == QUrl()) {
        ui->graphicsView->updateArt(albumArt.toString());
    }
    else {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateImageReply(QNetworkReply*)));
        manager->get(QNetworkRequest(QUrl(albumArt)));
    }
}

void Window::updateImageReply(QNetworkReply* reply)
{
    //Set current album art image by network
    ui->graphicsView->updateArt(reply);
}

void Window::samDidMetaUpdate()
{
    //Update the UI parts
    updateImage(mediaPlayer->metaData(SamMetaType::CoverArtImage).toUrl());
    ui->musicListeners->setText(mediaPlayer->metaData(SamMetaType::Listeners).toString());
    ui->musicMaxListeners->setText(mediaPlayer->metaData(SamMetaType::ListenersMax).toString());
    ui->musicArtist->setText(mediaPlayer->metaData(SamMetaType::AlbumArtist).toString());
    ui->musicAlbum->setText(mediaPlayer->metaData(SamMetaType::AlbumTitle).toString());
    ui->musicTitle->setText(mediaPlayer->metaData(SamMetaType::Title).toString());
    ui->musicYear->setText(mediaPlayer->metaData(SamMetaType::Year).toString());
    ui->musicType->setText(mediaPlayer->metaData(SamMetaType::Genre).toString());
    QString nextAlbumArtist = mediaPlayer->metaData(SamMetaType::NextAlbumArtist).toString();
    QString nextAlbumTitle = mediaPlayer->metaData(SamMetaType::NextAlbumTitle).toString();
    QString nextTitle = mediaPlayer->metaData(SamMetaType::NextTitle).toString();

    //Update next song UI parts
    ui->nextSong->setText("Next up is " + nextAlbumArtist + "'s \"" + nextTitle + "\" from " + nextAlbumTitle + "...");

    //Try to start main loop
    if(!timer->isActive()) {
        playing = true;
        mediaPlayer->play();
        timer->start(50);
    }
}

void Window::fullscrean()
{
    if(isFullScreen())
        showNormal();
    else
        showFullScreen();
}

void Window::on_playButton_clicked()
{
    playing = true;
    mediaPlayer->play();
}

void Window::on_stopButton_clicked()
{
    playing = false;
    mediaPlayer->stop();
}

Window::~Window()
{
    delete ui;
}
