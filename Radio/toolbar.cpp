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
    playStop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    playStop->setCheckable(true);
    playStop->setChecked(true);
    QLabel *turnDown = new QLabel("-", this);
    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slider->setValue(mediaPlayer->volume());
    QLabel *turnUp = new QLabel("+", this);

    //Setup layout
    QHBoxLayout *holder = new QHBoxLayout(this);
    holder->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *sliderHolder = new QHBoxLayout(this);
    sliderHolder->setContentsMargins(0, 0, 0, 0);
    QFrame *sliderFrame = new QFrame(this);
    sliderFrame->setLayout(sliderHolder);
    setLayout(holder);
    holder->addWidget(playStop);
    holder->addWidget(sliderFrame);
    sliderHolder->addWidget(turnDown);
    sliderHolder->addWidget(slider);
    sliderHolder->addWidget(turnUp);
    holder->addLayout(sliderHolder);

    //Setup events
    connect(mediaPlayer, SIGNAL(isPlayingState(bool)), SLOT(togglePlay(bool)));
    connect(playStop, SIGNAL(toggled(bool)), SLOT(togglePlay(bool)));
    connect(slider, SIGNAL(valueChanged(int)), SLOT(changeVolume(int)));
}

void ToolBar::togglePlay(bool play)
{
    if(play) {
        if(!mediaPlayer->isPlaying())
            mediaPlayer->play();
        playStop->setText("Stop");
    }
    else {
        if(mediaPlayer->isPlaying())
            mediaPlayer->stop();
        playStop->setText("Play");
    }
}

void ToolBar::changeVolume(int volume)
{
    mediaPlayer->setVolume(volume);
}
