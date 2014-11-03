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

#ifndef TEXTSETTING_H
#define TEXTSETTING_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>

class TextSetting : public QFrame
{
    Q_OBJECT
    public:
        explicit TextSetting(QString note, QString value="", QWidget *parent=0);
        QString getValue();
    private:
        QString valueDefault;
        QLabel *titleText;
        QLineEdit *valueText;
        QToolButton *boolReset;
    signals:
        void newValue(QString value);
    public slots:
        void setValue(QString value);
        void resetValue();
};

#endif // TEXTSETTING_H
