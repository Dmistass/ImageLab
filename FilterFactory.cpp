#include "FilterFactory.h"

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

Filter* FilterFactory::getByName(MyString name)
{
	return filtersLibrary[name];
}
