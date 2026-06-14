#include "FileManager.h"
#include <fstream>
#include <iostream>
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

// Reads the entire file into a binary buffer
bool FileManager::readToFileBuffer(const MyString& path, MyVector<char>& outBuffer)
{
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error: could not open file: " << path << "\n";
        return false;
    }

    // Get file size - seek to the end and read position
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);  // return to the beginning

    if (size <= 0) {
        std::cout << "Error: empty file\n";
        return false;
    }

    outBuffer.resize(static_cast<size_t>(size));  // allocate memory
    file.read(outBuffer.data(), size);            // read everything at once

    if (!file.good()) {
        std::cout << "Error: failed to read file\n";
        return false;
    }

    file.close();
    return true;
}

// Writes a buffer to a file.
bool FileManager::writeFromFileBuffer(const MyString& path, const MyVector<char>& inBuffer)
{
    std::ofstream file(path.c_str(), std::ios::binary);
    if (!file.is_open()) return false;

    file.write(inBuffer.data(), inBuffer.size());
    file.close();
    return !file.fail();
}

// Skips spaces, tabs, newlines and comments
void FileManager::skipWhitespaceAndComments(const MyVector<char>& buffer, int& pos)
{
    while (pos < (int)buffer.size()) {
        if (buffer[pos] == ' ' || buffer[pos] == '\t' || buffer[pos] == '\n' || buffer[pos] == '\r') {
            pos++;
            continue;
        }
        if (buffer[pos] == '#') {
            // comment - skip everything until \n
            while (pos < (int)buffer.size() && buffer[pos] != '\n') {
                pos++;
            }
            continue;
        }
        break;  // found a meaningful character
    }
}

// Reads an integer from the buffer, first skipping whitespace/comments.
// Used for reading width, height, maxVal and pixel data in ASCII formats.
int FileManager::readInt(const MyVector<char>& buffer, int& pos)
{
    skipWhitespaceAndComments(buffer, pos);

    int result = 0;
    bool negative = false;

    if (pos < (int)buffer.size() && buffer[pos] == '-') {
        negative = true;
        pos++;
    }

    if (pos >= (int)buffer.size() || !std::isdigit(buffer[pos])) {
        std::cout << "Error: expected number at position " << pos << "\n";
        return -1;
    }

    // Build the number character by character: '5' -> 5, '3' -> 53, etc.
    while (pos < (int)buffer.size() && std::isdigit(buffer[pos])) {
        result = result * 10 + (buffer[pos] - '0');
        pos++;
    }

    return negative ? -result : result;
}

// Loading an image from a file
// 1. Read the entire file into a buffer
// 2. Find the type "P1".."P6" after possible comments
// 3. Determine PType and call the corresponding parser
bool FileManager::load(Image* image)
{
    if (!image) {
        std::cout << "Error: image pointer is null\n";
        return false;
    }

    MyVector<char> buffer;
    if (!readToFileBuffer(image->path, buffer)) {
        return false;
    }

    if (buffer.size() < 2) {
        std::cout << "Error: file too small to be NETpbm\n";
        return false;
    }

    // Skip comments and whitespace before the type
    int pos = 0;
    skipWhitespaceAndComments(buffer, pos);

    // Read the "magic number" (P1, P2, ..., P6)
    if (pos + 1 >= (int)buffer.size() || buffer[pos] != 'P' || buffer[pos + 1] < '1' || buffer[pos + 1] > '6') {
        std::cout << "Error: not a NETpbm file (expected P1-P6)\n";
        return false;
    }

    image->PType = buffer[pos + 1] - '0';
    pos += 2;

    // Parse depending on P type
    switch (image->PType) {
        case 1: parseP1(image, buffer, pos); break;
        case 2: parseP2(image, buffer, pos); break;
        case 3: parseP3(image, buffer, pos); break;
        case 4: parseP4(image, buffer, pos); break;
        case 5: parseP5(image, buffer, pos); break;
        case 6: parseP6(image, buffer, pos); break;
        default:
            std::cout << "Error: unknown P-type: P" << image->PType << "\n";
            return false;
    }

    std::cout << "Loaded: " << image->getName() << " (P" << image->PType
              << ", " << image->width << "x" << image->height << ")\n";
    return true;
}

// ---- Saving ----
// Any image is saved in binary format (P4, P5, P6)
static void writeIntToBuffer(int val, MyVector<char>& buf) {
    if (val == 0) { buf.push_back('0'); return; }
    char digits[16];
    int len = 0;
    while (val > 0) {
        digits[len++] = '0' + (val % 10);
        val /= 10;
    }
    for (int i = len - 1; i >= 0; i--) buf.push_back(digits[i]);  // reverse
}

// Saves the image to a file. Automatically selects format:
// P4 for P1/P4, P5 for P2/P5, P6 for P3/P6
bool FileManager::save(Image* image, const MyString& savePath)
{
    if (!image || !image->isLoaded()) {
        std::cout << "Error: image not loaded, cannot save\n";
        return false;
    }

    MyVector<char> outBuffer;
    int imagePType = image->GetPType();

    if (imagePType == 1 || imagePType == 4) {
        // Black-and-white → P4
        image->setPType(4);
        saveP4(image, outBuffer);
    }
    else if (imagePType == 2 || imagePType == 5) {
        // Grayscale → P5
        image->setPType(5);
        saveP5(image, outBuffer);
    }
    else {
        // Color → P6
        image->setPType(6);
        saveP6(image, outBuffer);
    }

    if (!writeFromFileBuffer(savePath, outBuffer)) {
        std::cout << "Error: failed to write file: " << savePath << "\n";
        return false;
    }

    std::cout << "Saved: " << savePath << " (P" << image->PType
              << ", " << image->width << "x" << image->height << ")\n";
    return true;
}

// Saves in P4 (binary black-and-white).
// Each row is packed: 8 pixels -> 1 byte.
// MSB of the byte = leftmost pixel
void FileManager::saveP4(Image* image, MyVector<char>& outBuffer)
{
    // Header: "P4\nwidth height\n"
    outBuffer.push_back('P');
    outBuffer.push_back('4');
    outBuffer.push_back('\n');

    writeIntToBuffer(image->width, outBuffer);
    outBuffer.push_back(' ');
    writeIntToBuffer(image->height, outBuffer);
    outBuffer.push_back('\n');

    int bytesPerRow = (image->width + 7) / 8;

    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < bytesPerRow; x++) {
            unsigned char byteVal = 0;
            for (int bit = 0; bit < 8; bit++) {
                int pixelX = x * 8 + bit;
                if (pixelX < image->width) {
                    int pixelIndex = y * image->width + pixelX;
                    if (image->pixels[pixelIndex].g == 1) {
                        byteVal |= (1 << (7 - bit));
                    }
                }
            }
            outBuffer.push_back(byteVal);
        }
    }
}

// Saves in P5 (binary grayscale).
// Each pixel - 1 byte (grayscale value).
void FileManager::saveP5(Image* image, MyVector<char>& outBuffer)
{
    // Header: "P5\nwidth height\n255\n"
    outBuffer.push_back('P');
    outBuffer.push_back('5');
    outBuffer.push_back('\n');

    writeIntToBuffer(image->width, outBuffer);
    outBuffer.push_back(' ');
    writeIntToBuffer(image->height, outBuffer);
    outBuffer.push_back('\n');

    outBuffer.push_back('2');
    outBuffer.push_back('5');
    outBuffer.push_back('5');
    outBuffer.push_back('\n');

    int totalPixels = image->width * image->height;
    for (int i = 0; i < totalPixels; i++) {
        unsigned char gray = static_cast<unsigned char>(image->pixels[i].r);
        outBuffer.push_back(gray);
    }
}

// Saves in P6 (binary full-color RGB).
// Each pixel - 3 consecutive bytes: R, G, B.
void FileManager::saveP6(Image* image, MyVector<char>& outBuffer)
{
    // Header: "P6\nwidth height\n255\n"
    outBuffer.push_back('P');
    outBuffer.push_back('6');
    outBuffer.push_back('\n');

    writeIntToBuffer(image->width, outBuffer);
    outBuffer.push_back(' ');
    writeIntToBuffer(image->height, outBuffer);
    outBuffer.push_back('\n');

    outBuffer.push_back('2');
    outBuffer.push_back('5');
    outBuffer.push_back('5');
    outBuffer.push_back('\n');

    int totalPixels = image->width * image->height;
    for (int i = 0; i < totalPixels; i++) {
        outBuffer.push_back(static_cast<unsigned char>(image->pixels[i].r));
        outBuffer.push_back(static_cast<unsigned char>(image->pixels[i].g));
        outBuffer.push_back(static_cast<unsigned char>(image->pixels[i].b));
    }
}

// ---- Parsers for loading ----

// P1: ASCII black-and-white (0 = white, 1 = black).
// Read width, height, then characters '0'/'1' separated by whitespace
void FileManager::parseP1(Image* image, const MyVector<char>& buffer, int& pos)
{
    image->width = readInt(buffer, pos);
    image->height = readInt(buffer, pos);
    image->maxVal = 1;

    image->pixels.clear();
    image->pixels.reserve(image->width * image->height);

    int pixelCount = 0;
    int totalPixels = image->width * image->height;

    // Read characters, skipping whitespace, until we have totalPixels
    while (pixelCount < totalPixels && pos < (int)buffer.size()) {
        skipWhitespaceAndComments(buffer, pos);
        if (pos >= (int)buffer.size()) break;

        if (buffer[pos] == '0' || buffer[pos] == '1') {
            short gray = buffer[pos] - '0';
            image->pixels.push_back(Pixel(gray));
            pos++;
            pixelCount++;
        } else {
            pos++;  // skip garbage characters
        }
    }

    if (pixelCount != totalPixels) {
        std::cout << "Warning: P1 file ended early (" << pixelCount
                  << "/" << totalPixels << " pixels)\n";
    }
}

// P2: ASCII grayscale.
// Read width, height, maxVal. Then read numbers via readInt().
// Each number - grayscale value from 0 to maxVal.
// Scale to 0..255: gray = val * 255 / maxVal.
void FileManager::parseP2(Image* image, const MyVector<char>& buffer, int& pos)
{
    image->width = readInt(buffer, pos);
    image->height = readInt(buffer, pos);
    image->maxVal = readInt(buffer, pos);  // maximum value in the file (may not be 255)

    if (image->maxVal <= 0) {
        std::cout << "Error: invalid maxVal for P2\n";
        return;
    }

    image->pixels.clear();
    image->pixels.reserve(image->width * image->height);

    int pixelCount = 0;
    int totalPixels = image->width * image->height;

    while (pixelCount < totalPixels && pos < (int)buffer.size()) {
        int val = readInt(buffer, pos);
        if (val < 0) break;

        // Scale: if file stores 0..65535, then val * 255 / maxVal gives 0..255
        short gray = static_cast<short>(val * 255 / image->maxVal);
        image->pixels.push_back(Pixel(gray));
        pixelCount++;
    }

    if (pixelCount != totalPixels) {
        std::cout << "Warning: P2 file ended early (" << pixelCount
                  << "/" << totalPixels << " pixels)\n";
    }
}

// P3: ASCII color (RGB).
// Read width, height, maxVal. Then read R, G, B.
// Each channel is scaled to 0..255
void FileManager::parseP3(Image* image, const MyVector<char>& buffer, int& pos)
{
    image->width = readInt(buffer, pos);
    image->height = readInt(buffer, pos);
    image->maxVal = readInt(buffer, pos);

    if (image->maxVal <= 0) {
        std::cout << "Error: invalid maxVal for P3\n";
        return;
    }

    image->pixels.clear();
    image->pixels.reserve(image->width * image->height);

    int pixelCount = 0;
    int totalPixels = image->width * image->height;

    // Read 3 numbers (R, G, B) per pixel
    while (pixelCount < totalPixels && pos < (int)buffer.size()) {
        int r = readInt(buffer, pos);
        if (r < 0) break;
        int g = readInt(buffer, pos);
        if (g < 0) break;
        int b = readInt(buffer, pos);
        if (b < 0) break;

        short red   = static_cast<short>(r * 255 / image->maxVal);
        short green = static_cast<short>(g * 255 / image->maxVal);
        short blue  = static_cast<short>(b * 255 / image->maxVal);

        image->pixels.push_back(Pixel(red, green, blue));
        pixelCount++;
    }

    if (pixelCount != totalPixels) {
        std::cout << "Warning: P3 file ended early (" << pixelCount
                  << "/" << totalPixels << " pixels)\n";
    }
}

// P4: Binary black-and-white (1 bit per pixel).
// After the header comes binary data: each row is packed into ceil(width/8) bytes.
// MSB of the first byte = leftmost pixel of the row.
void FileManager::parseP4(Image* image, const MyVector<char>& buffer, int& pos)
{
    image->width = readInt(buffer, pos);
    image->height = readInt(buffer, pos);
    image->maxVal = 1;

    image->pixels.clear();
    image->pixels.reserve(image->width * image->height);

    skipWhitespaceAndComments(buffer, pos);

    int bytesPerRow = (image->width + 7) / 8;  // how many bytes per row

    // Go through all rows and pixels, unpacking bits
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            int byteIndex = pos + x / 8; // which byte the pixel is in
            int bitIndex = 7 - (x % 8); // which bit (MSB = 7, LSB = 0)

            if (byteIndex >= (int)buffer.size()) {
                std::cout << "Warning: P4 file ended early at row " << y << "\n";
                return;
            }

            short gray = (buffer[byteIndex] >> bitIndex) & 1;  // extract bit
            image->pixels.push_back(Pixel(gray));
        }
        pos += bytesPerRow;  // move to the next row
    }
}

// P5: Binary grayscale.
// After the header comes binary data:
// - if maxVal <= 255 - 1 byte per pixel
// - if maxVal > 255 - 2 bytes per pixel
void FileManager::parseP5(Image* image, const MyVector<char>& buffer, int& pos)
{
    image->width = readInt(buffer, pos);
    image->height = readInt(buffer, pos);
    image->maxVal = readInt(buffer, pos);

    if (image->maxVal <= 0) {
        std::cout << "Error: invalid maxVal for P5\n";
        return;
    }

    image->pixels.clear();
    image->pixels.reserve(image->width * image->height);

    skipWhitespaceAndComments(buffer, pos);

    int totalPixels = image->width * image->height;

    if (image->maxVal <= 255) {
        for (int i = 0; i < totalPixels; i++) {
            if (pos >= (int)buffer.size()) {
                std::cout << "Warning: P5 file ended early (" << i
                          << "/" << totalPixels << ")\n";
                return;
            }
            short gray = static_cast<short>((unsigned char)buffer[pos] * 255 / image->maxVal);
            image->pixels.push_back(Pixel(gray));
            pos++;
        }
    } else {
        for (int i = 0; i < totalPixels; i++) {
            if (pos + 1 >= (int)buffer.size()) {
                std::cout << "Warning: P5 file ended early (" << i
                          << "/" << totalPixels << ")\n";
                return;
            }
            int val = ((unsigned char)buffer[pos] << 8) | (unsigned char)buffer[pos + 1];
            short gray = static_cast<short>(val * 255 / image->maxVal);
            image->pixels.push_back(Pixel(gray));
            pos += 2;
        }
    }
}

// P6: Binary color (RGB)
// - if maxVal <= 255 - 3 bytes per pixel (R, G, B)
// - if maxVal > 255 - 6 bytes per pixel
void FileManager::parseP6(Image* image, const MyVector<char>& buffer, int& pos)
{
    image->width = readInt(buffer, pos);
    image->height = readInt(buffer, pos);
    image->maxVal = readInt(buffer, pos);

    if (image->maxVal <= 0) {
        std::cout << "Error: invalid maxVal for P6\n";
        return;
    }

    image->pixels.clear();
    image->pixels.reserve(image->width * image->height);

    skipWhitespaceAndComments(buffer, pos);

    int totalPixels = image->width * image->height;

    if (image->maxVal <= 255) {
        for (int i = 0; i < totalPixels; i++) {
            if (pos + 2 >= (int)buffer.size()) {
                std::cout << "Warning: P6 file ended early (" << i
                          << "/" << totalPixels << ")\n";
                return;
            }
            short r = static_cast<short>((unsigned char)buffer[pos] * 255 / image->maxVal);
            short g = static_cast<short>((unsigned char)buffer[pos + 1] * 255 / image->maxVal);
            short b = static_cast<short>((unsigned char)buffer[pos + 2] * 255 / image->maxVal);
            image->pixels.push_back(Pixel(r, g, b));
            pos += 3;
        }
    } else {
        for (int i = 0; i < totalPixels; i++) {
            if (pos + 5 >= (int)buffer.size()) {
                std::cout << "Warning: P6 file ended early (" << i
                          << "/" << totalPixels << ")\n";
                return;
            }
            int r = ((unsigned char)buffer[pos] << 8) | (unsigned char)buffer[pos + 1];
            int g = ((unsigned char)buffer[pos + 2] << 8) | (unsigned char)buffer[pos + 3];
            int b = ((unsigned char)buffer[pos + 4] << 8) | (unsigned char)buffer[pos + 5];
            image->pixels.push_back(Pixel(
                static_cast<short>(r * 255 / image->maxVal),
                static_cast<short>(g * 255 / image->maxVal),
                static_cast<short>(b * 255 / image->maxVal)
            ));
            pos += 6;
        }
    }
}