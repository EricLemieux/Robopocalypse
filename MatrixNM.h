#ifndef MATRIX_NM_H
#define MATRIX_NM_H

#include <vector>
#include "Vector.h"

template<typename T> class MatrixNM{
private:
	std::vector<std::vector<T>>mat;
	unsigned rows;
	unsigned cols;
public:
	MatrixNM(unsigned trows, unsigned tcols, const T& tinit);
	MatrixNM(const MatrixNM<T>& rhs);
	virtual ~MatrixNM();

	//overload operators
	MatrixNM<T>& operator=(const MatrixNM<T>&rhs);

	//math operations
	MatrixNM<T> operator+(const MatrixNM<T>& rhs);
	MatrixNM<T>& operator+=(const MatrixNM<T>& rhs);
	MatrixNM<T> operator-(const MatrixNM<T>& rhs);
	MatrixNM<T>& operator-=(const MatrixNM<T>& rhs);
	MatrixNM<T> operator*(const MatrixNM<T>& rhs);
	MatrixNM<T>& operator*=(const MatrixNM<T>& rhs);
	MatrixNM<T> transpose();

	//scalar operations
	MatrixNM<T> operator+(const T& rhs);
	MatrixNM<T> operator-(const T& rhs);
	MatrixNM<T> operator*(const T& rhs);
	MatrixNM<T> operator/(const T& rhs);

	//vector operations
	std::vector<T> operator*(const std::vector<T>& rhs);
	std::vector<T> diag_vec();

	//get individual elements
	T& operator()(const unsigned& row, const unsigned& col);
	const T& operator()(const unsigned& row, const unsigned& col) const;

	//row and column getter
	unsigned getRows() const;
	unsigned getCols() const;
};

#include "MatrixNM.cpp"

#endif