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

#ifndef HISTORY_H
#define HISTORY_H

#include <QFrame>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QApplication>
#include <QStyle>
#include <QScreen>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>

#include "sammedia.h"
#include "memory.h"
#include "song.h"

class History : public QFrame
{
    Q_OBJECT
    public:
        explicit History(QWidget *parent = 0);
        QTableWidget *table;
        Memory *memory;
    public slots:
        void addToTable(Song *song);
        void songChanged(SamMedia *mediaPlayer);
};

#endif // HISTORY_H
