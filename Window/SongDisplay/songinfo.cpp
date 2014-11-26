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

#include "songinfo.h"

SongInfo::SongInfo(SamMedia *mediaPlayer, QWidget *parent) :
    QFrame(parent), mediaPlayer(mediaPlayer)
{  
    //Setup widgets
    QSpacerItem *spacerLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding);
    albumArtist = new QLabel("Artist", this);
    QLabel *albumDiv = new QLabel("-", this);
    albumTitle = new QLabel("Album", this);
    QSpacerItem *spacerRight = new QSpacerItem(1, 1, QSizePolicy::Expanding);
    Songtitle = new QLabel("Title", this);
    Songtitle->setAlignment(Qt::AlignCenter);

    //Setup layouts
    QVBoxLayout *holder = new QVBoxLayout(this);
    QHBoxLayout *albumHolder = new QHBoxLayout(this);
    holder->setContentsMargins(10, 0, 10, 0);
    setLayout(holder);
    holder->addLayout(albumHolder);
    albumHolder->addItem(spacerLeft);
    albumHolder->addWidget(albumArtist);
    albumHolder->addWidget(albumDiv);
    albumHolder->addWidget(albumTitle);
    albumHolder->addItem(spacerRight);
    holder->addWidget(Songtitle);

    //Setup events
    connect(mediaPlayer, SIGNAL(samMetaDataChanged()), SLOT(metaUpdate()));
}

void SongInfo::metaUpdate()
{
    albumArtist->setText(mediaPlayer->metaData(SamMetaType::AlbumArtist).toString());
    albumTitle->setText(mediaPlayer->metaData(SamMetaType::AlbumTitle).toString());
    Songtitle->setText(mediaPlayer->metaData(SamMetaType::Title).toString());
}
