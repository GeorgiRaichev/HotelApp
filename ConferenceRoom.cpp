#include "ConferenceRoom.h"

ConferenceRoom::ConferenceRoom() : Room() {}

ConferenceRoom::ConferenceRoom(int number, const MyString& status, double price)
    : Room(number, status, price) {
}

MyString ConferenceRoom::getType() const {
    return "conference";
}

double ConferenceRoom::calculatePrice(bool isWeekend, bool isHighSeason, int nights) const {
    double finalPrice = basePrice;
    if (isWeekend) finalPrice *= 1.30;
    if (isHighSeason) finalPrice *= 1.25;
    return finalPrice * nights;
}

Room* ConferenceRoom::clone() const {
    return new ConferenceRoom(*this);
}

void ConferenceRoom::print(std::ostream& os) const {
    os << getType().c_str() << ",";
    Room::print(os);
}

void ConferenceRoom::read(std::istream& is) {
    Room::read(is);
}
