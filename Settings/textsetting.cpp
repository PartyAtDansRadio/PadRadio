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

#include "textsetting.h"
#include <QDebug>

TextSetting::TextSetting(QString note, QString value, QWidget *parent) :
     QFrame(parent), valueDefault(value)
{   
    //Setup widgets
    titleText = new QLabel(note, this);
    valueText = new QLineEdit(value, this);
    boolReset = new QToolButton(this);
    boolReset->setCheckable(true);
    boolReset->setArrowType(Qt::LeftArrow);
    setValue(value);

    //Show widgets
    QHBoxLayout *holder = new QHBoxLayout(this);
    holder->addWidget(titleText);
    holder->addWidget(valueText);
    holder->addWidget(boolReset);
    this->setLayout(holder);

    //Connect widgets
    connect(boolReset, SIGNAL(clicked()), SLOT(resetValue()));
    connect(valueText, SIGNAL(textChanged(QString)), SLOT(setValue(QString)));
    connect(valueText, SIGNAL(textEdited(QString)), SLOT(setValue(QString)));
}

QString TextSetting::getValue()
{
    return valueText->text();
}

void TextSetting::setValue(QString value)
{
    if(value == valueDefault)
        boolReset->setChecked(false);
    else
        boolReset->setChecked(true);
    valueText->setText(value);
    emit newValue(value);
}

void TextSetting::resetValue()
{
    setValue(valueDefault);
}
