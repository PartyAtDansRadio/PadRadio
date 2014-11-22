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

#include "scrolltext.h"

ScrollText::ScrollText(QWidget *parent) : QWidget(parent), scrollPos(0)
{
    staticText.setTextFormat(Qt::PlainText);
    setFixedHeight(height());
    leftMargin = height() / 3;
    updateText();
    update();
    connect(&timer, SIGNAL(timeout()), SLOT(timer_timeout()));
    timer.setInterval(25);
}

void ScrollText::setText(QString text)
{
    textDisplay = text;
    updateText();
    update();
}

void ScrollText::updateText()
{
    timer.stop();
    singleTextWidth = fontMetrics().width(textDisplay);
    scrollEnabled = (singleTextWidth > width() - leftMargin);
    if(scrollEnabled)
    {
        scrollPos = -64;
        staticText.setText(textDisplay + "     ");
        timer.start();
    }
    else
    {
        staticText.setText(textDisplay);
    }
    staticText.prepare(QTransform(), font());
    wholeTextSize = QSize(fontMetrics().width(staticText.text()), fontMetrics().height());
}

void ScrollText::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    if(scrollEnabled)
    {
        buffer.fill(qRgba(0, 0, 0, 0));
        QPainter bufferPainter(&buffer);
        bufferPainter.setPen(painter.pen());
        bufferPainter.setFont(painter.font());
        int x = qMin(-scrollPos, 0) + leftMargin;
        while(x < width())
        {
            bufferPainter.drawStaticText(QPointF(x, (height() - wholeTextSize.height())), staticText);
            x += wholeTextSize.width();
        }

        //Apply Alpha Channel
        bufferPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        bufferPainter.drawImage(0, 0, alphaChannel);
        bufferPainter.setClipRect(0, 0, 15, height());
        if(scrollPos < 0)
            bufferPainter.setOpacity((qreal)(qMax(-8, scrollPos) + 8) / 8.0);
        bufferPainter.drawImage(0, 0, alphaChannel);
        painter.drawImage(0, 0, buffer);
    }
    else
    {
        painter.drawStaticText(QPointF(leftMargin, (height() - wholeTextSize.height())), staticText);
    }
}

void ScrollText::resizeEvent(QResizeEvent*)
{
    //When the widget is resized, we need to update the alpha channel.
    alphaChannel = QImage(size(), QImage::Format_ARGB32_Premultiplied);
    buffer = QImage(size(), QImage::Format_ARGB32_Premultiplied);

    //Create Alpha Channel:
    if(width() > 64)
    {
        //create first scanline
        QRgb* scanline1 = (QRgb*)alphaChannel.scanLine(0);
        for(int x = 1; x < 16; ++x)
            scanline1[x - 1] = scanline1[width() - x] = qRgba(0, 0, 0, x << 4);
        for(int x = 15; x < width() - 15; ++x)
            scanline1[x] = qRgb(0, 0, 0);

        //copy scanline to the other ones
        for(int y = 1; y < height(); ++y)
            memcpy(alphaChannel.scanLine(y), (uchar*)scanline1, width() * 4);
    }
    else
    {
        alphaChannel.fill(qRgb(0, 0, 0));
    }
    //Update scrolling state
    bool newScrollEnabled = (singleTextWidth > width() - leftMargin);
    if(newScrollEnabled != scrollEnabled)
        updateText();
}

void ScrollText::timer_timeout()
{
    scrollPos = (scrollPos + 1) % wholeTextSize.width();
    update();
}
