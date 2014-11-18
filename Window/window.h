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
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QSettings>
#include <QScreen>
#include <QResizeEvent>

//#include "scrolltext.h" //Need this for later
#include "sammedia.h"
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
    void updateImage(QUrl albumArt = QUrl());
    ~Window();

protected:
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Window *ui;
    About *aboutWindow;
    Settings *settingsWindow;
    QSettings *settings;
    QSystemTrayIcon* systemTray;
    QAction *buttonAction;
    QAction *windowAction;
    bool playing; //Temp var, need to find or add this value in mediaPlayer
    SamMedia *mediaPlayer;
    QTimer *timer;

private slots:
    void mainLoop();
    void updateImageReply(QNetworkReply* reply);
    void samDidMetaUpdate();
    void showWindow();
    void on_actionReport_Bug_triggered();
    void on_actionAbout_triggered();
    void on_actionSettings_triggered();
    void on_actionExit_triggered();
    void on_toolAlbumArt_clicked();
    void on_playButton_toggled(bool checked);
};

#endif // WINDOW_H
