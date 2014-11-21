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

#include "timebar.h"

TimeBar::TimeBar(SamMedia *mediaPlayer, QWidget *parent) :
    QFrame(parent), mediaPlayer(mediaPlayer)
{
    //Setup widgets
    progressBar = new QProgressBar(this);
    progressBar->setTextVisible(false);
    progressBar->setRange(0, 1000);
    time = new QLabel("0:00 / 0:00", this);

    //Setup layout
    QHBoxLayout *holder = new QHBoxLayout(this);
    setLayout(holder);
    holder->addWidget(progressBar);
    holder->addWidget(time);

    //Setup events
    QTimer *timer = new QTimer(this);
    timer->start(250);
    connect(timer, SIGNAL(timeout()), SLOT(metaUpdate()));
}

void TimeBar::metaUpdate()
{
    //Calculate time left
    QTime start = mediaPlayer->metaData(SamMetaType::MetaUpdateTime).toTime();
    QTime end = mediaPlayer->metaData(SamMetaType::Duration).toTime();
    if(end != QTime()) {
        QTime now = QTime::currentTime();
        int toEnd = start.msecsTo(now);
        int atEnd = QTime(0, 0, 0).msecsTo(end);
        QString secs = QString::number((int)(toEnd/1000)%60);
        if(secs.length() == 1)
            secs = '0' + secs;
        QString currentTime = QString::number((int)((toEnd/(1000*60))%60)) + ":" + secs;

        //Output current time and end time
        time->setText(currentTime + " / " + end.toString("m:ss"));
        progressBar->setValue(((float)toEnd/(float)atEnd)*1000);
    }
}
