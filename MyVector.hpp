#pragma once

#include "MyVector.h"

template <typename T>
void MyVector<T>::copyFrom(const MyVector<T>& other) {
    size = other.size;
    capacity = other.capacity;
    data = new T[capacity];
    for (size_t i = 0; i < size; i++)
        data[i] = other.data[i];
}

template <typename T>
void MyVector<T>::moveFrom(MyVector<T>&& other) {
    data = other.data;
    size = other.size;
    capacity = other.capacity;
    other.data = nullptr;
    other.size = 0;
    other.capacity = 0;
}

template <typename T>
void MyVector<T>::free() {
    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 0;
}

template <typename T>
void MyVector<T>::resize() {
    capacity = capacity == 0 ? 2 : capacity * 2;
    T* newData = new T[capacity];
    for (size_t i = 0; i < size; i++)
        newData[i] = data[i];
    delete[] data;
    data = newData;
}

template <typename T>
MyVector<T>::MyVector() : data(nullptr), size(0), capacity(0) {}

template <typename T>
MyVector<T>::MyVector(const MyVector<T>& other) {
    copyFrom(other);
}

template <typename T>
MyVector<T>::MyVector(MyVector<T>&& other) noexcept {
    moveFrom(std::move(other));
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other) noexcept {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

template <typename T>
MyVector<T>::~MyVector() {
    free();
}

template <typename T>
void MyVector<T>::push_back(const T& element) {
    if (size >= capacity)
        resize();
    data[size++] = element;
}

template <typename T>
void MyVector<T>::pop_back() {
    if (size == 0)
        throw std::out_of_range("Vector is empty.");
    --size;
}

template <typename T>
T& MyVector<T>::operator[](size_t index) {
    if (index >= size)
        throw std::out_of_range("Index out of bounds.");
    return data[index];
}

template <typename T>
const T& MyVector<T>::operator[](size_t index) const {
    if (index >= size)
        throw std::out_of_range("Index out of bounds.");
    return data[index];
}

template <typename T>
size_t MyVector<T>::getSize() const {
    return size;
}

template <typename T>
bool MyVector<T>::isEmpty() const {
    return size == 0;
}
