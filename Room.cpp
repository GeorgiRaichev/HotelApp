#include "Room.h"
#include <iostream>
#include <cstring>
#include <iomanip>
#include "SingleRoom.h"
#include "DoubleRoom.h"
#include "LuxuryRoom.h"
#include "ConferenceRoom.h"
#include "Apartment.h"

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
    char typeBuf[32];
    is.getline(typeBuf, 32, ',');

    Room* temp = nullptr;

    if (strcmp(typeBuf, "single") == 0) temp = new SingleRoom();
    else if (strcmp(typeBuf, "double") == 0) temp = new DoubleRoom();
    else if (strcmp(typeBuf, "luxury") == 0) temp = new LuxuryRoom();
    else if (strcmp(typeBuf, "conference") == 0) temp = new ConferenceRoom();
    else if (strcmp(typeBuf, "apartment") == 0) temp = new Apartment();
    else {
        std::cerr << "Unknown room type: " << typeBuf << std::endl;
        is.setstate(std::ios::failbit);
        return is;
    }

    try {
        temp->read(is);
        room = temp;
    }
    catch (...) {
        std::cerr << "Failed to read room.\n";
        delete temp;
        is.setstate(std::ios::failbit);
    }

    return is;
}
