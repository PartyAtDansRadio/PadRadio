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

#ifndef WINDOW_H
#define WINDOW_H

//Qt widgets
#include <QMainWindow>
#include <QMediaPlayer>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QSettings>
#include <QScreen>
#include <QStackedLayout>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMessageBox>

//Custom widgets
#include "Radio/radio.h"
#include "About/about.h"
#include "Settings/settings.h"
#include "History/history.h"

class Window : public QMainWindow
{
    Q_OBJECT
public:
    //System
    QSettings *settings;
    SamMedia *mediaPlayer;
    //Layout
    QStackedLayout *stackLayout;
    QPushButton *closeWindow;
    //TaskBar
    QSystemTrayIcon* systemTray;
    QAction *buttonAction;
    QAction *windowAction;
    //Windows
    Radio *radioWindow;
    About *aboutWindow;
    Settings *settingsWindow;
    History *historyWindow;
    explicit Window(QWidget *parent = 0);
    ~Window();

    signals:

    private slots:
        void update_settings();
        void taskbarIcon_showPopup();
        void actionHistory_triggered();
        void actionSettings_triggered();
        void actionReportBug_triggered();
        void actionAbout_triggered();
        void closeWindow_clicked();
};

#endif // WINDOW_H
