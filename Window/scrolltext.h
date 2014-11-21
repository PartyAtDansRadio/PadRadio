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

NOTE:
    This class scrolltext is mostly the work of Sebastian Lehmann and was found at
    https://stackoverflow.com/questions/10651514/text-scrolling-marquee-in-qlabel
    from 2012 under the Stackoverflow username Leemes.
*/

#ifndef SCROLLTEXT_H
#define SCROLLTEXT_H

#include <QWidget>
#include <QStaticText>
#include <QPainter>
#include <QTimer>

class ScrollText : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollText(QWidget *parent = 0);

public slots:
    QString text() const;
    void setText(QString text);
    QString separator() const;
    void setSeparator(QString separator);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);

private:
    void updateText();
    QString _text;
    QString _separator;
    QStaticText staticText;
    int singleTextWidth;
    QSize wholeTextSize;
    int leftMargin;
    bool scrollEnabled;
    int scrollPos;
    QImage alphaChannel;
    QImage buffer;
    QTimer timer;

private slots:
    virtual void timer_timeout();
};

#endif // SCROLLTEXT_H
