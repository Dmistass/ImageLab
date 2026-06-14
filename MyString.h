#pragma once
#include <iostream>

class MyString {
private:
    char* data_;   // pointer to string (with terminating '\0')
    size_t size_;  // length of the string (without null terminator)

    // Assigns new content (copies string)
    void assign(const char* str);

    // Creates a new buffer of size len1 + len2 + 1 and copies s1 + s2
    static char* concat(const char* s1, size_t len1, const char* s2, size_t len2);

public:
    static const size_t npos = static_cast<size_t>(-1);

    // Constructors
    MyString();
    MyString(const char* str);
    MyString(const MyString& other);

    // Destructor
    ~MyString();

    // Copy assignment
    MyString& operator=(const MyString& other);

    // Move
    MyString(MyString&& other) noexcept;
    MyString& operator=(MyString&& other) noexcept;

    // Access methods
    const char* c_str() const;
    bool empty() const;
    size_t size() const;

    // Search
    size_t find_last_of(const char* chars) const;
    size_t find_last_of(char c) const;

    // Substring
    MyString substr(size_t pos = 0, size_t count = npos) const;

    // Comparison operators
    friend bool operator==(const MyString& lhs, const MyString& rhs);
    friend bool operator==(const MyString& lhs, const char* rhs);
    friend bool operator==(const char* lhs, const MyString& rhs);
    friend bool operator<(const MyString& lhs, const MyString& rhs);

    // Concatenation assignment
    MyString& operator+=(const char* rhs);
    MyString& operator+=(const MyString& rhs);

    // Concatenation
    friend MyString operator+(const MyString& lhs, const char* rhs);
    friend MyString operator+(const MyString& lhs, const MyString& rhs);

    // Input/Output
    friend std::ostream& operator<<(std::ostream& os, const MyString& str);
    friend std::istream& operator>>(std::istream& is, MyString& str);
    friend std::istream& getline(std::istream& is, MyString& str, char delim = '\n');
};