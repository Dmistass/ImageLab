#pragma once
#include "Image.h"
#include "MyVector.h"

class ImageList {
private:
	MyImageVector images;

	ImageList();
	~ImageList();

	Image getByName(MyString& name);

public:
	ImageList(const ImageList&) = delete;
	ImageList& operator=(const ImageList&) = delete;

	static ImageList& getInstance();

	ImageList& operator[](MyString& name);

	const ImageList& operator[](MyString& name) const;
};

