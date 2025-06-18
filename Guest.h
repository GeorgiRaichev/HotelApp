#pragma once

#include "MyString.h"
#include <iostream>

class Guest {
private:
    MyString name;
    MyString phone;
    MyString email;
    int clientID;
    MyString status;

public:
    Guest();
    Guest(const MyString& name, const MyString& phone, const MyString& email, int id, const MyString& status = "regular");

    const MyString& getName() const;
    const MyString& getPhone() const;
    const MyString& getEmail() const;
    int getClientID() const;
    const MyString& getStatus() const;

    void setName(const MyString& newName);
    void setPhone(const MyString& newPhone);
    void setEmail(const MyString& newEmail);
    void setStatus(const MyString& newStatus);

    bool isValidStatus() const;

    friend std::ostream& operator<<(std::ostream& os, const Guest& guest);
    friend std::istream& operator>>(std::istream& is, Guest& guest);
};
