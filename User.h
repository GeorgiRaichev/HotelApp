#pragma once

#include "MyString.h"
#include <iostream>

class User {
private:
    MyString username;
    MyString password;
    MyString role;

public:
    User();
    User(const MyString& username, const MyString& password, const MyString& role);

    const MyString& getUsername() const;
    const MyString& getPassword() const;
    const MyString& getRole() const;

    void setUsername(const MyString& newUsername);
    void setPassword(const MyString& newPassword);
    void setRole(const MyString& newRole);

    bool checkCredentials(const MyString& username, const MyString& password) const;
    bool isValidRole() const;

    friend std::ostream& operator<<(std::ostream& os, const User& user);
    friend std::istream& operator>>(std::istream& is, User& user);
};
