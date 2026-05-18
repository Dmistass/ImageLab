#include <iostream>
#include "MyString.h"
#include "Image.h"
#include "FileManager.h"

Image::Image(MyString& path): path(path), name(path) {

}

Image::~Image()
{
}

MyString Image::getName()
{
	return name;
}

void Image::addFilter(MyString& filter)
{
	filters.push_back(filter);
	std::cout << filter << " added to " << name << "\n";
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

Pixel::Pixel(unsigned char v) : r(v), g(v), b(v)
{
}

Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b)
{
}
