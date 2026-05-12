#pragma once
#include "MyString.h"
#include "MyVector.h"

class Image {
private:
	MyString path;
	MyString name;

public:
	Image(MyString& path);
	~Image();
};