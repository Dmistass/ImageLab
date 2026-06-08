#pragma once
#include <map>
#include "MyString.h"
#include "Filter.h"

class FilterFactory {
private:
	FilterFactory();
	~FilterFactory();

	static std::map<MyString, Filter*> filtersLibrary;

public:
	FilterFactory(const FilterFactory&) = delete;
	FilterFactory& operator=(const FilterFactory&) = delete;

	static FilterFactory& getInstance();

	Filter* getByName(MyString name);
};
