#include "Room.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

bool Room::isAvailableForDates(const std::string& checkIn, const std::string& checkOut) const {
    if (booked) return false;
    // Check no overlap with existing slots
    for (const auto& slot : bookingSlots) {
        // Overlap if: newStart < existingEnd AND newEnd > existingStart
        if (checkIn < slot.second && checkOut > slot.first) {
            return false;
        }
    }
    return true;
}

void Room::addBookingSlot(const std::string& checkIn, const std::string& checkOut) {
    bookingSlots.push_back({checkIn, checkOut});
}

void Room::removeBookingSlot(const std::string& checkIn, const std::string& checkOut) {
    bookingSlots.erase(
        std::remove_if(bookingSlots.begin(), bookingSlots.end(),
                       [&](const std::pair<std::string,std::string>& s) {
                           return s.first == checkIn && s.second == checkOut;
                       }),
        bookingSlots.end());
}

void Room::displayRow() const {
    std::cout << std::left
              << std::setw(8)  << id
              << std::setw(14) << type
              << std::setw(10) << bedType
              << "$" << std::setw(10) << std::fixed << std::setprecision(2) << price
              << std::setw(6)  << capacity
              << std::setw(7)  << floor
              << std::setw(10) << view
              << std::setw(8)  << std::fixed << std::setprecision(1) << rating
              << std::setw(9) << (booked ? "Booked" : "Free")
              << amenities.substr(0, 20) << "\n";
}

void Room::displayFull() const {
    Utility::printLine();
    std::cout << "Room ID    : " << id        << "\n"
              << "Type       : " << type      << "\n"
              << "Bed Type   : " << bedType   << "\n"
              << "AC Type    : " << acType    << "\n"
              << "Price/Night: $" << std::fixed << std::setprecision(2) << price << "\n"
              << "Capacity   : " << capacity  << " persons\n"
              << "Floor      : " << floor     << "\n"
              << "View       : " << view      << "\n"
              << "Rating     : " << std::fixed << std::setprecision(1) << rating << " \u2605\n"
              << "Amenities  : " << amenities << "\n"
              << "Status     : " << (booked ? "Booked" : "Available") << "\n";
    Utility::printLine();
}
