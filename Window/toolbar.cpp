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

#include "toolbar.h"

ToolBar::ToolBar(SamMedia *mediaPlayer, QWidget *parent) :
    QFrame(parent), mediaPlayer(mediaPlayer)
{
    //Setup widgets
    playStop = new QPushButton("Stop", this);
    playStop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    playStop->setCheckable(true);
    playStop->setChecked(true);
    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    slider->setValue(mediaPlayer->volume());

    //Setup layout
    QHBoxLayout *holder = new QHBoxLayout(this);
    setLayout(holder);
    holder->addWidget(playStop);
    holder->addWidget(slider);

    //Setup events
    connect(playStop, SIGNAL(toggled(bool)), SLOT(togglePlay(bool)));
    connect(slider, SIGNAL(valueChanged(int)), SLOT(changeVolume(int)));
}

void ToolBar::togglePlay(bool play)
{
    if(play) {
        mediaPlayer->play();
        playStop->setText("Stop");
    }
    else {
        mediaPlayer->stop();
        playStop->setText("Start");
    }
}

void ToolBar::changeVolume(int volume)
{
    mediaPlayer->setVolume(volume);
}
