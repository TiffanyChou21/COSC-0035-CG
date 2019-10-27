#include "Point.h"

Point::Point()
{
	x = y = z = 0;
}

Point::Point(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Point Point::operator+(Point b)
{
	Point newPoint(x + b.x, y + b.y, z + b.z);
	return newPoint;
}

Point Point::operator-(Point & b)
{
	Point newPoint(x - b.x, y - b.y, z - b.z);
	return newPoint;
}

Point Point::operator*(double p)
{
	return Point{ p*x, p*y, p*z };
}

Point Point::operator/(double p)
{
	return Point{ x / p,  y / p,  z / p };
}

Point Point::operator/=(double p)
{
	x /= p, y /= p, z /= p;
	return *this;
}

Point& Point::operator+=(Point &b)
{
	x += b.x, y += b.y, z += b.z;
	return *this;
}

Point & Point::operator-=(Point & b)
{
	x -= b.x, y -= b.y, z -= b.z;
	return *this;
}

double Point::length()
{
	return sqrt(x*x + y*y + z*z);
}


Point::~Point()
{
}
