#pragma once
#include <iostream>

class MyString {
private:
    char* data_;
    size_t size_;

    // Присваивает новое содержимое (копирует C-строку)
    void assign(const char* str);

    // Создаёт новый буфер размером len1 + len2 + 1 и копирует s1 + s2
    static char* concat(const char* s1, size_t len1, const char* s2, size_t len2);

public:
    static const size_t npos = static_cast<size_t>(-1);

    // Конструкторы
    MyString();
    MyString(const char* str);
    MyString(const MyString& other);

    // Деструктор
    ~MyString();

    // Копирующее присваивание
    MyString& operator=(const MyString& other);

    // Перемещающие
    MyString(MyString&& other) noexcept;
    MyString& operator=(MyString&& other) noexcept;

    // Методы доступа
    const char* c_str() const;
    bool empty() const;
    size_t size() const;

    // Поиск
    size_t find_last_of(const char* chars) const;
    size_t find_last_of(char c) const;

    // Подстрока
    MyString substr(size_t pos = 0, size_t count = npos) const;

    // Операторы сравнения
    friend bool operator==(const MyString& lhs, const MyString& rhs);
    friend bool operator==(const MyString& lhs, const char* rhs);
    friend bool operator==(const char* lhs, const MyString& rhs);
    friend bool operator<(const MyString& lhs, const MyString& rhs);

    // Присваивание с конкатенацией
    MyString& operator+=(const char* rhs);
    MyString& operator+=(const MyString& rhs);

    // Конкатенация
    friend MyString operator+(const MyString& lhs, const char* rhs);
    friend MyString operator+(const MyString& lhs, const MyString& rhs);

    // Ввод/вывод
    friend std::ostream& operator<<(std::ostream& os, const MyString& str);
    friend std::istream& operator>>(std::istream& is, MyString& str);
    friend std::istream& getline(std::istream& is, MyString& str, char delim = '\n');
};