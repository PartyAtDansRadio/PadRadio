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

#include "settings.h"

#define MEDIASTREAM "http://sc3.spacialnet.com:31560"
#define METADATA "http://www.mcttelecom.com/~d_libby/metaData.txt"

Settings::Settings(QWidget *parent) : QFrame(parent)
{
    //Theme window
    QFile theme(":/Settings/Theme");
    theme.open(QFile::ReadOnly | QFile::Text);
    QTextStream themeInput(&theme);
    setStyleSheet(themeInput.readAll());
    theme.close();

    //Setup widgets
    QLabel *title = new QLabel("Party At Dans Settings", this);
    DivSetting *sep1 = new DivSetting("Window Settings", this);
    BoolSetting *rememberLocation = new BoolSetting("Remember window location", true, this);
    BoolSetting *smallPlayer = new BoolSetting("Enable small player mode", false, this);
    BoolSetting *alwaysTop = new BoolSetting("Window always on top", false, this);
    DivSetting *sep2 = new DivSetting("Taskbar Settings", this);
    BoolSetting *taskbarIcon = new BoolSetting("Enable taskbar icon", true, this);
    taskbarMessages = new BoolSetting("Show song info in taskbar", true, this);
    taskbarStart = new BoolSetting("Start PadRadio in taskbar", false, this);
    DivSetting *sep3 = new DivSetting("Server Settings", this);
    mediaStream = new TextSetting("Stream URL", MEDIASTREAM, this);
    metaData = new TextSetting("MetaData URL", METADATA, this);
    QPushButton *closeWindow = new QPushButton("Close Window", this);

    //Show scroll bar widgets
    QVBoxLayout *holder = new QVBoxLayout(this);
    holder->addWidget(sep1);
    holder->addWidget(rememberLocation);
    holder->addWidget(smallPlayer);
    holder->addWidget(alwaysTop);
    holder->addWidget(sep2);
    holder->addWidget(taskbarIcon);
    holder->addWidget(taskbarMessages);
    holder->addWidget(taskbarStart);
    holder->addWidget(sep3);
    holder->addWidget(mediaStream);
    holder->addWidget(metaData);

    //Add right side scroll bar
    QScrollArea *scroll=new QScrollArea(this);
    QFrame *scrollZone = new QFrame(this);
    scrollZone->setObjectName("scrollZone");
    scrollZone->setLayout(holder);
    scroll->setWidget(scrollZone);
    scroll->setWidgetResizable(true);
    scroll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    QVBoxLayout *noScroll = new QVBoxLayout(this);
    noScroll->addWidget(title);
    noScroll->addWidget(scroll);
    noScroll->addWidget(closeWindow);
    setLayout(noScroll);

    //Set window type and location
    setWindowTitle("Pad Radio - Settings");
    resize(minimumWidth(), height());
    setMaximumWidth(minimumWidth());
    QScreen *screen = QApplication::screens().at(0);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                    size(), screen->availableGeometry()));

    //Connect widgets events
    connect(rememberLocation, SIGNAL(newValue(bool)), SLOT(rememberLocation_newValue(bool)));
    connect(smallPlayer, SIGNAL(newValue(bool)), SLOT(smallPlayer_newValue(bool)));
    connect(alwaysTop, SIGNAL(newValue(bool)), SLOT(alwaysTop_newValue(bool)));
    connect(taskbarIcon, SIGNAL(newValue(bool)), SLOT(taskbarIcon_newValue(bool)));
    connect(taskbarMessages, SIGNAL(newValue(bool)), SLOT(taskbarMessages_newValue(bool)));
    connect(taskbarStart, SIGNAL(newValue(bool)), SLOT(taskbarStart_newValue(bool)));
    connect(mediaStream, SIGNAL(newValue(QString)), SLOT(mediaStream_newValue(QString)));
    connect(metaData, SIGNAL(newValue(QString)), SLOT(metaData_newValue(QString)));
    connect(closeWindow, SIGNAL(clicked()), SLOT(close_window()));

    //Load INI Settings
    settings = new QSettings("Settings.ini", QSettings::IniFormat, this);
    rememberLocation->setChecked(settings->value("rememberLocation").toBool());
    smallPlayer->setChecked(settings->value("smallPlayer").toBool());
    taskbarIcon->setChecked(settings->value("showTaskbarIcon").toBool());
    taskbarMessages->setChecked(settings->value("showMessages").toBool());
    taskbarStart->setChecked(settings->value("startInTaskbar").toBool());
    mediaStream->setValue(settings->value("MediaStream").toString());
    metaData->setValue(settings->value("MetaData").toString());
}

void Settings::rememberLocation_newValue(bool value)
{
    settings->setValue("rememberLocation", value);
}

void Settings::smallPlayer_newValue(bool value)
{
    settings->setValue("smallPlayer", value);
}

void Settings::alwaysTop_newValue(bool value)
{
    settings->setValue("alwaysTop", value);
}

void Settings::taskbarIcon_newValue(bool value)
{
    settings->setValue("showTaskbarIcon", value);
    if(!value) {
        taskbarMessages->setEnabled(false);
        taskbarStart->setEnabled(false);
    }
    else{
        taskbarMessages->setEnabled(true);
        taskbarStart->setEnabled(true);
    }
}

void Settings::taskbarMessages_newValue(bool value)
{
    settings->setValue("showMessages", value);
}

void Settings::taskbarStart_newValue(bool value)
{
    settings->setValue("startInTaskbar", value);
}

void Settings::mediaStream_newValue(QString value)
{
    settings->setValue("MediaStream", value);
}

void Settings::metaData_newValue(QString value)
{
    settings->setValue("MetaData", value);
}

void Settings::close_window()
{
    emit updateSettings();
    close();
}

void Settings::loadDefaults(QSettings *settings)
{
    //Set default setting values
    if(!settings->contains("smallPlayer"))
        settings->setValue("smallPlayer", false);
    if(!settings->contains("rememberLocation"))
        settings->setValue("rememberLocation", true);
    if(!settings->contains("alwaysTop"))
        settings->setValue("alwaysTop", false);
    if(!settings->contains("showTaskbarIcon"))
        settings->setValue("showTaskbarIcon", true);
    if(!settings->contains("showMessages"))
        settings->setValue("showMessages", true);
    if(!settings->contains("startInTaskbar"))
        settings->setValue("startInTaskbar", false);
    if(!settings->contains("MetaData"))
        settings->setValue("MetaData", METADATA);
    if(!settings->contains("MediaStream"))
        settings->setValue("MediaStream", MEDIASTREAM);
}
