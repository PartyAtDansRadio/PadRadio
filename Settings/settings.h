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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QStyle>
#include <QDesktopWidget>
#include <QSettings>
#include <QScrollArea>
#include <QScreen>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QPushButton>

#include "boolsetting.h"
#include "textsetting.h"
#include "divsetting.h"


class Settings : public QFrame
{
    Q_OBJECT
    public:
        explicit Settings(QWidget *parent = 0);
        static void loadDefaults(QSettings *settings);
    private:
        QSettings *settings;
        BoolSetting *taskbarMessages;
        BoolSetting *taskbarStart;
        TextSetting *mediaStream;
        TextSetting *metaData;
    private slots:
        void rememberLocation_newValue(bool value);
        void smallPlayer_newValue(bool value);
        void alwaysTop_newValue(bool value);
        void taskbarIcon_newValue(bool value);
        void taskbarMessages_newValue(bool value);
        void taskbarStart_newValue(bool value);
        void mediaStream_newValue(QString value);
        void metaData_newValue(QString value);
        void close_window();
    signals:
        void updateSettings();
};

#endif // SETTINGS_H
