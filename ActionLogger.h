#pragma once

#include "Date.h"
#include "MyString.h"
#include <fstream>

class ActionLogger {
private:
    std::ofstream logFile;
    MyString filePath;

    MyString getCurrentTime() const;

public:
    ActionLogger(const char* file = "data/logs.txt");
    ~ActionLogger();

    void logAction(const MyString& username, const MyString& role, const MyString& action);
};
