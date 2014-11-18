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

#include "boolsetting.h"

BoolSetting::BoolSetting(QString note, bool checked, QWidget *parent) :
    QFrame(parent), checked(checked), checkedDefault(checked)
{  
    //Setup widgets
    boolBox = new QToolButton(this);
    boolBox->setCheckable(true);
    titleText = new QLabel(note, this);
    boolReset = new QToolButton(this);
    boolReset->setArrowType(Qt::LeftArrow);
    setChecked(checked);
    setEnabled(true);

    //Connect widgets
    connect(boolBox, SIGNAL(toggled(bool)), SLOT(setChecked(bool)));
    connect(boolReset, SIGNAL(clicked()), SLOT(resetValue()));

    //Show widgets
    QHBoxLayout *holder = new QHBoxLayout(this);
    holder->addWidget(boolBox);
    holder->addWidget(titleText);
    holder->addWidget(boolReset);
    this->setLayout(holder);
}

bool BoolSetting::getChecked()
{
    return checked;
}

bool BoolSetting::getEnabled()
{
    return enabled;
}

void BoolSetting::setChecked(bool checked)
{
    if(checked)
        boolBox->setIcon(QIcon(":/Settings/CheckChecked"));
    else
        boolBox->setIcon(QIcon(":/Settings/CheckUnchecked"));
    boolBox->setChecked(checked);
    this->checked = checked;
    emit newValue(checked);
}

void BoolSetting::setEnabled(bool enabled)
{
    boolBox->setEnabled(enabled);
    boolReset->setEnabled(enabled);
    this->enabled = enabled;
}

void BoolSetting::resetValue()
{
    setChecked(checkedDefault);
}
