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

#ifndef BOOLSETTING_H
#define BOOLSETTING_H

#include <QWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>

class BoolSetting : public QFrame
{
    Q_OBJECT
    public:
        explicit BoolSetting(QString note, bool enabled, QWidget *parent=0);
        bool getChecked();
        bool getEnabled();
    private:
        bool checked;
        bool enabled;
        bool checkedDefault;
        QToolButton *boolBox;
        QLabel *titleText;
        QToolButton *boolReset;
    signals:
        void newValue(bool value);
    public slots:
        void setChecked(bool checked);
        void setEnabled(bool enabled);
        void resetValue();
};

#endif // BOOLSETTING_H
