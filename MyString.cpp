#define _CRT_SECURE_NO_WARNINGS
#include "MyString.h"
#include <cstring>
#include <istream>

// Removes old buffer and creates a new one, copying str into it.
void MyString::assign(const char* str)
{
    delete[] data_;

    if (str) {
        size_ = std::strlen(str);
        data_ = new char[size_ + 1];
        std::strcpy(data_, str);
    }
    else {
        size_ = 0;
        data_ = new char[1];
        data_[0] = '\0';
    }
}

// Helper method for concatenating two strings
char* MyString::concat(const char* s1, size_t len1, const char* s2, size_t len2)
{
    char* buf = new char[len1 + len2 + 1];
    std::strcpy(buf, s1);
    std::strcat(buf, s2);
    return buf;
}

// Default constructor
MyString::MyString()
    : data_(new char[1])
    , size_(0)
{
    data_[0] = '\0';
}

// Constructor from string
MyString::MyString(const char* str)
    : data_(nullptr)
    , size_(0)
{
    assign(str);
}

// Copy constructor
// Simply copy the contents
MyString::MyString(const MyString& other)
    : data_(new char[other.size_ + 1])
    , size_(other.size_)
{
    std::strcpy(data_, other.data_);
}

// Destructor
MyString::~MyString()
{
    delete[] data_;
}

// Copy assignment
MyString& MyString::operator=(const MyString& other)
{
    if (this != &other) {
        assign(other.data_);
    }
    return *this;
}

// Move constructor
MyString::MyString(MyString&& other) noexcept
    : data_(other.data_)
    , size_(other.size_)
{
    other.data_ = nullptr;
    other.size_ = 0;
}

// Move assignment
// Free our old buffer, take data from other, nullify other
MyString& MyString::operator=(MyString&& other) noexcept
{
    if (this != &other) {
        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;

        other.data_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

const char* MyString::c_str() const
{
    return data_;
}

bool MyString::empty() const
{
    return size_ == 0;
}

// Returns the length of the string without '\0'
size_t MyString::size() const
{
    return size_;
}

// Finds the last occurrence of any character from chars
size_t MyString::find_last_of(const char* chars) const
{
    if (!chars || !*chars) return npos;

    size_t charsLen = std::strlen(chars);

    for (size_t i = size_; i > 0; i--) { // run from end to beginning
        for (size_t j = 0; j < charsLen; j++) {
            if (data_[i - 1] == chars[j]) {
                return i - 1; // found - return index
            }
        }
    }

    return npos; // didn't find anything
}

// Same, but looking for one specific character
size_t MyString::find_last_of(char c) const
{
    for (size_t i = size_; i > 0; i--) {
        if (data_[i - 1] == c) {
            return i - 1;
        }
    }
    return npos;
}

// Lexicographic comparison of strings (needed for std::map<MyString, ...>)
bool operator<(const MyString& lhs, const MyString& rhs)
{
    return std::strcmp(lhs.data_, rhs.data_) < 0;
}

// ===== substr =====
// Extracts a substring starting at pos with length count.
// If pos is out of bounds - return an empty string.
MyString MyString::substr(size_t pos, size_t count) const
{
    if (pos >= size_) return MyString(); // out of bounds

    size_t realCount = count;
    if (realCount == npos || pos + realCount > size_) {
        realCount = size_ - pos; // trim to end of string
    }

    char* buf = new char[realCount + 1];
    for (size_t i = 0; i < realCount; i++) {
        buf[i] = data_[pos + i]; // copy character by character
    }
    buf[realCount] = '\0';

    MyString result(buf);
    delete[] buf; // delete temporary buffer
    return result;
}

// ===== operator== (MyString, MyString) =====
bool operator==(const MyString& lhs, const MyString& rhs)
{
    return std::strcmp(lhs.data_, rhs.data_) == 0;
}

// ===== operator== (MyString, const char*) =====
bool operator==(const MyString& lhs, const char* rhs)
{
    return std::strcmp(lhs.data_, rhs) == 0;
}

// ===== operator== (const char*, MyString) =====
bool operator==(const char* lhs, const MyString& rhs)
{
    return std::strcmp(lhs, rhs.data_) == 0;
}


// Appends a string to the end.
// Uses concat() to join old and new strings in a new buffer.
MyString& MyString::operator+=(const char* rhs)
{
    if (!rhs || !*rhs) return *this;

    size_t rhsLen = std::strlen(rhs);
    char* buf = concat(data_, size_, rhs, rhsLen);

    delete[] data_;
    data_ = buf;
    size_ += rhsLen;

    return *this;
}


// Reuses += for const char*, passing data_ from rhs
MyString& MyString::operator+=(const MyString& rhs)
{
    return *this += rhs.data_;
}


// Creates a new string = lhs + rhs.
// Allocate buffer via concat(), then change data_ and size_ of result.
MyString operator+(const MyString& lhs, const char* rhs)
{
    size_t rhsLen = (rhs ? std::strlen(rhs) : 0);
    char* buf = MyString::concat(lhs.data_, lhs.size_, rhs ? rhs : "", rhsLen);

    MyString result;
    delete[] result.data_; // delete empty buffer created by constructor
    result.data_ = buf;
    result.size_ = lhs.size_ + rhsLen;

    return result;
}

// ===== operator+ (MyString, MyString) =====
MyString operator+(const MyString& lhs, const MyString& rhs)
{
    return lhs + rhs.data_;
}

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
    os << str.data_;
    return os;
}


std::istream& operator>>(std::istream& is, MyString& str)
{
    // Clear the string
    delete[] str.data_;
    str.data_ = new char[1];
    str.data_[0] = '\0';
    str.size_ = 0;

    // Skip whitespace
    char c;
    while (is.get(c)) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
    }

    if (!is) return is;

    // Read word until whitespace
    size_t capacity = 32;
    char* buf = new char[capacity];
    size_t len = 0;

    buf[len++] = c;

    while (is.get(c)) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            is.putback(c);
            break;
        }

        if (len + 1 >= capacity) {
            capacity *= 2;
            char* newBuf = new char[capacity];
            for (size_t i = 0; i < len; i++) newBuf[i] = buf[i];
            delete[] buf;
            buf = newBuf;
        }

        buf[len++] = c;
    }

    buf[len] = '\0';

    delete[] str.data_;
    str.data_ = buf;
    str.size_ = len;

    // Reset failbit/eofbit, since at least one token was successfully read
    is.clear();

    return is;
}


std::istream& getline(std::istream& is, MyString& str, char delim)
{
    // Clear the string
    delete[] str.data_;
    str.data_ = new char[1];
    str.data_[0] = '\0';
    str.size_ = 0;

    size_t capacity = 64;
    char* buf = new char[capacity];
    size_t len = 0;

    char c;
    while (is.get(c)) {
        if (c == delim) break;

        if (len + 1 >= capacity) {
            capacity *= 2;
            char* newBuf = new char[capacity];
            for (size_t i = 0; i < len; i++) newBuf[i] = buf[i];
            delete[] buf;
            buf = newBuf;
        }

        buf[len++] = c;
    }

    // Remove trailing \r
    while (len > 0 && (buf[len - 1] == '\r' || buf[len - 1] == '\n')) {
        len--;
    }

    buf[len] = '\0';

    delete[] str.data_;
    str.data_ = buf;
    str.size_ = len;

    return is;
}
