#include "Room.h"
#include <iostream>
#include <cstring>
#include <iomanip>

Room::Room() : roomNumber(0), status("free"), basePrice(0.0) {}

Room::Room(int number, const MyString& status, double price)
    : roomNumber(number), status(status), basePrice(price) {
}

int Room::getRoomNumber() const { return roomNumber; }
const MyString& Room::getStatus() const { return status; }
double Room::getBasePrice() const { return basePrice; }

void Room::setStatus(const MyString& newStatus) { status = newStatus; }
void Room::setBasePrice(double newPrice) { basePrice = newPrice; }

void Room::print(std::ostream& os) const {
    os << roomNumber << ',' << status.c_str() << ',' << basePrice << ',';
}

void Room::read(std::istream& is) {
    char statusBuf[64];
    is >> roomNumber;
    is.ignore(); 
    is.getline(statusBuf, 64, ',');
    is >> basePrice;
    status = MyString(statusBuf);
}

std::ostream& operator<<(std::ostream& os, const Room& room) {
    room.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, Room*& room) { 
    std::cerr << "Error: Cannot deserialize abstract Room. Use derived class.\n";
    is.setstate(std::ios::failbit);
    return is;
}
