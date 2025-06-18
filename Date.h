#pragma once

#include <iostream>

class Date {
private:
    unsigned short day;
    unsigned short month;
    unsigned short year;

    bool isLeapYear(unsigned short year) const;
    unsigned short daysInMonth(unsigned short month, unsigned short year) const;

public:
    Date(); 
    Date(unsigned short d, unsigned short m, unsigned short y);

    bool isValid() const;

    unsigned short getDay() const;
    unsigned short getMonth() const;
    unsigned short getYear() const;

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    friend std::istream& operator>>(std::istream& is, Date& date);
};
