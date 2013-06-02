/**
 * Copyright (c) 2013, Jose Vicente
 *
 * Cross-compiling compliance based on Paul Gideon Dann (see below)
 *
 * CMake files based on previous work by Lars Johannesen
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
 * Copyright (c) 2009, Paul Gideon Dann
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
 */

#include "main_window.h"
#include "moc_main_window.cpp"

#include <qwt/qwt_plot_marker.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

MainWindow::MainWindow(QMainWindow* parent, const po::variables_map &vm, const po::options_description &visible): QMainWindow(parent), _vm(vm), _visible(visible) {

    _ui.setupUi(this);

    qApp->processEvents();
    
    appname = QString("Bigfoot viewer");
    connect(_ui.actionE_xit, SIGNAL(triggered()), this, SLOT(close()));
    connect(_ui.action_Open_file, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(_ui.action_Export_to_SVG, SIGNAL(triggered()), this, SLOT(printsvg()));

    connect(_ui.action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect(_ui.action_License, SIGNAL(triggered()), this, SLOT(license()));

    _gridPlot = boost::shared_ptr<gridplot>(new gridplot(this));
    _ui.gridPlot->addWidget(_gridPlot.get());

    _sbTime = new QScrollBar(Qt::Horizontal,this);
    _sbTime->setPageStep(200); //page step 200ms
    _sbTime->setSingleStep(40); //page step 40ms
    connect(_sbTime, SIGNAL(valueChanged(int)), this, SLOT(setInitialTimeDisplay(int)));
    _ui.gridPlot->addWidget(_sbTime);

    if(_vm.count("input.file")) {
	    loadFile(_vm["input.file"].as<std::string>());
    }
}

void MainWindow::setInitialTimeDisplay(int start)
{
    std::size_t ncols = 25*3600*1000;
    //TODO: move plotting to appropiate method
//    std::size_t numofplottedsamples = 2*1000; //Plot 2 seconds
        std::size_t numofplottedsamples = 10*1000; //Plot 10 seconds
    _gridPlot->p->detachItems();
    _gridPlot->setoffset(start);

    double interrowoffset = -1.5;
    for (std::size_t row=0; row<12; ++row){
//    for (std::size_t row=0; row<1; ++row){
        std::vector<double> xs(numofplottedsamples);
        std::vector<double> ys(numofplottedsamples);

        std::stringstream strst;
        std::string strTitle = "Row";
        strst << strTitle << "-" << row;
        strTitle = strst.str();
        QwtPlotCurve *tscurve = new QwtPlotCurve((char *)strTitle.c_str());

        for (std::size_t x = 0; x < numofplottedsamples; ++x)
        {
            if (x+start<ncols){
                xs[x] = (x+start)/1000.0; //position in seconds
                ys[x] = row*interrowoffset + (*_bf)(0,x+start); //y value of that sample
            }
        }

        tscurve->setSamples(&xs[0],&ys[0],xs.size());
        tscurve->setPen(QPen(Qt::black));

        QwtPlotMarker *rowNameText = new QwtPlotMarker();
        rowNameText->setLabel(QString(strTitle.c_str()));
        rowNameText->setXValue(xs[0]+0.25);
        rowNameText->setYValue(row*interrowoffset-0.5);
        rowNameText->attach(_gridPlot->p);

        tscurve->attach(_gridPlot->p);
    }
    _gridPlot->resetzoom();
}

void MainWindow::loadFile(const std::string filename)
{
    boostfs::path ext = boostfs::path(filename).extension();
	std::size_t nrows = 12; //File of 12 rows (channels)
	std::size_t ncols = 25*3600*1000; //Containing 25 hours sampled at 1khz
	std::size_t nummappedels = 12*3600*1000;//map 1 hour
	std::size_t offset = 0; //only matrix, no header, no offset
	_bf = NULL;
	_bf = new bigfoot::bufferedfile(filename.c_str(), nrows, ncols, nummappedels, offset);

    //TODO: move scrollbar reset to appropiate method
    _sbTime->setRange(0, ncols);
    setInitialTimeDisplay(0);
}


void MainWindow::openFile()
{
    QString s = QFileDialog::getOpenFileName(
                this,
                "Open signal file",
                _lastDirectory,
                "All files (*.*);;");
    if (!s.isEmpty())
    {
        _lastDirectory = s;
        QFileInfo pathInfo(_lastDirectory);
        loadFile(pathInfo.filePath().toStdString());
    }
}

void MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(appname);
    QString message = QString("bfviewer %1 \nbigfoot %2 \n\n(c) Jose Vicente 2013. \n\nSee license details in Help->License").arg(BFVIEWER_VERSION).arg(BIGFOOT_VERSION);
    msgBox.setText(message);
    msgBox.exec();
}

void MainWindow::license()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(appname);
    msgBox.setText("Widget displaying LICENSE file \nincluded as resource in QT");
    msgBox.exec();
}
void MainWindow::closeEvent ( QCloseEvent * event )
{
    event->ignore();
    bool continueWithNext = false;
//    if (_arePendingChanges)
//    {
//        //ask user whether to save the changes or not and proceed
//        switch( QMessageBox::information( this, appname,
//                                          "There are not saved changes",
//                                          "Save changes", "Cancel", "Discard changes",
//                                          0, 1 ) ) {
//        case 0:
//            savechanges();
//            continueWithNext = !_arePendingChanges;
//            break;
//        case 1:
//        default:
//            continueWithNext = false;
//            //FIXME reset the tree's selected item to the previous one
//            break;
//        case 2:
//            continueWithNext = true;
//            _arePendingChanges = false;
//            break;
//        }
//    }
//    else
        continueWithNext = true;
    if (continueWithNext)
    {
        //Setting parent will keep messagebox in the center of QMainWindow
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setWindowTitle(appname);
        msgBox->setText("Are you sure you want to exit?");

        QPushButton *yesButton = msgBox->addButton(tr("Yes"), QMessageBox::ActionRole);
        msgBox->addButton(tr("No"), QMessageBox::ActionRole);
        msgBox->exec();

        //If user clicks 'Yes'  button , accept QCloseEvent (Close Window)
        if ((QPushButton*)msgBox->clickedButton() == yesButton)
        {
		_bf = NULL;
		event->accept();
	}
    }
}

void MainWindow::printsvg()
{

//    QString fileName(_record->name2display().append(".svg").c_str());
    QString fileName("bfviewer.svg");
    const QList<QByteArray> imageFormats =
        QImageWriter::supportedImageFormats();

    QStringList filter;
    filter += "SVG Documents (*.svg)";

    fileName = QFileDialog::getSaveFileName(
        this, "Export File Name", fileName,
        filter.join(";;"), NULL, QFileDialog::DontConfirmOverwrite);

    if (!fileName.isEmpty())
    {
        QSvgGenerator generator;
        QwtPlotRenderer plotrender;

        generator.setFileName( fileName );
        generator.setSize(_gridPlot->size());

        plotrender.renderTo(_gridPlot->p,generator);
    }
}
