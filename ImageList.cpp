#include "ImageList.h"

ImageList::~ImageList() {
}

Image ImageList::getByName(MyString& name)
{
}

ImageList& ImageList::getInstance() {
	static ImageList instance;
	return instance;
}

ImageList& ImageList::operator[](MyString& name)
{
	// TODO: вставьте здесь оператор return
}

const ImageList& ImageList::operator[](MyString& name) const
{
	// TODO: вставьте здесь оператор return
}
