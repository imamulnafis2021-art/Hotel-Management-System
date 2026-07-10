#include "Utility.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace Utility {

// ---- Date / Time ----

std::string getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm* tm_info = std::localtime(&now);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return std::string(buf);
}

std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* tm_info = std::localtime(&now);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm_info);
    return std::string(buf);
}

std::string calculateCheckOutDate(const std::string& checkIn, int days) {
    std::tm tm_info = {};
    std::istringstream ss(checkIn);
    ss >> std::get_time(&tm_info, "%Y-%m-%d");
    if (ss.fail()) {
        // Fallback: use current date
        std::time_t now = std::time(nullptr);
        now += static_cast<std::time_t>(days) * 86400;
        std::tm* tm2 = std::localtime(&now);
        char buf[16];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm2);
        return std::string(buf);
    }
    std::time_t t = std::mktime(&tm_info);
    t += static_cast<std::time_t>(days) * 86400;
    std::tm* tm2 = std::localtime(&t);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm2);
    return std::string(buf);
}

bool isValidDateFormat(const std::string& date) {
    if (date.size() != 10) return false;
    std::regex pattern(R"(\d{4}-\d{2}-\d{2})");
    if (!std::regex_match(date, pattern)) return false;
    int y, m, d;
    char c1, c2;
    std::istringstream ss(date);
    ss >> y >> c1 >> m >> c2 >> d;
    return (m >= 1 && m <= 12 && d >= 1 && d <= 31);
}

int daysBetween(const std::string& d1, const std::string& d2) {
    std::tm t1 = {}, t2 = {};
    std::istringstream ss1(d1), ss2(d2);
    ss1 >> std::get_time(&t1, "%Y-%m-%d");
    ss2 >> std::get_time(&t2, "%Y-%m-%d");
    std::time_t time1 = std::mktime(&t1);
    std::time_t time2 = std::mktime(&t2);
    return static_cast<int>(std::difftime(time2, time1) / 86400);
}

// ---- Console UI ----

void printHeader(const std::string& title) {
    std::cout << "\n" << Color::BOLD << Color::BLUE;
    std::cout << "============================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "============================================================" << Color::RESET << "\n";
}

void printLine() {
    std::cout << Color::CYAN
              << "------------------------------------------------------------"
              << Color::RESET << "\n";
}

void printSuccess(const std::string& msg) {
    std::cout << Color::GREEN << "[SUCCESS] " << msg << Color::RESET << "\n";
}

void printError(const std::string& msg) {
    std::cout << Color::RED << "[ERROR] " << msg << Color::RESET << "\n";
}

void printInfo(const std::string& msg) {
    std::cout << Color::YELLOW << "[INFO] " << msg << Color::RESET << "\n";
}

void printWarning(const std::string& msg) {
    std::cout << Color::MAGENTA << "[WARNING] " << msg << Color::RESET << "\n";
}

void pause() {
    std::cout << "\n" << Color::CYAN << "Press ENTER to continue..." << Color::RESET;
    std::cin.get();
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << std::string(50, '\n');
#endif
}

// ---- Validation ----

bool validatePhone(const std::string& phone) {
    if (phone.length() < 10 || phone.length() > 15) return false;
    for (char c : phone) {
        if (!std::isdigit(c) && c != '+' && c != '-') return false;
    }
    // At least 10 digits
    int digits = 0;
    for (char c : phone) if (std::isdigit(c)) digits++;
    return digits >= 10;
}

bool validateEmail(const std::string& email) {
    std::regex pattern(R"([a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, pattern);
}

bool validateNID(const std::string& nid) {
    if (nid.length() < 8) return false;
    for (char c : nid) {
        if (!std::isdigit(c) && c != '-') return false;
    }
    return true;
}

bool validateName(const std::string& name) {
    if (name.empty() || name.length() < 2) return false;
    for (char c : name) {
        if (!std::isalpha(c) && c != ' ' && c != '.' && c != '-') return false;
    }
    return true;
}

bool validatePositiveDouble(double val) {
    return val > 0.0;
}

bool validatePositiveInt(int val) {
    return val > 0;
}

// ---- String helpers ----

std::string toUpperStr(const std::string& s) {
    std::string result = s;
    for (char& c : result) c = static_cast<char>(std::toupper(c));
    return result;
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end   = s.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

std::string generateTransactionId(int id) {
    return "TXN" + std::to_string(id);
}

} // namespace Utility
