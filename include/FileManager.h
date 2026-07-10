#pragma once
#include <string>
#include <vector>
#include "Room.h"
#include "Customer.h"
#include "Staff.h"
#include "Booking.h"

// ============================================================
// FILE MANAGER  (Phase 4 - Full persistence for all entities)
// ============================================================
class FileManager {
public:
    static const std::string DATA_DIR;

    // Rooms
    static void saveRooms(const std::vector<Room>& rooms);
    static void loadRooms(std::vector<Room>& rooms);

    // Customers
    static void saveCustomers(const std::vector<Customer>& customers);
    static void loadCustomers(std::vector<Customer>& customers);

    // Staff
    static void saveStaff(const std::vector<Staff>& staff);
    static void loadStaff(std::vector<Staff>& staff);

    // Payments
    static void savePayments(const std::vector<Payment>& payments);
    static void loadPayments(std::vector<Payment>& payments);

    // Food Orders
    static void saveFoodOrders(const std::vector<FoodOrder>& orders);
    static void loadFoodOrders(std::vector<FoodOrder>& orders);

    // Complaints
    static void saveComplaints(const std::vector<Complaint>& complaints);
    static void loadComplaints(std::vector<Complaint>& complaints);

    // Laundry
    static void saveLaundry(const std::vector<Laundry>& laundry);
    static void loadLaundry(std::vector<Laundry>& laundry);

    // Transport
    static void saveTransport(const std::vector<Transport>& transport);
    static void loadTransport(std::vector<Transport>& transport);

    // Login / credentials
    static void initCredentials();
    static bool authenticate(const std::string& user, const std::string& pass, std::string& role);

    // Invoice generation (Phase 8)
    static void generateInvoice(const Customer& customer,
                                const std::vector<FoodOrder>& foodOrders,
                                const std::vector<Laundry>& laundry,
                                const std::vector<Transport>& transport);

    // ID counters persistence
    static void saveCounters(int bk, int st, int ord, int cmp,
                             int task, int pay, int laund, int trans);
    static void loadCounters(int& bk, int& st, int& ord, int& cmp,
                             int& task, int& pay, int& laund, int& trans);

private:
    static std::string filePath(const std::string& filename);
    static void writeLineEscaped(std::ofstream& out, const std::string& s);
    static std::string readLineEscaped(std::ifstream& in);
};
