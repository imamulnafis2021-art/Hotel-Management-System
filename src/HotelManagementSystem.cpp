#include "HotelManagementSystem.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <regex>

using namespace std;

// ----------------------------------------------------------------
// CONSTRUCTOR & RUN
// ----------------------------------------------------------------
HotelManagementSystem::HotelManagementSystem() {
    FileManager::initCredentials();
    loadAll();
}

void HotelManagementSystem::run() {
    cout << Color::BOLD << Color::MAGENTA
    
         << "     WELCOME TO HOTEL DELIGHT MANAGEMENT SYSTEM\n"
         << Color::RESET;

    if (!loginScreen()) {
        Utility::printError("Login failed. Exiting.");
        return;
    }

    cout << Color::GREEN << "\nWelcome, " << currentUser
         << " [" << currentRole << "]" << Color::RESET << "\n";

    if (currentRole == "Admin")
        adminMenu();
    else if (currentRole == "Receptionist")
        receptionistMenu();
    else if (currentRole == "Manager")
        managerMenu();
    else
        adminMenu(); // fallback

    saveAll();
    Utility::printSuccess("All data saved. Goodbye!");
}

// ----------------------------------------------------------------
// HELPERS
// ----------------------------------------------------------------
void HotelManagementSystem::saveAll() const {
    FileManager::saveRooms(rooms);
    FileManager::saveCustomers(customers);
    FileManager::saveStaff(staffList);
    FileManager::savePayments(payments);
    FileManager::saveFoodOrders(foodOrders);
    FileManager::saveComplaints(complaints);
    FileManager::saveLaundry(laundryOrders);
    FileManager::saveTransport(transportBookings);
    FileManager::saveCounters(nextBookingId, nextStaffId, nextOrderId,
                               nextComplaintId, nextTaskId, nextPaymentId,
                               nextLaundryId, nextTransportId);
}

void HotelManagementSystem::loadAll() {
    FileManager::loadRooms(rooms);
    FileManager::loadCustomers(customers);
    FileManager::loadStaff(staffList);
    FileManager::loadPayments(payments);
    FileManager::loadFoodOrders(foodOrders);
    FileManager::loadComplaints(complaints);
    FileManager::loadLaundry(laundryOrders);
    FileManager::loadTransport(transportBookings);
    FileManager::loadCounters(nextBookingId, nextStaffId, nextOrderId,
                               nextComplaintId, nextTaskId, nextPaymentId,
                               nextLaundryId, nextTransportId);

    // Recalculate revenue from payments
    totalRevenue = 0; pendingRevenue = 0;
    for (const auto& p : payments) totalRevenue += p.amount;
    for (const auto& c : customers)
        pendingRevenue += (c.getTotalBill() - c.getAdvancePayment());
}

Payment HotelManagementSystem::makePayment(int bookingId, const string& customerName,
                                            double amount, const string& method) {
    Payment p;
    p.paymentId     = nextPaymentId++;
    p.bookingId     = bookingId;
    p.customerName  = customerName;
    p.amount        = amount;
    p.method        = method;
    p.date          = Utility::getCurrentDateTime();
    p.status        = "Completed";
    p.transactionId = Utility::generateTransactionId(p.paymentId);
    payments.push_back(p);
    totalRevenue += amount;
    return p;
}

const Customer* HotelManagementSystem::findCustomer(int bookingId) const {
    for (const auto& c : customers)
        if (c.getBookingId() == bookingId) return &c;
    return nullptr;
}

Customer* HotelManagementSystem::findCustomerMut(int bookingId) {
    for (auto& c : customers)
        if (c.getBookingId() == bookingId) return &c;
    return nullptr;
}

const Room* HotelManagementSystem::findRoom(const string& id) const {
    for (const auto& r : rooms)
        if (r.getId() == id) return &r;
    return nullptr;
}

Room* HotelManagementSystem::findRoomMut(const string& id) {
    for (auto& r : rooms)
        if (r.getId() == id) return &r;
    return nullptr;
}

// ----------------------------------------------------------------
// PHASE 5 - LOGIN SYSTEM
// ----------------------------------------------------------------
bool HotelManagementSystem::loginScreen() {
    Utility::printHeader("STAFF LOGIN");

    for (int attempt = 1; attempt <= 3; ++attempt) {
        string user, pass;
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> pass;
        Utility::clearInput();

        string role;
        if (FileManager::authenticate(user, pass, role)) {
            currentUser = user;
            currentRole = role;
            Utility::printSuccess("Login successful!");
            return true;
        }
        Utility::printError("Invalid credentials. Attempt " + to_string(attempt) + "/3");
    }
    return false;
}

// ----------------------------------------------------------------
// ROLE-BASED MENUS (Phase 5)
// ----------------------------------------------------------------
void HotelManagementSystem::adminMenu() {
    int choice;
    while (true) {
        cout << "\n" << Color::BOLD << Color::CYAN
             << "----------------------------------------------------------------\n"
             << "       HOTEL DELIGHT MANAGEMENT SYSTEM \n"
             << "----------------------------------------------------------------\n"
             << " ROOM MANAGEMENT\n"
             << "  1.  Add New Room          2.  View All Rooms\n"
             << "  3.  Available Rooms       4.  Modify Room\n"
             << "  5.  Delete Room           6.  Search Room\n"
             << "  7.  Sort Rooms            8.  Recommend Room\n"
             << " BOOKING MANAGEMENT\n"
             << "  9.  Book Room            10. View All Bookings\n"
             << " 11.  Booking Details      12. Checkout\n"
             << " 13.  Search Customer      14. Sort Customers\n"
             << " SERVICES\n"
             << " 15.  Place Food Order     16. View Food Orders\n"
             << " 17.  Register Complaint   18. View Complaints\n"
             << " 19.  Resolve Complaint    20. Laundry Service\n"
             << " 21.  View Laundry         22. Transport Booking\n"
             << " 23.  View Transport\n"
             << " STAFF MANAGEMENT\n"
             << " 24.  Add Staff            25. View Staff\n"
             << " 26.  Search Staff         27. Sort Staff\n"
             << " 28.  Remove Staff\n"
             << " REPORTS & ANALYTICS\n"
             << " 29.  Dashboard            30. Revenue Report\n"
             << " 31.  Advanced Dashboard\n"
             << "  0.  Exit\n"
             << "----------------------------------------------------------------\n"
             << Color::RESET
             << "Choice: ";

        if (!(cin >> choice)) { Utility::clearInput(); continue; }
        Utility::clearInput();

        switch (choice) {
            case  1: addRoom();               break;
            case  2: showRooms();             break;
            case  3: showAvailableRooms();    break;
            case  4: modifyRoom();            break;
            case  5: deleteRoom();            break;
            case  6: searchRoom();            break;
            case  7: sortRooms();             break;
            case  8: recommendRoom();         break;
            case  9: bookRoom();              break;
            case 10: viewBookings();          break;
            case 11: viewBookingDetails();    break;
            case 12: checkout();              break;
            case 13: searchCustomer();        break;
            case 14: sortCustomers();         break;
            case 15: placeFoodOrder();        break;
            case 16: viewFoodOrders();        break;
            case 17: registerComplaint();     break;
            case 18: viewComplaints();        break;
            case 19: resolveComplaint();      break;
            case 20: placeLaundryOrder();     break;
            case 21: viewLaundryOrders();     break;
            case 22: bookTransport();         break;
            case 23: viewTransportBookings(); break;
            case 24: addStaff();              break;
            case 25: showStaff();             break;
            case 26: searchStaff();           break;
            case 27: sortStaff();             break;
            case 28: removeStaff();           break;
            case 29: showDashboard();         break;
            case 30: showRevenueReport();     break;
            case 31: showAdvancedDashboard(); break;
            case  0:
                Utility::printSuccess("Thank you for using HOTEL DELIGHT HMS. Goodbye!");
                return;
            default:
                Utility::printError("Invalid choice!");
        }
        cout << "\n" << Color::CYAN << "Press ENTER to continue..." << Color::RESET;
        cin.get();
    }
}

void HotelManagementSystem::receptionistMenu() {
    int choice;
    while (true) {
        cout << "\n" << Color::BOLD << Color::GREEN
             << "----------------------------------------------------------------\n"
             << "     HOTEL DELIGHT HMS [RECEPTIONIST: " << currentUser << "]\n"
             << "----------------------------------------------------------------\n"
             << "  1. View Rooms          2. Book Room\n"
             << "  3. View Bookings       4. Booking Details\n"
             << "  5. Checkout            6. Place Food Order\n"
             << "  7. View Food Orders    8. Register Complaint\n"
             << "  9. Laundry Service    10. Transport Booking\n"
             << " 11. Dashboard\n"
             << "  0. Exit\n"
             << "----------------------------------------------------------------\n"
             << Color::RESET
             << "Choice: ";

        if (!(cin >> choice)) { Utility::clearInput(); continue; }
        Utility::clearInput();

        switch (choice) {
            case 1: showAvailableRooms(); break;
            case 2: bookRoom();           break;
            case 3: viewBookings();       break;
            case 4: viewBookingDetails(); break;
            case 5: checkout();           break;
            case 6: placeFoodOrder();     break;
            case 7: viewFoodOrders();     break;
            case 8: registerComplaint();  break;
            case 9: placeLaundryOrder();  break;
            case 10: bookTransport();     break;
            case 11: showDashboard();     break;
            case 0:
                Utility::printSuccess("Goodbye!");
                return;
            default:
                Utility::printError("Invalid choice!");
        }
        cout << "\n" << Color::CYAN << "Press ENTER to continue..." << Color::RESET;
        cin.get();
    }
}

void HotelManagementSystem::managerMenu() {
    int choice;
    while (true) {
        cout << "\n" << Color::BOLD << Color::YELLOW
             << "----------------------------------------------------------------\n"
             << "       HOTEL DELIGHT HMS [MANAGER: " << currentUser << "]\n"
             << "----------------------------------------------------------------\n"
             << "  1. Dashboard           2. Revenue Report\n"
             << "  3. Advanced Dashboard  4. View All Bookings\n"
             << "  5. View Staff          6. Add Staff\n"
             << "  7. View Complaints     8. Resolve Complaint\n"
             << "  9. Sort Rooms         10. Sort Staff\n"
             << "  0. Exit\n"
             << "----------------------------------------------------------------\n"
             << Color::RESET
             << "Choice: ";

        if (!(cin >> choice)) { Utility::clearInput(); continue; }
        Utility::clearInput();

        switch (choice) {
            case 1: showDashboard();         break;
            case 2: showRevenueReport();     break;
            case 3: showAdvancedDashboard(); break;
            case 4: viewBookings();          break;
            case 5: showStaff();             break;
            case 6: addStaff();              break;
            case 7: viewComplaints();        break;
            case 8: resolveComplaint();      break;
            case 9: sortRooms();             break;
            case 10: sortStaff();            break;
            case 0:
                Utility::printSuccess("Goodbye!");
                return;
            default:
                Utility::printError("Invalid choice!");
        }
        cout << "\n" << Color::CYAN << "Press ENTER to continue..." << Color::RESET;
        cin.get();
    }
}

// ----------------------------------------------------------------
// ROOM MANAGEMENT
// ----------------------------------------------------------------
void HotelManagementSystem::addRoom() {
    Utility::printHeader("ADD NEW ROOM");
    Room r;
    string id;
    cout << "Room ID (e.g., A101, C211): ";
    cin >> id; Utility::clearInput();
    id = Utility::toUpperStr(Utility::trim(id));

    if (id.empty()) { Utility::printError("Room ID cannot be empty!"); return; }
    if (findRoom(id)) { Utility::printError("Room ID already exists!"); return; }

    r.setId(id);

    cout << "Type (Single/Double/Suite/Deluxe/Presidential): ";
    string t; getline(cin, t); r.setType(Utility::trim(t));

    cout << "Bed Type (Single/Double/King/Queen): ";
    string bt; getline(cin, bt); r.setBedType(Utility::trim(bt));

    cout << "AC Type (Central/Split/Window/None): ";
    string at; getline(cin, at); r.setAcType(Utility::trim(at));

    double price = 0;
    do {
        cout << "Price per night: $";
        if (!(cin >> price)) { Utility::clearInput(); price = -1; }
    } while (!Utility::validatePositiveDouble(price));
    Utility::clearInput();
    r.setPrice(price);

    int cap = 0;
    do {
        cout << "Capacity (persons): ";
        if (!(cin >> cap)) { Utility::clearInput(); cap = -1; }
    } while (!Utility::validatePositiveInt(cap));
    Utility::clearInput();
    r.setCapacity(cap);

    cout << "Floor (Ground/1st/2nd/3rd): ";
    string fl; getline(cin, fl); r.setFloor(Utility::trim(fl));

    cout << "View (Sea/City/Garden/Mountain/Pool): ";
    string vw; getline(cin, vw); r.setView(Utility::trim(vw));

    double rating = 0;
    do {
        cout << "Rating (1.0-5.0): ";
        if (!(cin >> rating)) { Utility::clearInput(); rating = -1; }
    } while (rating < 1.0 || rating > 5.0);
    Utility::clearInput();
    r.setRating(rating);

    cout << "Amenities (WiFi,TV,AC,...): ";
    string am; getline(cin, am); r.setAmenities(Utility::trim(am));

    r.setBooked(false);
    rooms.push_back(r);
    saveAll();
    Utility::printSuccess("Room " + id + " added successfully!");
    r.displayFull();
}

void HotelManagementSystem::showRooms() const {
    Utility::printHeader("ROOM INVENTORY");
    if (rooms.empty()) { Utility::printError("No rooms in system."); return; }

    cout << left
         << setw(8)  << "ID"
         << setw(14) << "Type"
         << setw(10) << "Bed"
         << setw(10) << "Price"
         << setw(6)  << "Cap"
         << setw(8)  << "Floor"
         << setw(10) << "View"
         << setw(7)  << "Rating"
         << setw(10) << "Status"
         << "Amenities\n";
    Utility::printLine();

    for (const auto& r : rooms) r.displayRow();
    cout << "\nTotal: " << rooms.size() << " rooms.\n";
}

void HotelManagementSystem::showAvailableRooms() const {
    Utility::printHeader("AVAILABLE ROOMS");
    bool found = false;
    cout << left
         << setw(8)  << "ID"
         << setw(14) << "Type"
         << setw(10) << "Bed"
         << setw(10) << "Price"
         << setw(6)  << "Cap"
         << setw(8)  << "Floor"
         << setw(10) << "View"
         << "Rating\n";
    Utility::printLine();

    for (const auto& r : rooms) {
        if (!r.isBooked()) { r.displayRow(); found = true; }
    }
    if (!found) Utility::printError("No available rooms.");
}

void HotelManagementSystem::modifyRoom() {
    Utility::printHeader("MODIFY ROOM");
    string id;
    cout << "Enter Room ID: "; cin >> id; Utility::clearInput();
    id = Utility::toUpperStr(id);

    Room* r = findRoomMut(id);
    if (!r) { Utility::printError("Room not found!"); return; }

    r->displayFull();
    cout << "\nEnter new details (press ENTER to keep current):\n";

    auto update = [](const string& prompt, const string& cur, string& target) {
        cout << prompt << " [" << cur << "]: ";
        string inp; getline(cin, inp);
        if (!inp.empty()) target = Utility::trim(inp);
    };

    string tmp;
    update("Type", r->getType(), tmp); if (!tmp.empty()) r->setType(tmp); tmp.clear();
    update("Bed Type", r->getBedType(), tmp); if (!tmp.empty()) r->setBedType(tmp); tmp.clear();
    update("Floor", r->getFloor(), tmp); if (!tmp.empty()) r->setFloor(tmp); tmp.clear();
    update("View", r->getView(), tmp); if (!tmp.empty()) r->setView(tmp); tmp.clear();
    update("Amenities", r->getAmenities(), tmp); if (!tmp.empty()) r->setAmenities(tmp); tmp.clear();

    cout << "Price [" << r->getPrice() << "]: ";
    string p; getline(cin, p);
    try { if (!p.empty()) r->setPrice(stod(p)); } catch (...) {}

    cout << "Capacity [" << r->getCapacity() << "]: ";
    string c2; getline(cin, c2);
    try { if (!c2.empty()) r->setCapacity(stoi(c2)); } catch (...) {}

    cout << "Rating [" << r->getRating() << "]: ";
    string rat; getline(cin, rat);
    try { if (!rat.empty()) r->setRating(stod(rat)); } catch (...) {}

    saveAll();
    Utility::printSuccess("Room modified successfully!");
}

void HotelManagementSystem::deleteRoom() {
    Utility::printHeader("DELETE ROOM");
    string id;
    cout << "Enter Room ID: "; cin >> id; Utility::clearInput();
    id = Utility::toUpperStr(id);

    auto it = find_if(rooms.begin(), rooms.end(),
                      [&id](const Room& r){ return r.getId() == id; });
    if (it == rooms.end()) { Utility::printError("Room not found!"); return; }
    if (it->isBooked()) { Utility::printError("Cannot delete a currently booked room!"); return; }

    char confirm;
    cout << "Delete room " << id << "? (y/n): ";
    cin >> confirm; Utility::clearInput();

    if (tolower(confirm) == 'y') {
        rooms.erase(it);
        saveAll();
        Utility::printSuccess("Room " + id + " deleted.");
    } else {
        Utility::printInfo("Deletion cancelled.");
    }
}

// Phase 6 - Search Room
void HotelManagementSystem::searchRoom() const {
    Utility::printHeader("SEARCH ROOM");
    cout << "1. Search by Room ID\n2. Search by Type\n3. Search by Price Range\nChoice: ";
    int ch; cin >> ch; Utility::clearInput();

    if (ch == 1) {
        string id;
        cout << "Room ID: "; cin >> id; Utility::clearInput();
        id = Utility::toUpperStr(id);
        const Room* r = findRoom(id);
        if (r) r->displayFull();
        else Utility::printError("Room not found.");
    } else if (ch == 2) {
        string type;
        cout << "Room type: "; getline(cin, type);
        bool found = false;
        for (const auto& r : rooms)
            if (r.getType() == type) { r.displayRow(); found = true; }
        if (!found) Utility::printError("No rooms of that type.");
    } else if (ch == 3) {
        double lo, hi;
        cout << "Min price: $"; cin >> lo;
        cout << "Max price: $"; cin >> hi; Utility::clearInput();
        bool found = false;
        for (const auto& r : rooms)
            if (r.getPrice() >= lo && r.getPrice() <= hi) { r.displayRow(); found = true; }
        if (!found) Utility::printError("No rooms in that price range.");
    }
}

// Phase 7 - Sort Rooms
void HotelManagementSystem::sortRooms() {
    Utility::printHeader("SORT ROOMS");
    cout << "1. Sort by Price (Low→High)\n2. Sort by Price (High→Low)\n"
         << "3. Sort by Rating (High→Low)\n4. Sort by ID\nChoice: ";
    int ch; cin >> ch; Utility::clearInput();

    if (ch == 1)
        sort(rooms.begin(), rooms.end(), [](const Room& a, const Room& b){ return a.getPrice() < b.getPrice(); });
    else if (ch == 2)
        sort(rooms.begin(), rooms.end(), [](const Room& a, const Room& b){ return a.getPrice() > b.getPrice(); });
    else if (ch == 3)
        sort(rooms.begin(), rooms.end(), [](const Room& a, const Room& b){ return a.getRating() > b.getRating(); });
    else if (ch == 4)
        sort(rooms.begin(), rooms.end(), [](const Room& a, const Room& b){ return a.getId() < b.getId(); });

    Utility::printSuccess("Rooms sorted.");
    showRooms();
}

// Phase 13 - Room Recommendation
void HotelManagementSystem::recommendRoom() const {
    Utility::printHeader("SMART ROOM RECOMMENDATION");
    double budget;
    int guests;
    string preferredType;

    cout << "Your budget per night: $"; cin >> budget; Utility::clearInput();
    cout << "Number of guests: "; cin >> guests; Utility::clearInput();
    cout << "Preferred type (Single/Double/Suite/Deluxe/Presidential/Any): ";
    getline(cin, preferredType);

    bool any = (preferredType == "Any" || preferredType.empty());
    bool found = false;

    Utility::printLine();
    cout << "RECOMMENDED ROOMS:\n";
    Utility::printLine();

    for (const auto& r : rooms) {
        if (r.isBooked()) continue;
        if (r.getPrice() > budget) continue;
        if (r.getCapacity() < guests) continue;
        if (!any && r.getType() != preferredType) continue;

        r.displayFull();
        cout << Color::GREEN << "WHY THIS ROOM:\n"
             << "  - Price $" << fixed << setprecision(2) << r.getPrice()
             << " is within your budget of $" << budget << "\n"
             << "  - Capacity " << r.getCapacity() << " can accommodate " << guests << " guests\n"
             << "  - Rating: " << r.getRating() << " stars\n"
             << Color::RESET;
        found = true;
    }
    if (!found) Utility::printError("No rooms match your criteria. Try adjusting budget or guests.");
}

// ----------------------------------------------------------------
// BOOKING MANAGEMENT
// ----------------------------------------------------------------
void HotelManagementSystem::bookRoom() {
    Utility::printHeader("BOOK A ROOM - COMPLETE REGISTRATION");

    if (rooms.empty()) { Utility::printError("No rooms in system."); return; }

    showAvailableRooms();

    Customer c;

    // ---- Step 1: Personal Info ----
    cout << "\n" << Color::BOLD << Color::MAGENTA
         << "========== STEP 1: PERSONAL INFORMATION ==========" << Color::RESET << "\n";

    string name;
    cout << "Full Name: "; getline(cin, name);
    if (!Utility::validateName(name)) {
        Utility::printError("Invalid name!"); return;
    }
    c.setName(name);

    string phone;
    do {
        cout << "Phone (10-15 digits): ";
        getline(cin, phone);
        if (!Utility::validatePhone(phone))
            Utility::printError("Invalid phone number!");
    } while (!Utility::validatePhone(phone));
    c.setPhone(phone);

    string email;
    do {
        cout << "Email: ";
        getline(cin, email);
        if (!Utility::validateEmail(email))
            Utility::printError("Invalid email address!");
    } while (!Utility::validateEmail(email));
    c.setEmail(email);

    string address;
    cout << "Present Address: "; getline(cin, address); c.setAddress(address);

    string nat;
    cout << "Nationality: "; getline(cin, nat); c.setNationality(nat);

    string ec, ep;
    cout << "Emergency Contact Name: "; getline(cin, ec); c.setEmergencyContact(ec);
    do {
        cout << "Emergency Contact Phone: ";
        getline(cin, ep);
        if (!Utility::validatePhone(ep)) Utility::printError("Invalid phone!");
    } while (!Utility::validatePhone(ep));
    c.setEmergencyPhone(ep);

    // ---- Step 2: ID Verification ----
    cout << "\n" << Color::BOLD << Color::MAGENTA
         << "========== STEP 2: ID VERIFICATION ==========" << Color::RESET << "\n";
    cout << "1. National ID  2. Passport  3. Driving License  4. Birth Certificate\nChoice: ";
    int idCh; cin >> idCh; Utility::clearInput();

    string nid, idType;
    switch (idCh) {
        case 1: idType = "NID";
            do {
                cout << "NID Number: "; getline(cin, nid);
                if (!Utility::validateNID(nid)) Utility::printError("Invalid NID!");
            } while (!Utility::validateNID(nid));
            break;
        case 2: idType = "Passport"; cout << "Passport No: "; getline(cin, nid); break;
        case 3: idType = "Driving License"; cout << "License No: "; getline(cin, nid); break;
        case 4: idType = "Birth Certificate"; cout << "Certificate No: "; getline(cin, nid); break;
        default: idType = "NID"; cout << "NID Number: "; getline(cin, nid);
    }
    c.setNid(nid); c.setIdType(idType);

    // ---- Step 3: Room Selection ----
    cout << "\n" << Color::BOLD << Color::MAGENTA
         << "========== STEP 3: ROOM SELECTION ==========" << Color::RESET << "\n";
    string roomId;
    cout << "Enter Room ID: "; cin >> roomId; Utility::clearInput();
    roomId = Utility::toUpperStr(roomId);

    int days = 0;
    do {
        cout << "Number of days: ";
        if (!(cin >> days)) { Utility::clearInput(); days = 0; }
    } while (days <= 0);
    Utility::clearInput();

    int guests = 0;
    do {
        cout << "Number of guests: ";
        if (!(cin >> guests)) { Utility::clearInput(); guests = 0; }
    } while (guests <= 0);
    Utility::clearInput();

    Room* room = findRoomMut(roomId);
    if (!room || room->isBooked()) {
        Utility::printError("Room not available or invalid ID!");
        return;
    }
    if (guests > room->getCapacity()) {
        Utility::printError("Room capacity is " + to_string(room->getCapacity()) +
                            " but you have " + to_string(guests) + " guests!");
        return;
    }

    c.setRoomId(roomId);
    c.setDays(days);
    c.setNumberOfGuests(guests);

    // Phase 11 - date-based booking
    string checkIn = Utility::getCurrentDate();
    string checkOut = Utility::calculateCheckOutDate(checkIn, days);
    c.setCheckInDate(checkIn);
    c.setCheckOutDate(checkOut);

    // ---- Step 4: Special Requests ----
    cout << "\n" << Color::BOLD << Color::MAGENTA
         << "========== STEP 4: SPECIAL REQUESTS ==========" << Color::RESET << "\n";
    cout << "Special requests (press ENTER to skip): ";
    string req; getline(cin, req); c.setSpecialRequests(req);

    // ---- Step 5: Payment ----
    cout << "\n" << Color::BOLD << Color::MAGENTA
         << "========== STEP 5: PAYMENT ==========" << Color::RESET << "\n";

    double roomCharge   = room->getPrice() * days;
    double tax          = roomCharge * 0.12;
    double serviceCharge= roomCharge * 0.05;
    double vat          = roomCharge * 0.15;
    double total        = roomCharge + tax + serviceCharge + vat;

    cout << "\nBILL BREAKDOWN:\n";
    Utility::printLine();
    cout << fixed << setprecision(2);
    cout << "Room (" << days << " days x $" << room->getPrice() << ")  : $" << roomCharge << "\n"
         << "Tax (12%)                        : $" << tax << "\n"
         << "Service Charge (5%)              : $" << serviceCharge << "\n"
         << "VAT (15%)                        : $" << vat << "\n";
    Utility::printLine();
    cout << Color::BOLD << "TOTAL BILL                       : $" << total << Color::RESET << "\n\n";

    c.setTotalBill(total);

    cout << "Payment Method:\n"
         << "1. Cash  2. Card  3. Mobile Banking  4. Bank Transfer  5. International Card\n"
         << "Choice: ";
    int pmCh; cin >> pmCh; Utility::clearInput();
    string method;
    switch (pmCh) {
        case 1: method = "Cash";              break;
        case 2: method = "Card";              break;
        case 3: method = "Mobile Banking";    break;
        case 4: method = "Bank Transfer";     break;
        case 5: method = "International Card";break;
        default: method = "Cash";
    }
    c.setPaymentMethod(method);

    double minAdv = total * 0.30;
    Utility::printWarning("Minimum advance payment is 30% ($" + to_string(minAdv) + ")");
    double adv = 0;
    do {
        cout << "Advance payment: $";
        if (!(cin >> adv)) { Utility::clearInput(); adv = 0; }
        if (adv < minAdv) Utility::printError("Minimum is $" + to_string(minAdv));
        if (adv > total)  Utility::printError("Cannot exceed total bill!");
    } while (adv < minAdv || adv > total);
    Utility::clearInput();

    c.setAdvancePayment(adv);
    c.setPaymentStatus(adv >= total ? "Paid" : "Partial");

    // ---- Step 6: Confirmation ----
    cout << "\n" << Color::BOLD << Color::MAGENTA
         << "========== STEP 6: CONFIRM BOOKING ==========" << Color::RESET << "\n";
    cout << "\nBOOKING SUMMARY:\n";
    Utility::printLine();
    cout << "Name           : " << c.getName()           << "\n"
         << "Phone          : " << c.getPhone()          << "\n"
         << "Email          : " << c.getEmail()          << "\n"
         << "Room           : " << roomId << " (" << room->getType() << ")\n"
         << "Dates          : " << checkIn << " -> " << checkOut << "\n"
         << "Days           : " << days                  << "\n"
         << "Guests         : " << guests                << "\n"
         << "Total Bill     : $" << fixed << setprecision(2) << total << "\n"
         << "Advance Paid   : $" << adv                 << "\n"
         << "Due Amount     : $" << (total - adv)       << "\n"
         << "Payment Method : " << method               << "\n";
    Utility::printLine();

    char confirm;
    cout << "Confirm booking? (y/n): ";
    cin >> confirm; Utility::clearInput();
    if (tolower(confirm) != 'y') { Utility::printError("Booking cancelled!"); return; }

    // Commit
    c.setBookingId(nextBookingId++);
    room->setBooked(true);
    customers.push_back(c);

    Payment p = makePayment(c.getBookingId(), c.getName(), adv, method);
    pendingRevenue += (total - adv);

    // Generate invoice
    FileManager::generateInvoice(c, foodOrders, laundryOrders, transportBookings);

    saveAll();

    cout << "\n" << Color::GREEN << Color::BOLD
         << "----------------------------------------------------------------\n"
         << "                  BOOKING CONFIRMED!\n"
         << "----------------------------------------------------------------\n" << Color::RESET;
    cout << "Booking ID     : " << c.getBookingId()  << "\n"
         << "Transaction ID : " << p.transactionId   << "\n"
         << "Check-In       : " << checkIn           << "\n"
         << "Check-Out      : " << checkOut          << "\n";
    Utility::printSuccess("Thank you for choosing HOTEL DELIGHT !");
    Utility::printInfo("Keep Booking ID " + to_string(c.getBookingId()) + " for checkout.");
}

void HotelManagementSystem::viewBookings() const {
    Utility::printHeader("ACTIVE BOOKINGS");
    if (customers.empty()) { Utility::printError("No active bookings."); return; }

    cout << left
         << setw(8)  << "BkgID"
         << setw(22) << "Name"
         << setw(14) << "Phone"
         << setw(8)  << "Room"
         << setw(6)  << "Days"
         << setw(12) << "Total"
         << setw(12) << "Paid"
         << setw(12) << "Due"
         << "Status\n";
    Utility::printLine();

    for (const auto& c : customers) c.displayRow();
    cout << "\nTotal bookings: " << customers.size() << "\n";
}

void HotelManagementSystem::viewBookingDetails() const {
    Utility::printHeader("BOOKING DETAILS");
    int bid;
    cout << "Booking ID: "; cin >> bid; Utility::clearInput();

    const Customer* c = findCustomer(bid);
    if (!c) { Utility::printError("Booking not found!"); return; }

    c->displayFull();

    const Room* r = findRoom(c->getRoomId());
    if (r) {
        cout << "ROOM DETAILS\n";
        r->displayFull();
    }
}

// Phase 6 - Search Customer
void HotelManagementSystem::searchCustomer() const {
    Utility::printHeader("SEARCH CUSTOMER");
    cout << "1. By Name  2. By Booking ID  3. By Room ID\nChoice: ";
    int ch; cin >> ch; Utility::clearInput();

    if (ch == 1) {
        string name;
        cout << "Name (partial): "; getline(cin, name);
        bool found = false;
        for (const auto& c : customers)
            if (c.getName().find(name) != string::npos) { c.displayRow(); found = true; }
        if (!found) Utility::printError("No customer found.");
    } else if (ch == 2) {
        int bid;
        cout << "Booking ID: "; cin >> bid; Utility::clearInput();
        const Customer* c = findCustomer(bid);
        if (c) c->displayFull();
        else Utility::printError("Not found.");
    } else if (ch == 3) {
        string rid;
        cout << "Room ID: "; cin >> rid; Utility::clearInput();
        rid = Utility::toUpperStr(rid);
        bool found = false;
        for (const auto& c : customers)
            if (c.getRoomId() == rid) { c.displayRow(); found = true; }
        if (!found) Utility::printError("No customer in that room.");
    }
}

// Phase 7 - Sort Customers
void HotelManagementSystem::sortCustomers() {
    Utility::printHeader("SORT CUSTOMERS");
    cout << "1. By Name  2. By Booking ID  3. By Total Bill\nChoice: ";
    int ch; cin >> ch; Utility::clearInput();

    if (ch == 1)
        sort(customers.begin(), customers.end(),
             [](const Customer& a, const Customer& b){ return a.getName() < b.getName(); });
    else if (ch == 2)
        sort(customers.begin(), customers.end(),
             [](const Customer& a, const Customer& b){ return a.getBookingId() < b.getBookingId(); });
    else if (ch == 3)
        sort(customers.begin(), customers.end(),
             [](const Customer& a, const Customer& b){ return a.getTotalBill() > b.getTotalBill(); });

    Utility::printSuccess("Customers sorted.");
    viewBookings();
}

// ----------------------------------------------------------------
// CHECKOUT
// ----------------------------------------------------------------
void HotelManagementSystem::checkout() {
    Utility::printHeader("CHECKOUT");
    if (customers.empty()) { Utility::printError("No active bookings!"); return; }

    viewBookings();
    cout << "\nEnter Booking ID for checkout: ";
    int bid; cin >> bid; Utility::clearInput();

    Customer* c = findCustomerMut(bid);
    if (!c) { Utility::printError("Booking not found!"); return; }

    Room* room = findRoomMut(c->getRoomId());

    // Collect all charges for this booking
    double foodTotal = 0, laundryTotal = 0, transportTotal = 0;
    for (const auto& o : foodOrders)      if (o.bookingId == bid) foodTotal += o.amount;
    for (const auto& l : laundryOrders)   if (l.bookingId == bid) laundryTotal += l.amount;
    for (const auto& t : transportBookings) if (t.bookingId == bid) transportTotal += t.amount;

    double dueAmount = c->getTotalBill() - c->getAdvancePayment();

    cout << "\n" << Color::BOLD << "FINAL BILL DETAILS" << Color::RESET << "\n";
    Utility::printLine();
    cout << fixed << setprecision(2);
    cout << "Booking ID      : " << c->getBookingId()   << "\n"
         << "Customer        : " << c->getName()        << "\n"
         << "Room            : " << c->getRoomId()      << "\n"
         << "Check-In        : " << c->getCheckInDate() << "\n"
         << "Days Stayed     : " << c->getDays()        << "\n";
    Utility::printLine();
    if (room) cout << "Room Charge     : $" << room->getPrice() * c->getDays() << "\n";
    if (foodTotal > 0) cout << "Food Charges    : $" << foodTotal << "\n";
    if (laundryTotal > 0) cout << "Laundry         : $" << laundryTotal << "\n";
    if (transportTotal > 0) cout << "Transport       : $" << transportTotal << "\n";
    Utility::printLine();
    cout << Color::BOLD << "TOTAL BILL      : $" << c->getTotalBill() << Color::RESET << "\n"
         << "Advance Paid    : $" << c->getAdvancePayment() << "\n"
         << Color::RED << Color::BOLD << "DUE AMOUNT      : $" << dueAmount << Color::RESET << "\n";
    Utility::printLine();

    if (dueAmount > 0.01) {
        cout << "Payment for due amount:\n"
             << "1. Cash  2. Card  3. Mobile Banking  4. Bank Transfer\nChoice: ";
        int pmCh; cin >> pmCh; Utility::clearInput();
        string method;
        switch (pmCh) {
            case 1: method = "Cash";           break;
            case 2: method = "Card";           break;
            case 3: method = "Mobile Banking"; break;
            case 4: method = "Bank Transfer";  break;
            default: method = "Cash";
        }

        double paid = 0;
        do {
            cout << "Enter payment ($" << dueAmount << " required): $";
            if (!(cin >> paid)) { Utility::clearInput(); paid = 0; }
            if (paid < dueAmount) Utility::printError("Insufficient payment!");
        } while (paid < dueAmount);
        Utility::clearInput();

        if (paid > dueAmount)
            cout << Color::GREEN << "Change: $" << (paid - dueAmount) << Color::RESET << "\n";

        makePayment(bid, c->getName(), dueAmount, method);
        pendingRevenue -= dueAmount;
    }

    // Feedback
    cout << "\n" << Color::BOLD << "CUSTOMER FEEDBACK" << Color::RESET << "\n";
    Utility::printLine();
    cout << "How was your stay? (1-5): ";
    int rating = 3; cin >> rating; Utility::clearInput();
    cout << "Comments/suggestions: ";
    string feedback; getline(cin, feedback);
    cout << "Recommend us? (y/n): ";
    char rec; cin >> rec; Utility::clearInput();

    // Generate final invoice
    FileManager::generateInvoice(*c, foodOrders, laundryOrders, transportBookings);

    // Release room and remove customer
    if (room) room->setBooked(false);
    customers.erase(remove_if(customers.begin(), customers.end(),
                              [bid](const Customer& x){ return x.getBookingId() == bid; }),
                    customers.end());

    saveAll();
    Utility::printSuccess("Checkout complete! Thank you for staying with us!");
    cout << Color::YELLOW << "We hope to see you again!\n" << Color::RESET;
}

// ----------------------------------------------------------------
// FOOD ORDERS
// ----------------------------------------------------------------
void HotelManagementSystem::placeFoodOrder() {
    Utility::printHeader("PLACE FOOD ORDER");
    if (customers.empty()) { Utility::printError("No active bookings!"); return; }

    viewBookings();
    cout << "\nBooking ID: ";
    int bid; cin >> bid; Utility::clearInput();

    Customer* c = findCustomerMut(bid);
    if (!c) { Utility::printError("Booking not found!"); return; }

    FoodOrder order;
    order.orderId      = nextOrderId++;
    order.bookingId    = bid;
    order.customerName = c->getName();
    order.roomNo       = c->getRoomId();
    order.orderTime    = Utility::getCurrentDateTime();

    cout << "\nRESTAURANT MENU\n";
    Utility::printLine();
    cout << " 1. Breakfast Combo         $15\n"
         << " 2. Lunch Combo             $25\n"
         << " 3. Dinner Combo            $30\n"
         << " 4. Burger & Fries          $12\n"
         << " 5. Pizza                   $18\n"
         << " 6. Pasta                   $14\n"
         << " 7. Sandwich                $ 8\n"
         << " 8. Coffee/Tea              $ 5\n"
         << " 9. Fresh Juice             $ 6\n"
         << "10. Ice Cream               $ 4\n"
         << "11. Mineral Water           $ 2\n"
         << "12. Custom Order\n";

    int itemCh; cout << "Choice: "; cin >> itemCh; Utility::clearInput();

    struct MenuItem { string name; double price; };
    MenuItem menu[] = {
        {"Breakfast Combo",15},{"Lunch Combo",25},{"Dinner Combo",30},
        {"Burger & Fries",12},{"Pizza",18},{"Pasta",14},{"Sandwich",8},
        {"Coffee/Tea",5},{"Fresh Juice",6},{"Ice Cream",4},{"Mineral Water",2}
    };

    if (itemCh >= 1 && itemCh <= 11) {
        order.items  = menu[itemCh-1].name;
        order.amount = menu[itemCh-1].price;
    } else if (itemCh == 12) {
        cout << "Item name: "; getline(cin, order.items);
        cout << "Price: $"; cin >> order.amount; Utility::clearInput();
    } else { Utility::printError("Invalid choice!"); return; }

    int qty = 1;
    cout << "Quantity: "; cin >> qty; Utility::clearInput();
    if (qty < 1) qty = 1;
    order.amount *= qty;

    cout << "Special instructions (ENTER to skip): ";
    string instr; getline(cin, instr);

    order.status = "Preparing";
    foodOrders.push_back(order);
    c->addToTotalBill(order.amount);

    saveAll();
    Utility::printSuccess("Order placed!");
    cout << "Order ID : " << order.orderId << "\n"
         << "Items    : " << order.items << " x" << qty << "\n"
         << "Amount   : $" << fixed << setprecision(2) << order.amount << "\n"
         << "Room No  : " << order.roomNo << "\n"
         << "ETA      : 20-30 minutes\n";
    if (!instr.empty()) cout << "Note     : " << instr << "\n";
}

void HotelManagementSystem::viewFoodOrders() const {
    Utility::printHeader("FOOD ORDERS");
    if (foodOrders.empty()) { Utility::printError("No food orders."); return; }

    cout << left
         << setw(10) << "OrderID" << setw(8) << "BkgID"
         << setw(22) << "Customer" << setw(8) << "Room"
         << setw(25) << "Items" << setw(10) << "Amount" << "Status\n";
    Utility::printLine();

    for (const auto& o : foodOrders)
        cout << left
             << setw(10) << o.orderId << setw(8) << o.bookingId
             << setw(22) << o.customerName.substr(0, 21) << setw(8) << o.roomNo
             << setw(25) << o.items.substr(0, 24) << "$" << setw(9)
             << fixed << setprecision(2) << o.amount << o.status << "\n";
}

// ----------------------------------------------------------------
// COMPLAINTS
// ----------------------------------------------------------------
void HotelManagementSystem::registerComplaint() {
    Utility::printHeader("REGISTER COMPLAINT");
    if (customers.empty()) { Utility::printError("No active bookings!"); return; }

    viewBookings();
    Complaint comp;
    comp.complaintId = nextComplaintId++;
    comp.status      = "Pending";
    comp.date        = Utility::getCurrentDate();

    cout << "\nBooking ID: "; cin >> comp.bookingId; Utility::clearInput();
    const Customer* c = findCustomer(comp.bookingId);
    if (!c) { Utility::printError("Booking not found!"); return; }

    comp.customerName = c->getName();
    comp.roomNo       = c->getRoomId();

    cout << "Complaint Category:\n"
         << "1. Room Cleanliness  2. Noise  3. AC/Heating  4. Food Quality\n"
         << "5. Staff Behaviour   6. Amenities  7. Other\nChoice: ";
    int catCh; cin >> catCh; Utility::clearInput();

    string cats[] = {"Room Cleanliness","Noise","AC/Heating","Food Quality",
                     "Staff Behaviour","Amenities","Other"};
    comp.category = (catCh >= 1 && catCh <= 7) ? cats[catCh-1] : "Other";

    cout << "Priority (1=High, 2=Medium, 3=Low): ";
    cin >> comp.priority; Utility::clearInput();
    if (comp.priority < 1 || comp.priority > 3) comp.priority = 2;

    cout << "Describe your complaint:\n";
    getline(cin, comp.complaint);

    complaints.push_back(comp);
    saveAll();
    Utility::printSuccess("Complaint #" + to_string(comp.complaintId) + " registered!");
    cout << "Category : " << comp.category << "\n"
         << "Priority : " << comp.priority << "\n"
         << "Status   : Pending\n";
}

void HotelManagementSystem::viewComplaints() const {
    Utility::printHeader("COMPLAINTS");
    if (complaints.empty()) { Utility::printError("No complaints."); return; }

    cout << left
         << setw(10) << "CmpID" << setw(8) << "BkgID"
         << setw(22) << "Customer" << setw(8) << "Room"
         << setw(18) << "Category" << setw(8) << "Priority"
         << setw(10) << "Status" << "Date\n";
    Utility::printLine();

    for (const auto& co : complaints)
        cout << left
             << setw(10) << co.complaintId << setw(8) << co.bookingId
             << setw(22) << co.customerName.substr(0, 21) << setw(8) << co.roomNo
             << setw(18) << co.category.substr(0, 17)
             << setw(8)  << co.priority
             << setw(10) << co.status << co.date << "\n";
}

void HotelManagementSystem::resolveComplaint() {
    Utility::printHeader("RESOLVE COMPLAINT");
    viewComplaints();

    int cid;
    cout << "\nComplaint ID: "; cin >> cid; Utility::clearInput();

    auto it = find_if(complaints.begin(), complaints.end(),
                      [cid](const Complaint& c){ return c.complaintId == cid; });
    if (it == complaints.end()) { Utility::printError("Complaint not found!"); return; }

    cout << "Resolution note: ";
    getline(cin, it->resolution);
    it->status = "Resolved";

    saveAll();
    Utility::printSuccess("Complaint #" + to_string(cid) + " resolved.");
}

// ----------------------------------------------------------------
// LAUNDRY
// ----------------------------------------------------------------
void HotelManagementSystem::placeLaundryOrder() {
    Utility::printHeader("LAUNDRY SERVICE");
    if (customers.empty()) { Utility::printError("No active bookings!"); return; }

    viewBookings();
    cout << "\nBooking ID: ";
    int bid; cin >> bid; Utility::clearInput();

    Customer* c = findCustomerMut(bid);
    if (!c) { Utility::printError("Booking not found!"); return; }

    Laundry laundry;
    laundry.laundryId    = nextLaundryId++;
    laundry.bookingId    = bid;
    laundry.customerName = c->getName();
    laundry.roomNo       = c->getRoomId();
    laundry.status       = "Pending";
    laundry.pickupDate   = Utility::getCurrentDate();

    struct LaundryItem { string name; double price; };
    LaundryItem items[] = {
        {"Shirt",3},{"Pant",4},{"T-Shirt",2},{"Jeans",5},
        {"Dress",6},{"Bed Sheet",5},{"Towel",2},{"Suit",10},{"Iron Only",2}
    };

    cout << "\nLAUNDRY MENU\n";
    Utility::printLine();
    for (int i = 0; i < 9; ++i)
        cout << (i+1) << ". " << items[i].name << "   $" << items[i].price << "\n";

    double total = 0;
    string itemStr;
    while (true) {
        cout << "Select (0 to finish): "; int ch; cin >> ch;
        if (ch == 0) break;
        if (ch < 1 || ch > 9) continue;
        int qty; cout << "Quantity: "; cin >> qty;
        total   += items[ch-1].price * qty;
        itemStr += to_string(qty) + "x" + items[ch-1].name + " ";
    }
    Utility::clearInput();

    laundry.items  = itemStr;
    laundry.amount = total;

    cout << "Delivery date (YYYY-MM-DD): ";
    getline(cin, laundry.deliveryDate);

    laundryOrders.push_back(laundry);
    saveAll();

    Utility::printSuccess("Laundry order placed!");
    cout << "Laundry ID : " << laundry.laundryId << "\n"
         << "Items      : " << itemStr << "\n"
         << "Total      : $" << fixed << setprecision(2) << total << "\n"
         << "Delivery   : " << laundry.deliveryDate << "\n";
}

void HotelManagementSystem::viewLaundryOrders() const {
    Utility::printHeader("LAUNDRY ORDERS");
    if (laundryOrders.empty()) { Utility::printError("No laundry orders."); return; }
    for (const auto& l : laundryOrders)
        cout << "ID:" << l.laundryId << " | Bkg:" << l.bookingId
             << " | " << l.customerName << " | " << l.roomNo
             << " | $" << fixed << setprecision(2) << l.amount
             << " | " << l.status << " | Del:" << l.deliveryDate << "\n";
}

// ----------------------------------------------------------------
// TRANSPORT
// ----------------------------------------------------------------
void HotelManagementSystem::bookTransport() {
    Utility::printHeader("TRANSPORT BOOKING");
    if (customers.empty()) { Utility::printError("No active bookings!"); return; }

    viewBookings();
    cout << "\nBooking ID: ";
    int bid; cin >> bid; Utility::clearInput();

    Customer* c = findCustomerMut(bid);
    if (!c) { Utility::printError("Booking not found!"); return; }

    Transport t;
    t.transportId   = nextTransportId++;
    t.bookingId     = bid;
    t.customerName  = c->getName();
    t.status        = "Confirmed";

    cout << "Pickup Point: "; getline(cin, t.pickupPoint);
    cout << "Drop Point: "; getline(cin, t.dropPoint);
    cout << "Date (YYYY-MM-DD): "; cin >> t.date; Utility::clearInput();

    cout << "Vehicle:\n1. Sedan $30  2. SUV $50  3. Luxury $80  4. Microbus $40  5. Van $25\nChoice: ";
    int vCh; cin >> vCh; Utility::clearInput();
    string vNames[] = {"Sedan","SUV","Luxury Car","Microbus","Van"};
    double vPrices[] = {30,50,80,40,25};
    if (vCh >= 1 && vCh <= 5) {
        t.vehicleType = vNames[vCh-1];
        t.amount      = vPrices[vCh-1];
    } else { t.vehicleType = "Sedan"; t.amount = 30; }

    transportBookings.push_back(t);
    c->addToTotalBill(t.amount);
    saveAll();

    Utility::printSuccess("Transport booked!");
    cout << "Transport ID : " << t.transportId << "\n"
         << "Vehicle      : " << t.vehicleType << "\n"
         << "Pickup       : " << t.pickupPoint << "\n"
         << "Drop         : " << t.dropPoint   << "\n"
         << "Date         : " << t.date        << "\n"
         << "Amount       : $" << fixed << setprecision(2) << t.amount << "\n";
}

void HotelManagementSystem::viewTransportBookings() const {
    Utility::printHeader("TRANSPORT BOOKINGS");
    if (transportBookings.empty()) { Utility::printError("No transport bookings."); return; }
    for (const auto& t : transportBookings)
        cout << "ID:" << t.transportId << " | " << t.customerName
             << " | " << t.vehicleType << " | From:" << t.pickupPoint
             << " -> " << t.dropPoint << " | " << t.date
             << " | $" << fixed << setprecision(2) << t.amount << " | " << t.status << "\n";
}

// ----------------------------------------------------------------
// STAFF MANAGEMENT
// ----------------------------------------------------------------
void HotelManagementSystem::addStaff() {
    Utility::printHeader("ADD STAFF MEMBER");
    Staff s;
    s.setStaffId(nextStaffId++);

    string name;
    cout << "Full Name: "; getline(cin, name);
    s.setName(name);

    cout << "Position (Manager/Receptionist/Housekeeping/Chef/Security): ";
    string pos; getline(cin, pos); s.setPosition(pos);

    cout << "Department: ";
    string dept; getline(cin, dept); s.setDepartment(dept);

    string phone;
    do {
        cout << "Phone: "; getline(cin, phone);
        if (!Utility::validatePhone(phone)) Utility::printError("Invalid phone!");
    } while (!Utility::validatePhone(phone));
    s.setPhone(phone);

    string email;
    cout << "Email (press ENTER to skip): "; getline(cin, email);
    s.setEmail(email);

    cout << "Address: ";
    string addr; getline(cin, addr); s.setAddress(addr);

    cout << "Shift (Morning/Evening/Night): ";
    string shift; getline(cin, shift); s.setShift(shift);

    double sal = 0;
    do {
        cout << "Monthly Salary: $";
        if (!(cin >> sal)) { Utility::clearInput(); sal = -1; }
    } while (sal <= 0);
    Utility::clearInput();
    s.setSalary(sal);

    s.setJoinDate(Utility::getCurrentDate());
    s.setActive(true);

    staffList.push_back(s);
    saveAll();

    Utility::printSuccess("Staff member added!");
    s.displayInfo();
}

void HotelManagementSystem::showStaff() const {
    Utility::printHeader("STAFF DIRECTORY");
    if (staffList.empty()) { Utility::printError("No staff records."); return; }

    cout << left
         << setw(8)  << "ID"
         << setw(22) << "Name"
         << setw(18) << "Position"
         << setw(15) << "Phone"
         << setw(10) << "Shift"
         << setw(12) << "Salary"
         << "Join Date\n";
    Utility::printLine();

    for (const auto& s : staffList) s.displayRow();
    cout << "\nTotal staff: " << staffList.size() << "\n";
}

void HotelManagementSystem::searchStaff() const {
    Utility::printHeader("SEARCH STAFF");
    cout << "1. By Name  2. By ID  3. By Position\nChoice: ";
    int ch; cin >> ch; Utility::clearInput();

    if (ch == 1) {
        string name; cout << "Name: "; getline(cin, name);
        for (const auto& s : staffList)
            if (s.getName().find(name) != string::npos) s.displayInfo();
    } else if (ch == 2) {
        int sid; cout << "Staff ID: "; cin >> sid; Utility::clearInput();
        for (const auto& s : staffList)
            if (s.getStaffId() == sid) s.displayInfo();
    } else if (ch == 3) {
        string pos; cout << "Position: "; getline(cin, pos);
        for (const auto& s : staffList)
            if (s.getPosition() == pos) s.displayRow();
    }
}

void HotelManagementSystem::sortStaff() {
    Utility::printHeader("SORT STAFF");
    cout << "1. By Name  2. By Salary (High→Low)  3. By Join Date\nChoice: ";
    int ch; cin >> ch; Utility::clearInput();

    if (ch == 1)
        sort(staffList.begin(), staffList.end(),
             [](const Staff& a, const Staff& b){ return a.getName() < b.getName(); });
    else if (ch == 2)
        sort(staffList.begin(), staffList.end(),
             [](const Staff& a, const Staff& b){ return a.getSalary() > b.getSalary(); });
    else if (ch == 3)
        sort(staffList.begin(), staffList.end(),
             [](const Staff& a, const Staff& b){ return a.getJoinDate() < b.getJoinDate(); });

    Utility::printSuccess("Staff sorted.");
    showStaff();
}

void HotelManagementSystem::removeStaff() {
    Utility::printHeader("REMOVE STAFF");
    showStaff();
    int sid;
    cout << "Staff ID to remove: "; cin >> sid; Utility::clearInput();

    auto it = find_if(staffList.begin(), staffList.end(),
                      [sid](const Staff& s){ return s.getStaffId() == sid; });
    if (it == staffList.end()) { Utility::printError("Staff not found!"); return; }

    char confirm;
    cout << "Remove " << it->getName() << "? (y/n): ";
    cin >> confirm; Utility::clearInput();

    if (tolower(confirm) == 'y') {
        staffList.erase(it);
        saveAll();
        Utility::printSuccess("Staff member removed.");
    }
}

// ----------------------------------------------------------------
// DASHBOARD & REPORTS
// ----------------------------------------------------------------
void HotelManagementSystem::showDashboard() const {
    Utility::printHeader("HOTEL MANAGEMENT DASHBOARD");

    int totalRooms  = static_cast<int>(rooms.size());
    int bookedRooms = static_cast<int>(count_if(rooms.begin(), rooms.end(),
                                               [](const Room& r){ return r.isBooked(); }));
    double avgPrice = 0;
    for (const auto& r : rooms) avgPrice += r.getPrice();
    if (totalRooms > 0) avgPrice /= totalRooms;
    double occupancy = totalRooms > 0 ? (bookedRooms * 100.0 / totalRooms) : 0;

    cout << Color::BOLD << Color::CYAN;
    cout << "\nHOTEL AT A GLANCE\n";
    Utility::printLine();
    cout << Color::RESET;

    cout << "ROOM STATISTICS\n"
         << "  Total Rooms      : " << totalRooms  << "\n"
         << "  Booked           : " << bookedRooms << "\n"
         << "  Available        : " << (totalRooms - bookedRooms) << "\n"
         << "  Occupancy Rate   : " << fixed << setprecision(1) << occupancy << "%\n"
         << "  Avg Room Price   : $" << fixed << setprecision(2) << avgPrice << "\n";

    cout << "\nCUSTOMER & STAFF\n"
         << "  Active Bookings  : " << customers.size()      << "\n"
         << "  Total Staff      : " << staffList.size()      << "\n"
         << "  Food Orders      : " << foodOrders.size()     << "\n"
         << "  Laundry Orders   : " << laundryOrders.size()  << "\n"
         << "  Transport        : " << transportBookings.size() << "\n"
         << "  Complaints       : " << complaints.size()     << "\n";

    cout << "\nREVENUE SNAPSHOT\n"
         << "  Collected        : $" << fixed << setprecision(2) << totalRevenue << "\n"
         << "  Pending          : $" << pendingRevenue << "\n"
         << "  Total Expected   : $" << (totalRevenue + pendingRevenue) << "\n";
}

void HotelManagementSystem::showRevenueReport() const {
    Utility::printHeader("COMPREHENSIVE REVENUE REPORT");

    cout << Color::BOLD << Color::GREEN;
    cout << "FINANCIAL SUMMARY\n";
    Utility::printLine();
    cout << fixed << setprecision(2);
    cout << "Revenue Collected  : $" << totalRevenue                   << "\n"
         << "Pending Revenue    : $" << pendingRevenue                 << "\n"
         << "Total Expected     : $" << (totalRevenue + pendingRevenue)<< "\n"
         << Color::RESET;

    // Payment method breakdown
    double cash = 0, card = 0, mobile = 0, bank = 0, intl = 0;
    for (const auto& p : payments) {
        if      (p.method == "Cash")              cash   += p.amount;
        else if (p.method == "Card")              card   += p.amount;
        else if (p.method == "Mobile Banking")    mobile += p.amount;
        else if (p.method == "Bank Transfer")     bank   += p.amount;
        else if (p.method == "International Card")intl   += p.amount;
    }
    cout << "\nPAYMENT METHOD BREAKDOWN\n";
    Utility::printLine();
    cout << "Cash               : $" << cash   << "\n"
         << "Card               : $" << card   << "\n"
         << "Mobile Banking     : $" << mobile << "\n"
         << "Bank Transfer      : $" << bank   << "\n"
         << "International Card : $" << intl   << "\n";

    // Service revenues
    double foodRev = 0, laundryRev = 0, transportRev = 0;
    for (const auto& o : foodOrders)        foodRev      += o.amount;
    for (const auto& l : laundryOrders)     laundryRev   += l.amount;
    for (const auto& t : transportBookings) transportRev += t.amount;

    cout << "\nSERVICE REVENUE\n";
    Utility::printLine();
    cout << "Food Orders        : $" << foodRev      << " (" << foodOrders.size() << " orders)\n"
         << "Laundry            : $" << laundryRev   << " (" << laundryOrders.size() << " orders)\n"
         << "Transport          : $" << transportRev << " (" << transportBookings.size() << " bookings)\n";

    // Occupancy
    int totalR  = static_cast<int>(rooms.size());
    int bookedR = static_cast<int>(count_if(rooms.begin(), rooms.end(),
                                            [](const Room& r){ return r.isBooked(); }));
    double occ = totalR > 0 ? bookedR * 100.0 / totalR : 0;
    cout << "\nOCCUPANCY REPORT\n";
    Utility::printLine();
    cout << "Total Rooms        : " << totalR << "\n"
         << "Booked             : " << bookedR << "\n"
         << "Available          : " << (totalR - bookedR) << "\n"
         << "Occupancy Rate     : " << fixed << setprecision(2) << occ << "%\n";

    // Complaints
    int resolved = static_cast<int>(count_if(complaints.begin(), complaints.end(),
                                             [](const Complaint& c){ return c.status == "Resolved"; }));
    cout << "\nCOMPLAINT REPORT\n";
    Utility::printLine();
    cout << "Total Complaints   : " << complaints.size() << "\n"
         << "Resolved           : " << resolved          << "\n"
         << "Pending            : " << (complaints.size() - resolved) << "\n";
}

// Phase 12 - Advanced Dashboard
void HotelManagementSystem::showAdvancedDashboard() const {
    Utility::printHeader("ADVANCED ANALYTICS DASHBOARD");

    // Occupancy Rate
    int total  = static_cast<int>(rooms.size());
    int booked = static_cast<int>(count_if(rooms.begin(), rooms.end(),
                                           [](const Room& r){ return r.isBooked(); }));
    double occ = total > 0 ? booked * 100.0 / total : 0;

    // Average Stay Duration
    double totalDays = 0;
    for (const auto& c : customers) totalDays += c.getDays();
    double avgStay = customers.empty() ? 0 : totalDays / customers.size();

    // Most Popular Room Type
    map<string,int> typeCount;
    for (const auto& c : customers) {
        const Room* r = findRoom(c.getRoomId());
        if (r) typeCount[r->getType()]++;
    }
    string topType = "N/A"; int topTypeCount = 0;
    for (const auto& kv : typeCount)
        if (kv.second > topTypeCount) { topType = kv.first; topTypeCount = kv.second; }

    // Top Revenue Room
    map<string,double> roomRevenue;
    for (const auto& c : customers) roomRevenue[c.getRoomId()] += c.getTotalBill();
    string topRoom = "N/A"; double topRev = 0;
    for (const auto& kv : roomRevenue)
        if (kv.second > topRev) { topRoom = kv.first; topRev = kv.second; }

    // Top Spending Customer
    string topCustomer = "N/A"; double topSpend = 0;
    for (const auto& c : customers)
        if (c.getTotalBill() > topSpend) { topSpend = c.getTotalBill(); topCustomer = c.getName(); }

    cout << fixed << setprecision(2);
    cout << "\nKEY PERFORMANCE INDICATORS\n";
    Utility::printLine();
    cout << "  Occupancy Rate         : " << occ << "%\n"
         << "  Average Stay Duration  : " << avgStay << " days\n"
         << "  Most Popular Room Type : " << topType << " (" << topTypeCount << " bookings)\n"
         << "  Top Revenue Room       : " << topRoom << " ($" << topRev << ")\n"
         << "  Top Spending Customer  : " << topCustomer << " ($" << topSpend << ")\n";

    cout << "\nSERVICE PERFORMANCE\n";
    Utility::printLine();
    double foodRev = 0, laundryRev = 0, transportRev = 0;
    for (const auto& o : foodOrders) foodRev += o.amount;
    for (const auto& l : laundryOrders) laundryRev += l.amount;
    for (const auto& t : transportBookings) transportRev += t.amount;

    cout << "  Food Revenue           : $" << foodRev << " (" << foodOrders.size() << " orders)\n"
         << "  Laundry Revenue        : $" << laundryRev << " (" << laundryOrders.size() << " orders)\n"
         << "  Transport Revenue      : $" << transportRev << " (" << transportBookings.size() << " bookings)\n";

    // Complaint stats
    int highPriority = static_cast<int>(count_if(complaints.begin(), complaints.end(),
                                                 [](const Complaint& c){ return c.priority == 1; }));
    int resolved = static_cast<int>(count_if(complaints.begin(), complaints.end(),
                                             [](const Complaint& c){ return c.status == "Resolved"; }));
    cout << "\nCOMPLAINT STATISTICS\n";
    Utility::printLine();
    cout << "  Total Complaints       : " << complaints.size() << "\n"
         << "  High Priority          : " << highPriority << "\n"
         << "  Resolved               : " << resolved << "\n"
         << "  Pending                : " << (complaints.size() - resolved) << "\n";

    // Revenue timeline (by payment date - monthly)
    map<string,double> monthly;
    for (const auto& p : payments) {
        string month = p.date.substr(0, 7); // YYYY-MM
        monthly[month] += p.amount;
    }
    if (!monthly.empty()) {
        cout << "\nMONTHLY REVENUE SUMMARY\n";
        Utility::printLine();
        for (const auto& kv : monthly)
            cout << "  " << kv.first << "  :  $" << kv.second << "\n";
    }
}
