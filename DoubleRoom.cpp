#include "DoubleRoom.h"

DoubleRoom::DoubleRoom() : Room() {}

DoubleRoom::DoubleRoom(int number, const MyString& status, double price)
    : Room(number, status, price) {
}

MyString DoubleRoom::getType() const {
    return "double";
}

double DoubleRoom::calculatePrice(bool isWeekend, bool isHighSeason, int nights) const {
    double finalPrice = basePrice;
    if (isWeekend) finalPrice *= 1.10;
    if (isHighSeason) finalPrice *= 1.20;
    return finalPrice * nights;
}

Room* DoubleRoom::clone() const {
    return new DoubleRoom(*this);
}

void DoubleRoom::print(std::ostream& os) const {
    os << getType().c_str() << ",";
    Room::print(os);
}

void DoubleRoom::read(std::istream& is) {
    Room::read(is);
}
