#pragma once
#include <map>
#include "MyString.h"
#include "MyVector.h"
#include "Filter.h"

class FilterFactory {
private:
	FilterFactory();
	~FilterFactory();

	static std::map<MyString, Filter*> filtersLibrary;  // name -> filter instance

public:
	FilterFactory(const FilterFactory&) = delete;
	FilterFactory& operator=(const FilterFactory&) = delete;

	static FilterFactory& getInstance();

	Filter* getByName(MyString name);                // get filter by name
	MyVector<MyString> getAvailableFilterNames();    // list of all available filter names
};
