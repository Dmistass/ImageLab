#include "Filter.h"
#include "Image.h"
#include <cmath>
#include <iostream>

// Clamps a channel value to the range [0, 255].
// Used after convolution, as the result may go out of bounds.
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

// Contrast matrix
Filter::Matrix ContrastFilter::contrastMatrix = {
	{0, 0, 0},
	{0, 1.5, 0},
	{0, 0, 0},
};

// Blur matrix
Filter::Matrix BlurFilter::blurMatrix = {
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1},
};

// Sharpen matrix
Filter::Matrix SharpenFilter::sharpenMatrix = {
    {0, -1, 0},
    {-1, 5, -1},
    {0, -1, 0},
};

// Contour matrix
Filter::Matrix ContourFilter::contourMatrix = {
    {0, -1, 0},
    {-1, 4, -1},
    {0, -1, 0},
};

const MyString& Filter::GetName() const
{
    return name;
}

// Base apply implementation
Image* Filter::apply(Image* image) const
{
	return image;
}

MatrixFilter::MatrixFilter(const MyString& name, const Matrix& matrix, double normKff)
	: Filter(name, matrix, normKff)
{
}

// Constructor for filters without a matrix (Negative, Threshold, ContrastNorm).
// Fills the matrix with zeros, but it is not used.
Filter::Filter(const MyString& name) : name(name), normKff(1.0)
{
    for (int i = 0; i < MATRIX_SIZE; ++i)
        for (int j = 0; j < MATRIX_SIZE; ++j)
            matrix[i][j] = 0.0;
}

// Constructor for matrix filters - copies the given matrix
Filter::Filter(const MyString& name, const Matrix& matrix, double normKff) : name(name), normKff(normKff)
{
	for (int i = 0; i < MATRIX_SIZE; ++i)
		for (int j = 0; j < MATRIX_SIZE; ++j)
			this->matrix[i][j] = matrix[i][j];
}

// Matrix filter application
// 1. Create a new image result
// 2. For each pixel (x, y) iterate through a 3x3 window around it
// 3. Edges are handled by clamping to the edge pixel
// 4. For each channel (R, G, B) compute weighted sum with matrix normalization
// 5. Clamp the result via clampChannel() to [0, 255]
// 6. Copy result back to image
Image* MatrixFilter::apply(Image* image) const
{
    if (image == nullptr)
    {
        return nullptr;
    }

    const int width = image->GetWidth();
    const int height = image->GetHeight();
    Image result(width, height);  // temporary image for the result

    const int offset = MATRIX_SIZE / 2;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double sumR = 0.0;
            double sumG = 0.0;
            double sumB = 0.0;

            // Iterate through 3x3 window around (x, y)
            for (int ky = 0; ky < MATRIX_SIZE; ++ky)
            {
                for (int kx = 0; kx < MATRIX_SIZE; ++kx)
                {
                    int sampleX = x + kx - offset;
                    int sampleY = y + ky - offset;

                    // Edge handling: if out of bounds - take the edge pixel
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
                    const double weight = matrix[ky][kx] * normKff;  // weight with normalization

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

    // Copy the result back to the original image
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

// Negative: new_val = 255 - old_val for each channel
NegativeFilter::NegativeFilter() : Filter("Negative") {}

// Apply negative: each channel is inverted (255 - value)
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

// Thresholding: if pixel brightness >= threshold - 1 (white), otherwise 0 (black).
// For color images first convert to grayscale using ITU-R BT.601 formula
ThresholdFilter::ThresholdFilter(int threshold) : Filter("Threshold"), threshold_(threshold) {}

Image* ThresholdFilter::apply(Image* image) const
{
    if (image == nullptr) return nullptr;

    int imagePType = image->GetPType();

    // If the image is already binary - do nothing
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
                gray = p.g;  // grayscale image - take any channel
            }
            else {
                // Color → convert to grayscale using ITU-R BT.601 standard
                gray = 0.299 * p.r + 0.587 * p.g + 0.114 * p.b;
            }

            int val = ((gray >= threshold_) ? 1 : 0);  // threshold comparison

            result.GetPixel(x, y) = Pixel(val);
        }
    }

    image->setPType(4);  // result - binary P4

    image->GetPixels() = result.GetPixels();
    return image;
}

// Contrast normalization
ContrastNormalizationFilter::ContrastNormalizationFilter() : Filter("ContrastNorm") {}

Image* ContrastNormalizationFilter::apply(Image* image) const
{
    if (image == nullptr) return nullptr;

    const int width = image->GetWidth();
    const int height = image->GetHeight();

    // Step 1: find global minimum and maximum across all three channels
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

    // Step 2: if the image is uniform - skip
    if (globalMin == globalMax) return image;

    // Step 3: scaling factor
    double scale = 255.0 / (globalMax - globalMin);
    Image result(width, height);

    // Step 4: apply normalization to each pixel
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