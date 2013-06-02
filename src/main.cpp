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

	po::options_description visible("Allowable options");
    visible.add(generic).add(input);

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
