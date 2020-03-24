//
// Created by ktulhu on 2018-12-13.
//

#include "ConfigParse.h"
#include <IOFunc.h>
#include <functional>


ConfigParse::ConfigParse(const std::string &path) noexcept
	:	path{path}	{
	config.reset();
}

void ConfigParse::parse() {
	// default parser
	std::function<void(kvpair)> current_parser = files;
	auto reader = [&](const std::string& line) {

		std::regex title("\\[[\\w]*\\][ \\t]*");

		if (!std::regex_match(line, title)) {
			std::regex form("[\\w]*=[\\w.\\/\\{, \\t\\}]*[ \\t]*");
			if (!std::regex_match(line, form)) {
				std::cout << "- '" << line << "'" << std::endl;
				return;
			} else {
				try {
					current_parser(split_key_value(line));
				} catch (const std::exception &exc) {
					std::cout 	<< "* ConfigParse::parse @ " << this << "caught exception\n"
								<< "  '" << exc.what() << "'; continuing..." << std::endl;
				}

			}
		} else {
			// find which parser to use
			if(std::regex_match(line, std::regex("\\[files\\][ \\t]*")))
				current_parser = files;
			else if (std::regex_match(line, std::regex("\\[options\\][ \\t]*")))
				current_parser = options;
			else if (std::regex_match(line, std::regex("\\[vectors\\][ \\t]*")))
				current_parser = vectors;
		}
	};

	iofunc::read(path, reader);

}

void ConfigParse::files(const kvpair& kv) {
	if (kv.first == "transforms") {
		config.transforms = "../configs/" + kv.second;

	} else if (kv.first == "in") {
		config.in = "../imgs/source_bmps/" + kv.second;

	} else if (kv.first == "out") {
		config.out = "../imgs/output/" + kv.second;

	} else if (kv.first == "pout") {
		config.pout = "../imgs/output/" + kv.second;

	} else if (kv.first == "savefile") {
		config.savefile = "../configs/" + kv.second;

	}  else {
		std::cout << "- key '" << kv.first << "' did not match a defined key" << std::endl;
	}
}

void ConfigParse::options(const kvpair& kv) {
	if (kv.first == "preview") {
		config.preview = (bool) std::stoi(kv.second);

	} else if (kv.first == "triangles") {
		config.triangles = (bool) std::stoi(kv.second);

	} else if (kv.first == "lines") {
		config.lines = (bool) std::stoi(kv.second);

	} else if (kv.first == "xmult") {
		config.shifts[0] = std::stod(kv.second);

	} else if (kv.first == "ymult") {
		config.shifts[1] = std::stod(kv.second);

	} else if (kv.first == "xmove") {
		config.shifts[2] = std::stod(kv.second);

	} else if (kv.first == "ymove") {
		config.shifts[3] = std::stod(kv.second);

	} else if (kv.first == "width") {
		config.width = std::stoi(kv.second);

	} else if (kv.first == "height") {
		config.height = std::stoi(kv.second);

	} else if (kv.first == "n") {
		config.n = (int) std::stod(kv.second);

	} else if (kv.first == "save") {
		config.save = (bool) std::stoi(kv.second);

	}  else {
		std::cout << "- key '" << kv.first << "' did not match a defined key" << std::endl;
	}
}

void ConfigParse::vectors(const kvpair& kv) {
	if (kv.first == "weights") {
		std::regex id("\\-?\\d+([.]\\d+)?");
		std::string line = kv.second;
		auto start = std::sregex_iterator(line.begin(), line.end(), id);
		auto end = std::sregex_iterator();

		for (; start != end; ++start) {
			config.weights.emplace_back(std::stod((*start).str()));
		}

	} else {
		std::cout << "- key '" << kv.first << "' did not match a defined key" << std::endl;
	}
}

ConfigParse::kvpair ConfigParse::split_key_value(std::string line) {
	kvpair ret;

	// match object
	std::smatch match;

	std::regex_search(line, match, std::regex{"="});

	int pos = match.position();
	ret.first = line.substr(0, pos);

	// get rid of quotes
	ret.second = line.substr(pos + 1, line.size() - 2);

	return ret;
}


CData::CData() noexcept
	:	shifts{1, 1, 0, 0},	weights{0},		transforms{""},
		in{""},				out{""},		pout{""},
		savefile{""},		width{1}, 		height{1},			n{0},
		preview{false},		lines{false},	triangles{false},	save{false}	{
}

void CData::reset() noexcept {
	shifts[0] = (shifts[1] = 1);
	shifts[2] = (shifts[3] = 0);
	weights.clear();
	transforms = "";
	in = "";
	out = "";
	pout = "";
	save = false;
	preview = false;
	lines = false;
	triangles = false;
	width = 1;
	height = 1;
	n = 0;
	savefile = "";
}
