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

//Resize widgets for better display
//QScreen *i = QApplication::screens().at(0);
//qDebug() << i->devicePixelRatio() << qApp->desktop()->logicalDpiX();
Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window)
{
    //Load settings
    ui->setupUi(this);
    settings = new QSettings("Settings.ini", QSettings::IniFormat, this);

    //Setup media player
    mediaPlayer = new SamMedia(QUrl(settings->value("MetaData").toString()), this);
    mediaPlayer->setMedia(QUrl(settings->value("MediaStream").toString()));
    mediaPlayer->setVolume(50);
    mediaPlayer->play();

    //Setup QWidgets
    serverInfo = new ServerInfo(mediaPlayer, this);
    songDisplay = new SongDisplay(mediaPlayer);
    timeBar = new TimeBar(mediaPlayer, this);
    toolBar = new ToolBar(mediaPlayer, this);
    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QFrame *div = new QFrame(this);
    div->setAutoFillBackground(true);
    div->setFrameShape(QFrame::HLine);
    div->setFrameShadow(QFrame::Sunken);
    songCaster = new SongCaster(mediaPlayer, this);
    centralWidget()->layout()->addWidget(serverInfo);
    centralWidget()->layout()->addWidget(songDisplay);
    centralWidget()->layout()->addWidget(timeBar);
    centralWidget()->layout()->addWidget(toolBar);
    centralWidget()->layout()->addItem(spacer);
    centralWidget()->layout()->addWidget(div);
    centralWidget()->layout()->addWidget(songCaster);

    //Setup window
    setMaximumHeight(0);
    setMinimumWidth(400);
    setMaximumWidth(minimumWidth());
    if(settings->value("alwaysTop").toBool())
        setWindowFlags(Qt::WindowStaysOnTopHint);
    if(settings->value("rememberLocation").toBool())
            restoreGeometry(settings->value("WindowGeometry").toByteArray());
    if(settings->value("smallPlayer").toBool()) {
        serverInfo->hide();
        songDisplay->hideAlbumArt(true);
        div->hide();
        songCaster->hide();
    }
    else {
        setMinimumHeight(600);
    }

    //Setup other windows
    aboutWindow = new About();
    settingsWindow = new Settings();

    //Theme ui
    QFile theme(":/Window/Theme");
    theme.open(QFile::ReadOnly | QFile::Text);
    QTextStream themeInput(&theme);
    setStyleSheet(themeInput.readAll());
    theme.close();

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

    //Setup events
    connect(mediaPlayer, SIGNAL(samMetaDataChanged()), SLOT(showNowPlaying()));
    connect(mediaPlayer, SIGNAL(isPlayingState(bool)), SLOT(mediaButton_togglePlay(bool)));
    connect(buttonAction, SIGNAL(triggered()), SLOT(mediaButton_clicked()));
    connect(windowAction, SIGNAL(triggered()), SLOT(showWindow()));
    connect(songDisplay, SIGNAL(albumArtToggled(bool)), SLOT(albumArt_toggled(bool)));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
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
        if(mediaPlayer->isPlaying()) {
            toolBar->togglePlay(false);
        }
        else {
            toolBar->togglePlay(true);
        }
    }
    else {
        event->ignore();
    }
}

void Window::showNowPlaying()
{
    //Update all UI parts
    if(settings->value("showTaskbarIcon").toBool() && settings->value("showMessages").toBool() && !isActiveWindow())
        systemTray->showMessage("PadRadio", "Playing " + mediaPlayer->metaData(SamMetaType::AlbumArtist).toString() +
                                "'s \"" + mediaPlayer->metaData(SamMetaType::Title).toString() +
                                "\" from " + mediaPlayer->metaData(SamMetaType::AlbumTitle).toString() +
                                ".", QSystemTrayIcon::Information, 30000);
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

void Window::albumArt_toggled(bool toggled)
{
    //Show fullscreen if using a desktop OS
    #if defined(Q_OS_WIN) || defined(Q_OS_OSX) || defined(Q_OS_LINUX)
    if(!isFullScreen() && !toggled) {
        setMaximumHeight(999999);
        setMaximumWidth(999999);
        showFullScreen();
        menuBar()->hide();
    }
    else {
        showNormal();
        menuBar()->show();
        setMaximumHeight(0);
        setMaximumWidth(0);
    }
    #endif
}

void Window::mediaButton_togglePlay(bool play)
{
    if(play) {
        if(!mediaPlayer->isPlaying())
            mediaPlayer->play();
        buttonAction->setText("Stop");
    }
    else {
        if(mediaPlayer->isPlaying())
            mediaPlayer->stop();
        buttonAction->setText("Play");
    }
}
void Window::mediaButton_clicked()
{
    if(mediaPlayer->isPlaying()) {
        mediaPlayer->stop();
        buttonAction->setText("Play");
    }
    else {
        mediaPlayer->play();
        buttonAction->setText("Stop");
    }
}
