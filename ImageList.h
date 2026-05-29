#pragma once
#include "Image.h"
#include "MyVector.h"

class ImageList {
private:
	MyVector<Image> images;

	ImageList();
	~ImageList();

	Image* getByName(MyString& name);

	int findImageIndex(MyString& name);

public:
	ImageList(const ImageList&) = delete;
	ImageList& operator=(const ImageList&) = delete;

	static ImageList& getInstance();

	Image* add(MyString& path);

	void remove(MyString& name);

	void printAll();

	MyVector<MyString> getImageList();

	Image* operator[](MyString& name);
};

