#pragma once
#include <string>
#include <iostream>
#include <limits>
#include <regex>
#include <stdexcept>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>

// ============================================================
// COLOR NAMESPACE
// ============================================================
namespace Color {
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* RED     = "\033[31m";
    constexpr const char* GREEN   = "\033[32m";
    constexpr const char* YELLOW  = "\033[33m";
    constexpr const char* BLUE    = "\033[34m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* CYAN    = "\033[36m";
    constexpr const char* WHITE   = "\033[37m";
    constexpr const char* BOLD    = "\033[1m";
    constexpr const char* BG_BLUE = "\033[44m";
}

// ============================================================
// UTILITY FUNCTIONS
// ============================================================
namespace Utility {

    // ---- Date / Time ----
    std::string getCurrentDateTime();
    std::string getCurrentDate();
    std::string calculateCheckOutDate(const std::string& checkIn, int days);
    bool isValidDateFormat(const std::string& date);
    int daysBetween(const std::string& d1, const std::string& d2);

    // ---- Console UI ----
    void printHeader(const std::string& title);
    void printLine();
    void printSuccess(const std::string& msg);
    void printError(const std::string& msg);
    void printInfo(const std::string& msg);
    void printWarning(const std::string& msg);
    void pause();
    void clearInput();
    void clearScreen();

    // ---- Validation ----
    bool validatePhone(const std::string& phone);
    bool validateEmail(const std::string& email);
    bool validateNID(const std::string& nid);
    bool validateName(const std::string& name);
    bool validatePositiveDouble(double val);
    bool validatePositiveInt(int val);

    // ---- String helpers ----
    std::string toUpperStr(const std::string& s);
    std::string trim(const std::string& s);
    std::string generateTransactionId(int id);
}
