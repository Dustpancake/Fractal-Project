//
// Created by ktulhu on 2018-12-10.
//

#ifndef PROJECT_VTIO_H
#define PROJECT_VTIO_H

#include <string>
#include <Shapes.h>
#include <vector>

// vertex triangle input output (mainly input, in fact entirely input)
struct Reader {
	using Pt = BMShape::Coord ;

	virtual ~Reader() noexcept = default ;

	virtual void read(const std::string& path) noexcept = 0 ;
};

struct TReader : public Reader {
	using Tri = BMShape::Triangle ;

	TReader() noexcept ;
	~TReader() noexcept final;

	void read(const std::string& path) noexcept final ;
	std::vector<Tri> triangles ;
};

struct LReader : public Reader {
	using Line = BMShape::Line ;

	LReader() noexcept = default ;
	~LReader() noexcept final = default ;

	void read(const std::string& path) noexcept final ;
	std::vector<Line> lines ;
};

std::vector<BMShape::Triangle> read_triangles(const std::string& path) noexcept ;
std::vector<BMShape::Line> read_lines(const std::string& path) noexcept ;


#endif //PROJECT_VTIO_H
