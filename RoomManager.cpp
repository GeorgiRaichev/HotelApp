#include "RoomManager.h"
#include <iostream>
#include <cstring>

Room* RoomManager::createRoomByType(const MyString& type) const {
    if (type == "single") return new SingleRoom();
    if (type == "double") return new DoubleRoom();
    if (type == "luxury") return new LuxuryRoom();
    if (type == "conference") return new ConferenceRoom();
    if (type == "apartment") return new Apartment();
    return nullptr;
}

RoomManager::RoomManager(const char* file) : filePath(file) {}

RoomManager::RoomManager(const RoomManager& other) {
    copyFrom(other);
}

RoomManager& RoomManager::operator=(const RoomManager& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

RoomManager::~RoomManager() {
    free();
}

void RoomManager::copyFrom(const RoomManager& other) {
    filePath = other.filePath;
    for (size_t i = 0; i < other.rooms.getSize(); i++)
        rooms.push_back(other.rooms[i]->clone());
}

void RoomManager::free() {
    for (size_t i = 0; i < rooms.getSize(); i++)
        delete rooms[i];
    rooms = MyVector<Room*>();
}

void RoomManager::loadRooms() {
    std::ifstream in(filePath.c_str());
    if (!in.is_open()) {
        std::cerr << "Failed to open room file: " << filePath.c_str() << std::endl;
        return;
    }

    free(); 

    char typeBuf[32];

    while (in.peek() != EOF) {
        in.getline(typeBuf, 32, ',');
        Room* room = createRoomByType(MyString(typeBuf));
        if (!room) {
            std::cerr << "Unknown room type: " << typeBuf << " – skipping line.\n";
            in.ignore(1024, '\n');
            continue;
        }

        if (in.good()) {
            room->read(in);
            rooms.push_back(room);
            in.ignore(1);
        }
        else {
            delete room;
            std::cerr << "Error reading room – skipping line.\n";
            in.clear();
            in.ignore(1024, '\n');
        }
    }

    in.close();
}


void RoomManager::saveRooms() const {
    std::ofstream out(filePath.c_str());
    if (!out.is_open()) {
        std::cerr << "Failed to write to: " << filePath.c_str() << std::endl;
        return;
    }

    for (size_t i = 0; i < rooms.getSize(); i++) {
        out << *rooms[i] << '\n';
    }

    out.close();
}

bool RoomManager::addRoom(Room* room) {
    if (findRoom(room->getRoomNumber()) != nullptr) {
        std::cerr << "Room already exists.\n";
        return false;
    }
    rooms.push_back(room->clone());
    saveRooms();
    return true;
}

Room* RoomManager::findRoom(int roomNumber) const {
    for (size_t i = 0; i < rooms.getSize(); i++) {
        if (rooms[i]->getRoomNumber() == roomNumber)
            return rooms[i];
    }
    return nullptr;
}

bool RoomManager::changeRoomStatus(int roomNumber, const MyString& newStatus) {
    Room* room = findRoom(roomNumber);
    if (!room) {
        std::cerr << "Room not found.\n";
        return false;
    }
    room->setStatus(newStatus);
    saveRooms();
    return true;
}

void RoomManager::printAllRooms() const {
    for (size_t i = 0; i < rooms.getSize(); i++) {
        std::cout << *rooms[i] << std::endl;
    }
}
