/**
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


#include <boost/program_options.hpp>
namespace po = boost::program_options;

MainWindow::MainWindow(QMainWindow* parent, const po::variables_map &vm, const po::options_description &visible): QMainWindow(parent), _vm(vm), _visible(visible) {

    _ui.setupUi(this);

    qApp->processEvents();
    
    appname = QString("Bigfoot viewer");

    connect(_ui.action_Open_file, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(_ui.action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect(_ui.action_License, SIGNAL(triggered()), this, SLOT(license()));

}

void MainWindow::loadFile(const std::string filename, const std::string fileformat)
{
    boostfs::path ext = boostfs::path(filename).extension();
//TODO: implement load based on bufferedfile 
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
        loadFile(pathInfo.filePath().toStdString(), _vm["input.format"].as<std::string>());
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

