#include "Staff.h"
#include "Utility.h"
#include <iomanip>
#include <iostream>

Staff::Staff()
    : Person(), staffId(0), salary(0.0), active(true) {}

void Staff::displayInfo() const {
    // Phase 3: runtime polymorphism
    std::cout << Color::BOLD << Color::YELLOW
              << "[ STAFF INFO ]" << Color::RESET << "\n";
    std::cout << "  Staff ID    : " << staffId   << "\n"
              << "  Name        : " << name      << "\n"
              << "  Position    : " << position  << "\n"
              << "  Department  : " << department<< "\n"
              << "  Phone       : " << phone     << "\n"
              << "  Shift       : " << shift     << "\n"
              << "  Salary      : $" << std::fixed << std::setprecision(2) << salary << "\n"
              << "  Join Date   : " << joinDate  << "\n"
              << "  Status      : " << (active ? "Active" : "Inactive") << "\n";
}

void Staff::displayRow() const {
    std::cout << std::left
              << std::setw(8)  << staffId
              << std::setw(22) << name.substr(0, 21)
              << std::setw(18) << position.substr(0, 17)
              << std::setw(15) << phone
              << std::setw(10) << shift
              << "$" << std::setw(10) << std::fixed << std::setprecision(2) << salary
              << joinDate << "\n";
}
