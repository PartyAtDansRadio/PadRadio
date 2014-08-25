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
#include "ui_settings.h"

#define MEDIASTREAM "http://sc3.spacialnet.com:31560"
#define METADATA "http://www.mcttelecom.com/~d_libby/metaData.txt"

Settings::Settings(QWidget *parent) :
    QWidget(parent), ui(new Ui::Settings)
{
    //Setup UI
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                    size(), qApp->desktop()->availableGeometry()));
    #if !defined(Q_OS_WIN) && !defined(Q_OS_OSX) && !defined(Q_OS_LINUX)
        ui->tabWidget->removeTab(0); //Window tab
    #endif

    //Load Settings
    settings = new QSettings("Settings.ini", QSettings::IniFormat, this);
    ui->editStream->setText(settings->value("MediaStream").toString());
    ui->editMetaData->setText(settings->value("MetaData").toString());
    if(!settings->value("showTaskbarIcon").toBool()) {
        settings->setValue("showMessages", false);
        settings->setValue("startInTaskbar", false);
        ui->checkShowSong->setEnabled(false);
        ui->checkStartInTaskbar->setEnabled(false);
    }
    else {
        ui->checkEnableTaskbar->toggle();
        if(settings->value("showMessages").toBool())
            ui->checkShowSong->toggle();
        if(settings->value("startInTaskbar").toBool())
            ui->checkStartInTaskbar->toggle();
    }
    if(settings->value("smallPlayer").toBool())
        ui->checkSmallPlayer->toggle();
    if(settings->value("rememberLocation").toBool())
        ui->checkRememberLocation->toggle();
}

void Settings::loadDefaults(QSettings *settings) {
    //Set default setting values
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
    if(!settings->contains("smallPlayer"))
        settings->setValue("smallPlayer", false);
    if(!settings->contains("rememberLocation"))
        settings->setValue("rememberLocation", true);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_closeButton_clicked()
{
    this->destroy();
}

void Settings::on_editStream_textChanged(const QString &arg1)
{
    settings->setValue("MediaStream", arg1);
}

void Settings::on_editMetaData_textChanged(const QString &arg1)
{
    settings->setValue("MetaData", arg1);
}

void Settings::on_resetStream_clicked()
{
    settings->setValue("MediaStream", MEDIASTREAM);
    ui->editStream->setText(MEDIASTREAM);
}

void Settings::on_resetMetaData_clicked()
{
    settings->setValue("MetaData", METADATA);
    ui->editMetaData->setText(METADATA);
}

void Settings::on_checkEnableTaskbar_toggled(bool checked)
{
    settings->setValue("showTaskbarIcon", checked);
    if(checked) {
        //Enable sub menu
        ui->checkShowSong->setEnabled(true);
        ui->checkStartInTaskbar->setEnabled(true);
        ui->resetShowSong->setEnabled(true);
        ui->resetStartInTaskbar->setEnabled(true);
    }
    else {
        //Disable sub menu
        settings->setValue("showMessages", false);
        settings->setValue("startInTaskbar", false);
        ui->checkShowSong->setEnabled(false);
        ui->checkStartInTaskbar->setEnabled(false);
        ui->resetShowSong->setEnabled(false);
        ui->resetStartInTaskbar->setEnabled(false);
        ui->checkShowSong->setChecked(false);
        ui->checkStartInTaskbar->setChecked(false);
    }
}

void Settings::on_checkShowSong_toggled(bool checked)
{
    settings->setValue("showMessages", checked);
}

void Settings::on_checkStartInTaskbar_toggled(bool checked)
{
    settings->setValue("startInTaskbar", checked);
}

void Settings::on_resetEnableTaskbar_clicked()
{
    settings->setValue("showTaskbarIcon", true);
    ui->checkEnableTaskbar->setChecked(true);
}

void Settings::on_resetShowSong_clicked()
{
    settings->setValue("showMessages", true);
    ui->checkShowSong->setChecked(true);
}

void Settings::on_resetStartInTaskbar_clicked()
{
    settings->setValue("startInTaskbar", false);
    ui->checkStartInTaskbar->setChecked(false);
}

void Settings::on_checkRememberLocation_toggled(bool checked)
{
    settings->setValue("rememberLocation", checked);
}

void Settings::on_resetRememberLocation_clicked()
{
    settings->remove("rememberLocation");
    ui->checkRememberLocation->toggled(true);
    ui->checkRememberLocation->setChecked(true);
}

void Settings::on_resetAllButton_clicked()
{
    settings->clear();
    Settings::loadDefaults(settings);
    ui->editStream->setText(MEDIASTREAM);
    ui->editMetaData->setText(METADATA);
    ui->checkEnableTaskbar->setChecked(true);
    ui->checkShowSong->setChecked(true);
    ui->checkStartInTaskbar->setChecked(false);
    ui->checkSmallPlayer->setChecked(false);
    ui->checkRememberLocation->setChecked(true);
}

void Settings::on_checkSmallPlayer_toggled(bool checked)
{
    settings->setValue("smallPlayer", checked);
}

void Settings::on_resetSmallplayer_clicked()
{
    settings->setValue("smallPlayer", false);
    ui->checkSmallPlayer->setChecked(false);
}
