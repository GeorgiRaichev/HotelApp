Georgi Raychev 0MI0600451 SE 1ST GROUP

# Hotel Managing System

**Object-Oriented Programming Course Project**  
Sofia University "St. Kliment Ohridski", Faculty of Mathematics and Informatics  
Specialty: Software Engineering  
Academic Year: 2024–2025

---

## 📌 Project Overview

The **Hotel Managing System** is a fully-featured console application written in C++ that allows hotel managers, receptionists, and accountants to manage rooms, guests, reservations, and revenues. The system supports multiple user roles with controlled access and implements persistent storage through text files.

---

## ✅ Features

### 🏨 Room Management
- Support for multiple room types: Single, Double, Luxury, Conference, Apartment
- Easy extensibility for new room types
- Room status tracking: Free, Reserved, Under Maintenance
- Dynamic pricing strategies (seasonal, weekend, high demand)

### 👤 Guest Management
- Guest registration with full info: name, phone, email, client ID
- Automatic classification: Regular, Gold, Platinum
- Discount system based on loyalty status
- Guest reservation history and analytics

### 🗓️ Reservation Management
- Create and cancel reservations (room + period)
- Automatic price calculation based on type and duration
- Real-time availability check
- Support for multiple active and future reservations

### 🔐 User & Access Management
- User registration and login with role-based access (Manager, Receptionist, Accountant)
- Password authentication (stored securely in `users.txt`)
- Access control to system features based on role
- Action logging for every operation

### 💰 Revenue & Reporting
- Revenue calculation by day, month, and year
- Revenue breakdown by room type and occupancy
- Generate and export reports (to text files)
- Most profitable rooms and time periods

### 📁 Additional Functionality
- Automatic price increase for high occupancy/seasons
- Multiple pricing models with easy switching
- File storage for all objects: rooms, guests, reservations, users, revenues
- Load data on startup, save on exit
- Input validation with `try-catch` and meaningful error handling

---

- Language: **C++**
- Compilation: **Separate Compilation (modular headers and source files)**
- No STL or third-party libraries
- Input/Output: **Standard Console I/O**
- Persistence: **Text files** (`rooms.txt`, `guests.txt`, `reservations.txt`, `users.txt`, `logs.txt`)

