#include "MyString.h"
#include <cstring>

void MyString::copyFrom(const MyString& other) {
    length = other.length;
    data = new char[length + 1];
    std::strcpy(data, other.data);
}

void MyString::moveFrom(MyString&& other) {
    data = other.data;
    length = other.length;
    other.data = nullptr;
    other.length = 0;
}

void MyString::free() {
    delete[] data;
    data = nullptr;
    length = 0;
}

MyString::MyString() : data(nullptr), length(0) {
    data = new char[1];
    data[0] = '\0';
}

MyString::MyString(const char* str) {
    if (str) {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
    }
    else {
        data = new char[1];
        data[0] = '\0';
        length = 0;
    }
}

MyString::MyString(const MyString& other) {
    copyFrom(other);
}

MyString::MyString(MyString&& other) noexcept {
    moveFrom(std::move(other));
}

MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

MyString& MyString::operator=(MyString&& other) noexcept {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

MyString::~MyString() {
    free();
}

const char* MyString::c_str() const {
    return data;
}

size_t MyString::size() const {
    return length;
}

char& MyString::operator[](size_t index) {
    return data[index]; 
}

const char& MyString::operator[](size_t index) const {
    return data[index];
}

bool MyString::operator==(const MyString& other) const {
    return std::strcmp(data, other.data) == 0;
}

bool MyString::operator!=(const MyString& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const MyString& str) {
    return os << str.data;
}

std::istream& operator>>(std::istream& is, MyString& str) {
    char buffer[1024];
    is >> buffer;
    str = MyString(buffer);
    return is;
}
