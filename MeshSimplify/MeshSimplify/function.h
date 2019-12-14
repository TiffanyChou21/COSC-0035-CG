#ifndef FUNCTION
#define FUNCTION

// 计算行列式
double det(
	double a11, double a12, double a13,
	double a21, double a22, double a23,
	double a31, double a32, double a33
	) 
{
	return a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a13*a22*a31 - a12*a21*a33 - a11*a32*a23;
}

// 向量叉乘
point cross(point &a, point &b)
{
	return point{
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
	};
}

// 向量点乘
double dot(point &a, point &b)
{
	return a.x * b.x + a.y*b.y + a.z*b.z;
}

// 求向量长度平方
double norm(const point &a)
{
	return a.x*a.x + a.y*a.y + a.z*a.z;
}

#endif