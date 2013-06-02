/**
 * Copyright (c) 2013, Jose Vicente
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef GRIDPLOT_H
#define GRIDPLOT_H

#include <QFrame>
#include <qwt/qwt_plot.h>
#include <qpen.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_plot_magnifier.h>

namespace Ui {
class gridplot;
}

class gridplot : public QFrame
{
    Q_OBJECT
    
public:
    //public methods
    explicit gridplot(QWidget *parent = 0);
    ~gridplot();

    void setoffset(int milliseconds);

    //Zomming tools
    void resetzoom();
    void setZoomEnabled(bool v);
    bool isZoomEnabled();
    void setPanEnabled(bool v);
    bool isPanEnabled();

    //public properties
    QwtPlot *p;

private Q_SLOTS:
    void resizeEvent(QResizeEvent * re);

private:
    //private methods
    void resizeContents();
    void generateGrid();
    void initializegrid();
    void plotgrid();

    //private properties
    Ui::gridplot *ui;

    //grid porperties

    //Top Left corner of the grid in the QwtPlot space (pixels)
    QPointF gridlocation;
    //Pens for major and minor lines of the grid
    QPen gridmajorpen;
    QPen gridminorpen;

    //Horizontal axis
    QString gridXunits;
    //Limits (in real units) of the horizontal axis of the grid
    double gridXmin;
    double gridXmax;
    //Major and minor steps (in real units) of the horizontal axis of the grid
    double gridXmajorstep;
    double gridXminorstep;
    //Scale factor in pixels per unit of the horizontal axis
    double gridXpixelsperunit;

    //Vertical axis
    QString gridYunits;
    //Limits (in real units) of the horizontal axis of the grid
    double gridYmin;
    double gridYmax;
    //Major and minor steps (in real units) of the horizontal axis of the grid
    double gridYmajorstep;
    double gridYminorstep;
    //Scale factor in pixels per unit of the horizontal axis
    double gridYpixelsperunit;

    double _timeoffset;

    //Zomming tools
    QwtPlotZoomer *zoomer;
    QwtPlotPanner *panner;
    QwtPlotMagnifier *magnifier;


};

#endif // GRIDPLOT_H
