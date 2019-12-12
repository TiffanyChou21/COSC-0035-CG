#ifndef MYPOINT
#define MYPOINT

#include <math.h>
class point
{
public:
	double x, y, z;
public:
	point();
	point(double _x, double _y, double _z);
	point operator+(point b);
	point operator-(point &b);
	point operator*(double p);
	point operator/(double p);
	point& operator+=(point &b);
	point& operator-=(point &b);
	double length();

	point operator/=(double p);
	~point();
};

#endif