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
#include "ui_about.h"

About::About(QWidget *parent) : QWidget(parent), ui(new Ui::About)
{
    //Setup UI
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);
    if(width() > qApp->desktop()->availableGeometry().width())
        resize(qApp->desktop()->availableGeometry().width(), height());
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                    size(), qApp->desktop()->availableGeometry()));
    ui->title->setText("About: " + QApplication::organizationName() +
                       " " + QGuiApplication::applicationVersion());

    //Theme ui
    QFile file(":/AboutTheme");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream input(&file);
    setStyleSheet(input.readAll());
}

void About::on_closeButton_clicked()
{
   this->destroy();
}

About::~About()
{
    delete ui;
}
