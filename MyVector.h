#pragma once
#include <new>

template <typename T>
class MyVector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

public:
    MyVector()
        : data_(nullptr), size_(0), capacity_(0)
    {
    }

    explicit MyVector(size_t count)
        : data_(static_cast<T*>(operator new(count * sizeof(T))))
        , size_(count), capacity_(count)
    {
        for (size_t i = 0; i < count; i++)
            new (&data_[i]) T();
    }

    ~MyVector()
    {
        clear();
        operator delete(data_);
    }

    MyVector(const MyVector& other)
        : data_(static_cast<T*>(operator new(other.size_ * sizeof(T))))
        , size_(other.size_), capacity_(other.size_)
    {
        for (size_t i = 0; i < size_; i++)
            new (&data_[i]) T(other.data_[i]);
    }

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

    MyVector(MyVector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

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

    void clear()
    {
        for (size_t i = 0; i < size_; i++)
            data_[i].~T();
        size_ = 0;
    }
};