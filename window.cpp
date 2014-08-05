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
    settings = new QSettings("Settings.ini", QSettings::IniFormat, this);
    ui->graphicsView->updateArt(":/PadImg");
    if(settings->value("rememberLocation").toBool())
            restoreGeometry(settings->value("WindowGeometry").toByteArray());

    //Create taskbar icon
    QMenu *trayIconMenu = new QMenu(this);
    buttonAction = new QAction("Stop", this);
    windowAction = new QAction(this);
    if(isHidden())
        windowAction->setText("Restore");
    else
        windowAction->setText("Minimize");
    QAction *quitAction = new QAction("Exit", this);
    trayIconMenu->addAction(buttonAction);
    trayIconMenu->addAction(windowAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIconMenu->setDefaultAction(quitAction);
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(QIcon(":/PadImg"));
    systemTray->setContextMenu(trayIconMenu);
    if(settings->value("showTaskbarIcon").toBool())
        systemTray->show();

    //Setup media player
    mediaPlayer = new SamMedia(QUrl(settings->value("MetaData").toString()), this);
    mediaPlayer->setMedia(QUrl(settings->value("MediaStream").toString()));
    mediaPlayer->setVolume(50);

    //Setup events
    timer = new QTimer(this);
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), mediaPlayer, SLOT(setVolume(int)));
    connect(timer, SIGNAL(timeout()), SLOT(mainLoop()));
    connect(mediaPlayer, SIGNAL(samMetaDataChanged()), SLOT(samDidMetaUpdate()));
    connect(ui->graphicsView, SIGNAL(clickedOn()), SLOT(fullscreen()));
    connect(buttonAction, SIGNAL(triggered()), SLOT(mediaButton_clicked()));
    connect(windowAction, SIGNAL(triggered()), SLOT(showWindow()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
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
    if(event->key() == 16777313) { //Android Back Button
        //parentWidget()->showMinimized(); //Need to fix this!!!
    }
    else if((isMaximized() || isFullScreen()) && event->key() == 16777216) { //ESC
        showNormal();
        menuBar()->show();
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
    //Try to run all timed tasks
    QTime start = mediaPlayer->metaData(SamMetaType::MetaUpdateTime).toTime();
    QTime end = mediaPlayer->metaData(SamMetaType::Duration).toTime();
    QTime now = QTime::currentTime();
    int toEnd = start.msecsTo(now);
    int atEnd = QTime(0, 0, 0).msecsTo(end);
    QString secs = QString::number((int)(toEnd/1000)%60);
    if(secs.length() == 1)
        secs = '0' + secs;
    ui->musicTimeLeft->setText(QString::number((int)((toEnd/(1000*60))%60)) +
                               ":" + secs + " / " + end.toString("m:ss"));
    ui->progressBar->setValue(((float)toEnd/(float)atEnd)*1000);
}

void Window::updateImage(QUrl albumArt)
{
    //Try to update current album art image by resource
    if(albumArt == QUrl()) {
        ui->graphicsView->updateArt(":/PadImg");
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
    //Update all UI parts
    if(settings->value("showTaskbarIcon").toBool() && settings->value("showMessages").toBool() && !isActiveWindow())
        systemTray->showMessage("PadRadio", "Playing " + mediaPlayer->metaData(SamMetaType::AlbumArtist).toString() +
                                "'s \"" + mediaPlayer->metaData(SamMetaType::Title).toString() +
                                "\" from " + mediaPlayer->metaData(SamMetaType::Title).toString() + ".");
    updateImage(mediaPlayer->metaData(SamMetaType::CoverArtImage).toUrl());
    ui->musicListeners->setText(mediaPlayer->metaData(SamMetaType::Listeners).toString());
    ui->musicMaxListeners->setText(mediaPlayer->metaData(SamMetaType::ListenersMax).toString());
    ui->musicArtist->setText(mediaPlayer->metaData(SamMetaType::AlbumArtist).toString());
    ui->musicAlbum->setText(mediaPlayer->metaData(SamMetaType::AlbumTitle).toString());
    ui->musicTitle->setText(mediaPlayer->metaData(SamMetaType::Title).toString());
    ui->musicYear->setText(mediaPlayer->metaData(SamMetaType::Year).toString());
    ui->musicGenre->setText(mediaPlayer->metaData(SamMetaType::Genre).toString());
    QString nextAlbumArtist = mediaPlayer->metaData(SamMetaType::NextAlbumArtist).toString();
    QString nextAlbumTitle = mediaPlayer->metaData(SamMetaType::NextAlbumTitle).toString();
    QString nextTitle = mediaPlayer->metaData(SamMetaType::NextTitle).toString();
    ui->nextSong->setText("Next up is " + nextAlbumArtist + "'s \"" + nextTitle + "\" from " + nextAlbumTitle + "...");

    //Try to start main loop
    if(!timer->isActive()) {
        playing = true;
        mediaPlayer->play();
        timer->start(50);
    }
}

void Window::showWindow()
{
    if(isHidden()) {
        showNormal();
        windowAction->setText("Minimize");
    }
    else {
        hide();
        windowAction->setText("Restore");
    }
}

void Window::fullscreen()
{
    //Show fullscreen if using a desktop OS
    #if defined(Q_OS_WIN) || defined(Q_OS_OSX) || defined(Q_OS_LINUX)
    if(isFullScreen()) {
        showNormal();
        menuBar()->show();
    }
    else {
        showFullScreen();
        menuBar()->hide();
    }
    #endif
}
void Window::mediaButton_clicked()
{
    if(playing) {
        on_stopButton_clicked();
        buttonAction->setText("Play");
    }
    else {
        on_playButton_clicked();
        buttonAction->setText("Stop");
    }
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
    //Try to save window geometry
    if(settings->value("rememberLocation").toBool())
        settings->setValue("WindowGeometry", saveGeometry());

    //Delete window
    settings->sync();
    delete ui;
}

void Window::on_actionReport_Bug_triggered()
{
    QString cmd = "mailto:partyatdans@gmail.com?subject=Bug Report " + QGuiApplication::applicationVersion();
    QDesktopServices::openUrl(QUrl(cmd + " on " + qApp->platformName() + "&body=Please describe the problem:"));
}

void Window::on_actionAbout_triggered()
{
    About *aboutWindow = new About(this);
    aboutWindow->show();
}

void Window::on_actionSettings_triggered()
{
    Settings *settingsWindow = new Settings(this);
    settingsWindow->show();
}

void Window::on_actionExit_triggered()
{
    qApp->quit();
}
