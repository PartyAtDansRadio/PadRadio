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

#ifndef SONGCASTER_H
#define SONGCASTER_H

#include <QFrame>
#include <QLabel>
#include <QFile>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../sammedia.h"
#include "scrolltext.h"

class SongCaster : public QFrame
{
    Q_OBJECT
    public:
        explicit SongCaster(SamMedia *mediaPlayer, QWidget *parent = 0);
    private:
        SamMedia *mediaPlayer;
        ScrollText *nextSong;
    private slots:
        void metaUpdate();
};

#endif // SONGCASTER_H
