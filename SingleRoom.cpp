#include "SingleRoom.h"
#include <iostream>
#include <cstring>

SingleRoom::SingleRoom() : Room() {}

SingleRoom::SingleRoom(int number, const MyString& status, double price)
    : Room(number, status, price) {
}

MyString SingleRoom::getType() const {
    return "single";
}

double SingleRoom::calculatePrice(bool isWeekend, bool isHighSeason, int nights) const {
    double finalPrice = basePrice;

    if (isWeekend)
        finalPrice *= 1.10;

    if (isHighSeason)
        finalPrice *= 1.20; 

    return finalPrice * nights;
}

Room* SingleRoom::clone() const {
    return new SingleRoom(*this);
}

void SingleRoom::print(std::ostream& os) const {
    Room::print(os); 
}


void SingleRoom::read(std::istream& is) {
    Room::read(is);
}
