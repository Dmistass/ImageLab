#include <iostream>
#include "MyString.h"
#include "Image.h"
#include "ImageList.h"
#include "FileManager.h"
#include "Filter.h"
#include "FilterFactory.h"

// Constructor from path immediately loads the image from file.
Image::Image(MyString& path): path(path), name(path), outputName(path) {
	load(path);
}

// Constructor from dimensions - creates an empty image
Image::Image(int width, int height) : width(width), height(height), pixels(static_cast<std::size_t>(width)* static_cast<std::size_t>(height))
{
}

Image::~Image()
{
}

// Returns the file name (example.ppm)
MyString Image::getName()
{
	return name;
}

// Name for saving
MyString Image::getOutputName()
{
	return outputName;
}

// Load check: if PType is set and dimensions are non-zero - consider it ok
bool Image::isLoaded()
{
	return PType > 0 && width > 0 && height > 0;
}

// Saves the image to a file via FileManager
bool Image::save(const MyString& savePath)
{
	return FileManager::getInstance().save(this, savePath);
}

// Adds a filter by name from FilterFactory
void Image::addFilter(MyString& filterName)
{
	Filter* filter = FilterFactory::getInstance().getByName(filterName);
	if (!filter) {
		std::cout << "Invalid filter!\n";
		return;
	}
	filters.push_back(filter);
	std::cout << filter->GetName() << " added to " << name << "\n";
}

// Loading image from file:
// 1. Save the full path into path
// 2. Extract the file name (after the last / or \)
// 3. Check if this name is already in the loaded list
// 4. Call FileManager::load(), which parses the NETpbm format
bool Image::load(MyString& imagePath)
{
	path = imagePath;
	size_t pos = path.find_last_of("/\\"); // find the last path separator
	name = path.substr(pos + 1); // extract the file name
	outputName = name;

	// Duplicate check
	if (ImageList::getInstance()[name]) {
		std::cout << "This image already exist!\n";
		return false;
	}

	// Load data via FileManager
	if (!FileManager::getInstance().load(this)) {
		std::cout << "Failed to load: " << path << "\n";
		return false;
	}

	return true;
}

// Removes a filter from the list by index from showFilters
void Image::removeFilter(int filterIndex)
{
	if (filterIndex > filters.size() - 1) {
		std::cout << "Invalid index!\n";
	}
	filters.erase(filters.begin() + filterIndex);

	std::cout << "Filter removed!\n";
}

void Image::showFilters()
{
	std::cout << name << " filters:\n";
	for (size_t i = 0; i < filters.size(); i++)
	{
		std::cout << " " << i << ". " << filters[i]->GetName() << "\n";
	}
}


int Image::GetWidth() const
{
	return width;
}

int Image::GetHeight() const
{
	return height;
}

std::size_t Image::GetPixelCount() const
{
	return pixels.size();
}

// Pixel access by coordinates (x, y)
Pixel& Image::GetPixel(int x, int y)
{
	return pixels[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)];
}

const Pixel& Image::GetPixel(int x, int y) const
{
	return pixels[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)];
}

MyVector<Pixel>& Image::GetPixels()
{
	return pixels;
}

const MyVector<Pixel>& Image::GetPixels() const
{
	return pixels;
}

int Image::GetPType() const
{
	return PType;
}

void Image::setPType(int type)
{
	PType = type;
}

// Applies all assigned filters in sequence.
// 1. Separate the file name from the extension
// 2. For each filter call filter->apply(this)
// 3. Add suffix _<FilterName> to the name
// 4. After all filters assemble outputName = baseName + extension and clear the filter list
void Image::applyFilters()
{
	// Split outputName into name without extension and extension
	MyString baseName = outputName;
	MyString extension;

	size_t dotPos = outputName.find_last_of('.');
	if (dotPos != MyString::npos) {
		extension = outputName.substr(dotPos);
		baseName = outputName.substr(0, dotPos);
	}

	for (auto filter : filters)
	{
		filter->apply(this);             // apply filter to the current image

		baseName += "_";
		baseName += filter->GetName();   // append suffix to the name

		std::cout << "Filter " << filter->GetName() << " was apllied to " << name << "\n";
	}

	outputName = baseName + extension;   // assemble the final name for saving
	filters.clear();                     // clear the filter list
}


// Default pixel constructor
Pixel::Pixel() : r(0), g(0), b(0)
{
}

// Constructor for grayscale/binary pixel - all channels equal
Pixel::Pixel(short v) : r(v), g(v), b(v)
{
}

// Full-color pixel (R, G, B)
Pixel::Pixel(short r, short g, short b) : r(r), g(g), b(b)
{
}
