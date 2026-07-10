#include "FileManager.h"
#include "Utility.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cerrno>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

const std::string FileManager::DATA_DIR = "data/";

static bool ensureDataDirExists() {
#ifdef _WIN32
    if (_mkdir(FileManager::DATA_DIR.c_str()) != 0 && errno != EEXIST) {
        return false;
    }
#else
    if (mkdir(FileManager::DATA_DIR.c_str(), 0755) != 0 && errno != EEXIST) {
        return false;
    }
#endif
    return true;
}

// ---- helpers ----
std::string FileManager::filePath(const std::string& filename) {
    return DATA_DIR + filename;
}

// Escape newlines in strings before writing
void FileManager::writeLineEscaped(std::ofstream& out, const std::string& s) {
    std::string esc;
    for (char c : s) {
        if (c == '\n') esc += "\\n";
        else if (c == '\\') esc += "\\\\";
        else esc += c;
    }
    out << esc << '\n';
}

std::string FileManager::readLineEscaped(std::ifstream& in) {
    std::string line;
    std::getline(in, line);
    std::string out;
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '\\' && i + 1 < line.size()) {
            if (line[i+1] == 'n')  { out += '\n'; ++i; }
            else if (line[i+1] == '\\') { out += '\\'; ++i; }
            else out += line[i];
        } else out += line[i];
    }
    return out;
}

// ============================================================
// ROOMS
// ============================================================
void FileManager::saveRooms(const std::vector<Room>& rooms) {
    ensureDataDirExists();
    std::ofstream out(filePath("rooms.txt"));
    if (!out) return;
    out << rooms.size() << '\n';
    for (const auto& r : rooms) {
        writeLineEscaped(out, r.getId());
        writeLineEscaped(out, r.getType());
        out << r.getPrice() << '\n' << r.getCapacity() << '\n' << r.isBooked() << '\n';
        writeLineEscaped(out, r.getAmenities());
        writeLineEscaped(out, r.getFloor());
        writeLineEscaped(out, r.getView());
        out << r.getRating() << '\n';
        writeLineEscaped(out, r.getBedType());
        writeLineEscaped(out, r.getAcType());
    }
}

void FileManager::loadRooms(std::vector<Room>& rooms) {
    std::ifstream in(filePath("rooms.txt"));
    if (!in) return;
    int size = 0;
    in >> size; in.ignore();
    rooms.clear();
    for (int i = 0; i < size; ++i) {
        Room r;
        r.setId(readLineEscaped(in));
        r.setType(readLineEscaped(in));
        double price; in >> price; in.ignore(); r.setPrice(price);
        int cap; in >> cap; in.ignore(); r.setCapacity(cap);
        bool bk; in >> bk; in.ignore(); r.setBooked(bk);
        r.setAmenities(readLineEscaped(in));
        r.setFloor(readLineEscaped(in));
        r.setView(readLineEscaped(in));
        double rat; in >> rat; in.ignore(); r.setRating(rat);
        r.setBedType(readLineEscaped(in));
        r.setAcType(readLineEscaped(in));
        rooms.push_back(r);
    }
}

// ============================================================
// CUSTOMERS
// ============================================================
void FileManager::saveCustomers(const std::vector<Customer>& customers) {
    ensureDataDirExists();
    std::ofstream out(filePath("customers.txt"));
    if (!out) return;
    out << customers.size() << '\n';
    for (const auto& c : customers) {
        out << c.getBookingId() << '\n';
        writeLineEscaped(out, c.getName());
        writeLineEscaped(out, c.getPhone());
        writeLineEscaped(out, c.getEmail());
        writeLineEscaped(out, c.getAddress());
        writeLineEscaped(out, c.getNid());
        writeLineEscaped(out, c.getIdType());
        writeLineEscaped(out, c.getNationality());
        writeLineEscaped(out, c.getRoomId());
        out << c.getDays() << '\n';
        writeLineEscaped(out, c.getCheckInDate());
        writeLineEscaped(out, c.getCheckOutDate());
        out << c.getAdvancePayment() << '\n' << c.getTotalBill() << '\n';
        writeLineEscaped(out, c.getPaymentStatus());
        writeLineEscaped(out, c.getPaymentMethod());
        writeLineEscaped(out, c.getSpecialRequests());
        out << c.getNumberOfGuests() << '\n';
        writeLineEscaped(out, c.getEmergencyContact());
        writeLineEscaped(out, c.getEmergencyPhone());
        out << c.isCheckedOut() << '\n';
    }
}

void FileManager::loadCustomers(std::vector<Customer>& customers) {
    std::ifstream in(filePath("customers.txt"));
    if (!in) return;
    int size = 0; in >> size; in.ignore();
    customers.clear();
    for (int i = 0; i < size; ++i) {
        Customer c;
        int bid; in >> bid; in.ignore(); c.setBookingId(bid);
        c.setName(readLineEscaped(in));
        c.setPhone(readLineEscaped(in));
        c.setEmail(readLineEscaped(in));
        c.setAddress(readLineEscaped(in));
        c.setNid(readLineEscaped(in));
        c.setIdType(readLineEscaped(in));
        c.setNationality(readLineEscaped(in));
        c.setRoomId(readLineEscaped(in));
        int days; in >> days; in.ignore(); c.setDays(days);
        c.setCheckInDate(readLineEscaped(in));
        c.setCheckOutDate(readLineEscaped(in));
        double adv, tot; in >> adv >> tot; in.ignore();
        c.setAdvancePayment(adv); c.setTotalBill(tot);
        c.setPaymentStatus(readLineEscaped(in));
        c.setPaymentMethod(readLineEscaped(in));
        c.setSpecialRequests(readLineEscaped(in));
        int guests; in >> guests; in.ignore(); c.setNumberOfGuests(guests);
        c.setEmergencyContact(readLineEscaped(in));
        c.setEmergencyPhone(readLineEscaped(in));
        bool co; in >> co; in.ignore(); c.setCheckedOut(co);
        customers.push_back(c);
    }
}

// ============================================================
// STAFF
// ============================================================
void FileManager::saveStaff(const std::vector<Staff>& staff) {
    ensureDataDirExists();
    std::ofstream out(filePath("staff.txt"));
    if (!out) return;
    out << staff.size() << '\n';
    for (const auto& s : staff) {
        out << s.getStaffId() << '\n';
        writeLineEscaped(out, s.getName());
        writeLineEscaped(out, s.getPosition());
        writeLineEscaped(out, s.getPhone());
        writeLineEscaped(out, s.getEmail());
        writeLineEscaped(out, s.getAddress());
        out << s.getSalary() << '\n';
        writeLineEscaped(out, s.getJoinDate());
        writeLineEscaped(out, s.getShift());
        writeLineEscaped(out, s.getDepartment());
        out << s.isActive() << '\n';
    }
}

void FileManager::loadStaff(std::vector<Staff>& staff) {
    std::ifstream in(filePath("staff.txt"));
    if (!in) return;
    int size = 0; in >> size; in.ignore();
    staff.clear();
    for (int i = 0; i < size; ++i) {
        Staff s;
        int sid; in >> sid; in.ignore(); s.setStaffId(sid);
        s.setName(readLineEscaped(in));
        s.setPosition(readLineEscaped(in));
        s.setPhone(readLineEscaped(in));
        s.setEmail(readLineEscaped(in));
        s.setAddress(readLineEscaped(in));
        double sal; in >> sal; in.ignore(); s.setSalary(sal);
        s.setJoinDate(readLineEscaped(in));
        s.setShift(readLineEscaped(in));
        s.setDepartment(readLineEscaped(in));
        bool act; in >> act; in.ignore(); s.setActive(act);
        staff.push_back(s);
    }
}

// ============================================================
// PAYMENTS
// ============================================================
void FileManager::savePayments(const std::vector<Payment>& payments) {
    ensureDataDirExists();
    std::ofstream out(filePath("payments.txt"));
    if (!out) return;
    out << payments.size() << '\n';
    for (const auto& p : payments) {
        out << p.paymentId << '\n' << p.bookingId << '\n';
        writeLineEscaped(out, p.customerName);
        out << p.amount << '\n';
        writeLineEscaped(out, p.method);
        writeLineEscaped(out, p.date);
        writeLineEscaped(out, p.status);
        writeLineEscaped(out, p.transactionId);
    }
}

void FileManager::loadPayments(std::vector<Payment>& payments) {
    std::ifstream in(filePath("payments.txt"));
    if (!in) return;
    int size = 0; in >> size; in.ignore();
    payments.clear();
    for (int i = 0; i < size; ++i) {
        Payment p;
        in >> p.paymentId >> p.bookingId; in.ignore();
        p.customerName  = readLineEscaped(in);
        in >> p.amount; in.ignore();
        p.method        = readLineEscaped(in);
        p.date          = readLineEscaped(in);
        p.status        = readLineEscaped(in);
        p.transactionId = readLineEscaped(in);
        payments.push_back(p);
    }
}

// ============================================================
// FOOD ORDERS
// ============================================================
void FileManager::saveFoodOrders(const std::vector<FoodOrder>& orders) {
    ensureDataDirExists();
    std::ofstream out(filePath("foodorders.txt"));
    if (!out) return;
    out << orders.size() << '\n';
    for (const auto& o : orders) {
        out << o.orderId << '\n' << o.bookingId << '\n';
        writeLineEscaped(out, o.customerName);
        writeLineEscaped(out, o.items);
        out << o.amount << '\n';
        writeLineEscaped(out, o.status);
        writeLineEscaped(out, o.orderTime);
        writeLineEscaped(out, o.roomNo);
    }
}

void FileManager::loadFoodOrders(std::vector<FoodOrder>& orders) {
    std::ifstream in(filePath("foodorders.txt"));
    if (!in) return;
    int size = 0; in >> size; in.ignore();
    orders.clear();
    for (int i = 0; i < size; ++i) {
        FoodOrder o;
        in >> o.orderId >> o.bookingId; in.ignore();
        o.customerName = readLineEscaped(in);
        o.items        = readLineEscaped(in);
        in >> o.amount; in.ignore();
        o.status     = readLineEscaped(in);
        o.orderTime  = readLineEscaped(in);
        o.roomNo     = readLineEscaped(in);
        orders.push_back(o);
    }
}

// ============================================================
// COMPLAINTS
// ============================================================
void FileManager::saveComplaints(const std::vector<Complaint>& complaints) {
    ensureDataDirExists();
    std::ofstream out(filePath("complaints.txt"));
    if (!out) return;
    out << complaints.size() << '\n';
    for (const auto& c : complaints) {
        out << c.complaintId << '\n' << c.bookingId << '\n';
        writeLineEscaped(out, c.customerName);
        writeLineEscaped(out, c.complaint);
        writeLineEscaped(out, c.status);
        writeLineEscaped(out, c.date);
        writeLineEscaped(out, c.resolution);
        writeLineEscaped(out, c.roomNo);
        writeLineEscaped(out, c.category);
        out << c.priority << '\n';
    }
}

void FileManager::loadComplaints(std::vector<Complaint>& complaints) {
    std::ifstream in(filePath("complaints.txt"));
    if (!in) return;
    int size = 0; in >> size; in.ignore();
    complaints.clear();
    for (int i = 0; i < size; ++i) {
        Complaint c;
        in >> c.complaintId >> c.bookingId; in.ignore();
        c.customerName = readLineEscaped(in);
        c.complaint    = readLineEscaped(in);
        c.status       = readLineEscaped(in);
        c.date         = readLineEscaped(in);
        c.resolution   = readLineEscaped(in);
        c.roomNo       = readLineEscaped(in);
        c.category     = readLineEscaped(in);
        in >> c.priority; in.ignore();
        complaints.push_back(c);
    }
}

// ============================================================
// LAUNDRY
// ============================================================
void FileManager::saveLaundry(const std::vector<Laundry>& laundry) {
    ensureDataDirExists();
    std::ofstream out(filePath("laundry.txt"));
    if (!out) return;
    out << laundry.size() << '\n';
    for (const auto& l : laundry) {
        out << l.laundryId << '\n' << l.bookingId << '\n';
        writeLineEscaped(out, l.customerName);
        writeLineEscaped(out, l.roomNo);
        writeLineEscaped(out, l.items);
        out << l.amount << '\n';
        writeLineEscaped(out, l.status);
        writeLineEscaped(out, l.pickupDate);
        writeLineEscaped(out, l.deliveryDate);
    }
}

void FileManager::loadLaundry(std::vector<Laundry>& laundry) {
    std::ifstream in(filePath("laundry.txt"));
    if (!in) return;
    int size = 0; in >> size; in.ignore();
    laundry.clear();
    for (int i = 0; i < size; ++i) {
        Laundry l;
        in >> l.laundryId >> l.bookingId; in.ignore();
        l.customerName = readLineEscaped(in);
        l.roomNo       = readLineEscaped(in);
        l.items        = readLineEscaped(in);
        in >> l.amount; in.ignore();
        l.status       = readLineEscaped(in);
        l.pickupDate   = readLineEscaped(in);
        l.deliveryDate = readLineEscaped(in);
        laundry.push_back(l);
    }
}

// ============================================================
// TRANSPORT
// ============================================================
void FileManager::saveTransport(const std::vector<Transport>& transport) {
    ensureDataDirExists();
    std::ofstream out(filePath("transport.txt"));
    if (!out) return;
    out << transport.size() << '\n';
    for (const auto& t : transport) {
        out << t.transportId << '\n' << t.bookingId << '\n';
        writeLineEscaped(out, t.customerName);
        writeLineEscaped(out, t.pickupPoint);
        writeLineEscaped(out, t.dropPoint);
        writeLineEscaped(out, t.date);
        out << t.amount << '\n';
        writeLineEscaped(out, t.status);
        writeLineEscaped(out, t.vehicleType);
    }
}

void FileManager::loadTransport(std::vector<Transport>& transport) {
    std::ifstream in(filePath("transport.txt"));
    if (!in) return;
    int size = 0; in >> size; in.ignore();
    transport.clear();
    for (int i = 0; i < size; ++i) {
        Transport t;
        in >> t.transportId >> t.bookingId; in.ignore();
        t.customerName = readLineEscaped(in);
        t.pickupPoint  = readLineEscaped(in);
        t.dropPoint    = readLineEscaped(in);
        t.date         = readLineEscaped(in);
        in >> t.amount; in.ignore();
        t.status      = readLineEscaped(in);
        t.vehicleType = readLineEscaped(in);
        transport.push_back(t);
    }
}

// ============================================================
// COUNTERS
// ============================================================
void FileManager::saveCounters(int bk, int st, int ord, int cmp,
                                int task, int pay, int laund, int trans) {
    ensureDataDirExists();
    std::ofstream out(filePath("counters.txt"));
    if (!out) return;
    out << bk << '\n' << st << '\n' << ord << '\n' << cmp << '\n'
        << task << '\n' << pay << '\n' << laund << '\n' << trans << '\n';
}

void FileManager::loadCounters(int& bk, int& st, int& ord, int& cmp,
                                int& task, int& pay, int& laund, int& trans) {
    std::ifstream in(filePath("counters.txt"));
    if (!in) return;
    in >> bk >> st >> ord >> cmp >> task >> pay >> laund >> trans;
}

// ============================================================
// CREDENTIALS (Phase 5)
// ============================================================
void FileManager::initCredentials() {
    ensureDataDirExists();
    std::string path = filePath("credentials.txt");
    std::ifstream check(path);
    if (check.good()) return; // Already exists
    check.close();

    std::ofstream out(path);
    if (!out) return;
    // Format: username|password|role
    out << "admin|admin123|Admin\n";
    out << "receptionist|recep456|Receptionist\n";
    out << "manager|mgr789|Manager\n";
}

bool FileManager::authenticate(const std::string& user, const std::string& pass, std::string& role) {
    std::ifstream in(filePath("credentials.txt"));
    if (!in) return false;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string u, p, r;
        std::getline(ss, u, '|');
        std::getline(ss, p, '|');
        std::getline(ss, r, '|');
        if (u == user && p == pass) {
            role = r;
            return true;
        }
    }
    return false;
}

// ============================================================
// INVOICE GENERATION (Phase 8)
// ============================================================
void FileManager::generateInvoice(const Customer& customer,
                                   const std::vector<FoodOrder>& foodOrders,
                                   const std::vector<Laundry>& laundry,
                                   const std::vector<Transport>& transport) {
    ensureDataDirExists();
    std::string filename = filePath("invoice_" + std::to_string(customer.getBookingId()) + ".txt");
    std::ofstream out(filename);
    if (!out) return;

    out << "============================================================\n";
    out << "           HOTEL DELIGHT - OFFICIAL INVOICE\n";
    out << "============================================================\n";
    out << "Invoice No  : INV-" << std::setfill('0') << std::setw(6) << customer.getBookingId() << "\n";
    out << "Date        : " << Utility::getCurrentDateTime() << "\n";
    out << "------------------------------------------------------------\n";
    out << "GUEST INFORMATION\n";
    out << "------------------------------------------------------------\n";
    out << "Name        : " << customer.getName()        << "\n";
    out << "Phone       : " << customer.getPhone()       << "\n";
    out << "Email       : " << customer.getEmail()       << "\n";
    out << "Nationality : " << customer.getNationality() << "\n";
    out << "ID Type     : " << customer.getIdType()      << "\n";
    out << "------------------------------------------------------------\n";
    out << "BOOKING DETAILS\n";
    out << "------------------------------------------------------------\n";
    out << "Booking ID  : " << customer.getBookingId()   << "\n";
    out << "Room        : " << customer.getRoomId()       << "\n";
    out << "Check-In    : " << customer.getCheckInDate()  << "\n";
    out << "Check-Out   : " << customer.getCheckOutDate() << "\n";
    out << "Days        : " << customer.getDays()         << "\n";
    out << "Guests      : " << customer.getNumberOfGuests()<< "\n";
    out << "------------------------------------------------------------\n";
    out << "BILLING DETAILS\n";
    out << "------------------------------------------------------------\n";

    // Room charges
    // Recalculate breakdown for invoice clarity
    // Find food, laundry, transport for this booking
    double foodTotal = 0, laundryTotal = 0, transportTotal = 0;
    for (const auto& f : foodOrders)
        if (f.bookingId == customer.getBookingId()) foodTotal += f.amount;
    for (const auto& l : laundry)
        if (l.bookingId == customer.getBookingId()) laundryTotal += l.amount;
    for (const auto& t : transport)
        if (t.bookingId == customer.getBookingId()) transportTotal += t.amount;

    double servicesTotal = foodTotal + laundryTotal + transportTotal;
    double billBase = customer.getTotalBill() - servicesTotal;

    out << std::fixed << std::setprecision(2);
    out << "Room Charges   : $" << billBase       << "\n";
    if (foodTotal > 0)
        out << "Food Services  : $" << foodTotal      << "\n";
    if (laundryTotal > 0)
        out << "Laundry        : $" << laundryTotal   << "\n";
    if (transportTotal > 0)
        out << "Transport      : $" << transportTotal << "\n";
    out << "------------------------------------------------------------\n";
    out << "TOTAL BILL     : $" << customer.getTotalBill()   << "\n";
    out << "Advance Paid   : $" << customer.getAdvancePayment() << "\n";
    out << "DUE AMOUNT     : $" << (customer.getTotalBill() - customer.getAdvancePayment()) << "\n";
    out << "Payment Method : " << customer.getPaymentMethod()  << "\n";
    out << "Payment Status : " << customer.getPaymentStatus()  << "\n";
    out << "------------------------------------------------------------\n";
    out << "Thank you for staying with HOTEL DELIGHT!\n";
    out << "We hope to see you again.\n";
    out << "============================================================\n";

    Utility::printSuccess("Invoice saved: " + filename);
}
