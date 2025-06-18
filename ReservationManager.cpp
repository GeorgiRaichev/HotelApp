#include "ReservationManager.h"
#include <iostream>

ReservationManager::ReservationManager(const char* file) : filePath(file), nextID(1) {}

void ReservationManager::loadReservations() {
    std::ifstream in(filePath.c_str());
    if (!in.is_open()) {
        std::cerr << "Could not open reservation file: " << filePath.c_str() << std::endl;
        return;
    }

    reservations = MyVector<Reservation>();
    Reservation r;

    while (!in.eof()) {
        std::streampos pos = in.tellg();  
        char first;
        in >> std::ws; 
        first = in.peek();

        if (first == '\n' || first == '\r' || first == EOF) {
            in.ignore(1024, '\n'); 
            continue;
        }

        in.seekg(pos); 

        if (in >> r) {
            reservations.push_back(r);
            if (r.getReservationID() >= nextID)
                nextID = r.getReservationID() + 1;
        }
        else {
            std::cerr << "Error reading reservation: Invalid format or date – skipping line.\n";
            in.clear();
            in.ignore(1024, '\n'); 
        }
    }

    in.close();
}


void ReservationManager::saveReservations() const {
    std::ofstream out(filePath.c_str());
    if (!out.is_open()) {
        std::cerr << "Failed to write to reservation file.\n";
        return;
    }

    for (size_t i = 0; i < reservations.getSize(); i++) {
        reservations[i].print(out);
        out << '\n';
    }

    out.close();
}

bool ReservationManager::isRoomAvailable(int roomNumber, const Date& start, const Date& end) const {
    for (size_t i = 0; i < reservations.getSize(); i++) {
        const Reservation& r = reservations[i];
        if (r.getRoomNumber() != roomNumber)
            continue;

        if (!(end <= r.getStartDate() || start >= r.getEndDate()))
            return false;
    }
    return true;
}

bool ReservationManager::addReservation(int guestID, int roomNumber, const Date& start, const Date& end, double totalPrice) {
    try {
        if (!start.isValid() || !end.isValid() || !(start < end)) {
            std::cerr << "Invalid date range.\n";
            return false;
        }

        if (!isRoomAvailable(roomNumber, start, end)) {
            std::cerr << "Room is not available for selected period.\n";
            return false;
        }

        Reservation r(nextID++, guestID, roomNumber, start, end, totalPrice);
        reservations.push_back(r);
        saveReservations();
        return true;

    }
    catch (...) {
        std::cerr << "Error adding reservation.\n";
        return false;
    }
}

bool ReservationManager::cancelReservation(int reservationID) {
    for (size_t i = 0; i < reservations.getSize(); i++) {
        if (reservations[i].getReservationID() == reservationID) {
            for (size_t j = i; j < reservations.getSize() - 1; j++) {
                reservations[j] = reservations[j + 1];
            }
            reservations.pop_back();
            saveReservations();
            return true;
        }
    }

    std::cerr << "Reservation not found.\n";
    return false;
}

const Reservation* ReservationManager::findByID(int resID) const {
    for (size_t i = 0; i < reservations.getSize(); i++) {
        if (reservations[i].getReservationID() == resID)
            return &reservations[i];
    }
    return nullptr;
}

void ReservationManager::printAllReservations() const {
    for (size_t i = 0; i < reservations.getSize(); i++) {
        std::cout << reservations[i] << std::endl;
    }
}

const MyVector<Reservation>& ReservationManager::getAllReservations() const {
    return reservations;
}
