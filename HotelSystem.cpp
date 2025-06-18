#include "HotelSystem.h"
#include <iostream>
#include <cstring>
#include <limits>
#include <fstream>
#include <filesystem> 

void HotelSystem::createDataFiles() {
	std::filesystem::path current = std::filesystem::current_path();
	std::filesystem::path dataPath = current / "data";

	std::filesystem::create_directory(dataPath);

	const char* filenames[] = {
		"logs.txt", "users.txt", "rooms.txt", "guests.txt", "reservations.txt"
	};

	for (const char* name : filenames) {
		std::filesystem::path filePath = dataPath / name;

		std::ifstream in(filePath);
		if (!in.good()) {
			std::ofstream out(filePath);
			out.close();
			std::cout << "Created: " << filePath << "\n";
		}
	}
}


HotelSystem::HotelSystem() : currentUser(nullptr) {
	createDataFiles();
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

		if (!(std::cin >> choice)) {
			std::cin.clear(); 
			std::cin.ignore(1024, '\n');
			std::cout << "Invalid input. Please enter a number (0–2).\n";
			continue;
		}

		std::cin.ignore(); 

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
			std::cout << "Invalid choice. Try again.\n";
		}
	}
}

void HotelSystem::run() {
	int command = -1;
	while (command != 0) {
		displayMenu();
		std::cin >> command;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		try {
			runCommand(command);
		}
		catch (...) {
			std::cout << "An error occurred while executing the command.\n";
		}
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

void HotelSystem::runCommand(int command) {
	switch (command) {
	case 0:
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Exited system");
		break;
	case 1: handleAddRoom(); break;
	case 2: handleAddGuest(); break;
	case 3: handleMakeReservation(); break;
	case 4: handleShowRooms(); break;
	case 5: handleShowGuests(); break;
	case 6: handleShowReservations(); break;
	case 7: handleChangePricingStrategy(); break;
	case 8: handleChangeRoomStatus(); break;
	case 9: handleViewTotalRevenue(); break;
	case 10: handleRevenueByRoomType(); break;
	case 11: handleExportRevenueReport(); break;
	case 12: handleMostBookedRoom(); break;
	case 13: handleViewActionLog(); break;
	case 14: handleCancelReservation(); break;
	case 15: handleSortedReservations(); break;
	case 16: handleSortedRevenueByType(); break;
	case 17: handleFindByID(); break;
	case 18: handleRevenueForPeriod(); break;
	case 19: handleRoomsByOccupancy(); break;
	default:
		std::cout << "Invalid option.\n";
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

	while (true) {
		std::cout << "Enter room type (single/double/luxury/conference/apartment): ";
		std::cin >> typeBuf;

		if (
			strcmp(typeBuf, "single") == 0 ||
			strcmp(typeBuf, "double") == 0 ||
			strcmp(typeBuf, "luxury") == 0 ||
			strcmp(typeBuf, "conference") == 0 ||
			strcmp(typeBuf, "apartment") == 0
			) {
			break;
		}
		std::cout << "Invalid room type. Try again.\n";
	}

	while (true) {
		std::cout << "Enter room number: ";
		std::cin >> number;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Invalid input. Please enter a valid room number.\n";
		}
		else {
			break;
		}
	}

	while (true) {
		std::cout << "Enter base price: ";
		std::cin >> price;

		if (std::cin.fail() || price < 0) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Invalid input. Please enter a valid non-negative price.\n";
		}
		else {
			break;
		}
	}

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

	while (true) {
		std::cout << "Enter guest name: ";
		std::cin.getline(nameBuf, 128);
		if (strlen(nameBuf) == 0) {
			std::cout << "Name cannot be empty. Try again.\n";
		}
		else {
			break;
		}
	}

	while (true) {
		std::cout << "Enter phone: ";
		std::cin.getline(phoneBuf, 64);
		if (strlen(phoneBuf) < 6) {
			std::cout << "Phone number too short. Try again.\n";
		}
		else {
			break;
		}
	}

	while (true) {
		std::cout << "Enter email: ";
		std::cin.getline(emailBuf, 128);
		if (strchr(emailBuf, '@') == nullptr || strchr(emailBuf, '.') == nullptr) {
			std::cout << "Invalid email format. Try again.\n";
		}
		else {
			break;
		}
	}

	while (true) {
		std::cout << "Enter status (regular / gold / platinum): ";
		std::cin.getline(statusBuf, 32);

		if (
			strcmp(statusBuf, "regular") == 0 ||
			strcmp(statusBuf, "gold") == 0 ||
			strcmp(statusBuf, "platinum") == 0
			) {
			break;
		}
		std::cout << "Invalid status. Please enter one of: regular / gold / platinum\n";
	}

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

	const Guest* guest = nullptr;
	while (true) {
		std::cout << "Enter guest ID: ";
		std::cin >> guestID;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Invalid input. Please enter a valid guest ID.\n";
			continue;
		}

		guest = guestManager.findGuestByID(guestID);
		if (!guest) {
			std::cout << "Guest not found. Try again.\n";
		}
		else {
			break;
		}
	}

	Room* room = nullptr;
	while (true) {
		std::cout << "Enter room number: ";
		std::cin >> roomNumber;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Invalid input. Please enter a valid room number.\n";
			continue;
		}

		room = roomManager.findRoom(roomNumber);
		if (!room) {
			std::cout << "Room not found. Try again.\n";
		}
		else {
			break;
		}
	}

	while (true) {
		std::cout << "Enter start date (dd.mm.yyyy): ";
		std::cin >> startDate;

		if (!startDate.isValid()) {
			std::cout << "Invalid start date. Try again.\n";
		}
		else {
			break;
		}
	}

	while (true) {
		std::cout << "Enter end date (dd.mm.yyyy): ";
		std::cin >> endDate;

		if (!endDate.isValid()) {
			std::cout << "Invalid end date. Try again.\n";
		}
		else if (!(startDate < endDate)) {
			std::cout << "End date must be after start date. Try again.\n";
		}
		else {
			break;
		}
	}

	if (!reservationManager.isRoomAvailable(roomNumber, startDate, endDate)) {
		std::cout << "Room is not available for the selected period.\n";
		return;
	}

	int nights = Reservation(0, 0, 0, startDate, endDate, 0).getNights();

	bool isWeekend = false;
	bool isHighSeason = false;

	Date current = startDate;
	while (current <= endDate) {
		int dow = current.dayOfWeek(); 
		if (dow == 0 || dow == 6) {
			isWeekend = true;
		}
		if (current.getMonth() == 7 || current.getMonth() == 8) {
			isHighSeason = true;
		}
		++current;
	}


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

	while (true) {
		std::cout << "Enable weekend pricing? (1 = yes, 0 = no): ";
		std::cin >> weekendFlag;

		if (std::cin.fail() || (weekendFlag != 0 && weekendFlag != 1)) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Invalid input. Please enter 1 or 0.\n";
			continue;
		}
		break;
	}

	while (true) {
		std::cout << "Enable high-season pricing? (1 = yes, 0 = no): ";
		std::cin >> seasonFlag;

		if (std::cin.fail() || (seasonFlag != 0 && seasonFlag != 1)) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Invalid input. Please enter 1 or 0.\n";
			continue;
		}
		break;
	}

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

	while (true) {
		std::cout << "Enter room number: ";
		std::cin >> roomNumber;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Invalid input. Please enter a valid room number.\n";
			continue;
		}

		if (!roomManager.findRoom(roomNumber)) {
			std::cout << "Room not found. Try again.\n";
			continue;
		}

		std::cin.ignore();
		break;
	}

	while (true) {
		std::cout << "Enter new status (free / reserved / under_maintenance): ";
		std::cin.getline(statusBuf, 64);

		if (
			strcmp(statusBuf, "free") == 0 ||
			strcmp(statusBuf, "reserved") == 0 ||
			strcmp(statusBuf, "under_maintenance") == 0
			) {
			break;
		}

		std::cout << "Invalid status. Please enter one of: free / reserved / under_maintenance\n";
	}

	if (roomManager.changeRoomStatus(roomNumber, statusBuf)) {
		std::cout << "Room status updated successfully.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), MyString("Changed status of room #") + roomNumber);
	}
	else {
		std::cout << "Failed to update room status.\n";
	}
}

void HotelSystem::handleViewTotalRevenue() {
	if (!hasAccess("accountant")) {
		std::cout << "Access denied.\n";
		return;
	}

	double total = 0.0;
	const MyVector<Reservation>& all = reservationManager.getAllReservations();

	if (all.getSize() == 0) {
		std::cout << "No reservations found. Revenue is 0.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed total revenue (0 reservations)");
		return;
	}

	for (size_t i = 0; i < all.getSize(); i++) {
		total += all[i].getTotalPrice();
	}

	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Total revenue: " << total << "\n";

	logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed total revenue");
}

void HotelSystem::handleRevenueByRoomType() {
	if (!hasAccess("accountant")) {
		std::cout << "Access denied.\n";
		return;
	}

	double single = 0, dbl = 0, lux = 0, conf = 0, apt = 0;
	const MyVector<Reservation>& all = reservationManager.getAllReservations();

	if (all.getSize() == 0) {
		std::cout << "No reservations found. Revenue by type is zero.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed revenue by room type (0 reservations)");
		return;
	}

	for (size_t i = 0; i < all.getSize(); i++) {
		const Reservation& r = all[i];
		Room* room = roomManager.findRoom(r.getRoomNumber());
		if (!room) continue;

		MyString type = room->getType();
		if (type == "single") single += r.getTotalPrice();
		else if (type == "double") dbl += r.getTotalPrice();
		else if (type == "luxury") lux += r.getTotalPrice();
		else if (type == "conference") conf += r.getTotalPrice();
		else if (type == "apartment") apt += r.getTotalPrice();
	}

	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Revenue by type:\n";
	std::cout << "Single:     " << single << "\n";
	std::cout << "Double:     " << dbl << "\n";
	std::cout << "Luxury:     " << lux << "\n";
	std::cout << "Conference: " << conf << "\n";
	std::cout << "Apartment:  " << apt << "\n";

	logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed revenue by room type");
}

void HotelSystem::handleExportRevenueReport() {
	if (!hasAccess("accountant")) {
		std::cout << "Access denied.\n";
		return;
	}

	const MyVector<Reservation>& all = reservationManager.getAllReservations();
	if (all.getSize() == 0) {
		std::cout << "No reservations to export. Report not created.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Tried to export empty revenue report");
		return;
	}

	std::ofstream out("data/revenue_report.txt");
	if (!out.is_open()) {
		std::cout << "Failed to open file for writing: data/revenue_report.txt\n";
		return;
	}

	bool success = true;
	for (size_t i = 0; i < all.getSize(); i++) {
		out << all[i] << '\n';
		if (!out) {
			success = false;
			break;
		}
	}

	out.close();

	if (success) {
		std::cout << "Report exported to data/revenue_report.txt\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Exported revenue report");
	}
	else {
		std::cout << "An error occurred while writing the report.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Failed to fully export revenue report");
	}
}

void HotelSystem::handleMostBookedRoom() {
	if (!hasAccess("accountant")) {
		std::cout << "Access denied.\n";
		return;
	}

	const MyVector<Reservation>& all = reservationManager.getAllReservations();
	if (all.getSize() == 0) {
		std::cout << "No reservations found.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed most booked room (0 reservations)");
		return;
	}

	int maxRoomNumber = 0;
	for (size_t i = 0; i < all.getSize(); i++) {
		int rn = all[i].getRoomNumber();
		if (rn > maxRoomNumber) {
			maxRoomNumber = rn;
		}
	}

	if (maxRoomNumber > 10000) {
		std::cout << "Room number exceeds limit. Cannot calculate.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Failed to view most booked room (too many rooms)");
		return;
	}

	int* roomBookings = new int[maxRoomNumber + 1]();
	int maxRoom = -1, maxCount = 0;

	for (size_t i = 0; i < all.getSize(); i++) {
		int rn = all[i].getRoomNumber();
		if (rn < 0 || rn > maxRoomNumber) continue;

		roomBookings[rn]++;
		if (roomBookings[rn] > maxCount) {
			maxCount = roomBookings[rn];
			maxRoom = rn;
		}
	}

	if (maxRoom != -1)
		std::cout << "Most booked room is #" << maxRoom << " with " << maxCount << " bookings.\n";
	else
		std::cout << "No valid room bookings found.\n";

	delete[] roomBookings;

	logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed most booked room");
}

void HotelSystem::handleViewActionLog() {
	if (!hasAccess("manager")) {
		std::cout << "Access denied.\n";
		return;
	}

	std::ifstream in("data/logs.txt");
	if (!in.is_open()) {
		std::cout << "Log file not found. Please make sure 'data/logs.txt' exists.\n";
		return;
	}

	in.seekg(0, std::ios::end);
	if (in.tellg() == 0) {
		std::cout << "Log file is empty.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed empty action log");
		in.close();
		return;
	}
	in.seekg(0, std::ios::beg); 

	std::cout << "\n--- Action Log ---\n";
	char line[256];
	while (in.getline(line, 256)) {
		std::cout << line << '\n';
	}

	in.close();
	logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed action log");
}

void HotelSystem::handleCancelReservation() {
	if (!hasAccess("receptionist")) {
		std::cout << "Access denied.\n";
		return;
	}

	int resID;

	while (true) {
		std::cout << "Enter reservation ID to cancel: ";
		std::cin >> resID;

		if (std::cin.fail() || resID <= 0) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Invalid reservation ID. Please enter a positive number.\n";
			continue;
		}

		std::cin.ignore(); 
		break;
	}

	if (reservationManager.cancelReservation(resID)) {
		std::cout << "Reservation canceled successfully.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), MyString("Canceled reservation #") + resID);
	}
	else {
		std::cout << "Reservation with ID #" << resID << " not found or already canceled.\n";
	}
}

void HotelSystem::handleSortedReservations() {
	if (!hasAccess("manager") && !hasAccess("receptionist") && !hasAccess("accountant")) {
		std::cout << "Access denied.\n";
		return;
	}

	const MyVector<Reservation>& all = reservationManager.getAllReservations();
	if (all.getSize() == 0) {
		std::cout << "No reservations found.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed sorted reservations (0 reservations)");
		return;
	}

	MyVector<Reservation> sorted = all;

	for (size_t i = 0; i < sorted.getSize(); i++) {
		size_t minIdx = i;
		for (size_t j = i + 1; j < sorted.getSize(); j++) {
			if (sorted[j].getStartDate() < sorted[minIdx].getStartDate()) {
				minIdx = j;
			}
		}
		if (minIdx != i) {
			Reservation tmp = sorted[i];
			sorted[i] = sorted[minIdx];
			sorted[minIdx] = tmp;
		}
	}

	std::cout << "\n--- Reservations Sorted by Start Date ---\n";
	for (size_t i = 0; i < sorted.getSize(); i++)
		std::cout << sorted[i] << '\n';

	logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed sorted reservations by date");
}


void HotelSystem::handleSortedRevenueByType() {
	if (!hasAccess("accountant")) {
		std::cout << "Access denied.\n";
		return;
	}

	struct TypeRevenue {
		MyString type;
		double revenue;
	};

	const int TYPE_COUNT = 5;
	TypeRevenue types[TYPE_COUNT] = {
		{"single", 0},
		{"double", 0},
		{"luxury", 0},
		{"conference", 0},
		{"apartment", 0}
	};

	const MyVector<Reservation>& all = reservationManager.getAllReservations();
	if (all.getSize() == 0) {
		std::cout << "No reservations found.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed sorted revenue by room type (0 reservations)");
		return;
	}
	for (size_t i = 0; i < all.getSize(); i++) {
		const Reservation& r = all[i];
		Room* room = roomManager.findRoom(r.getRoomNumber());
		if (!room) continue;

		MyString type = room->getType();
		for (int j = 0; j < TYPE_COUNT; j++) {
			if (types[j].type == type) {
				types[j].revenue += r.getTotalPrice();
				break;
			}
		}
	}

	for (int i = 0; i < TYPE_COUNT; i++) {
		for (int j = i + 1; j < TYPE_COUNT; j++) {
			if (types[j].revenue > types[i].revenue) {
				TypeRevenue temp = types[i];
				types[i] = types[j];
				types[j] = temp;
			}
		}
	}

	std::cout << std::fixed << std::setprecision(2);
	std::cout << "--- Revenue by room type (sorted descending) ---\n";
	for (int i = 0; i < TYPE_COUNT; i++) {
		std::cout << types[i].type.c_str() << ": " << types[i].revenue << " units\n";
	}

	logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed sorted revenue by room type");
}

void HotelSystem::handleFindByID() {
	int choice;

	while (true) {
		std::cout << "Find: 1. Guest  2. Reservation  3. Room\n> ";
		std::cin >> choice;

		if (std::cin.fail() || choice < 1 || choice > 3) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
			continue;
		}
		break;
	}

	int id;

	if (choice == 1) {
		while (true) {
			std::cout << "Enter guest ID: ";
			std::cin >> id;

			if (std::cin.fail() || id <= 0) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				std::cout << "Invalid ID. Try again.\n";
				continue;
			}
			break;
		}

		const Guest* g = guestManager.findGuestByID(id);
		if (g) std::cout << *g << "\n";
		else std::cout << "Guest not found.\n";
	}

	else if (choice == 2) {
		while (true) {
			std::cout << "Enter reservation ID: ";
			std::cin >> id;

			if (std::cin.fail() || id <= 0) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				std::cout << "Invalid ID. Try again.\n";
				continue;
			}
			break;
		}

		const Reservation* r = reservationManager.findByID(id);
		if (r) std::cout << *r << "\n";
		else std::cout << "Reservation not found.\n";
	}

	else if (choice == 3) {
		while (true) {
			std::cout << "Enter room number: ";
			std::cin >> id;

			if (std::cin.fail() || id <= 0) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				std::cout << "Invalid room number. Try again.\n";
				continue;
			}
			break;
		}

		Room* r = roomManager.findRoom(id);
		if (r) std::cout << *r << "\n";
		else std::cout << "Room not found.\n";
	}

	logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Searched object by ID");
}

void HotelSystem::handleRevenueForPeriod() {
	if (!hasAccess("accountant")) {
		std::cout << "Access denied.\n";
		return;
	}

	Date from, to;

	while (true) {
		std::cout << "Enter start date (dd.mm.yyyy): ";
		std::cin >> from;

		if (!from.isValid()) {
			std::cout << "Invalid start date. Try again.\n";
			continue;
		}
		break;
	}

	while (true) {
		std::cout << "Enter end date (dd.mm.yyyy): ";
		std::cin >> to;

		if (!to.isValid()) {
			std::cout << "Invalid end date. Try again.\n";
			continue;
		}
		if (!(from < to)) {
			std::cout << "End date must be after start date. Try again.\n";
			continue;
		}
		break;
	}

	const MyVector<Reservation>& all = reservationManager.getAllReservations();
	if (all.getSize() == 0) {
		std::cout << "No reservations found.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed revenue for period (0 reservations)");
		return;
	}

	double total = 0.0;

	for (size_t i = 0; i < all.getSize(); i++) {
		const Reservation& r = all[i];

		if (!(r.getEndDate() <= from || r.getStartDate() >= to)) {
			total += r.getTotalPrice();
		}
	}

	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Revenue from " << from << " to " << to << ": " << total << " units\n";

	logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed revenue for period");
}

void HotelSystem::handleRoomsByOccupancy() {
	if (!hasAccess("accountant")) {
		std::cout << "Access denied.\n";
		return;
	}

	struct RoomStat {
		int roomNumber;
		int count;
	};

	MyVector<RoomStat> stats;
	const MyVector<Reservation>& all = reservationManager.getAllReservations();

	if (all.getSize() == 0) {
		std::cout << "No reservations found.\n";
		logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed rooms sorted by occupancy (0 reservations)");
		return;
	}

	for (size_t i = 0; i < all.getSize(); i++) {
		int rn = all[i].getRoomNumber();
		bool found = false;
		for (size_t j = 0; j < stats.getSize(); j++) {
			if (stats[j].roomNumber == rn) {
				stats[j].count++;
				found = true;
				break;
			}
		}
		if (!found) {
			stats.push_back({ rn, 1 });
		}
	}

	for (size_t i = 0; i < stats.getSize(); i++) {
		size_t maxIdx = i;
		for (size_t j = i + 1; j < stats.getSize(); j++) {
			if (stats[j].count > stats[maxIdx].count) {
				maxIdx = j;
			}
		}
		if (maxIdx != i) {
			RoomStat tmp = stats[i];
			stats[i] = stats[maxIdx];
			stats[maxIdx] = tmp;
		}
	}

	std::cout << "--- Rooms sorted by occupancy ---\n";
	for (size_t i = 0; i < stats.getSize(); i++) {
		std::cout << "Room #" << stats[i].roomNumber << ": " << stats[i].count << " reservation"
			<< (stats[i].count != 1 ? "s" : "") << "\n";
	}

	logger.logAction(currentUser->getUsername(), currentUser->getRole(), "Viewed rooms sorted by occupancy");
}