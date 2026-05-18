#pragma once
#include "MyString.h"
#include "MyVector.h"

struct Pixel {
	unsigned char r, g, b;
	Pixel();
	Pixel(unsigned char v);
	Pixel(unsigned char r, unsigned char g, unsigned char b);
};

class Image {
private:
	MyString name;
	MyString path;
	MyVector<MyString> filters;
	MyVector<Pixel> pixels;
	int width = 0;
	int height = 0;
	int maxVal = 255;

public:
	Image(MyString& path);
	~Image();

	MyString getName();

	void addFilter(MyString& filter);
	void removeFilter(int filterIndex);
	void showFilters();
};