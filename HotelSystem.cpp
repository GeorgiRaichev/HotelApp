#include "HotelSystem.h"
#include <iostream>
#include <cstring>
#include <limits>

HotelSystem::HotelSystem()
    : currentUser(nullptr) {
}

bool HotelSystem::hasAccess(const char* requiredRole) const {
    if (!currentUser) return false;
    const MyString& userRole = currentUser->getRole();
    return userRole == requiredRole || userRole == "manager";
}

void HotelSystem::start() {
    userManager.loadUsers();
    roomManager.loadRooms();
    guestManager.loadGuests();
    reservationManager.loadReservations();

    std::cout << "Welcome to Hotel Management System\n";

    while (!currentUser) {
        std::cout << "1. Login\n2. Register\n0. Exit\n> ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            MyString username, password;
            std::cout << "Username: "; std::cin >> username;
            std::cout << "Password: "; std::cin >> password;

            const User* user = userManager.login(username, password);
            if (user) {
                currentUser = user;
                logger.logAction(username, user->getRole(), "Logged in");
                std::cout << "Login successful!\n";
                break;
            }
            else {
                std::cout << "Invalid credentials.\n";
            }

        }
        else if (choice == 2) {
            MyString username, password, role;
            std::cout << "Choose username: "; std::cin >> username;
            std::cout << "Choose password: "; std::cin >> password;
            std::cout << "Role (manager / receptionist / accountant): "; std::cin >> role;

            if (userManager.registerUser(username, password, role)) {
                std::cout << "Registered successfully. Please login.\n";
            }
            else {
                std::cout << "Registration failed.\n";
            }

        }
        else if (choice == 0) {
            std::exit(0);
        }
        else {
            std::cout << "Invalid option.\n";
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void HotelSystem::run() {
    int command = -1;
    while (command != 0) {
        displayMenu();
        std::cin >> command;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        runCommand(command);
    }

    std::cout << "Saving data...\n";
    userManager.saveUsers();
    roomManager.saveRooms();
    guestManager.saveGuests();
    reservationManager.saveReservations();
    std::cout << "Goodbye!\n";
}

void HotelSystem::displayMenu() const {
    std::cout << "\n--- MENU ---\n";

    std::cout << "1. Add room" << (hasAccess("manager") ? "" : " (access denied)") << "\n";
    std::cout << "2. Add guest" << (hasAccess("receptionist") ? "" : " (access denied)") << "\n";
    std::cout << "3. Make reservation" << (hasAccess("receptionist") ? "" : " (access denied)") << "\n";
    std::cout << "4. Show rooms\n";
    std::cout << "5. Show guests\n";
    std::cout << "6. Show reservations\n";
    std::cout << "7. Change pricing strategy" << (hasAccess("manager") ? "" : " (access denied)") << "\n";
    std::cout << "8. Change room status" << (hasAccess("manager") ? "" : " (access denied)") << "\n";
    std::cout << "9. View total revenue" << (hasAccess("accountant") ? "" : " (access denied)") << "\n";
    std::cout << "10. View revenue by room type" << (hasAccess("accountant") ? "" : " (access denied)") << "\n";
    std::cout << "11. Export revenue report to file" << (hasAccess("accountant") ? "" : " (access denied)") << "\n";
    std::cout << "12. View most booked room" << (hasAccess("accountant") ? "" : " (access denied)") << "\n";
    std::cout << "13. View action log" << (hasAccess("manager") ? "" : " (access denied)") << "\n";
    std::cout << "14. Cancel reservation" << (hasAccess("receptionist") ? "" : " (access denied)") << "\n";
    std::cout << "15. View reservations sorted by date\n";
    std::cout << "16. View revenue by room type (sorted)" << (hasAccess("accountant") ? "" : " (access denied)") << "\n";
    std::cout << "17. Find object by ID\n";
    std::cout << "18. View revenue for selected period" << (hasAccess("accountant") ? "" : " (access denied)") << "\n";
    std::cout << "19. View rooms sorted by occupancy" << (hasAccess("accountant") ? "" : " (access denied)") << "\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

// TODO: Implement runCommand(...) per case
void HotelSystem::runCommand(int command) {
    switch (command) {
    case 0:
        logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Exited system");
        break;
    default:
        std::cout << "Functionality not yet implemented.\n";
        break;
    }
}


void HotelSystem::handleAddRoom() {
    if (!hasAccess("manager")) {
        std::cout << "Access denied.\n";
        return;
    }

    char typeBuf[32];
    int number;
    double price;

    std::cout << "Enter room type (single/double/luxury/conference/apartment): ";
    std::cin >> typeBuf;

    std::cout << "Enter room number: ";
    std::cin >> number;

    std::cout << "Enter base price: ";
    std::cin >> price;

    Room* room = nullptr;

    if (strcmp(typeBuf, "single") == 0)
        room = new SingleRoom(number, "free", price);
    else if (strcmp(typeBuf, "double") == 0)
        room = new DoubleRoom(number, "free", price);
    else if (strcmp(typeBuf, "luxury") == 0)
        room = new LuxuryRoom(number, "free", price);
    else if (strcmp(typeBuf, "conference") == 0)
        room = new ConferenceRoom(number, "free", price);
    else if (strcmp(typeBuf, "apartment") == 0)
        room = new Apartment(number, "free", price);
    else {
        std::cout << "Invalid room type.\n";
        return;
    }

    if (roomManager.addRoom(room)) {
        std::cout << "Room added successfully.\n";
        logger.logAction(currentUser->getUsername(), currentUser->getRole(), MyString("Added room #") + number);
    }
    else {
        std::cout << "Failed to add room.\n";
    }

    delete room;
}

void HotelSystem::handleAddGuest() {
    if (!hasAccess("receptionist")) {
        std::cout << "Access denied.\n";
        return;
    }

    char nameBuf[128], phoneBuf[64], emailBuf[128], statusBuf[32];

    std::cout << "Enter guest name: ";
    std::cin.getline(nameBuf, 128);

    std::cout << "Enter phone: ";
    std::cin.getline(phoneBuf, 64);

    std::cout << "Enter email: ";
    std::cin.getline(emailBuf, 128);

    std::cout << "Enter status (regular / gold / platinum): ";
    std::cin.getline(statusBuf, 32);

    if (guestManager.addGuest(nameBuf, phoneBuf, emailBuf, statusBuf)) {
        std::cout << "Guest added successfully.\n";
        logger.logAction(currentUser->getUsername(), currentUser->getRole(), MyString("Added guest: ") + nameBuf);
    }
    else {
        std::cout << "Failed to add guest.\n";
    }
}

void HotelSystem::handleMakeReservation() {
    if (!hasAccess("receptionist")) {
        std::cout << "Access denied.\n";
        return;
    }

    int guestID, roomNumber;
    Date startDate, endDate;

    std::cout << "Enter guest ID: ";
    std::cin >> guestID;

    const Guest* guest = guestManager.findGuestByID(guestID);
    if (!guest) {
        std::cout << "Guest not found.\n";
        return;
    }

    std::cout << "Enter room number: ";
    std::cin >> roomNumber;

    Room* room = roomManager.findRoom(roomNumber);
    if (!room) {
        std::cout << "Room not found.\n";
        return;
    }

    std::cout << "Enter start date (dd.mm.yyyy): ";
    std::cin >> startDate;

    std::cout << "Enter end date (dd.mm.yyyy): ";
    std::cin >> endDate;

    if (!startDate.isValid() || !endDate.isValid() || !(startDate < endDate)) {
        std::cout << "Invalid date range.\n";
        return;
    }

    if (!reservationManager.isRoomAvailable(roomNumber, startDate, endDate)) {
        std::cout << "Room is not available for the selected period.\n";
        return;
    }

    int nights = Reservation(0, 0, 0, startDate, endDate, 0).getNights();
    bool isWeekend = false;      
    bool isHighSeason = false;  

    double price = room->calculatePrice(isWeekend, isHighSeason, nights);

    if (reservationManager.addReservation(guestID, roomNumber, startDate, endDate, price)) {
        std::cout << "Reservation created successfully. Total price: " << price << "\n";
        logger.logAction(currentUser->getUsername(), currentUser->getRole(), MyString("Created reservation for guest ID ") + guestID);
    }
    else {
        std::cout << "Failed to create reservation.\n";
    }
}

void HotelSystem::handleShowRooms() {
    std::cout << "\n--- All Rooms ---\n";
    roomManager.printAllRooms();
    logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed all rooms");
}
void HotelSystem::handleShowGuests() {
    std::cout << "\n--- All Guests ---\n";
    guestManager.printAllGuests();
    logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed all guests");
}
void HotelSystem::handleShowReservations() {
    std::cout << "\n--- All Reservations ---\n";
    reservationManager.printAllReservations();
    logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed all reservations");
}
void HotelSystem::handleChangePricingStrategy() {
    if (!hasAccess("manager")) {
        std::cout << "Access denied.\n";
        return;
    }

    int weekendFlag, seasonFlag;

    std::cout << "Enable weekend pricing? (1 = yes, 0 = no): ";
    std::cin >> weekendFlag;

    std::cout << "Enable high-season pricing? (1 = yes, 0 = no): ";
    std::cin >> seasonFlag;

    isWeekend = (weekendFlag == 1);
    isHighSeason = (seasonFlag == 1);

    std::cout << "Pricing strategy updated.\n";
    logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Changed pricing strategy");
}
void HotelSystem::handleChangeRoomStatus() {
    if (!hasAccess("manager")) {
        std::cout << "Access denied.\n";
        return;
    }

    int roomNumber;
    char statusBuf[64];

    std::cout << "Enter room number: ";
    std::cin >> roomNumber;
    std::cin.ignore();

    std::cout << "Enter new status (free / reserved / under_maintenance): ";
    std::cin.getline(statusBuf, 64);

    if (roomManager.changeRoomStatus(roomNumber, statusBuf)) {
        std::cout << "Room status updated successfully.\n";
        logger.logAction(currentUser->getUsername(), currentUser->getRole(), MyString("Changed status of room #") + roomNumber);
    }
    else {
        std::cout << "Failed to update room status.\n";
    }
}







