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

#include <QMainWindow>
#include <QMediaPlayer>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QSettings>
#include <QScreen>
#include <QVBoxLayout>
#include <QKeyEvent>

#include "sammedia.h"
#include "serverinfo.h"
#include "SongDisplay/songdisplay.h"
#include "timebar.h"
#include "toolbar.h"
#include "songcaster.h"
#include "../About/about.h"
#include "../Settings/settings.h"

namespace Ui
{
    class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    //System
    QSettings *settings;
    SamMedia *mediaPlayer;
    //TaskBar
    QSystemTrayIcon* systemTray;
    QAction *buttonAction;
    QAction *windowAction;
    //Widgets
    ServerInfo *serverInfo;
    SongDisplay *songDisplay;
    TimeBar *timeBar;
    ToolBar *toolBar;
    SongCaster *songCaster;
    //Windows
    About *aboutWindow;
    Settings *settingsWindow; 
    //EOL code
    Ui::Window *ui;
private slots:
    void samDidMetaUpdate();
    void showWindow();
    void on_actionReport_Bug_triggered();
    void on_actionAbout_triggered();
    void on_actionSettings_triggered();
    void on_actionExit_triggered();
    void albumArt_toggled(bool toggled);
};

#endif // WINDOW_H
