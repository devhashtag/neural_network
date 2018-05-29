#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using namespace std;

/// Matrix class, with operator overloading. Can also be used for vectors
class Matrix {

// dimensions
int x;
int y;

// values of the matrix
vector<double> values;

public:
	// constructor, with matrix dimensions
	Matrix(int x, int y, int randomize = true);

	// elementwise operation overloading
	Matrix operator*(Matrix other);
	Matrix operator*(double other);

	Matrix operator/(Matrix other);
	Matrix operator/(double other);

	Matrix operator+(Matrix other);
	Matrix operator+(double other);

	Matrix operator-(Matrix other);
	Matrix operator-(double other);

	// matrix multiplication operator overload
	Matrix operator^(Matrix other);

	// index operator
	double& operator()(int x, int y = -1);

	// assignment operator
	void operator=(Matrix other);

	//returns the transposed matrix
	Matrix T();

	// returns size (x * y) if dim == 0, x if dim == 1, y if dim == 2)
	int size(int dim = 0);

	// prints the matrix
	void print();
};

#endif