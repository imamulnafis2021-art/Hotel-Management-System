#pragma once
#include <string>

// ============================================================
// SUPPORTING DATA STRUCTURES
// ============================================================

struct FoodOrder {
    int    orderId      = 0;
    int    bookingId    = 0;
    std::string customerName;
    std::string items;
    double amount       = 0.0;
    std::string status;
    std::string orderTime;
    std::string roomNo;
};

struct Complaint {
    int    complaintId  = 0;
    int    bookingId    = 0;
    std::string customerName;
    std::string complaint;
    std::string status;
    std::string date;
    std::string resolution;
    std::string roomNo;
    std::string category;
    int    priority     = 2; // 1=High,2=Medium,3=Low
};

struct Housekeeping {
    int    taskId       = 0;
    std::string roomId;
    std::string status;
    std::string assignedTo;
    std::string date;
    std::string taskType;
};

struct Payment {
    int    paymentId    = 0;
    int    bookingId    = 0;
    std::string customerName;
    double amount       = 0.0;
    std::string method;
    std::string date;
    std::string status;
    std::string transactionId;
};

struct Laundry {
    int    laundryId    = 0;
    int    bookingId    = 0;
    std::string customerName;
    std::string roomNo;
    std::string items;
    double amount       = 0.0;
    std::string status;
    std::string pickupDate;
    std::string deliveryDate;
};

struct Transport {
    int    transportId  = 0;
    int    bookingId    = 0;
    std::string customerName;
    std::string pickupPoint;
    std::string dropPoint;
    std::string date;
    double amount       = 0.0;
    std::string status;
    std::string vehicleType;
};
