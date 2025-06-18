#include "Reservation.h"
#include <iostream>
#include <cmath>

Reservation::Reservation()
    : reservationID(0), guestID(0), roomNumber(0), totalPrice(0.0) {
}

Reservation::Reservation(int resID, int guestID, int roomNumber, const Date& start, const Date& end, double price)
    : reservationID(resID), guestID(guestID), roomNumber(roomNumber),
    startDate(start), endDate(end), totalPrice(price) {
}

int Reservation::getReservationID() const { return reservationID; }
int Reservation::getGuestID() const { return guestID; }
int Reservation::getRoomNumber() const { return roomNumber; }
const Date& Reservation::getStartDate() const { return startDate; }
const Date& Reservation::getEndDate() const { return endDate; }
double Reservation::getTotalPrice() const { return totalPrice; }

void Reservation::setTotalPrice(double newPrice) { totalPrice = newPrice; }

int Reservation::getNights() const {
    int startDays = startDate.getYear() * 365 + startDate.getMonth() * 30 + startDate.getDay();
    int endDays = endDate.getYear() * 365 + endDate.getMonth() * 30 + endDate.getDay();
    int diff = endDays - startDays;
    return diff > 0 ? diff : 0;
}

std::ostream& operator<<(std::ostream& os, const Reservation& res) {
    os << res.reservationID << ','
        << res.guestID << ','
        << res.roomNumber << ','
        << res.startDate << ','
        << res.endDate << ','
        << res.totalPrice;
    return os;
}

std::istream& operator>>(std::istream& is, Reservation& res) {
    int rid, gid, room;
    Date start, end;
    double price;
    char comma;

    try {
        is >> rid >> comma;
        is >> gid >> comma;
        is >> room >> comma;
        is >> start >> comma;
        is >> end >> comma;
        is >> price;

        if (!is) throw "Invalid reservation format or date.";

        res = Reservation(rid, gid, room, start, end, price);
    }
    catch (const char* msg) {
        std::cerr << "Error reading reservation: " << msg << std::endl;
        is.setstate(std::ios::failbit);
    }

    return is;
}
