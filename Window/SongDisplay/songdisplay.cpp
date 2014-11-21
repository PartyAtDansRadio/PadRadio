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

#include "songdisplay.h"

SongDisplay::SongDisplay(SamMedia *mediaPlayer, QWidget *parent) :
    QFrame(parent), mediaPlayer(mediaPlayer)
{

    //Setup widgets
    QFrame *songDisplay = new QFrame(this);
    albumArt = new AlbumArt(mediaPlayer, this);

    QSpacerItem *spacerLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding);
    songInfo = new SongInfo(mediaPlayer, songDisplay);
    QSpacerItem *spacerRight = new QSpacerItem(1, 1, QSizePolicy::Expanding);

    //Setup layouts
    QVBoxLayout *holder = new QVBoxLayout(this);
    QHBoxLayout *textHolder = new QHBoxLayout(this);

    setLayout(holder);
    holder->addWidget(albumArt);
    holder->addLayout(textHolder);
    textHolder->addItem(spacerLeft);
    textHolder->addWidget(songInfo);
    textHolder->addItem(spacerRight);

    //Setup events
    connect(albumArt, SIGNAL(toggled(bool)), SIGNAL(albumArtToggled(bool)));
}

void SongDisplay::hideAlbumArt(bool hide)
{
    albumArt->setVisible(!hide);
}
