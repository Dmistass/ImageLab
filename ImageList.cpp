#include <iostream>
#include "ImageList.h"

ImageList::ImageList()
{
}

ImageList::~ImageList() {
}

// Finds image by name via findImageIndex, returns pointer or nullptr
Image* ImageList::getByName(MyString& name)
{
	int i = findImageIndex(name);
	if (i >= 0) {
		return &images[i];
	}

	return nullptr;
}

// Find index of an image
int ImageList::findImageIndex(MyString& name)
{
	for (size_t i = 0; i < images.size(); i++)
	{
		if (images[i].getName() == name)
		{
			return i; // found - return index
		}
	}

	return -1; // not found
}

// Singleton
ImageList& ImageList::getInstance() {
	static ImageList instance;
	return instance;
}

Image* ImageList::add(MyString& path)
{
	Image* image = new Image(path);

	// If loading failed
	if (!image->isLoaded()) {
		std::cout << "Failed to add: " << image->getName() << "\n";
		delete image;
		return nullptr;
	}

	images.push_back(*image); // copy into vector

	std::cout << "Added: " << images[images.size() - 1].getName() << "\n";
	
	return &images[images.size() - 1];
}

// Removes image by name: finds index, erases from vector
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

// Prints names of all loaded images
void ImageList::printAll()
{
	for (size_t i = 0; i < images.size(); i++)
	{
		std::cout << images[i].getName() << "\n";
	}
}

// Returns a vector of names of all loaded images
MyVector<MyString> ImageList::getImageList()
{
	MyVector<MyString> imagesName;
	for (size_t i = 0; i < images.size(); i++)
	{
		imagesName.push_back(images[i].getName());
	}

	return imagesName;
}

// Access operator by name
Image* ImageList::operator[](MyString& name)
{
	return getByName(name);
}

