#pragma once

#include "Guest.h"
#include "MyVector.hpp"
#include "MyString.h"
#include <fstream>

class GuestManager {
private:
    MyVector<Guest> guests;
    MyString filePath;
    int nextID;

public:
    GuestManager(const char* file = "data/guests.txt");

    void loadGuests();
    void saveGuests() const;

    bool addGuest(const MyString& name, const MyString& phone, const MyString& email, const MyString& status = "regular");
    const Guest* findGuestByID(int id) const;

    void printAllGuests() const;
    const MyVector<Guest>& getAllGuests() const;
};
