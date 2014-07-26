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


#include "sammedia.h"

SamMedia::SamMedia(QUrl samMetaData, QObject *parent) :
    QMediaPlayer(parent, QMediaPlayer::StreamPlayback), samMetaData(samMetaData)
{
    connect(this, SIGNAL(metaDataChanged()), SLOT(updateSamMetaData()));
}

QVariant SamMedia::metaData(QString &key) const
{
    //Look for meta value using key
    if(key == "AlbumArtist") {
        return QVariant(extraMeta.AlbumArtist);
    }
    else if(key == "AlbumTitle") {
        return QVariant(extraMeta.AlbumTitle);
    }
    else if(key == "CoverArtImage") {
        return QVariant(extraMeta.CoverArtImage);
    }
    else if(key == "Duration") {
        return QVariant(extraMeta.Duration);
    }
    else if(key == "Genre") {
        return QVariant(extraMeta.Genre);
    }
    else if(key == "Listeners") {
        return QVariant(extraMeta.Listeners);
    }
    else if(key == "ListenersMax") {
        return QVariant(extraMeta.ListenersMax);
    }
    else if(key == "MetaUpdateTime") {
        return QVariant(extraMeta.MetaUpdateTime);
    }
    else if(key == "NextAlbumArtist") {
        return QVariant(extraMeta.NextAlbumArtist);
    }
    else if(key == "NextAlbumTitle") {
        return QVariant(extraMeta.NextAlbumTitle);
    }
    else if(key == "NextTitle") {
        return QVariant(extraMeta.NextTitle);
    }
    else if(key == "Title") {
        return QVariant(extraMeta.Title);
    }
    else if(key == "Year") {
        return QVariant(extraMeta.Year);
    }
    else {
        return QMediaPlayer::metaData(key);
    }
}

QStringList SamMedia::availableMetaData() const
{
    //Get types of meta data
    QStringList metaData;
    metaData.append("AlbumArtist");
    metaData.append("AlbumTitle");
    metaData.append("CoverArtImage");
    metaData.append("Duration");
    metaData.append("Listeners");
    metaData.append("ListenersMax");
    metaData.append("MetaUpdateTime");
    metaData.append("NextAlbumArtist");
    metaData.append("NextAlbumTitle");
    metaData.append("NextTitle");
    metaData.append("Title");
    metaData.append("Year");
    return metaData;
}

void SamMedia::updateSamMetaData()
{
    //Update music info
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(samMetaDataReply(QNetworkReply*)));
    manager->get(QNetworkRequest(samMetaData));
}

void SamMedia::samMetaDataReply(QNetworkReply* reply)
{
    //Try to set music info
    QList<QString> rawData = QList<QString>(((QString)reply->readAll()).split("\r\n"));
    QTime updateTime = QTime();
    updateTime.fromString(rawData[7], "h:mm:ss ap");
    if(extraMeta.MetaUpdateTime.toString().length() == 0 || extraMeta.MetaUpdateTime.toString() != updateTime.toString()) {
        qDebug() << "Doing Sam Meta Data Update...";
        extraMeta.AlbumArtist = rawData[0];
        extraMeta.AlbumTitle = rawData[1];
        if(rawData[2] == "http://www.mcttelecom.com/~d_libby/pictures/")
            extraMeta.CoverArtImage = QUrl("");
        else
            extraMeta.CoverArtImage = QUrl(rawData[2]);
        extraMeta.Duration = QTime::fromString(rawData[3], "m:ss");
        extraMeta.Genre = rawData[4];
        extraMeta.Listeners = rawData[5].toInt();
        extraMeta.ListenersMax = rawData[6].toInt();
        extraMeta.MetaUpdateTime = QTime::fromString( rawData[7], "h:mm:ss ap");
        extraMeta.NextAlbumArtist = rawData[8];
        extraMeta.NextAlbumTitle = rawData[9];
        extraMeta.NextTitle = rawData[10];
        extraMeta.Title = rawData[11];
        extraMeta.Year = rawData[12].toInt();
        emit samMetaDataChanged();
   }
    else {
        qDebug() << "Sam Meta Data Not Updating!";
        QTimer *redo = new QTimer(this);
        connect(redo, SIGNAL(timeout()), this, SLOT(updateSamMetaData()));
        redo->setSingleShot(true);
        redo->setInterval(50);
        redo->start();
    }
}
