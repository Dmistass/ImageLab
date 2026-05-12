#pragma once
#include "MyString.h"
#include "Image.h"
#include <vector>

class MyDoubleVector : public std::vector<double> {

};


class MyStringVector : public std::vector<MyString> {

};

class MyImageVector : public std::vector<Image> {

};