#pragma once
#include <vector>
#include <string>
#include "Room.h"
#include "Customer.h"
#include "Staff.h"
#include "Booking.h"
#include "FileManager.h"
#include "Utility.h"

// ============================================================
// HOTEL MANAGEMENT SYSTEM  (Phase 1 - Main class)
// Encapsulates all subsystems, role-based menus (Phase 5)
// ============================================================
class HotelManagementSystem {
private:
    // ---- Data Collections ----
    std::vector<Room>      rooms;
    std::vector<Customer>  customers;
    std::vector<Staff>     staffList;
    std::vector<FoodOrder> foodOrders;
    std::vector<Complaint> complaints;
    std::vector<Housekeeping> housekeepingTasks;
    std::vector<Payment>   payments;
    std::vector<Laundry>   laundryOrders;
    std::vector<Transport> transportBookings;

    // ---- ID Counters ----
    int nextBookingId   = 1001;
    int nextStaffId     = 2001;
    int nextOrderId     = 3001;
    int nextComplaintId = 4001;
    int nextTaskId      = 5001;
    int nextPaymentId   = 6001;
    int nextLaundryId   = 7001;
    int nextTransportId = 8001;

    // ---- Revenue Tracking ----
    double totalRevenue   = 0.0;
    double pendingRevenue = 0.0;

    // ---- Login Session ----
    std::string currentUser;
    std::string currentRole; // Admin / Receptionist / Manager

    // ============================================================
    // ROOM MANAGEMENT
    // ============================================================
    void addRoom();
    void showRooms() const;
    void showAvailableRooms() const;
    void modifyRoom();
    void deleteRoom();
    void searchRoom() const;            // Phase 6
    void sortRooms();                   // Phase 7
    void recommendRoom() const;         // Phase 13

    // ============================================================
    // BOOKING MANAGEMENT
    // ============================================================
    void bookRoom();
    void viewBookings() const;
    void viewBookingDetails() const;
    void checkout();
    void searchCustomer() const;        // Phase 6
    void sortCustomers();               // Phase 7

    // ============================================================
    // SERVICE MODULES
    // ============================================================
    void placeFoodOrder();
    void viewFoodOrders() const;
    void registerComplaint();
    void viewComplaints() const;
    void resolveComplaint();
    void placeLaundryOrder();
    void viewLaundryOrders() const;
    void bookTransport();
    void viewTransportBookings() const;

    // ============================================================
    // STAFF MANAGEMENT
    // ============================================================
    void addStaff();
    void showStaff() const;
    void searchStaff() const;           // Phase 6
    void sortStaff();                   // Phase 7
    void removeStaff();

    // ============================================================
    // REPORTS & DASHBOARD
    // ============================================================
    void showDashboard() const;
    void showRevenueReport() const;
    void showAdvancedDashboard() const; // Phase 12

    // ============================================================
    // MENUS (role-based)
    // ============================================================
    void adminMenu();
    void receptionistMenu();
    void managerMenu();
    bool loginScreen();

    // ============================================================
    // HELPERS
    // ============================================================
    void saveAll() const;
    void loadAll();
    Payment makePayment(int bookingId, const std::string& customerName,
                        double amount, const std::string& method);
    const Customer* findCustomer(int bookingId) const;
    Customer*       findCustomerMut(int bookingId);
    const Room*     findRoom(const std::string& id) const;
    Room*           findRoomMut(const std::string& id);

public:
    HotelManagementSystem();
    void run();
};
