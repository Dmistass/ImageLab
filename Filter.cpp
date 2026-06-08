#include "Filter.h"
#include "Image.h"
#include <cmath>
#include <iostream>

int clampChannel(double value)
{
    if (value < 0.0)
    {
        return 0;
    }
    if (value > 255.0)
    {
        return 255;
    }
    return static_cast<int>(std::lround(value));
}

Filter::Matrix ContrastFilter::contrastMatrix = {
	{0, 0, 0},
	{0, 1.5, 0},
	{0, 0, 0},
};

Filter::Matrix BlurFilter::blurMatrix = {
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1},
};

Filter::Matrix SharpenFilter::sharpenMatrix = {
    {0, -1, 0},
    {-1, 5, -1},
    {0, -1, 0},
};

Filter::Matrix ContourFilter::contourMatrix = {
    {0, -1, 0},
    {-1, 4, -1},
    {0, -1, 0},
};

const MyString& Filter::GetName() const
{
    return name;
}

Image* Filter::apply(Image* image) const
{
	return image;
}

MatrixFilter::MatrixFilter(const MyString& name, const Matrix& matrix, double normKff)
	: Filter(name, matrix, normKff)
{
}

// Конструктор для фильтров без матрицы
Filter::Filter(const MyString& name) : name(name), normKff(1.0)
{
    for (int i = 0; i < MATRIX_SIZE; ++i)
        for (int j = 0; j < MATRIX_SIZE; ++j)
            matrix[i][j] = 0.0;
}

Filter::Filter(const MyString& name, const Matrix& matrix, double normKff) : name(name), normKff(normKff)
{
	for (int i = 0; i < MATRIX_SIZE; ++i)
		for (int j = 0; j < MATRIX_SIZE; ++j)
			this->matrix[i][j] = matrix[i][j];
}

Image* MatrixFilter::apply(Image* image) const
{
    if (image == nullptr)
    {
        return nullptr;
    }

    const int width = image->GetWidth();
    const int height = image->GetHeight();
    Image result(width, height);

    const int offset = MATRIX_SIZE / 2;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double sumR = 0.0;
            double sumG = 0.0;
            double sumB = 0.0;

            for (int ky = 0; ky < MATRIX_SIZE; ++ky)
            {
                for (int kx = 0; kx < MATRIX_SIZE; ++kx)
                {
                    int sampleX = x + kx - offset;
                    int sampleY = y + ky - offset;

                    if (sampleX < 0)
                    {
                        sampleX = 0;
                    }
                    else if (sampleX >= width)
                    {
                        sampleX = width - 1;
                    }

                    if (sampleY < 0)
                    {
                        sampleY = 0;
                    }
                    else if (sampleY >= height)
                    {
                        sampleY = height - 1;
                    }

                    const Pixel& pixel = image->GetPixel(sampleX, sampleY);
                    const double weight = matrix[ky][kx] * normKff;

                    sumR += pixel.r * weight;
                    sumG += pixel.g * weight;
                    sumB += pixel.b * weight;
                }
            }

            result.GetPixel(x, y) = Pixel(
                clampChannel(sumR),
                clampChannel(sumG),
                clampChannel(sumB));
        }
    }

    image->GetPixels() = result.GetPixels();
    return image;
}

ContrastFilter::ContrastFilter() : MatrixFilter("Contrast", contrastMatrix)
{
}

BlurFilter::BlurFilter() : MatrixFilter("Blur", blurMatrix, 1. / 9)
{
}

SharpenFilter::SharpenFilter() : MatrixFilter("Sharpen", sharpenMatrix)
{
}

ContourFilter::ContourFilter() : MatrixFilter("Contour", contourMatrix)
{
}

// ================= NEGATIVE =================
NegativeFilter::NegativeFilter() : Filter("Negative") {}

Image* NegativeFilter::apply(Image* image) const
{
    if (image == nullptr) return nullptr;

    const int width = image->GetWidth();
    const int height = image->GetHeight();
    Image result(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& p = image->GetPixel(x, y);
            result.GetPixel(x, y) = Pixel(
                255 - p.r,
                255 - p.g,
                255 - p.b
            );
        }
    }

    image->GetPixels() = result.GetPixels();
    return image;
}

// ================= THRESHOLD =================
ThresholdFilter::ThresholdFilter(int threshold) : Filter("Threshold"), threshold_(threshold) {}

Image* ThresholdFilter::apply(Image* image) const
{
    if (image == nullptr) return nullptr;

    int imagePType = image->GetPType();

    if (imagePType == 1 || imagePType == 4) return image;

    const int width = image->GetWidth();
    const int height = image->GetHeight();
    Image result(width, height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const Pixel& p = image->GetPixel(x, y);

            short gray;
            if (imagePType == 2 || imagePType == 5) {
                gray = p.g;
            }
            else {
               // Переводим в оттенки серого (стандарт ITU-R BT.601)
               gray = 0.299 * p.r + 0.587 * p.g + 0.114 * p.b;
            }

            int val = ((gray >= threshold_) ? 1 : 0);
            //std::cout << gray << " >= " << threshold_ << " - " << val << "\n";

            result.GetPixel(x, y) = Pixel(val);
        }
    }

    image->setPType(4);

    image->GetPixels() = result.GetPixels();
    return image;
}

// ================= CONTRAST NORMALIZATION =================
ContrastNormalizationFilter::ContrastNormalizationFilter() : Filter("ContrastNorm") {}

Image* ContrastNormalizationFilter::apply(Image* image) const
{
    if (image == nullptr) return nullptr;

    const int width = image->GetWidth();
    const int height = image->GetHeight();

    
    int globalMin = 255, globalMax = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& p = image->GetPixel(x, y);
            if (p.r < globalMin) globalMin = p.r;
            if (p.g < globalMin) globalMin = p.g;
            if (p.b < globalMin) globalMin = p.b;

            if (p.r > globalMax) globalMax = p.r;
            if (p.g > globalMax) globalMax = p.g;
            if (p.b > globalMax) globalMax = p.b;
        }
    }

    
    if (globalMin == globalMax) return image;

    
    double scale = 255.0 / (globalMax - globalMin);
    Image result(width, height);

    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& p = image->GetPixel(x, y);
            result.GetPixel(x, y) = Pixel(
                clampChannel((p.r - globalMin) * scale),
                clampChannel((p.g - globalMin) * scale),
                clampChannel((p.b - globalMin) * scale)
            );
        }
    }

    image->GetPixels() = result.GetPixels();
    return image;
}