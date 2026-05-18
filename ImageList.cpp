#include <iostream>
#include "ImageList.h"

ImageList::ImageList()
{
}

ImageList::~ImageList() {
}

Image* ImageList::getByName(MyString& name)
{
	int i = findImageIndex(name);
	if (i >= 0) {
		return &images[i];
	}

	return nullptr;
}

int ImageList::findImageIndex(MyString& name)
{
	for (size_t i = 0; i < images.size(); i++)
	{
		if (images[i].getName() == name)
		{
			return i;
		}
	}

	return -1;
}

ImageList& ImageList::getInstance() {
	static ImageList instance;
	return instance;
}

Image* ImageList::add(MyString& path)
{
	Image* image = new Image(path);

	images.push_back(*image);

	std::cout << "Added: " << images[images.size() - 1].getName() << "\n";
	
	return image;
}

void ImageList::remove(MyString& name)
{
	int index = findImageIndex(name);
	if (index < 0) {
		std::cout << "Image doesnt exist!\n";
		return;
	}

	images.erase(images.begin() + index);
	
	std::cout << "Image removed!\n";
}

void ImageList::printAll()
{
	for (size_t i = 0; i < images.size(); i++)
	{
		std::cout << images[i].getName() << "\n";
	}
}

Image* ImageList::operator[](MyString& name)
{
	return getByName(name);
}

