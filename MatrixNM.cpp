#ifndef MATRIX_NM_CPP
#define MATRIX_NM_CPP

#include "MatrixNM.h"

//param constructor
template<typename T>
MatrixNM<T>::MatrixNM(unsigned trows, unsigned tcols, const T& tinit){
	mat.resize(trows);
	for(unsigned i=0; i<mat.size(); i+){
		mat[i].resize(tcols,tinit);
	}
	rows = trows;
	cols = tcols;
}

//copy constructor
template<typename T>
MatrixNM<T>::MatrixNM(const MatrixNM<T>& rhs){
	mat = rhs.mat;
	rows = rhs.getRows();
	cols = rhs.getRols();
}

//virtual destructor
template<typename T>
MatrixNM<T>::~MatrixNM(){
}

//assignment operator
template<typename T>
MatrixNM<T>& MatrixNM<T>::operator=(const MatrixNM<T>& rhs){
	if(&rhs == this)
		return this;

	unsigned newRows = rhs.getRows();
	unsigned newCols = rhs.getCols();

	mat.resize(newRows);
	for(unsigned i = 0; i<mat.size();i++){
		mat[i].resize(newCols);
	}

	for(unsigned i=0; i<newRows; i++){
		for(unsigned j=0;j<newCols;j++){
			mat[i][j] = rhs(i,j);
		}
	}
	rows=newRows;
	cols=newCols;

	return *this;
}

//addition of two matrices
template<typename T>
MatrixNM<T> MatrixNM<T>::operator+(const MatrixNM<T>& rhs){
	MatrixNM result(rows,cols,0.f);

	for(unsigned i=0; i<rows;i++){
		for(unsigned j=0;j<cols;j++){
			result(i,j) = this->mat[i][j] + rhs(i,j);
		}
	}
	return result;
}

//cumulative addition +=
template<typename T>
MatrixNM<T>& MatrixNM<T>::operator+=(const MatrixNM<T>& rhs){
	unsigned rows = rhs.getRows();
	unsigned cols = rhs.getCols();

	for(unsigned i=0; i<rows;i++){
		for(unsigned j=0;j<cols;j++){
			this->mat[i][j] += rhs(i,j);
		}
	}
	return *this;
}

//subtraction 
template<typename T>
MatrixNM<T> MatrixNM<T>::operator-(const MatrixNM<T>& rhs){
	unsigned rows = rhs.getRows();
	unsigned cols = rhs.getCols();
	MatrixNM result(rows,cols,0.f);

	for(unsigned i=0;i<rows;i++){
		for(unsigned j=0;j<cols;j++){
			result(i,j) = this->mat[i][j] - rhs(i,j);
		}
	}
	return result;
}

//cumulative subtraction -=
template<typename T>
MatrixNM<T>& MatrixNM<T>::operator-=(const MatrixNM<T>& rhs){
	unsigned rows = rhs.getRows();
	unsigned cols = rhs.getCols();

	for(unsigned i=0; i<rows;i++){
		for(unsigned j=0;j<cols;j++){
			this->mat[i][j] -= rhs(i,j);
		}
	}
	return *this;
}

//left multiplication
template<typename T>
MatrixNM<T> MatrixNM<T>::operator*(const MatrixNM<T>& rhs) {
  unsigned rows = rhs.getRows();
  unsigned cols = rhs.getCols();
  MatrixNM result(rows, cols, 0.0);

  for (unsigned i=0; i<rows; i++) {
    for (unsigned j=0; j<cols; j++) {
      for (unsigned k=0; k<rows; k++) {
        result(i,j) += this->mat[i][k] * rhs(k,j);
      }
    }
  }

  return result;
}

//cumulative left multiplication
template<typename T>
MatrixNM<T>& MatrixNM<T>::operator*=(const MatrixNM<T>& rhs) {
  MatrixNM result = (*this) * rhs;
  (*this) = result;
  return *this;
}

//transpose
template<typename T>
MatrixNM<T> MatrixNM<T>::transpose() {
  MatrixNM result(rows, cols, 0.0);

  for (unsigned i=0; i<rows; i++) {
    for (unsigned j=0; j<cols; j++) {
      result(i,j) = this->mat[j][i];
    }
  }

  return result;
}

// scalar addition                                                                                                                                                     
template<typename T>
MatrixNM<T> MatrixNM<T>::operator+(const T& rhs) {
  MatrixNM result(rows, cols, 0.0);

  for (unsigned i=0; i<rows; i++) {
    for (unsigned j=0; j<cols; j++) {
      result(i,j) = this->mat[i][j] + rhs;
    }
  }

  return result;
}

// scalar subtraction                                                                                                                                                  
template<typename T>
MatrixNM<T> MatrixNM<T>::operator-(const T& rhs) {
  MatrixNM result(rows, cols, 0.0);

  for (unsigned i=0; i<rows; i++) {
    for (unsigned j=0; j<cols; j++) {
      result(i,j) = this->mat[i][j] - rhs;
    }
  }

  return result;
}

// scalar multiplication                                                                                                                                               
template<typename T>
MatrixNM<T> MatrixNM<T>::operator*(const T& rhs) {
  MatrixNM result(rows, cols, 0.0);

  for (unsigned i=0; i<rows; i++) {
    for (unsigned j=0; j<cols; j++) {
      result(i,j) = this->mat[i][j] * rhs;
    }
  }

  return result;
}

// scalar division                                                                                                                                                     
template<typename T>
MatrixNM<T> MatrixNM<T>::operator/(const T& rhs) {
  MatrixNM result(rows, cols, 0.0);

  for (unsigned i=0; i<rows; i++) {
    for (unsigned j=0; j<cols; j++) {
      result(i,j) = this->mat[i][j] / rhs;
    }
  }

  return result;
}

// Multiply a matrix with a vector                                                                                                                                            
template<typename T>
std::vector<T> MatrixNM<T>::operator*(const std::vector<T>& rhs) {
  std::vector<T> result(rhs.size(), 0.0);

  for (unsigned i=0; i<rows; i++) {
    for (unsigned j=0; j<cols; j++) {
      result[i] = this->mat[i][j] * rhs[j];
    }
  }

  return result;
}

// Obtain a vector of the diagonal elements                                                                                                                                   
template<typename T>
std::vector<T> MatrixNM<T>::diag_vec() {
  std::vector<T> result(rows, 0.0);

  for (unsigned i=0; i<rows; i++) {
    result[i] = this->mat[i][i];
  }

  return result;
}

// Access the individual elements                                                                                                                                             
template<typename T>
T& MatrixNM<T>::operator()(const unsigned& row, const unsigned& col) {
  return this->mat[row][col];
}

// Access the individual elements (const)                                                                                                                                     
template<typename T>
const T& MatrixNM<T>::operator()(const unsigned& row, const unsigned& col) const {
  return this->mat[row][col];
}

// Get the number of rows of the matrix                                                                                                                                       
template<typename T>
unsigned MatrixNM<T>::getRows() const {
  return this->rows;
}

// Get the number of columns of the matrix                                                                                                                                    
template<typename T>
unsigned MatrixNM<T>::getCols() const {
  return this->cols;
}

#endif