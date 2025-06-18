#include "User.h"
#include <cstring>
#include <iostream>

User::User() : username(""), password(""), role("") {}

User::User(const MyString& username, const MyString& password, const MyString& role)
    : username(username), password(password), role(role) {
}

const MyString& User::getUsername() const { return username; }
const MyString& User::getPassword() const { return password; }
const MyString& User::getRole() const { return role; }

void User::setUsername(const MyString& newUsername) { username = newUsername; }
void User::setPassword(const MyString& newPassword) { password = newPassword; }
void User::setRole(const MyString& newRole) { role = newRole; }

bool User::checkCredentials(const MyString& uname, const MyString& pwd) const {
    return username == uname && password == pwd;
}

bool User::isValidRole() const {
    return role == "manager" || role == "receptionist" || role == "accountant";
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    return os << user.username.c_str() << ','
        << user.password.c_str() << ','
        << user.role.c_str();
}

std::istream& operator>>(std::istream& is, User& user) {
    char usernameBuf[128], passwordBuf[128], roleBuf[128];

    try {
        is.getline(usernameBuf, 128, ',');
        is.getline(passwordBuf, 128, ',');
        is.getline(roleBuf, 128, '\n');

        if (!is) throw "Input error while reading user.";

        User temp(usernameBuf, passwordBuf, roleBuf);
        if (!temp.isValidRole())
            throw "Invalid role. Must be: manager, receptionist, or accountant.";

        user = temp;
    }
    catch (const char* msg) {
        std::cerr << "Error reading user: " << msg << std::endl;
        is.setstate(std::ios::failbit);
    }

    return is;
}
