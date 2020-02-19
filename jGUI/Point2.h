#pragma once
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// 2D vector
///////////////////////////////////////////////////////////////////////////////
struct Point2
{
	double x;
	double y;

	// ctors
	Point2() : x(0), y(0) {};
	Point2(double x, double y) : x(x), y(y) {};

	// utils functions
	void        set(double x, double y);
	double       length() const;                         //
	double       distance(const Point2& vec) const;     // distance between two vectors
	Point2&    normalize();                            //
	double       dot(const Point2& vec) const;          // dot product
	double       cross(const Point2& vec) const;          // cross product
	bool        equal(const Point2& vec, double e) const; // compare with epsilon
	Point2&		rotate(double radian);

	// operators
	Point2     operator-() const;                      // unary operator (negate)
	Point2     operator+(const Point2& rhs) const;    // add rhs
	Point2     operator-(const Point2& rhs) const;    // subtract rhs
	Point2&    operator+=(const Point2& rhs);         // add rhs and update this object
	Point2&    operator-=(const Point2& rhs);         // subtract rhs and update this object
	Point2     operator*(const double scale) const;     // scale
	Point2     operator*(const Point2& rhs) const;    // multiply each element
	Point2&    operator*=(const double scale);          // scale and update this object
	Point2&    operator*=(const Point2& rhs);         // multiply each element and update this object
	Point2     operator/(const double scale) const;     // inverse scale
	Point2&    operator/=(const double scale);          // scale and update this object
	bool        operator==(const Point2& rhs) const;   // exact compare, no epsilon
	bool        operator!=(const Point2& rhs) const;   // exact compare, no epsilon
	bool        operator<(const Point2& rhs) const;    // comparison for sort
	double       operator[](int index) const;            // subscript operator v[0], v[1]
	double&      operator[](int index);                  // subscript operator v[0], v[1]

	friend Point2 operator*(const double a, const Point2 vec);
};


inline Point2 Point2::operator-() const {
	return Point2(-x, -y);
}

inline Point2 Point2::operator+(const Point2& rhs) const {
	return Point2(x + rhs.x, y + rhs.y);
}

inline Point2 Point2::operator-(const Point2& rhs) const {
	return Point2(x - rhs.x, y - rhs.y);
}

inline Point2& Point2::operator+=(const Point2& rhs) {
	x += rhs.x; y += rhs.y; return *this;
}

inline Point2& Point2::operator-=(const Point2& rhs) {
	x -= rhs.x; y -= rhs.y; return *this;
}

inline Point2 Point2::operator*(const double a) const {
	return Point2(x*a, y*a);
}

inline Point2 Point2::operator*(const Point2& rhs) const {
	return Point2(x*rhs.x, y*rhs.y);
}

inline Point2& Point2::operator*=(const double a) {
	x *= a; y *= a; return *this;
}

inline Point2& Point2::operator*=(const Point2& rhs) {
	x *= rhs.x; y *= rhs.y; return *this;
}

inline Point2 Point2::operator/(const double a) const {
	return Point2(x / a, y / a);
}

inline Point2& Point2::operator/=(const double a) {
	x /= a; y /= a; return *this;
}

inline bool Point2::operator==(const Point2& rhs) const {
	return (x == rhs.x) && (y == rhs.y);
}

inline bool Point2::operator!=(const Point2& rhs) const {
	return (x != rhs.x) || (y != rhs.y);
}

inline bool Point2::operator<(const Point2& rhs) const {
	if (x < rhs.x) return true;
	if (x > rhs.x) return false;
	if (y < rhs.y) return true;
	if (y > rhs.y) return false;
	return false;
}

inline double Point2::operator[](int index) const {
	return (&x)[index];
}

inline double& Point2::operator[](int index) {
	return (&x)[index];
}

inline void Point2::set(double x, double y) {
	this->x = x; this->y = y;
}

inline double Point2::length() const {
	return sqrt(x*x + y * y);
}

inline double Point2::distance(const Point2& vec) const {
	return sqrt((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y));
}

inline Point2& Point2::normalize() {
	const double EPSILON = 0.000001;
	double xxyy = x * x + y * y;
	if (xxyy < EPSILON)
		return *this;

	double invLength = 1.0f / sqrt(xxyy);
	x *= invLength;
	y *= invLength;
	return *this;
}

inline Point2& Point2::rotate(double radian) {
	double _x = x * cos(radian) - y * sin(radian);
	double _y = x * sin(radian) + y * cos(radian);
	x = _x; y = _y;
	return *this;
}

inline double Point2::dot(const Point2& rhs) const {
	return (x*rhs.x + y * rhs.y);
}
//LeftHand System
inline double Point2::cross(const Point2& rhs) const {
	return (x*rhs.y - y * rhs.x);
}
inline bool Point2::equal(const Point2& rhs, double epsilon) const {
	return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon;
}

inline Point2 operator*(const double a, const Point2 vec) {
	return Point2(a*vec.x, a*vec.y);
}

// END OF Point2 /////////////////////////////////////////////////////////////

