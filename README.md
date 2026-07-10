#  Hotel Management System (C++)

A console-based **Hotel Management System** developed in **C++17** using Object-Oriented Programming (OOP) concepts and file handling. The system provides room management, customer management, booking, staff management, authentication, and persistent data storage.

---

##  Features

-  User Authentication (Admin, Manager, Receptionist)
-  Room Management
-  Customer Management
-  Room Booking
-  Check-In & Check-Out
-  Staff Management
-  Invoice Generation
-  File-Based Data Storage
-  Reports
-  Menu-Driven Console Interface

---

##  Technologies Used

- C++17
- Object-Oriented Programming (OOP)
- File Handling
- Standard Template Library (STL)
- GCC / MinGW
- Makefile

---

##  Project Structure

```text
HotelMS/
├── include/
│   ├── Utility.h
│   ├── Person.h
│   ├── Room.h
│   ├── Customer.h
│   ├── Staff.h
│   ├── Booking.h
│   ├── FileManager.h
│   └── HotelManagementSystem.h
│
├── src/
│   ├── main.cpp
│   ├── Utility.cpp
│   ├── Room.cpp
│   ├── Customer.cpp
│   ├── Staff.cpp
│   ├── FileManager.cpp
│   └── HotelManagementSystem.cpp
│
├── docs/
├── data/
├── Makefile
└── README.md
```

---

##  Compilation

### Windows (MinGW / Git Bash)

```bash
g++ -std=c++17 -Wall -O2 -Iinclude ^
src/main.cpp ^
src/Utility.cpp ^
src/Room.cpp ^
src/Customer.cpp ^
src/Staff.cpp ^
src/FileManager.cpp ^
src/HotelManagementSystem.cpp ^
-o HotelMS.exe

HotelMS.exe
```

### Linux / macOS

```bash
make
./HotelMS
```

### Clean Build

```bash
make clean && make
```

---

## 👥 Default Login Credentials

| Role | Username | Password |
|------|----------|----------|
| Administrator | admin | admin123 |
| Receptionist | receptionist | recep456 |
| Manager | manager | mgr789 |

---

##  Data Storage

The application automatically creates a **data/** folder on its first run.

This folder stores:

- Customer Information
- Room Records
- Booking Details
- Staff Records
- Generated Invoices
- System Counters

> **Note:** Do **not** manually edit `counters.txt`.

---

##  Documentation

The `docs/` folder includes:

- Project Report
- User Manual
- Project Synopsis
- Viva Questions & Answers
- Presentation Script
- UML Diagrams
- Sample Test Data

---

##  Compiler Requirements

- GCC 7+
- C++17 Support

Tested With:

- GCC 11
- GCC 12
- GCC 13
- Clang 14

---

##  Build Status

Compilation Result:

-  0 Errors
-  0 Warnings

---

##  Learning Objectives

This project demonstrates:

- Object-Oriented Programming (OOP)
- Inheritance
- Encapsulation
- Polymorphism
- File Handling
- Modular Programming
- Console Application Development

---

##  Author

**MD Imamul Hasan**
**Siam Md Shakil Ahmed**
**Hossain Md Pial**
**HASAN RATUL**
**IMANE TAHIRI ALAOUI**
Yunnan University

---

## 📄 License

This project is developed for educational and academic purposes only.
