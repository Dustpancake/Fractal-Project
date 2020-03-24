//
// Created by ktulhu on 27/11/2018.
//

#ifndef PROJECT_CHAOSFRACTAL_H
#define PROJECT_CHAOSFRACTAL_H

#include "ChaosShapes.h"
#include <Canvas.h>
#include <vector>


class ChaosFractal {
public:
	ChaosFractal() = delete ;
	virtual ~ChaosFractal() noexcept ;

	// shortcut typedef
	typedef CShape::Pt Pt;

	// general case constructors for custom vertices
	ChaosFractal(std::initializer_list<Pt> verts) noexcept ;
	explicit ChaosFractal(std::vector<Pt> verts) noexcept ;

	// for a chaos Shape
	explicit ChaosFractal(const CShape::Shape& shape) noexcept ;


	void trace_n_points(BWrap::Canvas& c, int n) noexcept ;
	void trace_n_weighted_points(BWrap::Canvas& c, int n, const std::vector<double>& v) noexcept ;

	// setter methods
	void set_fraction(double frac) noexcept ;
	void set_init_point(double x, double y) noexcept ;

private:
	// vertices
	std::vector<Pt> Vts;
	//fraction
	double fraction;
	// current point
	Pt point;

	// member functions
	Pt point_fraction(const Pt &p1, const Pt &p2) const noexcept ;
	void trace_n(BWrap::Canvas& c, int n, const std::function<int (std::vector<Pt>&)>& populate) noexcept ;
};


#endif //PROJECT_CHAOSFRACTAL_H
