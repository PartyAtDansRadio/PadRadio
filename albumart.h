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

#ifndef ALBUMART_H
#define ALBUMART_H

#include <QNetworkReply>
#include <QGraphicsView>
#include <QImage>
#include <QResizeEvent>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

class AlbumArt : public QGraphicsView
{
    Q_OBJECT

public:
    explicit AlbumArt(QWidget *parent = 0);
    void updateArt(QNetworkReply *img);
    void updateArt(QString img);

protected:
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent*);

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;

signals:
    void clickedOn();
};

#endif // ALBUMART_H
