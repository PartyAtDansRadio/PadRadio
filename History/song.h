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

#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTableWidgetItem>
#include <QIcon>

class Song : public QObject
{
    Q_OBJECT
    public:
        struct Info {
            QUrl albumArt;
            QString time;
            QString albumArtist;
            QString albumTitle;
            QString title;
            QString genre;
            QString year;
        };
        explicit Song(Info info, QObject *parent = 0);
        Info getInfo() const;
        QDate getDateAdded();
        void setDateAdded(QDate date);
        void setIcon(QTableWidgetItem *albumArtItem);
    private:
        Info info;
        QDate date;
        QTableWidgetItem *albumArtItem;
    private slots:
        void loadIcon(QNetworkReply* reply);

};

#endif // SONG_H
