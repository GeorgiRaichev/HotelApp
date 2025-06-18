#include "UserManager.h"
#include <iostream>
#include <fstream>
#include <cstring>

UserManager::UserManager(const char* file) : filePath(file) {}

void UserManager::loadUsers() {
    std::ifstream in(filePath.c_str());

    if (!in.is_open()) {
        std::cerr << "Could not open user file for reading: " << filePath.c_str() << std::endl;
        return;
    }

    users = MyVector<User>(); 
    User temp;

    while (in.peek() != EOF) {
        if (in >> temp) {
            users.push_back(temp);
        }
        else {
            std::cerr << "Error reading user: skipping invalid line.\n";
            in.clear();               
            in.ignore(1024, '\n');  
        }
    }

    in.close();
}


void UserManager::saveUsers() const {
    std::ofstream out(filePath.c_str());

    if (!out.is_open()) {
        std::cerr << "Could not open user file for writing: " << filePath.c_str() << std::endl;
        return;
    }

    for (size_t i = 0; i < users.getSize(); i++) {
        users[i].print(out);
        out << '\n';
    }

    out.close();
}

bool UserManager::usernameExists(const MyString& username) const {
    for (size_t i = 0; i < users.getSize(); i++) {
        if (users[i].getUsername() == username)
            return true;
    }
    return false;
}

bool UserManager::registerUser(const MyString& username, const MyString& password, const MyString& role) {
    try {
        if (usernameExists(username)) {
            std::cerr << "Username already exists.\n";
            return false;
        }

        User newUser(username, password, role);
        if (!newUser.isValidRole()) {
            std::cerr << "Invalid role. Allowed: manager, receptionist, accountant.\n";
            return false;
        }

        users.push_back(newUser);
        saveUsers();
        return true;

    }
    catch (...) {
        std::cerr << "Unexpected error during registration.\n";
        return false;
    }
}

const User* UserManager::login(const MyString& username, const MyString& password) const {
    for (size_t i = 0; i < users.getSize(); i++) {
        if (users[i].checkCredentials(username, password)) {
            return &users[i];
        }
    }

    return nullptr;
}

const MyVector<User>& UserManager::getAllUsers() const {
    return users;
}
