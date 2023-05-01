#include <iostream>		// for standard i/o
#include <iomanip>		// for setw()
#include <tchar.h>		// for t datatype
#include "Matrix.h"

// Matrix class functions
Matrix::Matrix(char* fileName) {
	char line[MAX_LINE], *tok, *remain;
	FILE* fp;

	if (fopen_s(&fp, fileName, "r") != 0) {
		std::wcout << _T("Error: file open\n");
		exit(0);
	}
	else {
		this->row = 0;
		while (fgets(line, MAX_LINE, fp)) {
			this->array.push_back(std::vector<int>());
			tok = strtok_s(line, DELIM, &remain);
			do {
				this->array[this->row].push_back(atoi(tok));
			} while (tok = strtok_s(NULL, DELIM, &remain));
			this->row++;
		}
		this->col = this->array[0].size();
		fclose(fp);
	}
}
Matrix::Matrix(int row, int col) {
	this->row = row;
	this->col = col;
	for (int i = 0; i < row; i++) {
		this->array.push_back(std::vector<int>());
		for (int j = 0; j < col; j++)
			this->array[i].push_back(0);
	}
}
Matrix::Matrix(): Matrix(0, 0) {};
// get matrix information
int Matrix::getRow() {
	return this->row;
}
int Matrix::getCol() {
	return this->col;
}
int Matrix::getValue(int i, int j) {
	return this->array[i][j];
}
void Matrix::printDim() {
	std::wcout << _T("(  ") << std::setw(3) << this->row << _T("x");
	std::cout << std::setw(3) << std::left << this->col;
	std::wcout << _T("  )");
}
void Matrix::printAll() {
	for (int i = 0; i < this->row; i++) {
		for (int j = 0; j < this->col; j++) {
			std::cout << std::setw(3) << this->array[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
// change matrix
void Matrix::updateValue(int i, int j, int val) {
	this->array[i][j] = val;
}
void Matrix::clear() {
	for (int i = 0; i < this->row; i++) {
		for (int j = 0; j < this->col; j++)
			this->array[i].clear();
	}
	this->array.clear();
	this->row = 0;
	this->col = 0;
}
void Matrix::change(char* fileName) {
	char line[MAX_LINE], *tok, *remain;
	FILE* fp;

	if (fopen_s(&fp, fileName, "r") != 0) {
		std::wcout << _T("Error: file open\n");
		exit(0);
	}
	else {
		this->clear();
		while (fgets(line, MAX_LINE, fp)) {
			this->array.push_back(std::vector<int>());
			tok = strtok_s(line, DELIM, &remain);
			do {
				this->array[this->row].push_back(atoi(tok));
			} while (tok = strtok_s(NULL, DELIM, &remain));
			this->row++;
		}
		this->col = this->array[0].size();
		fclose(fp);
	}

}
void Matrix::copy(Matrix *src) {
	this->clear();
	this->row = src->row;
	this->col = src->col;
	for (int i = 0; i < this->row; i++) {
		this->array.push_back(std::vector<int>());
		for (int j = 0; j < this->col; j++)
			this->array[i].push_back(src->array[i][j]);
	}
}
void Matrix::setNewRowCol(int row, int col) {
	this->clear();
	this->row = row;
	this->col = col;
	for (int i = 0; i < this->row; i++) {
		this->array.push_back(std::vector<int>());
		for (int j = 0; j < this->col; j++)
			this->array[i].push_back(0);
	}
}
// save matrix
void Matrix::saveMatrixToFile(const char* fileName) {
	FILE* fp;
	if (fopen_s(&fp, fileName, "w") != 0) {
		std::wcout << _T("Error: file open\n");
	}
	else {
		for (int i = 0; i < this->row; i++) {
			for (int j = 0; j < this->col; j++) 
				fprintf(fp, "%10d ", this->array[i][j]);
			fprintf(fp, "\n");
		}
		fclose(fp);
	}
}

// MultiMatrix class functions
MultiMatrix::MultiMatrix(Matrix *A, Matrix *B, Matrix *C) {
	this->A = A;
	this->B = B;
	this->C = C;
}
// for calculate multiple matrix
void MultiMatrix::printMultiDim() {
	this->A->printDim();
	std::wcout << _T(" X ");
	this->B->printDim();
	std::wcout << _T(" = ");
	this->C->printDim();
	std::cout << std::endl;
}
// get pointer of matrix
Matrix* MultiMatrix::getA() {
	return A;
}
Matrix* MultiMatrix::getB() {
	return B;
}
Matrix* MultiMatrix::getC() {
	return C;
}