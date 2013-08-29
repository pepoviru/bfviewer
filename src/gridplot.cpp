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

#include "gridplot.h"
#include "ui_gridplot.h"

#include <qwt_plot.h>

#include <qlayout.h>
#include <qpen.h>

#include <qwt_plot_zoomer.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>

gridplot::gridplot(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::gridplot),
    gridXunits("second"),
    gridXmin(0.0),
    gridXmax(10.0),
    gridXmajorstep(0.20),
    gridXminorstep(0.04),
    gridXpixelsperunit(1000.0),
    gridXaxisenabled(true),
    gridYunits("mV"),
    gridYmin(-18.0),
    gridYmax(2.0),
    gridYmajorstep(0.5),
    gridYminorstep(0.1),
    gridYpixelsperunit(1000.0),
    gridYaxisenabled(true)
{
    ui->setupUi(this);

    p = new QwtPlot(this);

    //initialize grid
    timeoffset=0.0;
    setoffset(timeoffset);

    //initialize zooming tools

    zoomer = new QwtPlotZoomer( p->xBottom,p->yLeft,p->canvas() );
    zoomer->setZoomBase(true);
    setZoomEnabled(true);
    panner = new QwtPlotPanner( p->canvas() );
    panner->setMouseButton(Qt::RightButton);
    setPanEnabled(true);
    magnifier = new QwtPlotMagnifier( p->canvas() );
    magnifier->setMouseButton(Qt::MiddleButton);

    //Pens for major and minor lines of the grid
    QBrush orangeBrush = QBrush(QColor::fromRgb(255,127,0));
    gridmajorpen = QPen(orangeBrush, 0, Qt::SolidLine);
    gridminorpen = QPen(orangeBrush, 0, Qt::DotLine);

    p->setCanvasBackground(QColor(Qt::white));
    p->enableAxis(QwtPlot::yLeft,gridXaxisenabled);
    p->enableAxis(QwtPlot::xBottom,gridYaxisenabled);
    ui->vlPlot->addWidget(p);
}

gridplot::~gridplot()
{
    delete ui;
}
//resize methods

void gridplot::resizeEvent(QResizeEvent * re)
{
    resizeContents();
}

void gridplot::resizeContents()
{
    int margin = 50;
    int containerWidth = this->parentWidget()->width() - margin;
    int containerHeight = this->parentWidget()->height() - margin;
    double aspectRatio = ((gridXmax - gridXmin)/gridXmajorstep)/((gridYmax - gridYmin)/gridYmajorstep);//1.0;
    int contentsHeight = containerHeight ;
    int contentsWidth = containerHeight * aspectRatio;
    if (contentsWidth > containerWidth ) {
        contentsWidth = containerWidth ;
        contentsHeight = containerWidth / aspectRatio;
    }
    this->resize(contentsWidth,contentsHeight);
}

//grid methods

void gridplot::plotgrid()
{
    std::vector<double> xlocs;
    std::vector<double> ylocs;
    xlocs.resize(2);
    ylocs.resize(2);
    QwtPlotCurve *anyline;

    //horizontal major lines
    int numberoflines = (gridYmax - gridYmin)/gridYmajorstep;
    xlocs[0]=gridXmin + timeoffset;
    xlocs[1]=gridXmax + timeoffset;
    for (int i=0;i<=numberoflines;++i)
    {
        anyline = new QwtPlotCurve();
        ylocs[0] = ylocs[1] = gridYmin + gridYmajorstep*(i);
        anyline->setSamples(&xlocs[0],&ylocs[0],xlocs.size());
        anyline->setPen(gridmajorpen);
        anyline->setStyle(QwtPlotCurve::Lines);
        anyline->attach(p);
    }
    //horizontal minor lines
    numberoflines = (gridYmax - gridYmin)/gridYminorstep;
    for (int i=0;i<=numberoflines;++i)
    {
        anyline = new QwtPlotCurve();
        ylocs[0] = ylocs[1] = gridYmin + gridYminorstep*(i);
        anyline->setSamples(&xlocs[0],&ylocs[0],xlocs.size());
        anyline->setPen(gridminorpen);
        anyline->setStyle(QwtPlotCurve::Lines);
        anyline->attach(p);
    }

    //vertical major lines
    numberoflines = (gridXmax - gridXmin+ timeoffset)/gridXmajorstep;
    ylocs[0]=gridYmin;
    ylocs[1]=gridYmax;
    for (int i=0;i<=numberoflines;++i)
    {
        xlocs[0] = xlocs[1] = gridXmin + gridXmajorstep*(i);
        if (xlocs[0]>=timeoffset){
            anyline = new QwtPlotCurve();
            anyline->setSamples(&xlocs[0],&ylocs[0],xlocs.size());
            anyline->setPen(gridmajorpen);
            anyline->setStyle(QwtPlotCurve::Lines);
            anyline->attach(p);
        }
    }
    //vertical minor lines
    numberoflines = (gridXmax - gridXmin + timeoffset)/gridXminorstep;
    for (int i=0;i<=numberoflines;++i)
    {
        xlocs[0] = xlocs[1] = gridXmin + gridXminorstep*(i);
        if (xlocs[0]>=timeoffset){
            anyline = new QwtPlotCurve();
            anyline->setSamples(&xlocs[0],&ylocs[0],xlocs.size());
            anyline->setPen(gridminorpen);
            anyline->setStyle(QwtPlotCurve::Lines);
            anyline->attach(p);
        }
    }

}

//zoom tools methods
void gridplot::resetzoom()
{
    QRectF zoombase = QRectF(gridXmin + timeoffset,gridYmin,gridXmax - gridXmin,gridYmax-gridYmin);
    bool ze = zoomer->isEnabled();
    bool pe = panner->isEnabled();
    zoomer->setEnabled(true);
    panner->setEnabled(true);
    zoomer->zoom(zoombase);
    p->replot();
    resizeContents();
    zoomer->setEnabled(ze);
    panner->setEnabled(pe);
    resizeContents();
}
void gridplot::setZoomEnabled(bool v)
{
    zoomer->setEnabled(v);
}

bool gridplot::isZoomEnabled()
{
    return zoomer->isEnabled();
}

void gridplot::setPanEnabled(bool v)
{
    panner->setEnabled(v);
}

bool gridplot::isPanEnabled()
{
    return panner->isEnabled();
}

void gridplot::setoffset(int milliseconds)
{
    timeoffset = milliseconds/gridXpixelsperunit;
    plotgrid();
}

void gridplot::enableAxis(int axisId, bool tf)
{
    if (axisId == QwtPlot::xBottom) {
        gridXaxisenabled = tf;
    } else if (axisId == QwtPlot::yLeft) {
        gridYaxisenabled = tf;
    }
    p->enableAxis(axisId,tf);
}
