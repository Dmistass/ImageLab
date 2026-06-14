#pragma once
#include "Image.h"
#include "MyVector.h"

// Stores a list of loaded images (singleton).
// Allows adding, removing, searching by name, and printing the list.
class ImageList {
private:
	MyVector<Image> images;  // all loaded images

	ImageList();
	~ImageList();

	// Find image by name (returns pointer or nullptr)
	Image* getByName(MyString& name);

	// Find index of image in the array (-1 if not found)
	int findImageIndex(MyString& name);

public:
	ImageList(const ImageList&) = delete;
	ImageList& operator=(const ImageList&) = delete;

	static ImageList& getInstance();

	Image* add(MyString& path);             // load and add an image
	void remove(MyString& name);            // remove an image by name
	void printAll();                        // print names of all loaded images
	MyVector<MyString> getImageList();      // get list of names
	Image* operator[](MyString& name);      // access by name
};

