#pragma once
#include "MyString.h"
#include "MyVector.h"


class FileManager
{
private:
    FileManager();

public:
    FileManager(const FileManager&) = delete;
    void operator=(const FileManager&) = delete;

    static FileManager& getInstance();

    bool readToFileBuffer(const MyString& path, MyVector<char>& outBuffer);
    bool writeFromFileBuffer(const MyString& path, const MyVector<char>& inBuffer);
};

