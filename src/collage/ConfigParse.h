//
// Created by ktulhu on 2018-12-13.
//

#ifndef PROJECT_CONFIGPARSE_H
#define PROJECT_CONFIGPARSE_H

#include <vector>
#include <string>
#include <regex>

struct CData {
	CData() noexcept ;
	double 					shifts[4] ;
	std::vector<double> 	weights ;
	std::string 			transforms, in, out, pout, savefile ;
	int 					width, height, n;
	bool 					preview ;
	bool					lines, triangles, save ;

	void reset() noexcept ;
};

struct ConfigParse {
	using kvpair = std::pair<std::string, std::string>;

	ConfigParse() = delete ;
	explicit ConfigParse(const std::string& path) noexcept ;

	void parse();

	// data structure to hold config
	static CData config;

private:
	static void files(const kvpair& kv) ;
	static void options(const kvpair& kv) ;
	static void vectors(const kvpair& kv) ;
	kvpair split_key_value(std::string line) ;
	std::string path;
};


#endif //PROJECT_CONFIGPARSE_H
