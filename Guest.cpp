#include "Guest.h"
#include <iostream>
#include <cstring>

Guest::Guest() : name(""), phone(""), email(""), clientID(0), status("regular") {}

Guest::Guest(const MyString& name, const MyString& phone, const MyString& email, int id, const MyString& status)
    : name(name), phone(phone), email(email), clientID(id), status(status) {
}

const MyString& Guest::getName() const { return name; }
const MyString& Guest::getPhone() const { return phone; }
const MyString& Guest::getEmail() const { return email; }
int Guest::getClientID() const { return clientID; }
const MyString& Guest::getStatus() const { return status; }

void Guest::setName(const MyString& newName) { name = newName; }
void Guest::setPhone(const MyString& newPhone) { phone = newPhone; }
void Guest::setEmail(const MyString& newEmail) { email = newEmail; }
void Guest::setStatus(const MyString& newStatus) { status = newStatus; }

bool Guest::isValidStatus() const {
    return status == "regular" || status == "gold" || status == "platinum";
}

std::ostream& operator<<(std::ostream& os, const Guest& guest) {
    return os << guest.name.c_str() << ','
        << guest.phone.c_str() << ','
        << guest.email.c_str() << ','
        << guest.clientID << ','
        << guest.status.c_str();
}

std::istream& operator>>(std::istream& is, Guest& guest) {
    char nameBuf[128], phoneBuf[64], emailBuf[128], statusBuf[32];
    int id;

    try {
        is.getline(nameBuf, 128, ',');
        is.getline(phoneBuf, 64, ',');
        is.getline(emailBuf, 128, ',');
        is >> id;
        is.ignore();
        is.getline(statusBuf, 32, '\n');

        Guest temp(nameBuf, phoneBuf, emailBuf, id, statusBuf);

        if (!temp.isValidStatus())
            throw "Invalid status. Allowed: regular, gold, platinum.";

        guest = temp;
    }
    catch (const char* msg) {
        std::cerr << "Error reading guest: " << msg << std::endl;
        is.setstate(std::ios::failbit);
    }

    return is;
}
