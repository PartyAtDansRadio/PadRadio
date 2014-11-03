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

#include "divsetting.h"

DivSetting::DivSetting(QString note, QWidget *parent) : QFrame(parent)
{
    //Make left divider line
    QFrame *lLine = new QFrame(this);
    lLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    lLine->setFrameShape(QFrame::HLine);
    lLine->setFrameShadow(QFrame::Sunken);

    //Make divider note
    QLabel *divNote = new QLabel(note, this);

    //Make right divider line
    QFrame *rLine = new QFrame(this);
    rLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    rLine->setFrameShape(QFrame::HLine);
    rLine->setFrameShadow(QFrame::Sunken);

    //Layout divider
    QHBoxLayout *holder = new QHBoxLayout(this);
    holder->addWidget(lLine);
    holder->addWidget(divNote);
    holder->addWidget(rLine);
    setLayout(holder);
}
