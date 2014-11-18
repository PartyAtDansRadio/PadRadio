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
    setWindowTitle("Party At Dans Radio");
    setMaximumWidth(425);
    setMaximumHeight(300);
    setMinimumWidth(maximumWidth());
    setMinimumHeight(maximumHeight());
    QScreen *screen = QApplication::screens().at(0);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                    size(), screen->availableGeometry()));

    //Theme ui
    QFile theme(":/About/Theme");
    theme.open(QFile::ReadOnly | QFile::Text);
    QTextStream themeInput(&theme);
    setStyleSheet(themeInput.readAll());
    theme.close();

    //Setup widgets
    QTextEdit *textZone = new QTextEdit(this);
    textZone->setReadOnly(true);
    QPushButton *close = new QPushButton("Close Window", this);
    connect(close, SIGNAL(clicked()), SLOT(closeButton_clicked()));
    QVBoxLayout *holder = new QVBoxLayout(this);
    holder->addWidget(textZone);
    holder->addWidget(close);
    setLayout(holder);

    //Set textedit html file
    QFile html(":/About/Note");
    html.open(QFile::ReadOnly | QFile::Text);
    QTextStream htmlInput(&html);
    textZone->setHtml(htmlInput.readAll());
    html.close();
}

void About::closeButton_clicked()
{
   this->destroy();
}
