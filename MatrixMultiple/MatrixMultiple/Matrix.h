#pragma once
#ifndef _MATRIX
#define _MATRIX

#include <vector>		// for vector STL

#define MAX_ROW 100
#define MAX_COL 100
#define MAX_LINE 1000
#define DELIM	" \r\t"

class Matrix {
	std::vector<std::vector<int>> array;		// 2 dimension array
	int row, col;								// row, col information
public:
	Matrix(char* fileName);
	Matrix(int row, int col);
	Matrix();

	// get matrix information
	int getRow();								// get row
	int getCol();								// get col
	int getValue(int i, int j);					// get value using index
	void printDim();							// print matrix row and col
	void printAll();							// print matrix for testing
	
	// change matrix
	void updateValue(int i, int j, int val);	// update value using index
	void clear();								// make array to empty array
	void change(char* fileName);				// change array using file
	void copy(Matrix *src);						// copy array to another array
	void setNewRowCol(int row, int col);		// for calculating matrix multiple

	// save matrix
	void saveMatrixToFile(const char *fileName); // save martix to file 
};

class MultiMatrix {
	Matrix *A, *B, *C;							// src1, src2, dest
public:
	MultiMatrix(Matrix *A, Matrix *B, Matrix *C);
	
	// for calculate multiple matrix
	void printMultiDim();

	// get pointer of matrix
	Matrix* getA();
	Matrix* getB();
	Matrix* getC();
};

#endif
