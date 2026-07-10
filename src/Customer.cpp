#include "Customer.h"
#include "Utility.h"
#include <iomanip>

Customer::Customer()
    : Person(), bookingId(0), days(0), advancePayment(0.0),
      totalBill(0.0), numberOfGuests(1), checkedOut(false) {}

void Customer::displayInfo() const {
    // Phase 3: runtime polymorphism implementation
    std::cout << Color::BOLD << Color::CYAN
              << "[ CUSTOMER INFO ]" << Color::RESET << "\n";
    std::cout << "  Booking ID  : " << bookingId           << "\n"
              << "  Name        : " << name                << "\n"
              << "  Phone       : " << phone               << "\n"
              << "  Email       : " << email               << "\n"
              << "  Room        : " << roomId              << "\n"
              << "  Check-In    : " << checkInDate         << "\n"
              << "  Check-Out   : " << checkOutDate        << "\n"
              << "  Total Bill  : $"<< std::fixed << std::setprecision(2) << totalBill << "\n"
              << "  Status      : " << paymentStatus       << "\n";
}

void Customer::displayRow() const {
    double due = totalBill - advancePayment;
    std::cout << std::left
              << std::setw(8)  << bookingId
              << std::setw(22) << name.substr(0, 21)
              << std::setw(14) << phone
              << std::setw(8)  << roomId
              << std::setw(6)  << days
              << "$" << std::setw(10) << std::fixed << std::setprecision(2) << totalBill
              << "$" << std::setw(10) << advancePayment
              << "$" << std::setw(10) << due
              << paymentStatus << "\n";
}

void Customer::displayFull() const {
    Utility::printLine();
    std::cout << "BOOKING ID        : " << bookingId         << "\n"
              << "CHECK-IN DATE     : " << checkInDate       << "\n"
              << "CHECK-OUT DATE    : " << checkOutDate      << "\n";
    Utility::printLine();
    std::cout << "CUSTOMER INFORMATION\n";
    Utility::printLine();
    std::cout << "Name              : " << name              << "\n"
              << "Phone             : " << phone             << "\n"
              << "Email             : " << email             << "\n"
              << "Address           : " << address           << "\n"
              << "Nationality       : " << nationality       << "\n"
              << "ID Type           : " << idType            << "\n"
              << "ID Number         : " << nid               << "\n"
              << "Emergency Contact : " << emergencyContact  << "\n"
              << "Emergency Phone   : " << emergencyPhone    << "\n";
    Utility::printLine();
    std::cout << "ROOM INFORMATION\n";
    Utility::printLine();
    std::cout << "Room ID           : " << roomId            << "\n"
              << "Days              : " << days              << "\n"
              << "Guests            : " << numberOfGuests    << "\n";
    Utility::printLine();
    std::cout << "PAYMENT INFORMATION\n";
    Utility::printLine();
    std::cout << "Total Bill        : $" << std::fixed << std::setprecision(2) << totalBill      << "\n"
              << "Advance Paid      : $" << advancePayment  << "\n"
              << "Due Amount        : $" << (totalBill - advancePayment) << "\n"
              << "Payment Method    : " << paymentMethod     << "\n"
              << "Payment Status    : " << paymentStatus     << "\n";
    if (!specialRequests.empty()) {
        Utility::printLine();
        std::cout << "SPECIAL REQUESTS\n";
        Utility::printLine();
        std::cout << specialRequests << "\n";
    }
    Utility::printLine();
}
