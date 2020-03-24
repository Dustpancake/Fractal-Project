//
// Created by ktulhu on 27/11/2018.
//

#ifndef PROJECT_CHAOSSHAPES_H
#define PROJECT_CHAOSSHAPES_H

#include <Shapes.h>
#include <vector>

namespace CShape {
	typedef BMShape::Coord Pt;

	class Shape {
	public:

		Shape();
		Shape(int xsize, int ysize) noexcept ;
		virtual ~Shape();

		// get vector of vertices
		virtual std::vector<Pt> get_vertices() const noexcept = 0;

	protected:
		int xsize, ysize;
	};


	class RegularPolygon : public Shape {
	public:

		RegularPolygon() = delete;
		RegularPolygon(int n, int xsize, int ysize) noexcept ;
		virtual ~RegularPolygon();

		std::vector<Pt> get_vertices() const noexcept;

	private:
		const int n;
		uint padding;
	};

	// my idea was to generalize for other, more irregular shapes but never got round to it

}



#endif //PROJECT_CHAOSSHAPES_H
