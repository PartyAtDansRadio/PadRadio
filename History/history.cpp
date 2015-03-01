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

#include "history.h"

History::History(QWidget *parent) : QFrame(parent)
{
    //Setup window
    setWindowTitle("Pad Radio - History");

    //Theme ui
    QFile theme(":/History/Theme");
    theme.open(QFile::ReadOnly | QFile::Text);
    QTextStream themeInput(&theme);
    setStyleSheet(themeInput.readAll());
    theme.close();

    //Setup widget
    QLabel *title = new QLabel("Party At Dans History", this);
    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Artist"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Album"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Title"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Genre"));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem("Year"));

    //Init song list
    memory = new Memory("History.csv", this);
    for(int id=0; id < memory->songCount(); id++) {
        addToTable(memory->readSongById(id));
    }

    //Setup layout
    QVBoxLayout *holder = new QVBoxLayout(this);
    setLayout(holder);
    holder->addWidget(title);
    holder->addWidget(table);
}

void History::addToTable(Song *song)
{
    //Build new row
    QTableWidgetItem *time = new QTableWidgetItem(song->getInfo().time);
    QTableWidgetItem *albumArtist = new QTableWidgetItem(song->getInfo().albumArtist);
    QTableWidgetItem *albumTitle = new QTableWidgetItem(song->getInfo().albumTitle);
    QTableWidgetItem *title = new QTableWidgetItem(song->getInfo().title);
    QTableWidgetItem *genre = new QTableWidgetItem(song->getInfo().genre);
    QTableWidgetItem *year = new QTableWidgetItem(song->getInfo().year);

    //Config new row
    song->setIcon(time);
    time->setFlags(time->flags() ^ Qt::ItemIsEditable);
    albumArtist->setFlags(albumArtist->flags() ^ Qt::ItemIsEditable);
    albumTitle->setFlags(albumTitle->flags() ^ Qt::ItemIsEditable);
    title->setFlags(title->flags() ^ Qt::ItemIsEditable);
    genre->setFlags(genre->flags() ^ Qt::ItemIsEditable);
    year->setFlags(year->flags() ^ Qt::ItemIsEditable);

    //Set new row
    table->insertRow(table->rowCount());
    table->setVerticalHeaderItem(table->rowCount()-1, time);
    table->setItem(table->rowCount()-1, 0, albumArtist);
    table->setItem(table->rowCount()-1, 1, albumTitle);
    table->setItem(table->rowCount()-1, 2, title);
    table->setItem(table->rowCount()-1, 3, genre);
    table->setItem(table->rowCount()-1, 4, year);

}

void History::songChanged(SamMedia *mediaPlayer)
{ 
    Song::Info info;
    info.albumArt = mediaPlayer->metaData(SamMetaType::CoverArtImage).toUrl();
    info.time = QTime::currentTime().toString("h:mm:ss AP");
    info.albumArtist = mediaPlayer->metaData(SamMetaType::AlbumArtist).toString();
    info.albumTitle = mediaPlayer->metaData(SamMetaType::AlbumTitle).toString();
    info.title = mediaPlayer->metaData(SamMetaType::Title).toString();
    info.genre = mediaPlayer->metaData(SamMetaType::Genre).toString();
    info.year = mediaPlayer->metaData(SamMetaType::Year).toString();
    Song *song = new Song(info);
    addToTable(song);
    memory->appendSong(song);
}
