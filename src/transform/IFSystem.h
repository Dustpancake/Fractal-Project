//
// Created by ktulhu on 28/11/2018.
//

#ifndef PROJECT_IFSYSTEM_H
#define PROJECT_IFSYSTEM_H

#include "AffineTransform.h"
#include <vector>
#include <Shapes.h>
#include <Canvas.h>


class IFSystem {

public:

	IFSystem() noexcept ;
	// move constructor for file reading
	IFSystem(std::vector<AffineTransform>&& transforms, std::vector<double>&& weights) noexcept ;
	virtual ~IFSystem() noexcept ;

	// adds an AffineTransform object to the member vector
	void add_transform(const AffineTransform& tr, double weight) noexcept ;

	// transform point
	void n_point_transform(BWrap::Canvas &c, int n, const BMShape::Coord &start) noexcept ;

	// preview all transforms
	void preview_transforms(BWrap::Frame &frame, bool centre) noexcept ;

	// set image shift values
	void set_shift_special(const BWrap::Canvas &c, char choice) noexcept ;

	// shifts = {xmult, ymult, xmove, ymove}
	void set_shift_all(const BWrap::Canvas &c, double shifts[4]) noexcept ;

	// save the transforms
	void save_system(const std::string& path) const noexcept ;

	// copy operations
	IFSystem& operator=(const IFSystem&) = default ;

private:
	
	IFS::Vector						point;
	std::vector<AffineTransform> 	transforms;
	std::vector<double>				weights;

	// transform values
	double x_mult, y_mult;
	double x_move, y_move;

};


#endif //PROJECT_IFSYSTEM_H
