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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QStyle>
#include <QDesktopWidget>
#include <QSettings>

namespace Ui {
    class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    static void loadDefaults(QSettings *settings);
    ~Settings();

private slots:
    void on_closeButton_clicked();
    void on_editStream_textChanged(const QString &arg1);
    void on_editMetaData_textChanged(const QString &arg1);
    void on_resetStream_clicked();
    void on_resetMetaData_clicked();
    void on_checkEnableTaskbar_toggled(bool checked);
    void on_checkShowSong_toggled(bool checked);
    void on_checkStartInTaskbar_toggled(bool checked);
    void on_resetEnableTaskbar_clicked();
    void on_resetShowSong_clicked();
    void on_resetStartInTaskbar_clicked();
    void on_checkRememberLocation_toggled(bool checked);
    void on_resetRememberLocation_clicked();
    void on_resetAllButton_clicked();

    void on_checkSmallPlayer_toggled(bool checked);

    void on_resetSmallplayer_clicked();

private:
    Ui::Settings *ui;
    QSettings *settings;
};

#endif // SETTINGS_H
