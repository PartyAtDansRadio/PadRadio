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

#include "about.h"

About::About(QWidget *parent) : QFrame(parent)
{
    //Set window type and location
    setWindowTitle("Pad Radio - About");

    //Theme ui
    QFile theme(":/About/Theme");
    theme.open(QFile::ReadOnly | QFile::Text);
    QTextStream themeInput(&theme);
    setStyleSheet(themeInput.readAll());
    theme.close();

    //Setup widgets
    QLabel *title = new QLabel("About Party At Dans", this);
    QTextEdit *textZone = new QTextEdit(this);
    textZone->setReadOnly(true);

    //Set textedit html file
    QFile html(":/About/Note");
    html.open(QFile::ReadOnly | QFile::Text);
    QTextStream htmlInput(&html);
    textZone->setHtml(htmlInput.readAll());
    html.close();

    //Setup layout
    QVBoxLayout *holder = new QVBoxLayout(this);
    setLayout(holder);
    holder->addWidget(title);
    holder->addWidget(textZone);
}
