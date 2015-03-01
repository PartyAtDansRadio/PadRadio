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

#include "songcaster.h"

SongCaster::SongCaster(SamMedia *mediaPlayer, QWidget *parent) :
    QFrame(parent), mediaPlayer(mediaPlayer)
{
    //Setup widgets
    QFrame *div = new QFrame(this);
    div->setAutoFillBackground(true);
    div->setFrameShape(QFrame::HLine);
    div->setFrameShadow(QFrame::Sunken);
    nextSong = new ScrollText(this);
    nextSong->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    nextSong->setText("Loading, please stand by...");
    QLabel *serverLink = new QLabel(this);
    serverLink->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    serverLink->setAlignment(Qt::AlignRight);
    serverLink->setTextFormat(Qt::RichText);

    //Set label html file
    QFile html(":/Radio/ServerLink");
    html.open(QFile::ReadOnly | QFile::Text);
    QTextStream htmlInput(&html);
    serverLink->setText(htmlInput.readAll());
    html.close();

    //Setup layouts
    QVBoxLayout *holder = new QVBoxLayout(this);
    holder->setContentsMargins(0, 0, 0, 10);
    setLayout(holder);
    holder->addWidget(div);
    QHBoxLayout *textHolder = new QHBoxLayout(this);
    textHolder->setContentsMargins(0, 0, 0, 0);
    holder->addLayout(textHolder);
    textHolder->addWidget(nextSong);
    textHolder->addWidget(serverLink);

    //Setup events
    connect(mediaPlayer, SIGNAL(samMetaDataChanged()), SLOT(metaUpdate()));
}

void SongCaster::metaUpdate()
{
    QString nextAlbumArtist = mediaPlayer->metaData(SamMetaType::NextAlbumArtist).toString();
    QString nextAlbumTitle = mediaPlayer->metaData(SamMetaType::NextAlbumTitle).toString();
    QString nextTitle = mediaPlayer->metaData(SamMetaType::NextTitle).toString();
    nextSong->setText("Next up is " + nextAlbumArtist + "'s \"" + nextTitle + "\" from " + nextAlbumTitle + "...");
}
