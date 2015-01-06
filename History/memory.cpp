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

#include "memory.h"

Memory::Memory(QString fileName, QObject *parent) : fileName(fileName), QObject(parent)
{
    //Read and remove history file
    QFile file(fileName);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    QFile::remove(fileName);

    //Load file stream items into data vector
    data = QVector<QStringList>();
    while(!stream.atEnd()) {
        QStringList stringList = stream.readLine().split("\t");
        if(stringList.length() == 8) {
            qDebug() << "loading:" << stringList;
            QStringList date = stringList[0].split(' ');
            //if(QDate(date[0].toInt(), date[1].toInt(), date[2].toInt()).daysTo(QDate::currentDate()) <= 1 ) {
                data.append(stringList);
            //}
        }
    }
    for(QStringList row: data) //Re-save data vector into histoy file
            saveSong(row);
}

int Memory::songCount()
{
    return data.length();
}

Song *Memory::readSongById(int id)
{
    //Setup song
    Song::Info info;
    info.albumArt = QUrl(data[id][1]);
    info.time = data[id][2];
    info.albumArtist = data[id][3];
    info.albumTitle = data[id][4];
    info.title = data[id][5];
    info.genre = data[id][6];
    info.year = data[id][7];

    //Return song
    Song *song = new Song(info, this);
    QStringList dateParts = data[id][0].split(' ');
    song->setDateAdded(QDate(dateParts[0].toInt(), dateParts[1].toInt(), dateParts[2].toInt()));
    return song;
}

void Memory::appendSong(Song *song)
{
    QStringList songString = QStringList();
    songString.append(song->getDateAdded().toString("yyyy MM dd"));
    songString.append(song->getInfo().albumArt.toString());
    songString.append(song->getInfo().time);
    songString.append(song->getInfo().albumArtist);
    songString.append(song->getInfo().albumTitle);
    songString.append(song->getInfo().title);
    songString.append(song->getInfo().genre);
    songString.append(song->getInfo().year);
    if(!data.contains(songString)) {
        data.append(songString);
        saveSong(songString);
    }
}

void Memory::saveSong(QStringList song)
{
    QString lastSong = data.last()[3] + data.last()[4] + data.last()[5];
    QString thisSong = song[3] + song[4] + song[5];
    qDebug() << data.length() << 0 << lastSong << thisSong;
    if(data.length() == 0 || lastSong != thisSong) {
        qDebug() << "Updating history file...";
        QFile file(fileName);
        file.open(QFile::Append | QFile::Text);
        QTextStream stream(&file);
        bool isFirstLine = true;
        for(QString string : song) {
            qDebug() << "Save item:" << string;
            if(isFirstLine) {
                isFirstLine = false;
                stream << string;
            }
            else {
                stream << '\t' << string;
            }
        }
        stream << '\n';
        qDebug() << "Updated history file.\n";
    }
}
