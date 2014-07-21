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
#include <QMediaMetaData>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

namespace Ui
{
    class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    void updateImage();
    ~Window();

private slots:
    void mainLoop();
    void updateImageReply(QNetworkReply* reply);
    void updateMetaData();
    void metaDataReply(QNetworkReply* reply);
    void metaDidUpdate();

signals:
    void metaUpdate();

private:
    QList<QString> metaData;
    Ui::Window *ui;
    QMediaPlayer *mediaPlayer;
    QTimer *timer;
};

#endif // WINDOW_H
