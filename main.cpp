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

#include <QApplication>
#include <QSettings>
#include "Window/window.h"
#include "Settings/settings.h"

int main(int argc, char *argv[])
{
    //Create app
    QApplication app(argc, argv);
    app.setApplicationName("PadRadio");
    app.setApplicationVersion("0.2");
    app.setOrganizationName("Party at Dan's Radio");
    app.setOrganizationDomain("http://padradio.com");

    //Run app
    QSettings *settings = new QSettings("Settings.ini", QSettings::IniFormat);
    Settings::loadDefaults(settings);
    Window window;
    if(!(settings->value("showTaskbarIcon").toBool() && settings->value("startInTaskbar").toBool()))
        window.show();
    return app.exec();
}
