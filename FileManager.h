#pragma once
#include "MyString.h"
#include "MyVector.h"
#include "Image.h"


// Loading and saving images in NETpbm formats (P1-P6).
class FileManager
{
private:
    FileManager();

    // Reads the entire file into a buffer
    bool readToFileBuffer(const MyString& path, MyVector<char>& outBuffer);
    // Writes a buffer to a file
    bool writeFromFileBuffer(const MyString& path, const MyVector<char>& inBuffer);

    // Skips whitespace, newlines and comments
    void skipWhitespaceAndComments(const MyVector<char>& buffer, int& pos);
    
    // Reads a number from the buffer (skipping whitespace/comments)
    int readInt(const MyVector<char>& buffer, int& pos);

    // Parsers for each NETpbm format
    void parseP1(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP2(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP3(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP4(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP5(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP6(Image* image, const MyVector<char>& buffer, int& pos);

    // Saving in binary formats (P4, P5, P6)
    void saveP4(Image* image, MyVector<char>& outBuffer);
    void saveP5(Image* image, MyVector<char>& outBuffer);
    void saveP6(Image* image, MyVector<char>& outBuffer);

public:
    FileManager(const FileManager&) = delete;
    void operator=(const FileManager&) = delete;

    static FileManager& getInstance();

    bool load(Image* image);   // loads an image from its path
    bool save(Image* image, const MyString& savePath); // saves to a file
};