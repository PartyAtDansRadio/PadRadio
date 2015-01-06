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

#ifndef ALBUMART_H
#define ALBUMART_H

#include <QFrame>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>

#include "../../sammedia.h"

class AlbumArt : public QFrame
{
    Q_OBJECT
    public:
        explicit AlbumArt(SamMedia *mediaPlayer, QWidget *parent = 0);
    private:
        SamMedia *mediaPlayer;
        QToolButton *albumArt;
        void updateImage(QUrl albumArtUrl);
    public slots :
        void metaUpdate();
    private slots:
        void updateImageSize();
        void updateImageReply(QNetworkReply* reply);
        void albumArt_toggled(bool toggled);
    signals:
        void toggled(bool);
};

#endif // ALBUMART_H
