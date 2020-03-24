//
// Created by ktulhu on 27/11/2018.
//

#include "ChaosShapes.h"
#include <cmath>

CShape::Shape::Shape() {
}

CShape::Shape::~Shape() {
}

CShape::Shape::Shape(int xsize, int ysize) noexcept
		:	xsize{xsize},	ysize{ysize}	{
}


CShape::RegularPolygon::RegularPolygon(int n, int xsize, int ysize) noexcept
		:	Shape(xsize, ysize),	n{n}	{
}

CShape::RegularPolygon::~RegularPolygon() {
}

std::vector<CShape::Pt> CShape::RegularPolygon::get_vertices() const noexcept {
	std::vector<CShape::Pt> vts;
	double x, y;
	double radius;
	double xhalf = ((double) xsize - 20 ) / 2;
	double yhalf = ((double) ysize - 20 ) / 2;

	// find shortest side
	xhalf < yhalf ? radius = xhalf : radius = yhalf;

	for (int i = 0; i < n; ++i) {
		x = radius * cos((i * 2 * M_PI / n) - M_PI/2) + xhalf + 10;
		y = radius * sin((i * 2 * M_PI / n) - M_PI/2) + yhalf + 10;
		vts.emplace_back(Pt{x, y});
	}
	return vts;
}
