#include "Matrix.h"



Matrix::Matrix()
{
	Clear();
}


Matrix::~Matrix()
{
}


void Matrix::Clear()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			data[i][j] = 0;
}


Matrix& Matrix::operator+=(Matrix& matrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			data[i][j] += matrix.data[i][j];
	return *this;
}
Matrix& Matrix::operator-=(Matrix& matrix) 
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			data[i][j] -= matrix.data[i][j];
	return *this;
}

Matrix Matrix::operator+(Matrix & matrix)
{
	Matrix m;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m.data[i][j] = data[i][j] + matrix.data[i][j];
	return m;
}
