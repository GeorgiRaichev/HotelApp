#pragma once

#include "MyString.h"
#include "Date.h"
#include <iostream>

class Reservation {
private:
    int reservationID;
    int guestID;
    int roomNumber;
    Date startDate;
    Date endDate;
    double totalPrice;

public:
    Reservation();
    Reservation(int resID, int guestID, int roomNumber, const Date& start, const Date& end, double price);

    int getReservationID() const;
    int getGuestID() const;
    int getRoomNumber() const;
    const Date& getStartDate() const;
    const Date& getEndDate() const;
    double getTotalPrice() const;

    void setTotalPrice(double newPrice);

    int getNights() const;

    friend std::ostream& operator<<(std::ostream& os, const Reservation& res);
    friend std::istream& operator>>(std::istream& is, Reservation& res);

    void display(std::ostream& os) const;
    void print(std::ostream& os) const;
};
