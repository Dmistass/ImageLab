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
	MyString outputName;
	MyVector<Filter*> filters;
	MyVector<Pixel> pixels;
	int PType = 0;
	int width = 0;
	int height = 0;
	int maxVal = 255;

public:
	Image(MyString& path);
	Image(int width, int height);
	~Image();

	MyString getName();
	MyString getOutputName();
	bool isLoaded();

	bool load(MyString& imagePath);
	bool save(const MyString& savePath);

	int GetWidth() const;
	int GetHeight() const;
	std::size_t GetPixelCount() const;
	Pixel& GetPixel(int x, int y);
	const Pixel& GetPixel(int x, int y) const;
	MyVector<Pixel>& GetPixels();
	const MyVector<Pixel>& GetPixels() const;
	int GetPType() const;
	void setPType(int type);

	void addFilter(MyString& filter);
	void removeFilter(int filterIndex);
	void showFilters();
	void applyFilters();
};