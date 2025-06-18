#pragma once

#include "User.h"
#include "MyVector.hpp"
#include "MyString.h"
#include <fstream>

class UserManager {
private:
    MyVector<User> users;
    MyString filePath;

public:
    UserManager(const char* file = "data/users.txt");

    void loadUsers();
    void saveUsers() const;

    bool registerUser(const MyString& username, const MyString& password, const MyString& role);
    const User* login(const MyString& username, const MyString& password) const;

    bool usernameExists(const MyString& username) const;
    const MyVector<User>& getAllUsers() const;
};
