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
//#include <QMediaPlayerControl> //I may need this for making a playlist...

SamMedia::SamMedia(QUrl samMetaData, QObject *parent) :
    QMediaPlayer(parent, QMediaPlayer::StreamPlayback), samMetaData(samMetaData)
{
    //Setup object
    playing = false;
    doingUpdate = false;
    hasData = false;    
    timer = new QTimer(this);
    timer->start(100);

    connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(setPlaying(QMediaPlayer::State)));
    connect(timer, SIGNAL(timeout()), SLOT(timeTriggerUpdate()));
}

QVariant SamMedia::metaData(QString &key) const
{
    //Look for meta value using key
    if(hasData) {
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
    else {
        return QMediaPlayer::metaData(key);
    }
}

QStringList SamMedia::availableMetaData() const
{
    //Get types of meta data
    if(hasData) {
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
        for(QString item : QMediaPlayer::availableMetaData()) {
            if(!metaData.contains(item))
                metaData.append(item);
        }
        return metaData;
    }
    return QMediaPlayer::availableMetaData();
}

void SamMedia::timeTriggerUpdate()
{
    //Try to update meta data by finding the start of the next song
    if(!doingUpdate) {
        QTime *zero = new QTime(0, 0);
        int playedAt = zero->secsTo(extraMeta.MetaUpdateTime);
        int endTime = zero->secsTo(extraMeta.Duration);
        int thisTime = zero->secsTo(QTime::currentTime());
        if(thisTime >= playedAt + endTime) {
            updateSamMetaData();
            timer->setInterval(100);
        }
        else {
            timer->setInterval(endTime*1000 - (thisTime*1000 - playedAt*1000));
        }
    }
    else {
        timer->setInterval(100);
    }
}

void SamMedia::updateSamMetaData()
{
    //Update music info
    qDebug() << "Trying to Update Sam MetaData...";
    doingUpdate = true;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(samMetaDataReply(QNetworkReply*)));
    manager->get(QNetworkRequest(samMetaData));
}

void SamMedia::samMetaDataReply(QNetworkReply* reply)
{
    //Try to set music info
    if(reply->error() == reply->NoError) {
        QList<QString> rawData = QList<QString>(((QString)reply->readAll()).split("\r\n"));
        QTime updateTime = QTime::fromString(rawData[7], "h:mm:ss ap");
        if(extraMeta.MetaUpdateTime.toString() != updateTime.toString()) {
            qDebug() << "Found Sam MetaData Update...";
            extraMeta.AlbumArtist = rawData[0];
            extraMeta.AlbumTitle = rawData[1];
            if(rawData[2].right(1) == "/")
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
            hasData = true;
            qDebug() << "Got:" << availableMetaData();
            emit samMetaDataChanged();
        }
        else {
            qDebug() << "MetaData is already up to date!";
        }
    }
    else {
        qDebug() << "Sam MetaData Not Found!";
        hasData = false;
        timer->setInterval(100);
    }
    doingUpdate = false;
}

void SamMedia::togglePlayStop()
{
    if(isPlaying())
        stop();
    else
        play();
}

void SamMedia::setPlaying(QMediaPlayer::State state)
{
    if(state == PlayingState)
        playing = true;
    else
        playing = false;
    emit isPlayingState(playing);
}

bool SamMedia::isPlaying()
{
    return playing;
}
