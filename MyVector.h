#pragma once
#include <new>

template <typename T>
class MyVector {
private:
    T* data_;         // pointer to array of elements
    size_t size_;     // how many elements are currently in the array
    size_t capacity_; // how many elements the allocated memory can hold

public:
    // Default constructor - empty vector
    MyVector()
        : data_(nullptr), size_(0), capacity_(0)
    {
    }

    // Constructor from count - creates a vector of count elements, default-initialized
    explicit MyVector(size_t count)
        : data_(static_cast<T*>(operator new(count * sizeof(T))))
        , size_(count), capacity_(count)
    {
        for (size_t i = 0; i < count; i++)
            new (&data_[i]) T();
    }

    // Destructor - calls clear() (calls destructors of elements) and frees memory
    ~MyVector()
    {
        clear();
        operator delete(data_);
    }

    // Copy constructor - allocates new memory and copies elements via placement new
    MyVector(const MyVector& other)
        : data_(static_cast<T*>(operator new(other.size_ * sizeof(T))))
        , size_(other.size_), capacity_(other.size_)
    {
        for (size_t i = 0; i < size_; i++)
            new (&data_[i]) T(other.data_[i]);
    }

    // Copy assignment - delete current state and copy from other
    MyVector& operator=(const MyVector& other)
    {
        if (this != &other) {
            this->~MyVector();
            data_ = static_cast<T*>(operator new(other.size_ * sizeof(T)));
            size_ = other.size_;
            capacity_ = other.size_;
            for (size_t i = 0; i < size_; i++)
                new (&data_[i]) T(other.data_[i]);
        }
        return *this;
    }

    // Move constructor
    MyVector(MyVector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // Move assignment
    MyVector& operator=(MyVector&& other) noexcept
    {
        if (this != &other) {
            this->~MyVector();
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Resize the vector
    void resize(size_t newSize)
    {
        if (newSize == size_) return;

        T* newData = static_cast<T*>(operator new(newSize * sizeof(T)));
        size_t copyCount = newSize < size_ ? newSize : size_;
        for (size_t i = 0; i < copyCount; i++)
            new (&newData[i]) T(data_[i]);

        this->~MyVector();
        data_ = newData;
        size_ = newSize;
        capacity_ = newSize;
    }

    // Reserve memory for at least newCapacity elements, without changing size
    void reserve(size_t newCapacity)
    {
        if (newCapacity <= capacity_) return;

        T* newData = static_cast<T*>(operator new(newCapacity * sizeof(T)));
        for (size_t i = 0; i < size_; i++)
            new (&newData[i]) T(data_[i]);
        for (size_t i = 0; i < size_; i++)
            data_[i].~T();
        operator delete(data_);
        data_ = newData;
        capacity_ = newCapacity;
    }

    T* data() { return data_; }
    const T* data() const { return data_; }

    // Adds an element to the end, doubles the array if necessary
    void push_back(const T& value)
    {
        if (size_ >= capacity_)
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        new (&data_[size_++]) T(value);
    }

    size_t size() const { return size_; }

    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }

    T* begin() { return data_; }
    const T* begin() const { return data_; }
    T* end() { return data_ + size_; }
    const T* end() const { return data_ + size_; }

    // Removes an element by pointer, calls destructors and moves elements
    void erase(T* pos)
    {
        if (pos < data_ || pos >= data_ + size_) return;
        pos->~T();
        for (T* it = pos; it < data_ + size_ - 1; it++) {
            new (it) T(*(it + 1));
            (it + 1)->~T();
        }
        size_--;
    }

    // Clears the vector - calls destructors of all elements, size = 0
    void clear()
    {
        for (size_t i = 0; i < size_; i++)
            data_[i].~T();
        size_ = 0;
    }
};