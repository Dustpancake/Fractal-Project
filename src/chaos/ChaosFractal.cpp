//
// Created by ktulhu on 27/11/2018.
//

#include "ChaosFractal.h"
#include <random>
#include <iterator>
#include <Canvas.h>
#include <Support.h>

ChaosFractal::~ChaosFractal() noexcept {
}

ChaosFractal::ChaosFractal(std::initializer_list<ChaosFractal::Pt> verts) noexcept
	:	Vts{verts},	fraction{0.5},	point{0, 0}	{
}

ChaosFractal::ChaosFractal(std::vector<ChaosFractal::Pt> verts) noexcept
	:	Vts{verts},	fraction{0.5},	point{0, 0}	{
}

ChaosFractal::ChaosFractal(const CShape::Shape& shape) noexcept
	:	Vts{shape.get_vertices()},	fraction{0.5},	point{0, 0} {
}


void ChaosFractal::trace_n_points(BWrap::Canvas& c, int n) noexcept {
	auto populate = [&](std::vector<Pt>& vt) -> int {
		int i = 0;
		for (; i < 1e6 && i < n; ++i) {
			point = point_fraction(point, *prng::random_element(Vts.begin(), Vts.end()));
			vt.emplace_back(point);
		}
		return i;
	};

	trace_n(c, n, populate);
}

ChaosFractal::Pt ChaosFractal::point_fraction(const Pt &p1, const Pt &p2) const noexcept {
	double x = p1.x, y = p1.y;
	x += (p2.x - p1.x) * fraction;
	y += (p2.y - p1.y) * fraction;
	return Pt{x, y};
}

void ChaosFractal::set_fraction(double frac) noexcept {
	if (frac <= 0) {
		std::cerr 	<< "ChaosFractal @ " << this << " set_fraction\n"
					<< "arg 'double frac' is 0. Not updating fraction.\n"
					<< "fraction remains " << fraction << std::endl;
		return;
	} else if (frac > 1) {
		std::cerr 	<< "ChaosFractal @ " << this << " set_fraction\n"
					<< "arg 'double frac' is greater than 1. Not updating fraction.\n"
					<< "fraction remains " << fraction << std::endl;
		return;
	}
	fraction = frac;
}

void ChaosFractal::set_init_point(double x, double y) noexcept {
	point = Pt{x, y};
}

void ChaosFractal::trace_n_weighted_points(BWrap::Canvas &c, int n, const std::vector<double>& v) noexcept {
	double checksum = -1;
	for (const auto& i : v) {
		checksum += i;
	}
	if (checksum >= -0.01 && checksum <= 0.01) {
		if (Vts.size() == v.size()) {

			prng::RandElement next_element(v);
			auto populate = [&](std::vector<Pt>& vt) -> int {
				int i = 0;
				for (; i < 1e6 && i < n; ++i) {
					point = point_fraction(point, *next_element(Vts.begin()));
					vt.emplace_back(point);
				}
				return i;
			};

			// entry point for the trace loop
			trace_n(c, n, populate);

		} else {
			std::cerr << "ChaosFractal @ " << this << " trace_n_weighted_points\n"
					  << "arg 'const std::vector<double>& v' is not the right length.\n"
					  << "Must be equal to number of vertices." << std::endl;
		}
	} else {
		std::cerr << "ChaosFractal @ " << this << " trace_n_weighted_points\n"
				  << "arg 'const std::vector<double>& v' elements do not sum with 0.99 < sum < 1.01\n"
				  << "Current sum = " << checksum + 1.0 << std::endl;
	}
}

void ChaosFractal::trace_n(BWrap::Canvas &c, int n, const std::function<int(std::vector<Pt>&)>& populate) noexcept {
	// implements very basic caching defined in the populate function
	std::cout << "Tracing " << n << " points onto BWrap::Canvas @ " << &c << std::endl;
	std::vector<Pt> vt1;

	for (int i = 0; i < n; ) {
		i += populate(vt1);
		c.trace_points(vt1);
		vt1.clear();
	}
}

