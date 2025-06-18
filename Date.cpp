#include "Date.h"
#include <stdexcept>
#include <iomanip>

Date::Date() : day(1), month(1), year(2000) {}

Date::Date(unsigned short d, unsigned short m, unsigned short y) {
    if (m < 1 || m > 12 || d < 1 || d > daysInMonth(m, y))
        throw std::invalid_argument("Invalid date.");
    day = d;
    month = m;
    year = y;
}

bool Date::isValid() const {
    return month >= 1 && month <= 12 &&
        day >= 1 && day <= daysInMonth(month, year);
}

bool Date::isLeapYear(unsigned short y) const {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

unsigned short Date::daysInMonth(unsigned short m, unsigned short y) const {
    switch (m) {
    case 2: return isLeapYear(y) ? 29 : 28;
    case 4: case 6: case 9: case 11: return 30;
    default: return 31;
    }
}
unsigned short Date::getDay() const { return day; }
unsigned short Date::getMonth() const { return month; }
unsigned short Date::getYear() const { return year; }

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}
bool Date::operator!=(const Date& other) const { return !(*this == other); }
bool Date::operator<(const Date& other) const {
    return (year < other.year) ||
        (year == other.year && month < other.month) ||
        (year == other.year && month == other.month && day < other.day);
}
bool Date::operator<=(const Date& other) const { return *this < other || *this == other; }
bool Date::operator>(const Date& other) const { return !(*this <= other); }
bool Date::operator>=(const Date& other) const { return !(*this < other); }

std::ostream& operator<<(std::ostream& os, const Date& date) {
    return os << std::setw(2) << std::setfill('0') << date.day << '.'
        << std::setw(2) << std::setfill('0') << date.month << '.'
        << date.year;
}

std::istream& operator>>(std::istream& is, Date& date) {
    unsigned short d, m, y;
    char dot1, dot2;

    try {
        is >> d >> dot1 >> m >> dot2 >> y;
        if (!is || dot1 != '.' || dot2 != '.')
            throw std::invalid_argument("Invalid date format. Use DD.MM.YYYY");
        date = Date(d, m, y); 
    }
    catch (const std::exception& e) {
        std::cerr << "Error reading date: " << e.what() << std::endl;
        date = Date();
        is.setstate(std::ios::failbit);
    }

    return is;
}
