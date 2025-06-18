#pragma once

#include "Room.h"

class SingleRoom : public Room {
public:
    SingleRoom();
    SingleRoom(int number, const MyString& status, double price);

    MyString getType() const override;
    double calculatePrice(bool isWeekend, bool isHighSeason, int nights) const override;
    Room* clone() const override;

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
};
