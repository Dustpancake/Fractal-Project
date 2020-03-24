//
// Created by ktulhu on 2018-12-11.
//

#include <IOFunc.h>

void iofunc::read(const std::string &path, const std::function<void(const std::string &)> &callback) noexcept {
	std::ifstream fs;
	fs.open(path);
	if (fs) {
		std::string line;
		while (std::getline(fs, line)) {
			try {
				// lambda can't be noexcept, so need to handle possible exceptions
				callback(line);
			} catch (const std::exception &exc) {
				// terminate file stream, and unwind stack
				std::cout << "Exception caught - terminating read." << std::endl;
				std::cerr << exc.what() << std::endl;
				fs.close();
				return;
			}
		}
	} else {
		std::cerr << "failed to open file '" << path << "'" << std::endl;
	}
	fs.close();
}

void iofunc::write(const std::string &path, const std::function<int(std::string&)> &writer) noexcept {
	std::ofstream fs;
	fs.open(path);
	if (fs) {
		std::string line;
		try {
			// lambda can't be noexcept, so need to handle possible exceptions
			while (writer(line)) {
				fs << line;
			}
		} catch (...) {
			// terminate file stream, and unwind stack
			std::cout << "Exception caught - terminating read." << std::endl;
			fs.close();
			return;
		}
	} else {
		std::cerr << "failed to open file '" << path << "'" << std::endl;
	}
	fs.close();

}
