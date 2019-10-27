#ifndef POINT
#define POINT

#include <math.h>
class Point
{
public:
	double x, y, z;
public:
	Point();
	Point(double _x, double _y, double _z);
	Point operator+(Point b);
	Point operator-(Point &b);
	Point operator*(double p);
	Point operator/(double p);
	Point& operator+=(Point &b);
	Point& operator-=(Point &b);
	double length();

	Point operator/=(double p);
	~Point();
};

#endif