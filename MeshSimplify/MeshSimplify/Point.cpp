#include "Point.h"

point::point()
{
	x = y = z = 0;
}

point::point(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
}

point point::operator+(point b)
{
	point newpoint(x + b.x, y + b.y, z + b.z);
	return newpoint;
}

point point::operator-(point & b)
{
	point newpoint(x - b.x, y - b.y, z - b.z);
	return newpoint;
}

point point::operator*(double p)
{
	return point{ p*x, p*y, p*z };
}

point point::operator/(double p)
{
	return point{ x / p,  y / p,  z / p };
}

point point::operator/=(double p)
{
	x /= p, y /= p, z /= p;
	return *this;
}

point& point::operator+=(point &b)
{
	x += b.x, y += b.y, z += b.z;
	return *this;
}

point & point::operator-=(point & b)
{
	x -= b.x, y -= b.y, z -= b.z;
	return *this;
}

double point::length()
{
	return sqrt(x*x + y*y + z*z);
}


point::~point()
{
}
