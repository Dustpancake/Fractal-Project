//
// Created by ktulhu on 2018-12-05.
//

#include "Collage.h"
#include "VTio.h"

// ########################### TCollage ########################### //

TCollage::TCollage(const std::string &path) noexcept
	:	triangles{read_triangles(path)}	{
}

void TCollage::add_triangle_pair(const BMShape::Triangle &t1, const BMShape::Triangle &t2) noexcept {
	triangles.emplace_back(t1);
	triangles.emplace_back(t2);
}

AffineTransform TCollage::calc_transform(int i) const noexcept {
	using Mat = IFS::Matrix;
	using Vec = IFS::Vector;

	auto &tri1 = triangles[i];
	auto &tri2 = triangles[i+1];

	Mat t1 = GEN::from_coords(
			tri1.p2 - tri1.p1,
			tri1.p3 - tri1.p1
			).inverse();

	Mat t2 = GEN::from_coords(
			tri2.p2 - tri2.p1,
			tri2.p3 - tri2.p1
			);

	Vec v2{tri2.p1};

	Mat tmat = (t2 * t1);
	if (!tmat.check_transform()) {
		std::cerr 	<< "- transform for pair " << (i/2)+1 << " is not a contraction.\n"
					<< "  returning a no-effect transform, this will lead to undesired output\n"
					<< "  advise updating triangles and try again." << std::endl;
		return AffineTransform(Mat{1, 0, 0, 1}, Vec{0, 0});
	}

	// create temp transform to update point in order to determine the correct shift
	AffineTransform afs{tmat, {0,0}};
	Vec vtemp{tri1.p1};
	afs(vtemp);
	vtemp = Vec{tri2.p1.x - vtemp.b1, tri2.p1.y - vtemp.b2};
	return AffineTransform{tmat, vtemp};

}

std::vector<AffineTransform> TCollage::find_transform() const noexcept {
	std::cout << "TCollage @ " << this << " :: finding triangle transforms" << std::endl;
	// check has correct number of triangles
	int num = (int) triangles.size();
	if (num % 2 != 0 || num == 0) {
		std::cout << "- bad number of triangles, returning standard vector" << std::endl;
		return std::vector<AffineTransform>{AffineTransform{IFS::Matrix{1, 0, 0, 1}, IFS::Vector{0, 0}}};
	}
	std::cout << "+ " << num << " triangles in instance" << std::endl;

	// create return vector
	std::vector<AffineTransform> vts;

	// populate vector
	for (int i = 0; i < num; i += 2) {
		std::cout << "+ calculating for pair " << (i/2)+1 << std::endl;
		vts.emplace_back(calc_transform(i));
	}
	std::cout << "* all transformations found" << std::endl;
	return vts;
}

void TCollage::roll(int index) noexcept {
	triangles.at(index).roll();

}

void TCollage::shuffle(int index) noexcept {
	triangles.at(index).shuffle();
}

BMShape::Triangle* TCollage::get_triangle_ptr(int index) noexcept {
	return &triangles[index];
}

// ########################### LCollage ########################### //

LCollage::LCollage(const std::string &path) noexcept
	:	lines{read_lines(path)}	{
}

void LCollage::add_line_pair(const LCollage::Line &l1, const LCollage::Line &l2) noexcept {
	lines.emplace_back(l1);
	lines.emplace_back(l2);
}

std::vector<AffineTransform> LCollage::find_transform() const noexcept {
	std::cout << "LCollage @ " << this << " :: finding lines transforms" << std::endl;
	// check has correct number of triangles
	int num = (int) lines.size();
	if (num % 2 != 0 || num == 0) {
		std::cout << "- bad number of lines, returning standard vector" << std::endl;
		return std::vector<AffineTransform>{AffineTransform{IFS::Matrix{1, 0, 0, 1}, IFS::Vector{0, 0}}};
	}
	std::cout << "+ " << num << " lines in instance" << std::endl;

	// create return vector
	std::vector<AffineTransform> vts;

	// populate vector
	for (int i = 0; i < num; i += 2) {
		std::cout << "+ calculating for pair " << (i/2)+1 << std::endl;
		vts.emplace_back(calc_transform(i));
	}
	std::cout << "* all transformations found" << std::endl;
	return vts;
}

AffineTransform LCollage::calc_transform(int i) const noexcept {
	using Mat = IFS::Matrix;
	using Vec = IFS::Vector;

	// find angle between
	double angle = lines[i].get_angle_to(lines[i + 1]);

	// find ratio of lengths
	double ratio = lines[i].get_length() / lines[i + 1].get_length();

	Mat tmat = GEN::rotation(angle) * GEN::contraction(ratio);

	if (!tmat.check_transform()) {
		std::cerr 	<< "- transform for pair " << (i/2)+1 << " is not a contraction.\n"
					 << "  returning a no-effect transform, this will lead to undesired output\n"
					 << "  advise updating lines and try again." << std::endl;
		return AffineTransform(Mat{1, 0, 0, 1}, Vec{0, 0});
	}

	// create temp transform to update point in order to determine the correct shift
	AffineTransform afs{tmat, {0,0}};
	Vec vtemp{lines[i].get_first_point()};
	afs(vtemp);
	vtemp = Vec{lines[i + 1].get_first_point()} - vtemp;
	return AffineTransform(tmat, vtemp);
}
