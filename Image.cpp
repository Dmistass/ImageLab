#include <iostream>
#include "MyString.h"
#include "Image.h"
#include "ImageList.h"
#include "FileManager.h"
#include <algorithm>
#include "Filter.h"
#include "FilterFactory.h"

Image::Image(MyString& path): path(path), name(path) {
	load(path);
}

Image::~Image()
{
}

MyString Image::getName()
{
	return name;
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
	std::cout << filter << " added to " << name << "\n";
}

bool Image::load(MyString& imagePath)
{
	path = imagePath;
	size_t pos = path.find_last_of("/\\");
	name = path.substr(pos + 1).c_str();

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
		std::cout << " " << i << ". " << filters[i] << "\n";
	}
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
