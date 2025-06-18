#pragma once

#include <iostream>

class MyString {
private:
    char* data;
    size_t length;

    void copyFrom(const MyString& other);
    void moveFrom(MyString&& other);
    void free();

public:
    MyString();                                
    MyString(const char* str);                 
    MyString(const MyString& other);           
    MyString(MyString&& other) noexcept;       
    MyString& operator=(const MyString& other);
    MyString& operator=(MyString&& other) noexcept; 
    ~MyString();                               

    const char* c_str() const;
    size_t size() const;

    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    bool operator==(const MyString& other) const;
    bool operator!=(const MyString& other) const;

    friend std::ostream& operator<<(std::ostream& os, const MyString& str);
    friend std::istream& operator>>(std::istream& is, MyString& str);
};

