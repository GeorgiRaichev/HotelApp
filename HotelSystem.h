#pragma once

#include "UserManager.h"
#include "RoomManager.h"
#include "GuestManager.h"
#include "ReservationManager.h"
#include "ActionLogger.h"

class HotelSystem {
private:
    bool isWeekend = false;
    bool isHighSeason = false;
    UserManager userManager;
    RoomManager roomManager;
    GuestManager guestManager;
    ReservationManager reservationManager;
    ActionLogger logger;

    const User* currentUser;

    bool hasAccess(const char* requiredRole) const;
    void runCommand(int command);

public:
    HotelSystem();

    void start(); 
    void run(); 

    void displayMenu() const;

    void handleAddRoom();
    void handleAddGuest();
	void handleMakeReservation();
	void handleShowRooms();
    void handleShowGuests();
    void handleShowReservations();
    void handleChangePricingStrategy();
    void handleChangeRoomStatus();
};
