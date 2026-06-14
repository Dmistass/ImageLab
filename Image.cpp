#include <iostream>
#include "MyString.h"
#include "Image.h"
#include "ImageList.h"
#include "FileManager.h"
#include "Filter.h"
#include "FilterFactory.h"

Image::Image(MyString& path): path(path), name(path), outputName(path) {
	load(path);
}

Image::Image(int width, int height) : width(width), height(height), pixels(static_cast<std::size_t>(width)* static_cast<std::size_t>(height))
{
}

Image::~Image()
{
}

MyString Image::getName()
{
	return name;
}

MyString Image::getOutputName()
{
	return outputName;
}

bool Image::isLoaded()
{
	return PType > 0 && width > 0 && height > 0;
}

bool Image::save(const MyString& savePath)
{
	return FileManager::getInstance().save(this, savePath);
}

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

bool Image::load(MyString& imagePath)
{
	path = imagePath;
	size_t pos = path.find_last_of("/\\");
	name = path.substr(pos + 1);
	outputName = name;

	if (ImageList::getInstance()[name]) {
		std::cout << "This image already exist!\n";
		return false;
	}

	// Загружаем данные через FileManager (NETpbm P1-P6)
	if (!FileManager::getInstance().load(this)) {
		std::cout << "Failed to load: " << path << "\n";
		return false;
	}

	return true;
}

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

void Image::applyFilters()
{
	// Разделяем outputName на имя без расширения и расширение
	MyString baseName = outputName;
	MyString extension;

	size_t dotPos = outputName.find_last_of('.');
	if (dotPos != MyString::npos) {
		extension = outputName.substr(dotPos);
		baseName = outputName.substr(0, dotPos);
	}

	for (auto filter : filters)
	{
		filter->apply(this);

		baseName += "_";
		baseName += filter->GetName();

		std::cout << "Filter " << filter->GetName() << " was apllied to " << name << "\n";
	}

	outputName = baseName + extension;
	filters.clear();
}


Pixel::Pixel() : r(0), g(0), b(0)
{
}

Pixel::Pixel(short v) : r(v), g(v), b(v)
{
}

Pixel::Pixel(short r, short g, short b) : r(r), g(g), b(b)
{
}
