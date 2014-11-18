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
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    if(settings->value("rememberLocation").toBool())
            restoreGeometry(settings->value("WindowGeometry").toByteArray());
    if(settings->value("smallPlayer").toBool()) {
        ui->toolAlbumArt->hide();
        setMaximumHeight(300);
        setMaximumWidth(minimumWidth());
    }
    else {
        setMinimumHeight(610);
    }

    //Theme ui
    updateImage();
    QFile theme(":/Window/Theme");
    theme.open(QFile::ReadOnly | QFile::Text);
    QTextStream themeInput(&theme);
    setStyleSheet(themeInput.readAll());
    theme.close();

    //Setup other windows
    aboutWindow = new About();
    settingsWindow = new Settings();

    //Create taskbar icon
    QMenu *trayIconMenu = new QMenu(this);
    buttonAction = new QAction("Stop", this);
    windowAction = new QAction(this);
    if(!isHidden())
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
    systemTray->setIcon(QIcon(":/Window/PadLogo"));
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
    connect(buttonAction, SIGNAL(triggered()), SLOT(mediaButton_clicked())); //This one needs fixing
    connect(windowAction, SIGNAL(triggered()), SLOT(showWindow()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Window::resizeEvent(QResizeEvent* event)
{
    //Resize widgets for better display
    QScreen *i = QApplication::screens().at(0);
    qDebug() << i->devicePixelRatio() << qApp->desktop()->logicalDpiX();

    ui->progressBar->setMinimumWidth(event->size().width()*0.90*0.75);
    ui->volumeSlider->setMinimumWidth(event->size().width()*0.50*0.75);
    ui->playButton->setMinimumWidth(event->size().width()*0.50*0.75);
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
            ui->playButton->toggled(false);
            ui->playButton->setChecked(false);

        }
        else {
            ui->playButton->toggled(true);
            ui->playButton->setChecked(true);
        }
    }
    else {
        event->ignore();
    }
}

void Window::mainLoop()
{
    //Try to run all timed tasks
    if(ui->toolAlbumArt->iconSize() != QSize(ui->toolAlbumArt->height(), ui->toolAlbumArt->height()))
        ui->toolAlbumArt->setIconSize(QSize(ui->toolAlbumArt->height(), ui->toolAlbumArt->height()));
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
        QPixmap map;
        map.load(":/Window/PadLogo");
        ui->toolAlbumArt->setIcon(QIcon(map.scaled(1080, 1080, Qt::KeepAspectRatio)));
    }
    else {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateImageReply(QNetworkReply*)));
        manager->get(QNetworkRequest(QUrl(albumArt)));
    }
}

void Window::updateImageReply(QNetworkReply* reply)
{
    //Try to set current album art image by network
    if(reply->error() == reply->NoError) {
        QPixmap map;
        map.loadFromData(reply->readAll());
        ui->toolAlbumArt->setIcon(QIcon(map.scaled(1080, 1080, Qt::KeepAspectRatio)));
    }
    else {
        QPixmap map;
        map.load(":/Window/PadLogo");
        ui->toolAlbumArt->setIcon(QIcon(map.scaled(1080, 1080, Qt::KeepAspectRatio)));
    }
}

void Window::samDidMetaUpdate()
{
    //Update all UI parts
    if(settings->value("showTaskbarIcon").toBool() && settings->value("showMessages").toBool() && !isActiveWindow())
        systemTray->showMessage("PadRadio", "Playing " + mediaPlayer->metaData(SamMetaType::AlbumArtist).toString() +
                                "'s \"" + mediaPlayer->metaData(SamMetaType::Title).toString() +
                                "\" from " + mediaPlayer->metaData(SamMetaType::Title).toString() +
                                ".", QSystemTrayIcon::Information, 30000);
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
        ui->playButton->toggled(true);
        ui->playButton->setChecked(true);
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
    aboutWindow->show();
}

void Window::on_actionSettings_triggered()
{    
    settingsWindow->show();
}

void Window::on_actionExit_triggered()
{
    qApp->quit();
}

void Window::on_toolAlbumArt_clicked()
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

void Window::on_playButton_toggled(bool checked)
{
    playing = checked;
    if(checked) {
        ui->playButton->setText("Stop");
        mediaPlayer->play();
    }
    else {
        ui->playButton->setText("Play");
        mediaPlayer->stop();
    }
}
