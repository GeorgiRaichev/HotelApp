#pragma once

#include "Reservation.h"
#include "MyVector.hpp"
#include "MyString.h"
#include <fstream>

class ReservationManager {
private:
    MyVector<Reservation> reservations;
    MyString filePath;
    int nextID;

public:
    ReservationManager(const char* file = "data/reservations.txt");

    void loadReservations();
    void saveReservations() const;

    bool addReservation(int guestID, int roomNumber, const Date& start, const Date& end, double totalPrice);
    bool cancelReservation(int reservationID);

    const Reservation* findByID(int resID) const;
    bool isRoomAvailable(int roomNumber, const Date& start, const Date& end) const;

    void printAllReservations() const;
    const MyVector<Reservation>& getAllReservations() const;
};
