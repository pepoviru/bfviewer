#include "gridplot.h"
#include "ui_gridplot.h"

#include <qwt/qwt_plot.h>

#include <qlayout.h>
#include <qpen.h>

#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_magnifier.h>
#include <qwt/qwt_plot_panner.h>

gridplot::gridplot(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::gridplot)
{
    ui->setupUi(this);

    p = new QwtPlot(this);

    //initialize grid
    _timeooffset=0.0;
    initializegrid();
    //plotgrid();
    //initialize zooming tools

    zoomer = new QwtPlotZoomer( p->xBottom,p->yLeft,p->canvas() );
    zoomer->setZoomBase(true);
    setZoomEnabled(true);
    panner = new QwtPlotPanner( p->canvas() );
    panner->setMouseButton(Qt::RightButton,Qt::NoButton);
    setPanEnabled(true);
    magnifier = new QwtPlotMagnifier( p->canvas() );
    magnifier->setMouseButton(Qt::MiddleButton,Qt::NoButton);

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
    int margin = 10;
    int containerWidth = this->parentWidget()->width() - margin;
    int containerHeight = this->parentWidget()->height() - margin;
    //TODO: (Enhancement) Establish maximumSize and minimumSize for grid squares and update xEndTimeInSeconds and/or yMinValueInMicrovolts and replot as needed. Then go ahead with aspect-ratio
    //double aspectRatio = ((xEndTimeInSeconds - xStartTimeInSeconds)/gridXMajorStepInSeconds)/((yMaxValueInMicrovolts - yMinValueInMicrovolts)/gridYMajorStepInMicrovolts);//1.0;
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
void gridplot::initializegrid()
{
//    //Top Left corner of the grid in the QwtPlot space (pixels)
//    gridlocation.setX(0);
//    gridlocation.setY(0);
//    //Pens for major and minor lines of the grid
//    QBrush orangeBrush = QBrush(QColor::fromRgb(255,127,0));
//    gridmajorpen = QPen(orangeBrush, 0, Qt::SolidLine);
//    gridminorpen = QPen(orangeBrush, 0, Qt::DotLine);

//    //Horizontal axis
//    gridXunits = QString("seconds");
//    //Limits (in real units) of the horizontal axis of the grid
//    gridXmin = 0.0; // t0 = 0.0
//    gridXmax = 2.48; // tend = 2.48 seconds
//    //Major and minor steps (in real units) of the horizontal axis of the grid
//    gridXmajorstep = 0.20; // 200 ms;
//    gridXminorstep = 0.04; // 40 ms;
//    //Scale factor in pixels per unit of the horizontal axis
//    gridXpixelsperunit = 1000.0; // 1 pixel = 1ms

//    //Vertical axis
//    gridYunits = QString("mV");
//    //Limits (in real units) of the horizontal axis of the grid
//    gridYmin = -1.0; //-1.0 mV
//    gridYmax = 2.0; //1.5 mV
//    //Major and minor steps (in real units) of the horizontal axis of the grid
//    gridYmajorstep = 0.5; // 0.5 mV
//    gridYminorstep = 0.1; // 0.1 mV
//    //Scale factor in pixels per unit of the horizontal axis
//    gridYpixelsperunit = 1000.0; // 1 pixel = 1uV

    setoffset(_timeooffset);
}

void gridplot::plotgrid()
{
    std::vector<double> xlocs;
    std::vector<double> ylocs;
    xlocs.resize(2);
    ylocs.resize(2);
    QwtPlotCurve *anyline;

    //horizontal major lines
    int numberoflines = (gridYmax - gridYmin)/gridYmajorstep;
    xlocs[0]=gridXmin + _timeooffset;
    xlocs[1]=gridXmax + _timeooffset;
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
    numberoflines = (gridXmax - gridXmin)/gridXmajorstep;
    ylocs[0]=gridYmin;
    ylocs[1]=gridYmax;
    for (int i=0;i<=numberoflines;++i)
    {
        anyline = new QwtPlotCurve();
        xlocs[0] = xlocs[1] = gridXmin + gridXmajorstep*(i) + _timeooffset;
        anyline->setSamples(&xlocs[0],&ylocs[0],xlocs.size());
        anyline->setPen(gridmajorpen);
        anyline->setStyle(QwtPlotCurve::Lines);
        anyline->attach(p);
    }
    //vertical minor lines
    numberoflines = (gridXmax - gridXmin)/gridXminorstep;
    for (int i=0;i<=numberoflines;++i)
    {
        anyline = new QwtPlotCurve();
        xlocs[0] = xlocs[1] = gridXmin + gridXminorstep*(i) + _timeooffset;
        anyline->setSamples(&xlocs[0],&ylocs[0],xlocs.size());
        anyline->setPen(gridminorpen);
        anyline->setStyle(QwtPlotCurve::Lines);
        anyline->attach(p);
    }
}

//zoom tools methods
void gridplot::resetzoom()
{
    QRectF zoombase = QRectF(gridXmin + _timeooffset,gridYmin,gridXmax - gridXmin,gridYmax-gridYmin);
    bool ze = zoomer->isEnabled();
    bool pe = panner->isEnabled();
    zoomer->setEnabled(true);
    panner->setEnabled(false);
    zoomer->zoom(zoombase);
    //int numzooms = zoomer->
    p->replot();
    resizeContents();
    zoomer->setEnabled(ze);
    panner->setEnabled(pe);
//    //FIXME: Save previous zoom
//    generateGrid();
//    if (p!=NULL)
//        p->replot();
//    generateGrid();
    resizeContents();
//    //FIXME: Restore previous zoom
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
    _timeooffset = milliseconds/1000.0;
    //Top Left corner of the grid in the QwtPlot space (pixels)
    gridlocation.setX(0);
    gridlocation.setY(0);
    //Pens for major and minor lines of the grid
    QBrush orangeBrush = QBrush(QColor::fromRgb(255,127,0));
    gridmajorpen = QPen(orangeBrush, 0, Qt::SolidLine);
    gridminorpen = QPen(orangeBrush, 0, Qt::DotLine);

    //Horizontal axis
    gridXunits = QString("seconds");
    //Limits (in real units) of the horizontal axis of the grid
    gridXmin = 0.0; // t0 = 0.0
    gridXmax = 2.48; // tend = 2.48 seconds
    //Major and minor steps (in real units) of the horizontal axis of the grid
    gridXmajorstep = 0.20; // 200 ms;
    gridXminorstep = 0.04; // 40 ms;
    //Scale factor in pixels per unit of the horizontal axis
    gridXpixelsperunit = 1000.0; // 1 pixel = 1ms

    //Vertical axis
    gridYunits = QString("mV");
    //Limits (in real units) of the horizontal axis of the grid
    gridYmin = -1.0; //-1.0 mV
    gridYmax = 2.0; //1.5 mV
    //Major and minor steps (in real units) of the horizontal axis of the grid
    gridYmajorstep = 0.5; // 0.5 mV
    gridYminorstep = 0.1; // 0.1 mV
    //Scale factor in pixels per unit of the horizontal axis
    gridYpixelsperunit = 1000.0; // 1 pixel = 1uV
    plotgrid();
}
