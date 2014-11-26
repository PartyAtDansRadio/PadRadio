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

AlbumArt::AlbumArt(SamMedia *mediaPlayer, QWidget *parent) :
    QFrame(parent), mediaPlayer(mediaPlayer)
{
    //Setup widgets
    albumArt = new QToolButton(this);
    albumArt->isCheckable();
    albumArt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    //Setup layout
    QHBoxLayout *holder = new QHBoxLayout(this);
    holder->setContentsMargins(0, 0, 0, 0);
    setLayout(holder);
    holder->addWidget(albumArt);

    //Setup events
    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer, SIGNAL(timeout()), SLOT(updateImageSize()));
    connect(mediaPlayer, SIGNAL(samMetaDataChanged()), SLOT(metaUpdate()));
    connect(albumArt, SIGNAL(clicked(bool)), SIGNAL(toggled(bool)));

    //Setup calls
    updateImageSize();
    metaUpdate();
}

void AlbumArt::updateImageSize()
{
    if(albumArt->iconSize() != QSize(albumArt->height(), albumArt->height()))
        albumArt->setIconSize(QSize(albumArt->height(), albumArt->height()));
}

void AlbumArt::metaUpdate()
{
    updateImage(mediaPlayer->metaData(SamMetaType::CoverArtImage).toUrl());
}

void AlbumArt::updateImage(QUrl albumArtUrl)
{
    //Try to update current album art image by resource
    if(albumArtUrl == QUrl()) {
        QPixmap map;
        map.load(":/Window/PadLogo");
        albumArt->setIcon(QIcon(map.scaled(1080, 1080, Qt::KeepAspectRatio)));
    }
    else {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateImageReply(QNetworkReply*)));
        manager->get(QNetworkRequest(QUrl(albumArtUrl)));
    }
}

void AlbumArt::updateImageReply(QNetworkReply* reply)
{
    //Try to set current album art image by network
    if(reply->error() == reply->NoError) {
        QPixmap map;
        map.loadFromData(reply->readAll());
        albumArt->setIcon(QIcon(map.scaled(1080, 1080, Qt::KeepAspectRatio)));
    }
    else {
        QPixmap map;
        map.load(":/Window/PadLogo");
        albumArt->setIcon(QIcon(map.scaled(1080, 1080, Qt::KeepAspectRatio)));
    }
}
