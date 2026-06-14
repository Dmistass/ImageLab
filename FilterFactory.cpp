#include "FilterFactory.h"

// Static dictionary of all available filters
std::map<MyString, Filter*> FilterFactory::filtersLibrary = {
	{MyString("Blur"), new BlurFilter()},
	{MyString("Contrast"), new ContrastFilter()},
	{MyString("Sharpen"), new SharpenFilter()},
	{MyString("Negative"), new NegativeFilter()},
	{MyString("Threshold"), new ThresholdFilter()},
	{MyString("ContrastNorm"), new ContrastNormalizationFilter()},
	{MyString("Contour"), new ContourFilter()},
};

FilterFactory::FilterFactory()
{
}

// Destructor - frees memory of all filters in the dictionary
FilterFactory::~FilterFactory()
{
	for (auto filter : filtersLibrary) {
		delete filter.second;
	}
}

FilterFactory& FilterFactory::getInstance()
{
	static FilterFactory instance;
	return instance;
}

// Returns filter by name
Filter* FilterFactory::getByName(MyString name)
{
	return filtersLibrary[name];
}

// Returns the list of names of all available filters
MyVector<MyString> FilterFactory::getAvailableFilterNames()
{
	MyVector<MyString> names;
	for (const auto& pair : filtersLibrary) {
		names.push_back(pair.first);
	}
	return names;
}
