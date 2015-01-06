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

#ifndef SONGDISPLAY_H
#define SONGDISPLAY_H

#include <QFrame>

#include "../../sammedia.h"
#include "albumart.h"
#include "songinfo.h"

class SongDisplay : public QFrame
{
    Q_OBJECT
    public:
        explicit SongDisplay(SamMedia *mediaPlayer, QWidget *parent = 0);
        void hideAlbumArt(bool hide);
        void hideTimeBar(bool hide);
    protected:
        void resizeEvent(QResizeEvent *);
    private:
        SamMedia *mediaPlayer;
        AlbumArt *albumArt;
        SongInfo *songInfo;
    signals:
        void albumArtToggled(bool toggled);
};

#endif // SONGDISPLAY_H
