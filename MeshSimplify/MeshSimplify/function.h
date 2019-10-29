#ifndef FUNCTION
#define FUNCTION

double det(
	double a11, double a12, double a13,
	double a21, double a22, double a23,
	double a31, double a32, double a33
	) 
{
	return a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a13*a22*a31 - a12*a21*a33 - a11*a32*a23;
}

point cross(point &a, point &b)
{
	return point{
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
	};
}

double dot(point &a, point &b)
{
	return a.x * b.x + a.y*b.y + a.z*b.z;
}

double norm(const point &a)
{
	return a.x*a.x + a.y*a.y + a.z*a.z;
}

#endif