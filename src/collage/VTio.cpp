//
// Created by ktulhu on 2018-12-10.
//

#include "VTio.h"
#include <regex>
#include <IOFunc.h>

TReader::TReader() noexcept {

}

TReader::~TReader() noexcept {

}

void TReader::read(const std::string &path) noexcept {
	std::cout << "Reading triangles from path '" << path << "'." << std::endl;
	triangles.clear();

	auto reader = [&](const std::string& line) {
		// regex to check form {%f, %f, %f, %f} {%f, %f} %f
		std::regex form(
				"([ \\t]*\\[(\\-?\\d+([.]\\d+)?(,[ \\t]*)?){2}\\],?){3}"
			);

		// check if line is in correct format
		if (!std::regex_match(line, form)) {
			std::cout << "- '" << line << "'" << std::endl;
			return;
		}

		// regex for any decimal or integer
		std::regex id("\\-?\\d+([.]\\d+)?");
		auto start = std::sregex_iterator(line.begin(), line.end(), id);

		double x[3];
		double y[3];

		// get coords
		for (int i = 0; i < 3; ++start, ++i) {
			x[i] = std::stod((*start).str());
			++start;
			y[i] = std::stod((*start).str());
			std::cout << "Read x = " << x[i] << ", y = " << y[i] << std::endl;
		}

		// construct triangle
		using Pt = BMShape::Coord;
		triangles.emplace_back(Pt{x[0], y[0]}, Pt{x[1], y[1]}, Pt{x[2], y[2]});
	};

	iofunc::read(path, reader);
}

std::vector<BMShape::Triangle> read_triangles(const std::string &path) noexcept {
	TReader tr;
	tr.read(path);
	return tr.triangles;
}

std::vector<BMShape::Line> read_lines(const std::string &path) noexcept {
	LReader lr;
	lr.read(path);
	return lr.lines;
}

void LReader::read(const std::string &path) noexcept {
	std::cout << "Reading lines from path '" << path << "'." << std::endl;
	lines.clear();

	auto reader = [&](const std::string& line) {
		// regex to check form {%f, %f, %f, %f} {%f, %f} %f
		std::regex form(
				"([ \\t]*\\[(\\-?\\d+([.]\\d+)?(,[ \\t]*)?){2}\\],?){2}"
		);

		// check if line is in correct format
		if (!std::regex_match(line, form)) {
			std::cout << "- '" << line << "'" << std::endl;
			return;
		}

		// regex for any decimal or integer
		std::regex id("\\-?\\d+([.]\\d+)?");
		auto start = std::sregex_iterator(line.begin(), line.end(), id);

		double x[2];
		double y[2];

		// get coords
		for (int i = 0; i < 2; ++start, ++i) {
			x[i] = std::stod((*start).str());
			++start;
			y[i] = std::stod((*start).str());
			std::cout << "Read x = " << x[i] << ", y = " << y[i] << std::endl;
		}

		// construct triangle
		using Pt = BMShape::Coord;
		lines.emplace_back(Pt{x[0], y[0]}, Pt{x[1], y[1]});
	};
	iofunc::read(path, reader);
}
