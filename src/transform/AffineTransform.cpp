//
// Created by ktulhu on 28/11/2018.
//

#include "AffineTransform.h"
#include <cmath>

IFS::Matrix::Matrix() noexcept
		:	a11{0},		a12{0},		a21{0},		a22{0}		{
}

IFS::Matrix::Matrix(double a11, double a12, double a21, double a22) noexcept
		:	a11{a11},	a12{a12},	a21{a21},	a22{a22}	{
}

IFS::Matrix::~Matrix() noexcept {
}

double IFS::Matrix::det() const noexcept {
	return ((a11 * a22) - (a12 * a21));
}

IFS::Vector IFS::Matrix::operator*(const IFS::Vector &v) const noexcept {
	IFS::Vector temp{v};
	temp.b1 = (a11 * v.b1) + (a12 * v.b2);
	temp.b2 = (a21 * v.b1) + (a22 * v.b2);
	return temp;
}

IFS::Matrix::Matrix(double *vals) noexcept
	:	a11{vals[0]},	a12{vals[1]},	a21{vals[2]},	a22{vals[3]}	{
}

IFS::Matrix IFS::Matrix::inverse() const noexcept {
	double dtr = det();
	return IFS::Matrix(
			a22/dtr,	-a12/dtr,
			-a21/dtr,	a11/dtr
			);
}

IFS::Matrix IFS::Matrix::operator*(const IFS::Matrix &m) const noexcept {
	IFS::Matrix temp;
	temp.a11 = (a11 * m.a11) + (a12 * m.a21);
	temp.a12 = (a11 * m.a12) + (a12 * m.a22);
	temp.a21 = (a21 * m.a11) + (a22 * m.a21);
	temp.a22 = (a21 * m.a12) + (a22 * m.a22);
	return temp;
}

bool IFS::Matrix::check_transform() const noexcept {
	double delta = det();
	if ((a11*a11) + (a12*a12) + (a21*a21) + (a22*a22) - (delta*delta) >= 1) {
		return false;
	} else if ((a11*a11) + (a21*a21) >= 1) {
		return false;
	} else if ((a12*a12) + (a22*a22) >= 1) {
		return false;
	}
	return true;
}

IFS::Vector::Vector() noexcept
		:	b1{0},		b2{0}	{
}

IFS::Vector::Vector(double b1, double b2) noexcept
		:	b1{b1},		b2{b2}	{
}

IFS::Vector::~Vector() noexcept {
	// std::cout << "- Vector @ " << this << std::endl;
}

IFS::Vector::Vector(const IFS::Vector::Pt& pt) noexcept
		:	b1{pt.x},	b2{pt.y}	{
}

IFS::Vector IFS::Vector::operator+(const IFS::Vector &v1) const noexcept {
	IFS::Vector ret{*this};
	ret.b1 += v1.b1;
	ret.b2 += v1.b2;
	return ret;
}

IFS::Vector::Vector(double *vals) noexcept
	:	b1{vals[0]},	b2{vals[1]}{
}

IFS::Vector &IFS::Vector::operator=(const IFS::Vector::Pt &pt) noexcept {
	b1 = pt.x;
	b2 = pt.y;
	return *this;
}

IFS::Vector& IFS::Vector::invert() noexcept {
	double temp = b1;
	b1 = b2;
	b2 = temp;
	return *this;
}

IFS::Vector IFS::Vector::operator-(const IFS::Vector &v1) const noexcept {
	IFS::Vector ret{*this};
	ret.b1 -= v1.b1;
	ret.b2 -= v1.b2;
	return ret;
}

AffineTransform::~AffineTransform() noexcept {
	// std::cout << "Deleted AffineTransfrom @ " << this << std::endl;
}

AffineTransform::AffineTransform(const IFS::Matrix &a, const IFS::Vector &b) noexcept
		:	mat_A{a},	vec_B{b}	{
	// std::cout << "Created AffineTransfrom @ " << this << std::endl;
}

void AffineTransform::operator()(IFS::Vector& v) const noexcept {
	// std::cout << "calling transform @ " << this << std::endl;
	v = (mat_A * v) + vec_B;
}

IFS::Matrix AffineTransform::get_matrix() noexcept {
	return mat_A;
}

IFS::Vector AffineTransform::get_vector() noexcept {
	return vec_B;
}

// ######################### GENERATORS ######################### //

IFS::Matrix GEN::rotation(double angle) noexcept {
	return IFS::Matrix(cos(angle), -sin(angle), sin(angle), cos(angle));
}

IFS::Vector GEN::translation(double x, double y) noexcept {
	return IFS::Vector(x, y);
}

IFS::Contraction GEN::contraction(double factor) noexcept {
	return IFS::Contraction(factor);
}

IFS::Matrix GEN::from_coords(const BMShape::Coord &c1, const BMShape::Coord &c2) noexcept {
	return IFS::Matrix(c1.x, c2.x, c1.y, c2.y);
}

// ###################### INHERITED CLASSES ###################### //

IFS::Contraction::Contraction(double factor) noexcept {
	a11 = 1/factor;
	a12 = 0;
	a21 = 0;
	a22 = 1/factor;
}

