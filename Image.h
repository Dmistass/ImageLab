#pragma once
#include "MyString.h"
#include "MyVector.h"
#include "Filter.h"
#include "FilterFactory.h"

struct Pixel {
	short r, g, b;
	Pixel();
	Pixel(short v);
	Pixel(short r, short g, short b);
};

class Image {
	friend class FileManager;

private:
	MyString name;
	MyString path;
	MyVector<Filter*> filters;
	MyVector<Pixel> pixels;
	int PType = 0;
	int width = 0;
	int height = 0;
	int maxVal = 255;

public:
	Image(MyString& path);
	~Image();

	MyString getName();
	bool isLoaded();

	bool load(MyString& imagePath);
	bool save(const MyString& savePath);

	void addFilter(MyString& filter);
	void removeFilter(int filterIndex);
	void showFilters();
};