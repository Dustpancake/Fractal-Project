//
// Created by ktulhu on 26/11/2018.
//

#ifndef PROJECT_SHAPES_H
#define PROJECT_SHAPES_H

#include "Frame.h"
#include <iostream>

namespace BMShape {

	struct Coord {
		Coord() = delete;
		Coord(double x, double y) noexcept
			:	x{x},	y{y} {
			// std::cout << "Coord XY @ " << this << std::endl;
		}
		Coord(const Coord& c) noexcept
			:	x{c.x},	y{c.y} {
			// std::cout << "Coord COPY @ " << this << std::endl;
		}

		Coord& operator=(const Coord& c) noexcept = default ;

		friend Coord operator-(const Coord& c1, const Coord& c2) noexcept {
			Coord temp{c1};
			temp.x -= c2.x;
			temp.y -= c2.y;
			return temp;
		}

		double x, y;
	};

    class Shape {
    public:
        Shape();

        Shape(const Shape& s) = default ;
        Shape(Shape&& s) = delete ;

        virtual void trace(BWrap::Frame& frame) const noexcept = 0;

        virtual ~Shape();

        // define the position of top left corner of rectangle which trace populates
        //int posx, posy;
    };

    class Line : public Shape {
    public:
    	/*
    	 * TODO
    	 * change the line to store minimal x in x1, so that
    	 * you can get rid of half the conditional expressions
    	 */

        Line() = default ;
        Line(double x1, double y1, double x2, double y2) noexcept ;
        Line(double x1, double y1, double x2, double y2, int thickness) noexcept ;
        Line(const Coord& p1, const Coord& p2) noexcept ;
        ~Line() noexcept override;

        void trace(BWrap::Frame& frame) const noexcept override;

        Coord get_midpoint() const noexcept ;

        // descriptor methods
        double get_length() const noexcept ;
        double get_angle_to(const Line& line) const noexcept ;
        double get_gradient() const noexcept ;

        // accessor
        Coord get_first_point() const noexcept;
        Coord get_second_point() const noexcept;

    private:
        double x1, x2, y1, y2;
        int thickness;

        // for small or large gradients (draws x as y, or y as x)
        void small_grad_draw(BWrap::Frame& frame, double x1, double y1, double x2, double y2) const noexcept ;
        void big_grad_draw(BWrap::Frame& frame, double x1, double y1, double x2, double y2) const noexcept ;

        // special cases
        void vertical_draw(BWrap::Frame& frame, double y1, double y2, double x) const noexcept ;
        void horizontal_draw(BWrap::Frame& frame, double x1, double x2, double y) const noexcept ;
    };


    // test implementation, probably not actually going to be used in the project
    // just wanted to check how easy i could generalize the line
    class Triangle : public Shape {
	public:
    	Triangle() : p1{0,0}, p2{0,0}, p3{0,0} {} ;
    	Triangle(const Coord& p1, const Coord& p2, const Coord& p3) noexcept ;

    	// copy constructor and copy assignment
    	Triangle(const Triangle& t1) noexcept ;

    	Triangle& operator=(const Triangle& t1) noexcept ;

    	void trace(BWrap::Frame& frame) const noexcept override ;

    	// cylce points
    	Triangle& roll() {
    		auto temp = p1;
    		p1 = p3;
    		p3 = p2;
    		p2 = temp;

    		return *this;
    	}

    	// switch p2 and p3
    	Triangle& shuffle() {
    		auto temp = p2;
    		p2 = p3;
    		p3 = temp;

    		return *this;
    	}

    	Coord p1, p2, p3;
    };

}

inline std::ostream& operator<<(std::ostream& out, const BMShape::Coord& p) {
	out << p.x << ", " << p.y;
	return out;
}


#endif //PROJECT_SHAPES_H
