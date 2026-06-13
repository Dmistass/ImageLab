#define _CRT_SECURE_NO_WARNINGS
#include "MyString.h"
#include <cstring>
#include <istream>

// ===== Приватные вспомогательные функции =====

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

char* MyString::concat(const char* s1, size_t len1, const char* s2, size_t len2)
{
    char* buf = new char[len1 + len2 + 1];
    std::strcpy(buf, s1);
    std::strcat(buf, s2);
    return buf;
}

// ===== Конструктор по умолчанию =====
MyString::MyString()
    : data_(new char[1])
    , size_(0)
{
    data_[0] = '\0';
}

// ===== Конструктор от C-строки =====
MyString::MyString(const char* str)
    : data_(nullptr)
    , size_(0)
{
    assign(str);
}

// ===== Конструктор копирования =====
MyString::MyString(const MyString& other)
    : data_(new char[other.size_ + 1])
    , size_(other.size_)
{
    std::strcpy(data_, other.data_);
}

// ===== Деструктор =====
MyString::~MyString()
{
    delete[] data_;
}

// ===== Копирующее присваивание =====
MyString& MyString::operator=(const MyString& other)
{
    if (this != &other) {
        assign(other.data_);
    }
    return *this;
}

// ===== Перемещающий конструктор =====
MyString::MyString(MyString&& other) noexcept
    : data_(other.data_)
    , size_(other.size_)
{
    other.data_ = nullptr;
    other.size_ = 0;
}

// ===== Перемещающее присваивание =====
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

// ===== c_str() =====
const char* MyString::c_str() const
{
    return data_;
}

// ===== empty() =====
bool MyString::empty() const
{
    return size_ == 0;
}

// ===== size() =====
size_t MyString::size() const
{
    return size_;
}

// ===== find_last_of (const char*) =====
size_t MyString::find_last_of(const char* chars) const
{
    if (!chars || !*chars) return npos;

    size_t charsLen = std::strlen(chars);

    for (size_t i = size_; i > 0; i--) {
        for (size_t j = 0; j < charsLen; j++) {
            if (data_[i - 1] == chars[j]) {
                return i - 1;
            }
        }
    }

    return npos;
}

// ===== find_last_of (char) =====
size_t MyString::find_last_of(char c) const
{
    for (size_t i = size_; i > 0; i--) {
        if (data_[i - 1] == c) {
            return i - 1;
        }
    }
    return npos;
}

// ===== operator< =====
bool operator<(const MyString& lhs, const MyString& rhs)
{
    return std::strcmp(lhs.data_, rhs.data_) < 0;
}

// ===== substr =====
MyString MyString::substr(size_t pos, size_t count) const
{
    if (pos >= size_) return MyString();

    size_t realCount = count;
    if (realCount == npos || pos + realCount > size_) {
        realCount = size_ - pos;
    }

    char* buf = new char[realCount + 1];
    for (size_t i = 0; i < realCount; i++) {
        buf[i] = data_[pos + i];
    }
    buf[realCount] = '\0';

    MyString result(buf);
    delete[] buf;
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

// ===== operator+= (const char*) =====
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

// ===== operator+= (const MyString&) =====
MyString& MyString::operator+=(const MyString& rhs)
{
    return *this += rhs.data_;
}

// ===== operator+ (MyString, const char*) =====
MyString operator+(const MyString& lhs, const char* rhs)
{
    size_t rhsLen = (rhs ? std::strlen(rhs) : 0);
    char* buf = MyString::concat(lhs.data_, lhs.size_, rhs ? rhs : "", rhsLen);

    MyString result;
    delete[] result.data_;
    result.data_ = buf;
    result.size_ = lhs.size_ + rhsLen;

    return result;
}

// ===== operator+ (MyString, MyString) =====
MyString operator+(const MyString& lhs, const MyString& rhs)
{
    return lhs + rhs.data_;
}

// ===== operator<< (вывод в поток) =====
std::ostream& operator<<(std::ostream& os, const MyString& str)
{
    os << str.data_;
    return os;
}

// ===== operator>> (чтение слова из потока) =====
std::istream& operator>>(std::istream& is, MyString& str)
{
    // Очищаем строку
    delete[] str.data_;
    str.data_ = new char[1];
    str.data_[0] = '\0';
    str.size_ = 0;

    // Пропускаем пробелы
    char c;
    while (is.get(c)) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
    }

    if (!is) return is;

    // Читаем слово (до пробела)
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

    // Сбрасываем failbit/eofbit, так как хотя бы один токен был успешно прочитан
    is.clear();

    return is;
}

// ===== getline (чтение строки целиком) =====
std::istream& getline(std::istream& is, MyString& str, char delim)
{
    // Очищаем строку
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

    // Удаляем завершающий \r (для совместимости с CRLF в Windows)
    while (len > 0 && (buf[len - 1] == '\r' || buf[len - 1] == '\n')) {
        len--;
    }

    buf[len] = '\0';

    delete[] str.data_;
    str.data_ = buf;
    str.size_ = len;

    return is;
}
