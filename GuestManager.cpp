#include "GuestManager.h"
#include <iostream>

GuestManager::GuestManager(const char* file) : filePath(file), nextID(1) {}

void GuestManager::loadGuests() {
    std::ifstream in(filePath.c_str());

    if (!in.is_open()) {
        std::cerr << "Could not open guest file for reading: " << filePath.c_str() << std::endl;
        return;
    }

    guests = MyVector<Guest>();
    Guest temp;

    while (in.peek() != EOF) {
        if (in >> temp) {
            guests.push_back(temp);
            if (temp.getClientID() >= nextID)
                nextID = temp.getClientID() + 1;
        }
        else {
            std::cerr << "Error reading guest: skipping invalid line.\n";
            in.clear();
            in.ignore(1024, '\n');
        }
    }

    in.close();
}


void GuestManager::saveGuests() const {
    std::ofstream out(filePath.c_str());
    if (!out.is_open()) {
        std::cerr << "Could not open guest file for writing: " << filePath.c_str() << std::endl;
        return;
    }

    for (size_t i = 0; i < guests.getSize(); i++) {
        guests[i].print(out);
        out << '\n';
    }


    out.close();
}

bool GuestManager::addGuest(const MyString& name, const MyString& phone, const MyString& email, const MyString& status) {
    try {
        Guest newGuest(name, phone, email, nextID++, status);

        if (!newGuest.isValidStatus()) {
            std::cerr << "Invalid status. Allowed: regular, gold, platinum.\n";
            return false;
        }

        guests.push_back(newGuest);
        saveGuests();
        return true;

    }
    catch (...) {
        std::cerr << "Unexpected error while adding guest.\n";
        return false;
    }
}

const Guest* GuestManager::findGuestByID(int id) const {
    for (size_t i = 0; i < guests.getSize(); i++) {
        if (guests[i].getClientID() == id)
            return &guests[i];
    }
    return nullptr;
}

void GuestManager::printAllGuests() const {
    for (size_t i = 0; i < guests.getSize(); i++) {
        std::cout << guests[i] << std::endl;
    }
}

const MyVector<Guest>& GuestManager::getAllGuests() const {
    return guests;
}
