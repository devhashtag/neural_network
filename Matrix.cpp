#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iomanip>

#include "Matrix.h"

using namespace std;

// constructor
Matrix::Matrix(int x, int y, int randomize) {
	if (x < 0 || y < 0)
		throw "Matrix dimensions cannot be negative";

	// set the dimensions
	this->x = x;
	this->y = y;

	// reserve enough memory in the vector
	values.reserve(this->x * this->y);

	// fill in the vector with doubles ranging from -1.0 to 1.0, or just 1.0
	if (randomize)
		while (values.size() < values.capacity())
			values.push_back(rand() / (double)(RAND_MAX / 2) - 1.0);
	else
		while (values.size() < values.capacity())
			values.push_back(1);
}

// assignment, deep copy, no pointers to the same address
void Matrix::operator=(Matrix other) {
	x = other.x;
	y = other.y;
	values = other.values;
}

// implementation of the operators, very straightforward
Matrix Matrix::operator*(Matrix other) {
	if (other.x != x || other.y != y)
		throw "Matrix dimensions do not match";

	Matrix ret = *this;
	for (int i = values.size() - 1; i >= 0; i--) {
		ret(i) = (*this)(i) * other(i);
	}

	return ret;
}

Matrix Matrix::operator*(double other) {
	Matrix ret = *this;
	for (int i = values.size() - 1; i >= 0; i--)
		ret(i) = ret(i) * other;

	return ret;
}

Matrix Matrix::operator/(Matrix other) {
	if (other.x != x || other.y != y)
		throw "Matrix dimensions do not match";

	Matrix ret = *this;
	for (int i = values.size() - 1; i >= 0; i--) {
		cout << "i :" << i << endl;
		ret(i) = (*this)(i) / other(i);
	}

	return ret;
}

Matrix Matrix::operator/(double other) {
	Matrix ret = *this;
	for (int i = values.size() - 1; i >= 0; i--)
		ret(i) = ret(i) / other;

	return ret;
}

Matrix Matrix::operator+(Matrix other) {
	if (other.x != x || other.y != y)
		throw "Matrix dimensions do not match";

	Matrix ret = *this;
	for (int i = values.size() - 1; i >= 0; i--) {
		ret(i) = (*this)(i) + other(i);
	}

	return ret;
}

Matrix Matrix::operator+(double other) {
	Matrix ret = *this;
	for (int i = values.size() - 1; i >= 0; i--)
		ret(i) = ret(i) + other;

	return ret;
}

Matrix Matrix::operator-(Matrix other) {
	if (other.x != x || other.y != y)
		throw "Matrix dimensions do not match";

	Matrix ret = *this;
	for (int i = values.size() - 1; i >= 0; i--)
		ret(i) = (*this)(i) - other(i);

	return ret;
}

Matrix Matrix::operator-(double other) {
	Matrix ret = *this;
	for (int i = values.size() - 1; i >= 0; i--)
		ret(i) = ret(i) - other;

	return ret;
}

Matrix Matrix::operator^(Matrix other) {
	if (x != other.y)
		throw "Matrices cannot be multiplied, a.x != b.y";

	Matrix ret = Matrix(other.x, y, false);

	for (int i = 0; i < ret.y; i++)
		for (int j = 0; j < ret.x; j++) {
			double dot = 0.0;
			for (int k = 0; k < x; k++) {
				dot += ((*this)(k, i) * other(j, k));
			}
			ret(j, i)= dot;
		}
		return ret;
}

// acces array through matrix(x, y), where y = optional
double& Matrix::operator()(int x, int y) {
	if (x < 0 || x >= (int)values.size()) {
		cout << "index bounds error" << endl;
		throw "index out of bounds";
	}

	if (y == -1)
		return values[x];
	if (x + y * this->x >= (int)values.size())
		throw "index out of bounds";

	return values[x + y * this->x];
}

Matrix Matrix::T() {
	Matrix ret = Matrix(y, x, false);


	for (int i = 0; i < x; i++)
		for (int j =  0; j < y; j++)
			ret(j, i) = (*this)(i, j);

	return ret;
}

int Matrix::size(int dim) {
	return dim == 0 ? x * y : dim == 1 ? x : y;
}

void Matrix::print() {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++)
			cout << setw(12) << (*this)(j, i);
		cout << endl;
	}
}