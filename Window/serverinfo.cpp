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

#include "serverinfo.h"

ServerInfo::ServerInfo(SamMedia *mediaPlayer, QWidget *parent) :
    QFrame(parent), mediaPlayer(mediaPlayer)
{
    //Setup widgets
    musicYear = new QLabel("Year: ", this);
    QLabel *musicDiv = new QLabel("-", this);
    musicGenre = new QLabel("Genre", this);
    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding);
    QLabel *listenersNote = new QLabel("Listeners:", this);
    listenersNote->setIndent(20);
    musicListeners = new QLabel("0", this);
    QLabel *listenersDiv = new QLabel("/", this);
    musicMaxListeners = new QLabel("0", this);

    //Setup layout
    QHBoxLayout *holder = new QHBoxLayout(this);
    setLayout(holder);
    holder->addWidget(musicYear);
    holder->addWidget(musicDiv);
    holder->addWidget(musicGenre);
    holder->addItem(spacer);
    holder->addWidget(listenersNote);
    holder->addWidget(musicListeners);
    holder->addWidget(listenersDiv);
    holder->addWidget(musicMaxListeners);

    //Setup events
    connect(mediaPlayer, SIGNAL(samMetaDataChanged()), SLOT(metaUpdate()));
}

void ServerInfo::metaUpdate()
{
    musicListeners->setText(mediaPlayer->metaData(SamMetaType::Listeners).toString());
    musicMaxListeners->setText(mediaPlayer->metaData(SamMetaType::ListenersMax).toString());
    musicYear->setText(mediaPlayer->metaData(SamMetaType::Year).toString());
    musicGenre->setText(mediaPlayer->metaData(SamMetaType::Genre).toString());
}
