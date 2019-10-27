#ifndef MATRIX
#define MATRIX

class Matrix
{
public:
	double data[4][4];
public:
	Matrix();
	~Matrix();
	void Clear();
	Matrix& operator+=(Matrix& matrix);
	Matrix& operator-=(Matrix& matrix);
	Matrix operator+(Matrix& matrix);
};
#endif

