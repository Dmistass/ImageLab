#pragma once
#include "MyString.h"
#include "MyVector.h"

void loadImages(MyVector<MyString>& paths);

void removeImage(MyString& name);

void printAll();

void addFilter(MyString& imageName, MyString& filterName);

void removeFilter(MyString& imageName, int filterIndex);

void run(MyString& imageName);

void runAll();

void showFilters(MyString& name);

void showAllFilters();

void saveImage(MyString& name, MyString& savePath);
