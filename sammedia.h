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

#ifndef SAMMEDIA_H
#define SAMMEDIA_H

#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

namespace SamMetaType
{
    static QString AlbumArtist = "AlbumArtist";
    static QString AlbumTitle = "AlbumTitle";
    static QString CoverArtImage = "CoverArtImage";
    static QString Duration = "Duration";
    static QString Genre = "Genre";
    static QString Listeners = "Listeners";
    static QString ListenersMax = "ListenersMax";
    static QString MetaUpdateTime = "MetaUpdateTime";
    static QString NextAlbumArtist = "NextAlbumArtist";
    static QString NextAlbumTitle = "NextAlbumTitle";
    static QString NextTitle = "NextTitle";
    static QString Title = "Title";
    static QString Year = "Year";
}

class SamMedia : public QMediaPlayer
{
    Q_OBJECT

public:
    explicit SamMedia(QUrl samMetaData, QObject *parent = 0);
    QVariant metaData(QString &key) const;
    QStringList availableMetaData() const;
    bool isPlaying();

private:
    struct SamExtraMetaData
    {
        QString AlbumArtist;
        QString AlbumTitle;
        QUrl CoverArtImage;
        QTime Duration;
        QString Genre;
        int Listeners;
        int ListenersMax;
        QTime MetaUpdateTime;
        QString NextAlbumArtist;
        QString NextAlbumTitle;
        QString NextTitle;
        QString Title;
        int Year;

    };
    bool playing;
    bool hasData;
    bool doingUpdate;
    QTimer *timer;
    SamExtraMetaData extraMeta;
    QUrl samMetaData;

signals:
    void samMetaDataChanged(SamMedia *);
    void samMetaDataChanged();
    void isPlayingState(bool);

private slots:
    void setPlaying(QMediaPlayer::State state);
    void timeTriggerUpdate();
    void samMetaDataReply(QNetworkReply* reply);

public slots:
    void togglePlayStop();
    void updateSamMetaData();
};

#endif // SAMMEDIA_H
