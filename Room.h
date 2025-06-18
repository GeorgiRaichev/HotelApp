#pragma once

#include "MyString.h"
#include <iostream>

class Room {
protected:
    int roomNumber;
    MyString status;      
    double basePrice;

public:
    Room();
    Room(int number, const MyString& status, double price);

    virtual ~Room() {}

    int getRoomNumber() const;
    const MyString& getStatus() const;
    double getBasePrice() const;

    void setStatus(const MyString& newStatus);
    void setBasePrice(double newPrice);

    virtual MyString getType() const = 0;
    virtual double calculatePrice(bool isWeekend, bool isHighSeason, int nights) const = 0;

    virtual Room* clone() const = 0;

    virtual void display(std::ostream& os) const;
    virtual void print(std::ostream& os) const;
    virtual void read(std::istream& is);

    friend std::ostream& operator<<(std::ostream& os, const Room& room);
    friend std::istream& operator>>(std::istream& is, Room*& room);
};
