//
// Created by ktulhu on 26/11/2018.
//

#include "Shapes.h"
#include <iostream>
#include <cmath>
#include <Shapes.h>


BMShape::Shape::Shape() {
}

BMShape::Shape::~Shape() {
}

BMShape::Line::Line(double x1, double y1, double x2, double y2) noexcept
        :   x1{x1}, x2{x2}, y1{y1}, y2{y2},
            thickness{0}    {
}

BMShape::Line::Line(double x1, double y1, double x2, double y2, int thickness) noexcept
        :   x1{x1}, x2{x2}, y1{y1}, y2{y2},
            thickness{thickness}    {
}

BMShape::Line::Line(const BMShape::Coord &p1, const BMShape::Coord &p2) noexcept
		:	x1{p1.x},	x2{p2.x},
			y1{p1.y},	y2{p2.y},
			thickness{0}	{
}

BMShape::Line::~Line() noexcept {
}

void BMShape::Line::trace(BWrap::Frame& frame) const noexcept {
	// using Bresenham's line algorithm

	// check special cases
	if ((int) y1 == (int) y2) {
		if (x1 < x2) {
			horizontal_draw(frame, x1, x2, y1);

		} else {
			horizontal_draw(frame, x2, x1, y1);

		}

	} else if ((int) x1 == (int) x2) {
		if (y1 < y2) {
			vertical_draw(frame, y1, y2, x1);

		} else {
			vertical_draw(frame, y2, y1, x1);

		}

	} else if (abs(y2 - y1) < abs(x2 - x1)) {

		// check whether to draw along x or y
		// then pick point order

		if (x1 > x2) {
			small_grad_draw(frame, x2, y2, x1, y1);
		} else {
			small_grad_draw(frame, x1, y1, x2, y2);
		}
	} else {
		if (y1 > y2) {
			big_grad_draw(frame, x2, y2, x1, y1);
		} else {
			big_grad_draw(frame, x1, y1, x2, y2);
		}
	}
}

// CAN PROBABLY NARROW THESE TWO FUNCTIONS INTO ONE :: TODO LATER

void BMShape::Line::small_grad_draw(BWrap::Frame &frame, double x1, double y1, double x2, double y2) const noexcept {
	double diff_x = x2 - x1;
	double diff_y = y2 - y1;
	double y_inc = 1;

	if (diff_y < 0) {
		y_inc *= -1;
		diff_y *= -1;
	}

	double mid_point = (2 * diff_y) - diff_x;
	int y = (int) y1;

	int xmax = frame.getX(), ymax = frame.getY();
	auto in_limit = [=](int x, int y) -> bool {
		return (x < xmax) && (y < ymax) && (x >= 0) && (y >= 0);
	};

	for (int x = (int) x1; x <= x2 && in_limit(x, y); ++x) {
		frame.index(x, y) += 1;
		if (mid_point > 0) {
			y += (int) y_inc;
			mid_point -= 2 * diff_x;
		}
		mid_point += 2 * diff_y;
	}
}

void BMShape::Line::big_grad_draw(BWrap::Frame &frame, double x1, double y1, double x2, double y2) const noexcept {
	double diff_x = x2 - x1;
	double diff_y = y2 - y1;
	double x_inc = 1;

	if (diff_x < 0) {
		x_inc *= -1;
		diff_x *= -1;
	}

	double mid_point = (2 * diff_x) - diff_y;
	int x = (int) x1;

	int xmax = frame.getX(), ymax = frame.getY();
	auto in_limit = [=](int x, int y) -> bool {
		return (x < xmax) && (y < ymax) && (x >= 0) && (y >= 0);
	};

	for (int y = (int) y1; y <= y2 && in_limit(x, y); ++y) {
		frame.index(x, y) += 1;
		if (mid_point > 0) {
			x += (int) x_inc;
			mid_point -= 2 * diff_y;
		}
		mid_point += 2 * diff_x;
	}
}

// CAN PROBABLY NARROW THESE TWO FUNCTIONS INTO ONE :: TODO LATER

void BMShape::Line::vertical_draw(BWrap::Frame &frame, double y1, double y2, double x) const noexcept {
	int ymax = frame.getY();
	auto in_limit = [=](int y) -> bool {
		return (y < ymax) && (y >= 0);
	};

	for (int y = (int) y1; y <= y2 && in_limit(y); ++y) {
		frame.index((int) x, y) += 1;
	}
}

void BMShape::Line::horizontal_draw(BWrap::Frame &frame, double x1, double x2, double y) const noexcept {
	int xmax = frame.getX();
	auto in_limit = [=](int x) -> bool {
		return (x < xmax) && (x >= 0);
	};

	for (int x = (int) x1; x <= x2 && in_limit(x); ++x) {
		frame.index(x, (int) y) += 1;
	}
}

BMShape::Coord BMShape::Line::get_midpoint() const noexcept {
	double xmax, xmin, ymax, ymin;

	if (x1 < x2) {
		xmax = x2;
		xmin = x1;
	} else {
		xmin = x2;
		xmax = x1;
	}

	if (y1 < y2) {
		ymax = y2;
		ymin = y1;
	} else {
		ymin = y2;
		ymax = y1;
	}

	xmin += ((xmax - xmin) / 2);
	ymin += ((ymax - ymin) / 2);

	return Coord{xmin, ymin};
}

double BMShape::Line::get_length() const noexcept {
	double x = abs(x2 - x1);
	double y = abs(y2 - y1);
	return sqrt((x * x) + (y * y));
}

double BMShape::Line::get_angle_to(const BMShape::Line &line) const noexcept {
	double g1 = get_gradient();
	double g2 = line.get_gradient();
	return atan((g2 - g1) / (1 + (g1 * g2)));
}

double BMShape::Line::get_gradient() const noexcept {
	return (y2 - y1) / (x2 - x1);
}

BMShape::Coord BMShape::Line::get_first_point() const noexcept {
	return BMShape::Coord(x1, y1);
}

BMShape::Coord BMShape::Line::get_second_point() const noexcept {
	return BMShape::Coord(x2, y2);
}

BMShape::Triangle::Triangle(const BMShape::Coord &p1, const BMShape::Coord &p2, const BMShape::Coord &p3) noexcept
	:	p1{p1},	p2{p2},	p3{p3}	{

}

void BMShape::Triangle::trace(BWrap::Frame &frame) const noexcept {
	// construct lines
	BMShape::Line l1{p1, p2};
	BMShape::Line l2{p1, p3};
	BMShape::Line l3{p2, p3};

	// trace each line
	l1.trace(frame);
	l2.trace(frame);
	l3.trace(frame);
}

BMShape::Triangle::Triangle(const BMShape::Triangle &t1) noexcept
	:	p1{t1.p1},	p2{t1.p2},	p3{t1.p3}	{
}

BMShape::Triangle &BMShape::Triangle::operator=(const BMShape::Triangle &t1) noexcept {
	p1 = t1.p1;
	p2 = t1.p2;
	p3 = t1.p3;
	return *this;
}

