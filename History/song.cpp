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

#include "song.h"

Song::Song(Info info, QObject *parent) : QObject(parent), info(info)
{
    albumArtItem = new QTableWidgetItem();
    date = QDate::currentDate();
}

Song::Info Song::getInfo() const
{
    return info;
}

QDate Song::getDateAdded()
{
    return date;
}

void Song::setDateAdded(QDate date)
{
    this->date = date;
}

void Song::setIcon(QTableWidgetItem *albumArtItem)
{
    this->albumArtItem = albumArtItem;
    //Try to set icon
    if(info.albumArt == QUrl()) {
        QPixmap map;
        map.load(":/PadLogo");
        albumArtItem->setIcon(QIcon(map.scaled(1080, 1080, Qt::KeepAspectRatio)));
    }
    else {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(loadIcon(QNetworkReply*)));
        manager->get(QNetworkRequest(QUrl(info.albumArt)));
    }
}

void Song::loadIcon(QNetworkReply* reply)
{
    //Try to set image by network
    if(reply->error() == reply->NoError) {
        QPixmap map;
        map.loadFromData(reply->readAll());
        albumArtItem->setIcon(QIcon(map.scaled(1080, 1080, Qt::KeepAspectRatio)));
    }
    else {
        QPixmap map;
        map.load(":/PadLogo");
        albumArtItem->setIcon(QIcon(map.scaled(1080, 1080, Qt::KeepAspectRatio)));
    }
}
