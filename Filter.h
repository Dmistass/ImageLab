#pragma once
#include "MyString.h"

class Image;

class Filter {
public:
	static constexpr int MATRIX_SIZE = 3;     // matrix size
	using Matrix = double[MATRIX_SIZE][MATRIX_SIZE]; // type for matrix
protected:
	MyString name;      // filter name
	Matrix matrix;      // convolution matrix (for matrix filters)
	double normKff = 1; // normalization coefficient

public:
	// Constructor for filters without a matrix
	Filter(const MyString& name);
	// Constructor for matrix filters
	Filter(const MyString& name, const Matrix& matrix, double normKff = 1);
	virtual ~Filter() = default;

	Filter(const Filter&) = delete;
	Filter& operator=(const Filter&) = delete;
	Filter(Filter&&) = delete;
	Filter& operator=(Filter&&) = delete;

	const MyString& GetName() const;        // filter name
	const Matrix& GetMatrix() const;        // matrix
	double GetNormKff() const;              // normalization coefficient

	// Apply filter to an image (by default - does nothing)
	virtual Image* apply(Image* image) const;

};

// Filter based on a matrix
class MatrixFilter : public Filter {
public:
	MatrixFilter(const MyString& name, const Matrix& matrix, double normKff = 1);
	virtual Image* apply(Image* image) const override;
};

// Contrast filter (enhancement of the central pixel *1.5)
class ContrastFilter : public MatrixFilter {
public:
	static Matrix contrastMatrix;
	ContrastFilter();
};

// Blur filter
class BlurFilter : public MatrixFilter {
public:
	static Matrix blurMatrix;
	BlurFilter();
};

// Sharpen filter
class SharpenFilter : public MatrixFilter {
public:
	static Matrix sharpenMatrix;
	SharpenFilter();
};

// Contour detection filter
class ContourFilter : public MatrixFilter {
public:
	static Matrix contourMatrix;
	ContourFilter();
};

// Negative filter
class NegativeFilter : public Filter {
public:
	NegativeFilter();
	Image* apply(Image* image) const override;
};

// Threshold filter: if pixel brightness >= threshold -> white (1), otherwise black (0)
// Result - P4
class ThresholdFilter : public Filter {
	int threshold_;  // brightness threshold (0..255)
public:
	explicit ThresholdFilter(int threshold = 128);
	Image* apply(Image* image) const override;
};

// Contrast normalization filter
class ContrastNormalizationFilter : public Filter {
public:
	ContrastNormalizationFilter();
	Image* apply(Image* image) const override;
};
