#include "LuxuryRoom.h"

LuxuryRoom::LuxuryRoom() : Room() {}

LuxuryRoom::LuxuryRoom(int number, const MyString& status, double price)
    : Room(number, status, price) {
}

MyString LuxuryRoom::getType() const {
    return "luxury";
}

double LuxuryRoom::calculatePrice(bool isWeekend, bool isHighSeason, int nights) const {
    double finalPrice = basePrice;
    if (isWeekend) finalPrice *= 1.15;
    if (isHighSeason) finalPrice *= 1.25;
    return finalPrice * nights;
}

Room* LuxuryRoom::clone() const {
    return new LuxuryRoom(*this);
}

void LuxuryRoom::print(std::ostream& os) const {
    Room::print(os);
}

void LuxuryRoom::read(std::istream& is) {
    Room::read(is);
}
