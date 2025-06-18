#include "Apartment.h"

Apartment::Apartment() : Room() {}

Apartment::Apartment(int number, const MyString& status, double price)
    : Room(number, status, price) {
}

MyString Apartment::getType() const {
    return "apartment";
}

double Apartment::calculatePrice(bool isWeekend, bool isHighSeason, int nights) const {
    double finalPrice = basePrice;
    if (isWeekend) finalPrice *= 1.20;
    if (isHighSeason) finalPrice *= 1.30;
    return finalPrice * nights;
}

Room* Apartment::clone() const {
    return new Apartment(*this);
}

void Apartment::print(std::ostream& os) const {
    os << getType().c_str() << ",";
    Room::print(os);
}

void Apartment::read(std::istream& is) {
    Room::read(is);
}
