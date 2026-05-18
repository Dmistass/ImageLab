#include "FileManager.h"
#include <fstream>
#include "MyString.h"
#include "MyVector.h"

FileManager::FileManager()
{
}

FileManager& FileManager::getInstance()
{
    static FileManager instance;
    return instance;
}

bool FileManager::readToFileBuffer(const MyString& path, MyVector<char>& outBuffer)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    auto size = file.tellg();
    outBuffer.resize(static_cast<size_t>(size));
    file.seekg(0, std::ios::beg);
    return file.read(outBuffer.data(), size).good();
}

bool FileManager::writeFromFileBuffer(const MyString& path, const MyVector<char>& inBuffer) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    file.write(inBuffer.data(), inBuffer.size());
    file.close(); // —брасываем буфер и ловим ошибки записи
    return !file.fail(); // true, если записалось успешно
}
