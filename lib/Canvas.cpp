//
// Created by ktulhu on 26/11/2018.
//

#include "Canvas.h"
#include <iostream>
#include <Canvas.h>

#include "bmplib/EasyBMP.h"

BWrap::Canvas::Canvas(BWrap::Frame&& f) noexcept
	: frame{std::move(f)}	{
    std::cout << "Canvas created with frame move @ " << this << std::endl;
}

BWrap::Canvas::~Canvas() {
	std::cout << "Canvas deleted @ " << this << std::endl;
}

void BWrap::Canvas::trace_shape(const BMShape::Shape &s) noexcept {
    s.trace(frame);
}

BWrap::Canvas::Canvas(int xsize, int ysize) noexcept
	:	frame{Frame{xsize, ysize}}	{
	std::cout << "Canvas created with x, y @ " << this << std::endl;

}

void BWrap::Canvas::save_image(const std::string &name, bool regular) const noexcept {
	frame.save(name, regular);
}

void BWrap::Canvas::trace_point(int x, int y) noexcept {
	frame.index(x, y) += 1;
}

void BWrap::Canvas::trace_point(const BMShape::Coord &c) noexcept {
	frame.index((int) c.x, (int) c.y) += 1;
}

void BWrap::Canvas::trace_points(const std::vector<BMShape::Coord> &v) noexcept {
	for (const auto& i : v) {
		frame.index((int) i.x, (int) i.y) += 1;
	}
}

int BWrap::Canvas::getXsize() const noexcept {
	return frame.getX();
}

int BWrap::Canvas::getYsize() const noexcept {
	return frame.getY();
}

void BWrap::Canvas::fill_point(const BMShape::Coord &c) noexcept {
	frame.index((int) c.x, (int) c.y) = 1;
}

void BWrap::Canvas::set_detail_factor(double fact) noexcept {
	frame.set_detail_factor(fact);
}

void BWrap::Canvas::set_delta_max(double d_max) noexcept {
	frame.set_delta_max(d_max);
}

void BWrap::Canvas::clear() noexcept {
	frame.clear();
}

BWrap::Frame BWrap::image_to_frame(const std::string &path, int tolerance) noexcept {
	BMP image;

	// ensure image read properly
	if (!image.ReadFromFile(path.c_str())) {
		std::cerr << "Failed to open file: " << path << std::endl;
		return Frame(1, 1);
	}

	// get image details
	const int width = image.TellWidth();
	const int height = image.TellHeight();

	// create frame
	Frame frame = Frame(width, height);

	// write image to frame
	double temp;
	for (int i = 0, j; i < width; ++i) {
		for (j = 0; j < height; ++j) {
			temp = 0.30*(image(i, j)->Red) + 0.59*(image(i, j)->Green) + 0.11*(image(i, j)->Blue);
			if (temp < tolerance) {
				frame.index(i, j) = temp;
			} else {
				frame.index(i, j) = 0;
			}
		}
	}

	return frame;
}