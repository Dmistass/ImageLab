#pragma once
#include <vector>

template <typename T>
class MyVector : public std::vector<T> {
public:
    // Хорошая практика: наследовать конструкторы базового класса,
    // чтобы работали инициализаторы списков {1, 2, 3} и другие конструкторы
    using std::vector<T>::vector;
};