#pragma once
#include "MyString.h"

void loadImage(MyString& path);

void removeImage(MyString& name);

void printAll();

void addFilter(MyString& imageName, MyString& filterName);

void removeFilter(MyString& imageName, int filterIndex);

void showFilters(MyString& name);