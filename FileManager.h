#pragma once
#include "MyString.h"
#include "MyVector.h"
#include "Image.h"


class FileManager
{
private:
    FileManager();

    bool readToFileBuffer(const MyString& path, MyVector<char>& outBuffer);
    bool writeFromFileBuffer(const MyString& path, const MyVector<char>& inBuffer);

    // Пропускает пробелы, переносы строк и комментарии (#)
    void skipWhitespaceAndComments(const MyVector<char>& buffer, int& pos);
    
    // Читает число из буфера (пропуская пробелы/комментарии)
    int readInt(const MyVector<char>& buffer, int& pos);

    // Парсеры для каждого формата NETpbm
    void parseP1(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP2(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP3(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP4(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP5(Image* image, const MyVector<char>& buffer, int& pos);
    void parseP6(Image* image, const MyVector<char>& buffer, int& pos);

    // Сохранение в бинарные форматы (P4, P5, P6)
    void saveP4(Image* image, MyVector<char>& outBuffer);
    void saveP5(Image* image, MyVector<char>& outBuffer);
    void saveP6(Image* image, MyVector<char>& outBuffer);

public:
    FileManager(const FileManager&) = delete;
    void operator=(const FileManager&) = delete;

    static FileManager& getInstance();

    bool load(Image* image);
    bool save(Image* image, const MyString& savePath);
};