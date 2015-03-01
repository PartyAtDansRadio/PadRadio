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

Window::Window(QWidget *parent) : QMainWindow(parent)
{
    //Load settings
    settings = new QSettings("Settings.ini", QSettings::IniFormat, this);

    //Setup QWidgets
    QWidget *cWidget = new QWidget(this);
    cWidget->setObjectName("cWidget");
    setCentralWidget(cWidget);
    radioWindow = new Radio(this);
    aboutWindow = new About(this);
    settingsWindow = new Settings(this);
    historyWindow = new History(this);
    closeWindow = new QPushButton("Back to player", this);
    closeWindow->hide();

    //Config Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(cWidget);
    stackLayout = new QStackedLayout(this);
    cWidget->setLayout(mainLayout);
    mainLayout->addLayout(stackLayout);
    stackLayout->setContentsMargins(15, 10, 15, 10);
    stackLayout->addWidget(radioWindow);
    stackLayout->addWidget(aboutWindow);
    stackLayout->addWidget(settingsWindow);
    stackLayout->addWidget(historyWindow);
    mainLayout->addWidget(closeWindow);

    //Theme window
    QFile theme(":/Theme");
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
    systemTray->setIcon(QIcon(":/PadLogo"));
    systemTray->setContextMenu(trayIconMenu);
    systemTray->show();

    //Setup main events
    connect(settingsWindow, SIGNAL(changed()), SLOT(update_settings()));
    connect(settingsWindow, SIGNAL(changed()), radioWindow, SLOT(update_settings()));
    connect(radioWindow, SIGNAL(song_changed()), SLOT(taskbarIcon_showPopup()));

    //Setup subwindow events
    connect(actionHistory, SIGNAL(triggered()), SLOT(actionHistory_triggered()));
    connect(actionSettings, SIGNAL(triggered()), SLOT(actionSettings_triggered()));
    connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionReportBug, SIGNAL(triggered()), SLOT(actionReportBug_triggered()));
    connect(actionAbout, SIGNAL(triggered()), SLOT(actionAbout_triggered()));
    connect(closeWindow, SIGNAL(clicked()), SLOT(closeWindow_clicked()));
}

void Window::update_settings()
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
}

void Window::taskbarIcon_showPopup()
{
    if(settings->value("showTaskbarIcon").toBool() && settings->value("showMessages").toBool() && !isActiveWindow())
        systemTray->showMessage("Party At Dans Radio...", radioWindow->getNowPlaying(), QSystemTrayIcon::Information, 30000);
}

void Window::actionHistory_triggered()
{
    stackLayout->setCurrentWidget(historyWindow);
    closeWindow->show();
}

void Window::actionSettings_triggered()
{
    //connect settingsWindow settingsUpdate Radio updatesettings
    stackLayout->setCurrentWidget(settingsWindow);
    closeWindow->show();
}

void Window::actionReportBug_triggered()
{
    QString cmd = "mailto:partyatdans@gmail.com?subject=Bug Report " + QGuiApplication::applicationVersion();
    if(!QDesktopServices::openUrl(QUrl(cmd + " on " + qApp->platformName() + "&body=Please describe the problem:"))) {
        QMessageBox::about(this, "Bug Report", "Send bug info to: partyatdans@gmail.com\nsubject: Bug Report.");
    }
}

void Window::actionAbout_triggered()
{
    stackLayout->setCurrentWidget(aboutWindow);
    closeWindow->show();
}

void Window::closeWindow_clicked()
{
    stackLayout->setCurrentWidget(radioWindow);
    closeWindow->hide();
}

Window::~Window()
{
    //Try to save window geometry
    if(settings->value("rememberLocation").toBool())
        settings->setValue("WindowGeometry", saveGeometry());
    settings->sync();
}
