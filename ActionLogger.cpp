#include "ActionLogger.h"
#include <ctime>
#include <iostream>
#include <cstring>
#include <cstdio>

ActionLogger::ActionLogger(const char* file) : filePath(file) {
    logFile.open(file, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Logger error: Cannot open file " << file << std::endl;
    }
}

ActionLogger::~ActionLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

MyString ActionLogger::getCurrentTime() const {
    time_t now = time(0);
    tm* local = localtime(&now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);
    return MyString(buffer);
}

void ActionLogger::logAction(const MyString& username, const MyString& role, const MyString& action) {
    if (!logFile.is_open()) {
        std::cerr << "Logging error: file not open.\n";
        return;
    }

    logFile << "[" << getCurrentTime().c_str() << "] "
        << "(Role: " << role.c_str() << ") "
        << username.c_str() << ": "
        << action.c_str() << std::endl;
}
