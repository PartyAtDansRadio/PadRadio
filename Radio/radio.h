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

#ifndef RADIO_H
#define RADIO_H

#include <QWidget>

//Qt widgets
#include <QMainWindow>
#include <QMediaPlayer>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QSettings>
#include <QScreen>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QGraphicsDropShadowEffect>
#include <QMenuBar>

//Custom widgets
#include "../sammedia.h"
#include "serverinfo.h"
#include "SongDisplay/songdisplay.h"
#include "timebar.h"
#include "toolbar.h"
#include "songcaster.h"

class Radio : public QFrame
{
    Q_OBJECT
    public:
        explicit Radio(QWidget *parent = 0);
        QString getNowPlaying();
        ~Radio();
    private:
        //System
        QSettings *settings;
        SamMedia *mediaPlayer;
        //Widgets
        ServerInfo *serverInfo;
        SongDisplay *songDisplay;
        TimeBar *timeBar;
        ToolBar *toolBar;
        SongCaster *songCaster;
    signals:
        void song_changed();
    public slots:
        void update_settings();
};

#endif // RADIO_H
