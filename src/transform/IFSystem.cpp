//
// Created by ktulhu on 28/11/2018.
//

#include "IFSystem.h"
#include <numeric>
#include <Support.h>
#include <map>
#include <cmath>
#include <sstream>
#include <IOFunc.h>

IFSystem::IFSystem() noexcept
	:	point{0, 0},	x_mult{1},	y_mult{1},
						x_move{0},	y_move{0}	{
}

IFSystem::~IFSystem() noexcept {
}

void IFSystem::add_transform(const AffineTransform &tr, double weight) noexcept {
	transforms.emplace_back(tr);
	weights.emplace_back(weight);
}

void IFSystem::n_point_transform(BWrap::Canvas &c, int n, const BMShape::Coord &start) noexcept {
	std::cout << "Starting " << n << " transformations..." <<std::endl;
	{
		double sum = 0;
		for (const auto& i : weights) {
			sum += i;
		}
		if (sum < 1) {
			std::cerr << "- weights vector does not sum to 1, terminating" << std::endl;
			return;
		}
		if (weights.size() != transforms.size()) {
			std::cerr << "- weights and transformation vectors are different sizes, terminating" << std::endl;
			return;
		}
	}
	std::cout 	<< "* x_mult = " << x_mult << "\n"
				<< "* y_mult = " << y_mult << "\n"
				<< "* x_move = " << x_move << "\n"
				<< "* y_move = " << y_move << std::endl;
	point = start;

	const int x_max = c.getXsize() - 1, y_max = c.getYsize() - 1;

	auto check_and_draw = [&](const BMShape::Coord& pt) {
		//std::cout << "PAINT " << pt.x << ", " << pt.y << std::endl;
		if (pt.x >= 0 && pt.x < x_max && pt.y >= 0 && pt.y < y_max) {
			c.trace_point(pt);
		}
	};

	auto tform_image = [&]() -> BMShape::Coord {
		return BMShape::Coord{point.b1 * x_mult + x_move, point.b2 * y_mult + y_move};
	};

	// acquire random element instance
	prng::RandElement next_element{weights};

	// mainloop
	AffineTransform *trans;
	for (int i = 0; i < n; ++i) {
		trans = &*next_element(transforms.begin());
		check_and_draw(tform_image());
		// std::cout << "BEFORE " << point.b1 << ", " << point.b2 << std::endl;
		(*trans)(point);
		// std::cout << "AFTER " << point.b1 << ", " << point.b2 << std::endl;
	}
	check_and_draw(tform_image());
	std::cout << "Done transformations." <<std::endl;

}

IFSystem::IFSystem(std::vector<AffineTransform> &&transforms, std::vector<double> &&weights) noexcept
	:	point{0, 0},	transforms{std::move(transforms)},	weights{std::move(weights)},
		x_mult{1},	y_mult{1},
		x_move{0},	y_move{0}	{
}

void IFSystem::preview_transforms(BWrap::Frame &frame, bool centre) noexcept {
	std::cout << "Starting preview frame transformations..." <<std::endl;

	const int x_max = frame.getX() - 1, y_max = frame.getY() - 1;

	auto check_and_draw = [&](const IFS::Vector& v) {
		if (v.b1 >= 0 && v.b1 < x_max && v.b2 >= 0 && v.b2 < y_max) {
			frame.index((int) round(v.b1), (int) round(v.b2)) += 100;
		}
	};

	// transformation constants
	if (centre) {
		y_move = y_max / 2.0;
		x_move = x_max / 2.0;
	} else {
		x_move = (y_move = 0);
	}

	auto tform_image = [&](const IFS::Vector& v) -> IFS::Vector {
		return IFS::Vector{v.b1 + x_move, v.b2 + y_move};
	};

	// populate Coord vector for each non-zero point on frame
	// for ease of manipulation, centre points around centre of frame
	std::vector<IFS::Vector> points;
	for (int y, x = 0; x < x_max; ++x) {
		for (y = 0; y < y_max; ++y) {
			if(frame.index(x, y) != 0) {
				points.emplace_back(x - x_move, y - y_move);
			}
		}
	}

	// mainloop
	for (auto& trans : transforms) {
		// for each vector point that is non zero
		std::vector<IFS::Vector> copy{points};
		for (auto& vec : copy) {
			(trans)(vec);
			check_and_draw(tform_image(vec));
		}
	}

	std::cout << "Done transformations." <<std::endl;

}

void IFSystem::set_shift_special(const BWrap::Canvas &c, char choice) noexcept {
	const int x_max = c.getXsize() - 1, y_max = c.getYsize() - 1;

	if (choice == 'b') {
		x_mult = x_max / 7;
		y_mult = y_max / 11;
		x_move = x_max / 2.5;
	}
}

void IFSystem::set_shift_all(const BWrap::Canvas &c, double *shifts) noexcept {
	const int x_max = c.getXsize() - 1, y_max = c.getYsize() - 1;

	x_mult = x_max / shifts[0];
	y_mult = y_max / shifts[1];
	shifts[2] == 0 ? x_move = 0 : x_move = x_max / shifts[2];
	shifts[3] == 0 ? y_move = 0 : y_move = y_max / shifts[3];
}

void IFSystem::save_system(const std::string &path) const noexcept {
	std::cout << "Saving transform system" << std::endl;

	std::stringstream ss;
	int i = 0;
	auto writer = [&](std::string& line) {
		if (i == transforms.size()) {
			return 0;
		}
		std::cout << "+ saving transformation " << i + 1 << std::endl;
		ss.str("");
		auto trans = transforms.at(i);
		auto mat = trans.get_matrix();
		auto vec = trans.get_vector();

		ss 	<< "{" << mat.a11 << ",\t" << mat.a12 << ",\t" << mat.a21 << ",\t" << mat.a22 << "}\t"
			<< "{" << vec.b1 << ",\t" << vec.b2 << "}\t" << weights.at(i) << "\n";

		line = ss.str();

		++i;
		return 1;
	};
	iofunc::write(path, writer);
	std::cout << "* system saved as '" << path << "'" << std::endl;
}