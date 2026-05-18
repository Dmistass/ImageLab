#include <iostream>
#include "Commands.h"
#include "ImageList.h"


void loadImage(MyString& path)
{
	ImageList::getInstance().add(path);
}

void removeImage(MyString& name)
{
	ImageList::getInstance().remove(name);
}

void printAll() {
	ImageList::getInstance().printAll();
}

void addFilter(MyString& imageName, MyString& filterName)
{
	Image* image = ImageList::getInstance()[imageName];
	if (!image) {
		std::cout << "Image not found\n";
		return;
	}

	image->addFilter(filterName);
}

void removeFilter(MyString& imageName, int filterIndex)
{
	Image* image = ImageList::getInstance()[imageName];
	if (!image) {
		std::cout << "Image not found\n";
		return;
	}

	image->removeFilter(filterIndex);
}

void showFilters(MyString& name)
{
	Image* image = ImageList::getInstance()[name];
	if (!image) {
		std::cout << "Image not found\n";
		return;
	}

	image->showFilters();
}
