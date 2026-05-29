#pragma once
#include "MyString.h"
#include "Image.h"

class Filter {
public:
	static const int MATRIX_SIZE = 3;
protected:
	MyString name;
	double matrix[MATRIX_SIZE][MATRIX_SIZE];
	double normKff = 1;

public:

	Filter(MyString name, const double matrix[MATRIX_SIZE][MATRIX_SIZE], double normKff = 1);

	virtual void apply(Image* image);

};


class ContrastFilter : public Filter {
public:
	const double contrastMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0},
	};
	ContrastFilter();
};

class BlurFilter : public Filter {
public:
	const double blurMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 1},
	};
	BlurFilter();
};