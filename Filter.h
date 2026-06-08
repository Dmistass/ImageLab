#pragma once
#include "MyString.h"

class Image;

class Filter {
public:
	static constexpr int MATRIX_SIZE = 3;
	using Matrix = double[MATRIX_SIZE][MATRIX_SIZE];
protected:
	MyString name;
	Matrix matrix;
	double normKff = 1;

public:
	Filter(const MyString& name);
	Filter(const MyString& name, const Matrix& matrix, double normKff = 1);
	virtual ~Filter() = default;

	Filter(const Filter&) = delete;
	Filter& operator=(const Filter&) = delete;
	Filter(Filter&&) = delete;
	Filter& operator=(Filter&&) = delete;

	const MyString& GetName() const;
	const Matrix& GetMatrix() const;
	double GetNormKff() const;

	virtual Image* apply(Image* image) const;

};

class MatrixFilter : public Filter {
public:
	MatrixFilter(const MyString& name, const Matrix& matrix, double normKff = 1);
	virtual Image* apply(Image* image) const override;
};

class ContrastFilter : public MatrixFilter {
public:
	static Matrix contrastMatrix;
	ContrastFilter();
};

class BlurFilter : public MatrixFilter {
public:
	static Matrix blurMatrix;
	BlurFilter();
};

class SharpenFilter : public MatrixFilter {
public:
	static Matrix sharpenMatrix;
	SharpenFilter();
};

class ContourFilter : public MatrixFilter {
public:
	static Matrix contourMatrix;
	ContourFilter();
};

class NegativeFilter : public Filter {
public:
	NegativeFilter();
	Image* apply(Image* image) const override;
};

class ThresholdFilter : public Filter {
	int threshold_;
public:
	explicit ThresholdFilter(int threshold = 128);
	Image* apply(Image* image) const override;
};

class ContrastNormalizationFilter : public Filter {
public:
	ContrastNormalizationFilter();
	Image* apply(Image* image) const override;
};