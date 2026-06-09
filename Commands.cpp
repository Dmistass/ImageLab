#include <iostream>
#include "Commands.h"
#include "ImageList.h"


void loadImages(MyVector<MyString>& paths)
{
	for (size_t i = 0; i < paths.size(); i++)
	{
		ImageList::getInstance().add(paths[i]);
	}
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

void run(MyString& imageName)
{
	Image* image = ImageList::getInstance()[imageName];
	if (!image) {
		std::cout << "Image not found\n";
		return;
	}

	image->applyFilters();
}

void runAll()
{
	MyVector<MyString> images = ImageList::getInstance().getImageList();

	for (size_t i = 0; i < images.size(); i++)
	{
		run(images[i]);
	}
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

void saveImage(MyString& name, MyString& savePath)
{
	Image* image = ImageList::getInstance()[name];
	if (!image) {
		std::cout << "Image not found\n";
		return;
	}

	if (savePath.empty()) {
		image->save(image->getOutputName());
	}
	else {
		image->save(savePath);
	}
}

void showAllFilters()
{
	MyVector<MyString> images = ImageList::getInstance().getImageList();

	for (size_t i = 0; i < images.size(); i++)
	{
		showFilters(images[i]);
	}
}
