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

#include <QApplication>
#include "main_window.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	po::options_description generic("Generic options");
	generic.add_options()
		("help,h","Help")
		("config,c",po::value<std::string>()->default_value("bfviewer.cfg"),"name of cfg file");

	po::options_description input("Input");
	input.add_options()
		("input.file",po::value<std::string>(), "Input file");

    po::options_description bfviewer("Viewer options");
    bfviewer.add_options()
            ("page.step",po::value<int>()->default_value(200), "Scrollbar page (big) step")
            ("single.step",po::value<int>()->default_value(40), "Scrollbar single (small) step")
            ("plot.num.samples",po::value<int>()->default_value(10000), "Number of samples of each row to display")
            ("inter.row.offset",po::value<double>()->default_value(-1.5), "Plot offset between rows");

    po::options_description bfbufferedfile("Bigfoot bufferedfile options");
    bfbufferedfile.add_options()
            ("file.nrows",po::value<int>()->default_value(12), "Number of rows")
            ("file.ncols",po::value<int>()->default_value(25*3600*1000), "Number of columns")
            ("file.num.maped.elements",po::value<int>()->default_value(12*3600*1000), "Number of samples to map in memory")
            ("file.offset",po::value<int>()->default_value(0), "offset");

    po::options_description gridplot("Grid properties");
    gridplot.add_options()
            ("time.offset",po::value<double>()->default_value(0.0), "Initial timeoffset")
            ("zoom.enabled",po::value<bool>()->default_value(true), "Enable zooming capabilities")
            ("pan.enabled",po::value<bool>()->default_value(true), "Enable panning capabilities")
            ("horizontal.axis.enabled",po::value<bool>()->default_value(true), "Enable horizontal axis")
            ("horizontal.maxfs",po::value<double>()->default_value(1000.0), "Horizontal maximum sampling frequecy")
            ("horizontal.axis.units",po::value<std::string>()->default_value("seconds"), "Horizontal axis units label")
            ("Xmin",po::value<double>()->default_value(0.0), "X min")
            ("Xmax",po::value<double>()->default_value(10.0), "X max")
            ("Xstep.minor",po::value<double>()->default_value(0.04), "Horizontal grid minor step")
            ("Xstep.major",po::value<double>()->default_value(0.20), "Horizontal grid major step")
            ("vertical.axis.enabled",po::value<bool>()->default_value(true), "Enable vertical axis")
            ("vertical.maxfs",po::value<double>()->default_value(1000.0), "Vertical maximum sampling frequecy")
            ("vertical.axis.units",po::value<std::string>()->default_value("mV"), "Vertical axis units label")
            ("Ymin",po::value<double>()->default_value(-18.0), "Y min")
            ("Ymax",po::value<double>()->default_value(2.0), "Y max")
            ("Ystep.minor",po::value<double>()->default_value(0.1), "Vertical grid minor step")
            ("Ystep.major",po::value<double>()->default_value(0.5), "Vertical grid major step");

	po::options_description visible("Allowable options");
    visible.add(generic).add(input).add(bfviewer).add(bfbufferedfile).add(gridplot);

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv,visible), vm);
	po::notify(vm);

	if(vm.count("help")) {
        std::cout << "Usage: bfviewer [options]" << std::endl;
		std::cout << visible << std::endl;

		return EXIT_SUCCESS;
	}

	if(vm.count("config")) {
		std::string cfg_file = vm["config"].as<std::string>();
		std::ifstream cfg_fid(cfg_file.c_str());

		if(cfg_fid) {
			po::store(parse_config_file(cfg_fid, visible, true), vm);
			notify(vm);
		}
	}

	MainWindow mainWindow(0,vm,visible);

	mainWindow.show();

	return app.exec();
}
