#pragma once

#include "Room.h"
#include "SingleRoom.h"
#include "DoubleRoom.h"
#include "LuxuryRoom.h"
#include "ConferenceRoom.h"
#include "Apartment.h"
#include "MyVector.hpp"
#include "MyString.h"
#include <fstream>

class RoomManager {
private:
    MyVector<Room*> rooms;
    MyString filePath;

    void free();
    void copyFrom(const RoomManager& other);

    Room* createRoomByType(const MyString& type) const;

public:
    RoomManager(const char* file = "data/rooms.txt");
    RoomManager(const RoomManager& other);
    RoomManager& operator=(const RoomManager& other);
    ~RoomManager();

    void loadRooms();
    void saveRooms() const;

    bool addRoom(Room* room);
    Room* findRoom(int roomNumber) const;
    bool changeRoomStatus(int roomNumber, const MyString& newStatus);

    void printAllRooms() const;
};
