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

#include "radio.h"

Radio::Radio(QWidget *parent) : QFrame(parent)
{
    //Load settings
    setWindowTitle("Party At Dans Radio");
    settings = new QSettings("Settings.ini", QSettings::IniFormat, this);

    //Setup media player
    mediaPlayer = new SamMedia(QUrl(settings->value("MetaData").toString()), this);
    mediaPlayer->setMedia(QUrl(settings->value("MediaStream").toString()));
    mediaPlayer->setVolume(50); //***Add to settings
    mediaPlayer->play();

    //Setup QWidgets
    serverInfo = new ServerInfo(mediaPlayer, this);
    songDisplay = new SongDisplay(mediaPlayer, this);
    timeBar = new TimeBar(mediaPlayer, this);
    toolBar = new ToolBar(mediaPlayer, this);
    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    songCaster = new SongCaster(mediaPlayer, this);

    //Config Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(serverInfo);
    mainLayout->addWidget(songDisplay);
    mainLayout->addWidget(timeBar);
    mainLayout->addWidget(toolBar);
    mainLayout->addItem(spacer);
    mainLayout->addWidget(songCaster);

    //Add shadow drop to widgets - need to find a cleaner way to do this
    QGraphicsDropShadowEffect *effect1 = new QGraphicsDropShadowEffect();
    effect1->setBlurRadius(5);
    effect1->setOffset(5, 5);
    serverInfo->setGraphicsEffect(effect1);
    QGraphicsDropShadowEffect *effect2 = new QGraphicsDropShadowEffect();
    effect2->setBlurRadius(5);
    effect2->setOffset(5, 5);
    songDisplay->setGraphicsEffect(effect2);
    QGraphicsDropShadowEffect *effect3 = new QGraphicsDropShadowEffect();
    effect3->setBlurRadius(5);
    effect3->setOffset(5, 5);
    timeBar->setGraphicsEffect(effect3);
    QGraphicsDropShadowEffect *effect4 = new QGraphicsDropShadowEffect();
    effect4->setBlurRadius(5);
    effect4->setOffset(5, 5);
    toolBar->setGraphicsEffect(effect4);

    //Theme window
    QFile theme(":/Radio/Theme");
    theme.open(QFile::ReadOnly | QFile::Text);
    QTextStream themeInput(&theme);
    setStyleSheet(themeInput.readAll());
    theme.close();

    //Setup events
    connect(mediaPlayer, SIGNAL(samMetaDataChanged()), SIGNAL(song_changed()));
}

void Radio::update_settings()
{
    if(settings->value("smallPlayer").toBool()) {
       serverInfo->hide();
       songDisplay->hideAlbumArt(true);
       timeBar->hide();
       songCaster->hide();
   }
   else {
       serverInfo->show();
       songDisplay->hideAlbumArt(false);
       timeBar->show();
       songCaster->show();
   }
}

QString Radio::getNowPlaying()
{
    return "Playing " + mediaPlayer->metaData(SamMetaType::AlbumArtist).toString() +
      "'s \"" + mediaPlayer->metaData(SamMetaType::Title).toString() + "\" from " +
      mediaPlayer->metaData(SamMetaType::AlbumTitle).toString() + ".";
}

Radio::~Radio()
{

}

