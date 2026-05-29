#pragma once
#include <string>

class MyString : public std::string {
public:
    // Хорошая практика: наследовать конструкторы базового класса
    using std::string::string;
};