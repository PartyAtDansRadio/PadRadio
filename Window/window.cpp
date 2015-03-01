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

//Resize widgets for better display
//QScreen *i = QApplication::screens().at(0);
//qDebug() << i->devicePixelRatio() << qApp->desktop()->logicalDpiX();
Window::Window(QWidget *parent) : QMainWindow(parent)
{
    //Load settings
    setWindowTitle("Party At Dans Radio");
    settings = new QSettings("Settings.ini", QSettings::IniFormat, this);
    //settings_update();

    //Setup media player
    mediaPlayer = new SamMedia(QUrl(settings->value("MetaData").toString()), this);
    mediaPlayer->setMedia(QUrl(settings->value("MediaStream").toString()));
    mediaPlayer->setVolume(50); //***Add to settings
    mediaPlayer->play();

    //Setup QWidgets
    QWidget *cWidget = new QWidget(this);
    setCentralWidget(cWidget);
    serverInfo = new ServerInfo(mediaPlayer, this);
    songDisplay = new SongDisplay(mediaPlayer, this);
    timeBar = new TimeBar(mediaPlayer, this);
    toolBar = new ToolBar(mediaPlayer, this);
    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    songCaster = new SongCaster(mediaPlayer, this);

    //Config Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    cWidget->setLayout(mainLayout);
    mainLayout->setContentsMargins(15, 10, 15, 10);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(serverInfo);
    mainLayout->addWidget(songDisplay);
    mainLayout->addWidget(timeBar);
    mainLayout->addWidget(toolBar);
    mainLayout->addItem(spacer);
    mainLayout->addWidget(songCaster);

    //Add shadow drop to widgets - need to find a cleaner way to do this
    QGraphicsDropShadowEffect *effect1 = new QGraphicsDropShadowEffect();
    effect1->setBlurRadius(5);
    effect1->setOffset(5, 5);
    serverInfo->setGraphicsEffect(effect1);
    QGraphicsDropShadowEffect *effect2 = new QGraphicsDropShadowEffect();
    effect2->setBlurRadius(5);
    effect2->setOffset(5, 5);
    songDisplay->setGraphicsEffect(effect2);
    QGraphicsDropShadowEffect *effect3 = new QGraphicsDropShadowEffect();
    effect3->setBlurRadius(5);
    effect3->setOffset(5, 5);
    timeBar->setGraphicsEffect(effect3);
    QGraphicsDropShadowEffect *effect4 = new QGraphicsDropShadowEffect();
    effect4->setBlurRadius(5);
    effect4->setOffset(5, 5);
    toolBar->setGraphicsEffect(effect4);

    //Theme window
    QFile theme(":/Window/Theme");
    theme.open(QFile::ReadOnly | QFile::Text);
    QTextStream themeInput(&theme);
    setStyleSheet(themeInput.readAll());
    theme.close();

    //Create Menu Bar
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    QMenu *toolsMenu = menuBar->addMenu("Tools");
    QAction *actionHistory = toolsMenu->addAction("History");
    QAction *actionSettings = toolsMenu->addAction("Setings");
    QAction *actionExit = toolsMenu->addAction("Exit");
    QMenu *helpMenu = menuBar->addMenu("Help");
    QAction *actionReportBug = helpMenu->addAction("Report Bug");
    QAction *actionAbout = helpMenu->addAction("About");

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

    //Setup this window
    settings_update();

    //Setup other windows
    aboutWindow = new About();
    settingsWindow = new Settings();
    historyWindow = new History();

    //Setup events
    connect(mediaPlayer, SIGNAL(samMetaDataChanged()), SLOT(showNowPlaying()));
    connect(mediaPlayer, SIGNAL(isPlayingState(bool)), SLOT(mediaButton_togglePlay(bool)));
    connect(buttonAction, SIGNAL(triggered()), SLOT(mediaButton_clicked()));
    connect(windowAction, SIGNAL(triggered()), SLOT(showWindow()));
    connect(songDisplay, SIGNAL(albumArtToggled(bool)), SLOT(albumArt_toggled(bool)));
    connect(settingsWindow, SIGNAL(updateSettings()), SLOT(settings_update()));
    connect(mediaPlayer, SIGNAL(samMetaDataChanged(SamMedia *)), historyWindow, SLOT(songChanged(SamMedia *)));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    //MenuBar events
    connect(actionHistory, SIGNAL(triggered()), historyWindow, SLOT(show()));
    connect(actionSettings, SIGNAL(triggered()), settingsWindow, SLOT(show()));
    connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionReportBug, SIGNAL(triggered()), SLOT(actionReportBug_triggered()));
    connect(actionAbout, SIGNAL(triggered()), aboutWindow, SLOT(show()));
}

void Window::keyPressEvent(QKeyEvent *event)
{
    //Get key input events
    if(event->key() == 16777313) { //Android Back Button
        //parentWidget()->showMinimized(); //***Need to fix this!!!
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
}

void Window::actionReportBug_triggered()
{
    //***If no email app show a popup!
    QString cmd = "mailto:partyatdans@gmail.com?subject=Bug Report " + QGuiApplication::applicationVersion();
    QDesktopServices::openUrl(QUrl(cmd + " on " + qApp->platformName() + "&body=Please describe the problem:"));
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

void Window::settings_update()
{
    if(settings->value("showTaskbarIcon").toBool())
        systemTray->show();
    else
        systemTray->hide();
    if(settings->value("alwaysTop").toBool()) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        show();
        update();
    }
    else
    {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
        show();
        update();
    }
    setMaximumHeight(0);
    setMinimumWidth(400);
    setMaximumWidth(minimumWidth());
    if(settings->value("smallPlayer").toBool()) {
        serverInfo->hide();
        songDisplay->hideAlbumArt(true);
        timeBar->hide();
        songCaster->hide();
        setMinimumHeight(0);
        setMaximumHeight(0);
    }
    else {
        serverInfo->show();
        songDisplay->hideAlbumArt(false);
        timeBar->show();
        songCaster->show();
        setMinimumHeight(600);
        setMaximumHeight(0);
    }
}
