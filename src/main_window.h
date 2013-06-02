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
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
//#include <QVariant>
//#include <QList>
//#include <QAbstractItemModel>
//#include <QModelIndex>
// #include <QSvgGenerator>
#include "ui_main_window.h"

//#include <bigfoot.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/fstream.hpp>
//namespace boostfs = boost::filesystem;

//#include <boost/shared_ptr.hpp>
//#include <boost/enable_shared_from_this.hpp>
//#include <boost/weak_ptr.hpp>
//#include <boost/any.hpp>

//#include <map>

//#include <string>

//#include <sstream>

//#include <stdexcept>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	// Constructors:
	public:
		MainWindow(QMainWindow* parent, const po::variables_map &vm, const po::options_description &visible);

	// Public methods:
    public:
        void loadall();
        void closeEvent ( QCloseEvent * event );
	// Helpers:
	protected:
        void loadFile(const std::string filename, const std::string fileformat);

	// Attributes:
	protected:
		po::variables_map _vm;

	// Slots
	private Q_SLOTS:
    		void about();
		void license();
	private:
		// Private attributes:
		Ui::MainWindow _ui;
		po::options_description _visible;

		QString appname;
		//Private methods
};

#endif
