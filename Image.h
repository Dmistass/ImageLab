#pragma once
#include "MyString.h"
#include "MyVector.h"
#include "Filter.h"
#include "FilterFactory.h"

// Pixel structure - stores 3 channels (R, G, B) from 0 to 255.
// For grayscale images all three channels are equal (r == g == b).
// For binary (P1/P4) values are 0 or 1.
struct Pixel {
	short r, g, b;
	Pixel();
	Pixel(short v);            // fills all 3 channels with the same value
	Pixel(short r, short g, short b);
};

class Image {
	friend class FileManager;  // FileManager has access to private fields for loading/saving

private:
	MyString name;        // file name (without path)
	MyString path;        // full path to the file
	MyString outputName;  // output file name (changes after applyFilters)
	MyVector<Filter*> filters;  // list of filters to apply
	MyVector<Pixel> pixels;     // pixel array (row-major: first all X of row Y, then Y+1)
	int PType = 0;        // NETpbm type: 1..6
	int width = 0;        // width in pixels
	int height = 0;       // height in pixels
	int maxVal = 255;     // maximum channel value (from file header)

public:
	// Loads image from file
	Image(MyString& path);
	// Creates an empty image of the given size
	Image(int width, int height);
	~Image();

	MyString getName();      // returns the file name
	MyString getOutputName();// returns the name for saving (after applyFilters - with suffixes)
	bool isLoaded();         // checks if the image is loaded (PType > 0 and dimensions > 0)

	bool load(MyString& imagePath);  // loads from file (via FileManager)
	bool save(const MyString& savePath); // saves to file

	int GetWidth() const;
	int GetHeight() const;
	std::size_t GetPixelCount() const;        // total number of pixels (width * height)
	Pixel& GetPixel(int x, int y);            // pixel access by coordinates
	const Pixel& GetPixel(int x, int y) const;// pixel access by coordinates
	MyVector<Pixel>& GetPixels();             // entire pixel array (for Filter::apply)
	const MyVector<Pixel>& GetPixels() const;
	int GetPType() const;                     // NETpbm image type
	void setPType(int type);                  // changes NETpbm type

	// Working with filters
	void addFilter(MyString& filterName);    // add filter by name
	void removeFilter(int filterIndex);      // remove filter by index
	void showFilters();                      // show filter list for this image
	void applyFilters();                     // apply all filters in sequence
};