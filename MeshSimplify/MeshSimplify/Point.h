#ifndef MYPOINT
#define MYPOINT

#include <math.h>
class point
{
public:
	// 空间三点坐标
	double x, y, z;
public:
	point();
	point(double _x, double _y, double _z);
	
	// 运算符
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