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

#include "albumart.h"

AlbumArt::AlbumArt(QWidget *parent) : QGraphicsView(parent)
{
    //Init art
    scene = new QGraphicsScene(this);
    updateArt();
}

void AlbumArt::updateArt(QString img)
{
    //Update using local backup image
    scene->clear();
    pixmapItem = scene->addPixmap(QPixmap(":/PadImg"));
    fitInView(pixmapItem);
    setScene(scene);
}

void AlbumArt::updateArt(QNetworkReply *img)
{
    //Update using web album image
    QPixmap map;
    map.loadFromData(img->readAll());
    scene->clear();
    pixmapItem = scene->addPixmap(map);
    setScene(scene);
    fitInView(pixmapItem);
}

void AlbumArt::resizeEvent(QResizeEvent* event)
{ 
    fitInView(pixmapItem);
}

void AlbumArt::mousePressEvent(QMouseEvent *)
{
    emit clickedOn();
}
