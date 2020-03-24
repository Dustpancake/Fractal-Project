//
// Created by ktulhu on 2018-12-05.
//

#ifndef PROJECT_COLLAGE_H
#define PROJECT_COLLAGE_H

#include <string>
#include <Shapes.h>
#include "../transform/AffineTransform.h"

class Collage {
public:
	Collage() noexcept = default ;
	virtual ~Collage() noexcept = default ;

	virtual std::vector<AffineTransform> find_transform() const noexcept = 0;

protected:
	virtual AffineTransform calc_transform(int i) const noexcept = 0;
};

// triangle based collage
class TCollage : public Collage {
public:
	explicit TCollage(const std::string& path) noexcept ;

	void add_triangle_pair(const BMShape::Triangle &t1, const BMShape::Triangle &t2) noexcept ;
	std::vector<AffineTransform> find_transform() const noexcept ;

	void roll(int index) noexcept ;
	void shuffle(int index) noexcept ;

	BMShape::Triangle* get_triangle_ptr(int index) noexcept ;

private:
	std::vector<BMShape::Triangle> triangles ;
	AffineTransform calc_transform(int i) const noexcept final ;
};

// line based collage
class LCollage : public Collage {
	using Line = BMShape::Line;
public:

	explicit LCollage(const std::string& path) noexcept ;

	void add_line_pair(const Line& l1, const Line& l2) noexcept ;
	std::vector<AffineTransform> find_transform() const noexcept ;

private:
	std::vector<Line> lines ;
	AffineTransform calc_transform(int i) const noexcept final ;
};



#endif //PROJECT_COLLAGE_H
