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
	if (&os == &std::cout)
		res.display(os);
	else
		res.print(os);
	return os;
}
void Reservation::display(std::ostream& os) const {
	os << "Reservation #" << reservationID
		<< " | Guest ID: " << guestID
		<< " | Room: " << roomNumber
		<< " | From: " << startDate
		<< " to " << endDate
		<< " | Total: " << totalPrice;
}

void Reservation::print(std::ostream& os) const {
	os << reservationID << ',' << guestID << ',' << roomNumber << ','
		<< startDate << ',' << endDate << ',' << totalPrice;
}

std::istream& operator>>(std::istream& is, Reservation& res) {
	int rid, gid, room;
	Date start, end;
	double price;
	char comma;

	try {
		is >> rid >> comma;

		if (comma != ',') throw "Expected comma after reservation ID";

		is >> gid >> comma;

		if (comma != ',') throw "Expected comma after guest ID";

		is >> room >> comma;

		if (comma != ',') throw "Expected comma after room ID";

		is >> start >> comma;

		if (comma != ',') throw "Expected comma after start date";

		is >> end >> comma;

		if (comma != ',') throw "Expected comma after end date";

		is >> price;

		if (!is) throw "Invalid reservation format or data.";

		res = Reservation(rid, gid, room, start, end, price);
	}
	catch (const char* msg) {
		std::cerr << "Error reading reservation: " << msg << std::endl;
		is.setstate(std::ios::failbit);
	}

	return is;
}

