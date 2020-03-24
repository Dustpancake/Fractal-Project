//
// Created by ktulhu on 29/11/2018.
//

#include "IFSio.h"
#include <IOFunc.h>
#include <iostream>
#include <sstream>
#include <regex>

void IFio::ATReader::read(const std::string &path) noexcept {
	transforms.clear();
	weights.clear();

	std::cout << "Reading file from path '" << path << "'." << std::endl;

	auto reader = [&](const std::string& line) {
		// regex to check form {%f, %f, %f, %f} {%f, %f} %f
		std::regex form(
				"[ \\t]*\\{(\\-?\\d+([.]\\d+)?(,[ \\t]*)?){4}\\}[ \\t]*\\{(\\-?\\d+([.]\\d+)?(,[ \\t]*)?){2}\\}[ \\t]*\\-?\\d+([.]\\d+)?[ \\t]*"
			);

		// check if line is in correct format
		if (!std::regex_match(line, form)) {
			std::cout << "- '" << line << "'" << std::endl;
			return;
		}

		// regex for any decimal or integer
		std::regex id("\\-?\\d+([.]\\d+)?");
		auto start = std::sregex_iterator(line.begin(), line.end(), id);

		double matrix_values[4];
		double vector_values[2];

		// fill matrix
		for (int i = 0; i < 4; ++start, ++i) {
			matrix_values[i] = std::stod((*start).str());
		}
		// fill vector
		for (int i = 0; i < 2; ++start, ++i) {
			vector_values[i] = std::stod((*start).str());
		}

		auto mat = IFS::Matrix{matrix_values};

		if (mat.check_transform()) {
			transforms.emplace_back(mat, IFS::Vector{vector_values});

			// get probability
			weights.emplace_back(std::stod((*(start)).str()));

			std::cout << "+ '" << line << "'" << std::endl;
		} else {
			std::cout << "- '" << line << "' omitted, not a contraction" << std::endl;
		}
	};

	iofunc::read(path, reader);
	std::cout << "IFio::ATReader successfully read " << weights.size() << " affine transformation(s)." << std::endl;

}

IFio::ATReader::ATReader() noexcept {
}

IFio::ATReader::~ATReader() noexcept {
}

IFSystem IFio::readIFSystem(const std::string &path) noexcept {
	IFio::ATReader atread;
	atread.read(path);
	IFSystem ifs{std::move(atread.transforms), std::move(atread.weights)};
	std::cout << "IFSystem successfully constructed." << std::endl;
	return ifs;
}
