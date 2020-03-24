//
// Created by ktulhu on 2018-12-10.
//

#ifndef PROJECT_IOFUNC_H
#define PROJECT_IOFUNC_H

#include <fstream>
#include <iostream>
#include <regex>

namespace iofunc {
	void read(const std::string& path, const std::function<void(const std::string&)>& callback) noexcept ;

	void write(const std::string &path, const std::function<int(std::string&)> &writer) noexcept ;
}

#endif //PROJECT_IOFUNC_H
