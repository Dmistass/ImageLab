#include "FilterFactory.h"

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