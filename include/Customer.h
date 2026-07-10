#pragma once
#include "Person.h"
#include <string>
#include <iostream>
#include <iomanip>

// ============================================================
// CUSTOMER CLASS  (Phase 2: derived from Person)
// ============================================================
class Customer : public Person {
private:
    int         bookingId;
    std::string nid;
    std::string idType;
    std::string nationality;
    std::string roomId;
    int         days;
    std::string checkInDate;
    std::string checkOutDate;
    double      advancePayment;
    double      totalBill;
    std::string paymentStatus;
    std::string paymentMethod;
    std::string specialRequests;
    int         numberOfGuests;
    std::string emergencyContact;
    std::string emergencyPhone;
    bool        checkedOut;

public:
    Customer();

    // Phase 3: override pure virtual
    void displayInfo() const override;
    void displayRow()  const;
    void displayFull() const;

    // ---- Getters ----
    int         getBookingId()       const { return bookingId; }
    const std::string& getNid()          const { return nid; }
    const std::string& getIdType()       const { return idType; }
    const std::string& getNationality()  const { return nationality; }
    const std::string& getRoomId()       const { return roomId; }
    int         getDays()            const { return days; }
    const std::string& getCheckInDate()  const { return checkInDate; }
    const std::string& getCheckOutDate() const { return checkOutDate; }
    double      getAdvancePayment()  const { return advancePayment; }
    double      getTotalBill()       const { return totalBill; }
    const std::string& getPaymentStatus()  const { return paymentStatus; }
    const std::string& getPaymentMethod()  const { return paymentMethod; }
    const std::string& getSpecialRequests() const { return specialRequests; }
    int         getNumberOfGuests()  const { return numberOfGuests; }
    const std::string& getEmergencyContact() const { return emergencyContact; }
    const std::string& getEmergencyPhone()   const { return emergencyPhone; }
    bool        isCheckedOut()       const { return checkedOut; }

    // ---- Setters ----
    void setBookingId(int v)                        { bookingId = v; }
    void setNid(const std::string& v)               { nid = v; }
    void setIdType(const std::string& v)            { idType = v; }
    void setNationality(const std::string& v)       { nationality = v; }
    void setRoomId(const std::string& v)            { roomId = v; }
    void setDays(int v)                             { days = v; }
    void setCheckInDate(const std::string& v)       { checkInDate = v; }
    void setCheckOutDate(const std::string& v)      { checkOutDate = v; }
    void setAdvancePayment(double v)                { advancePayment = v; }
    void setTotalBill(double v)                     { totalBill = v; }
    void addToTotalBill(double v)                   { totalBill += v; }
    void setPaymentStatus(const std::string& v)     { paymentStatus = v; }
    void setPaymentMethod(const std::string& v)     { paymentMethod = v; }
    void setSpecialRequests(const std::string& v)   { specialRequests = v; }
    void setNumberOfGuests(int v)                   { numberOfGuests = v; }
    void setEmergencyContact(const std::string& v)  { emergencyContact = v; }
    void setEmergencyPhone(const std::string& v)    { emergencyPhone = v; }
    void setCheckedOut(bool v)                      { checkedOut = v; }
};
