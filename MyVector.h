#pragma once
#include <cstddef> 
#include <stdexcept>

template <typename T>
class MyVector {
private:
    T* data;
    size_t size;
    size_t capacity;

    void copyFrom(const MyVector<T>& other);
    void moveFrom(MyVector<T>&& other);
    void free();
    void resize();

public:
    MyVector();                                 
    MyVector(const MyVector<T>& other);         
    MyVector(MyVector<T>&& other) noexcept;     
    MyVector<T>& operator=(const MyVector<T>& other);
    MyVector<T>& operator=(MyVector<T>&& other) noexcept; 
    ~MyVector();                              

    void push_back(const T& element);
    void pop_back();
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    size_t getSize() const;
    bool isEmpty() const;
};
