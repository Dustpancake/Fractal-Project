//
// Created by ktulhu on 28/11/2018.
//

#ifndef PROJECT_COORDTRANSFORM_H
#define PROJECT_COORDTRANSFORM_H

#include <Shapes.h>
#include <vector>

namespace IFS {

	struct Vector {
		Vector() noexcept ;
		explicit Vector(double vals[2]) noexcept ;
		Vector(double b1, double b2) noexcept ;
		virtual ~Vector() noexcept ;

		// alias
		typedef BMShape::Coord Pt;
		explicit Vector(const Pt& pt) noexcept ;

		// trivial copies
		Vector(const Vector&) noexcept = default ;
		Vector& operator=(const Vector&) noexcept = default ;
		Vector& operator=(const Pt& pt) noexcept ;

		// addition
		Vector operator+(const Vector& v1) const noexcept ;
		Vector operator-(const Vector& v1) const noexcept ;

		// switch b1, b2
		IFS::Vector& invert() noexcept ;

		// data members
		double b1, b2;
	};

	struct Matrix {
		Matrix() noexcept ;
		explicit Matrix(double vals[4]) noexcept ;
		Matrix(double a11, double a12, double a21, double a22) noexcept ;

		virtual ~Matrix() noexcept ;

		// trivial copies
		Matrix(const Matrix&) noexcept = default ;
		Matrix& operator=(const Matrix&) noexcept = default;

		// implementing required operators ONLY -- syntactic specific for laziness
		IFS::Vector operator*(const IFS::Vector& v) const noexcept ;
		Matrix operator*(const Matrix& m) const noexcept ;

		// for finding transforms, require inverse
		Matrix inverse() const noexcept ;
		double det() const noexcept ;

		// check contraction
		bool check_transform() const noexcept ;

		// data members
		double a11, a12, a21, a22;
	};

	struct Contraction : public Matrix {
		explicit Contraction(double factor) noexcept ;
	};

}

class AffineTransform {
public:
	AffineTransform() = delete ;
	virtual ~AffineTransform() noexcept ;
	AffineTransform(const AffineTransform&) noexcept = default ;
	AffineTransform(const IFS::Matrix& a, const IFS::Vector& b) noexcept ;

	// alias
	typedef BMShape::Coord Pt;

	// function operator to perform transform
	virtual void operator()(IFS::Vector& v) const noexcept ;

	// returns by value only
	IFS::Matrix get_matrix() noexcept ;
	IFS::Vector get_vector() noexcept ;

private:
	// const so that i can't accidentally call the wrong operator
	// defined in IFS::Matrix or IFS::Vector
	const IFS::Matrix mat_A;
	const IFS::Vector vec_B;
};

namespace GEN {

	// todo
	// polymorphic

	// returns a rotation matrix for a given angle
	IFS::Matrix rotation(double angle) noexcept ;

	// returns a translation vector for a given distance
	IFS::Vector translation(double x, double y) noexcept ;

	// returns a contraction matrix by a given factor
	IFS::Contraction contraction(double factor) noexcept ;

	// for finding affine transforms, returns column vector c1, c2 matrix
	IFS::Matrix from_coords(const BMShape::Coord& c1, const BMShape::Coord& c2) noexcept ;
}


#endif //PROJECT_COORDTRANSFORM_H
