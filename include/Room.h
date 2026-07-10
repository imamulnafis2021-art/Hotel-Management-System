#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Utility.h"

// ============================================================
// ROOM CLASS
// ============================================================
class Room {
private:
    std::string id;
    std::string type;
    double      price;
    int         capacity;
    bool        booked;
    std::string amenities;
    std::string floor;
    std::string view;
    double      rating;
    std::string bedType;
    std::string acType;

    // Phase 11: date-based booking slots
    std::vector<std::pair<std::string,std::string>> bookingSlots; // {checkIn, checkOut}

public:
    Room() : price(0), capacity(0), booked(false), rating(0) {}

    // ---- Getters ----
    const std::string& getId()        const { return id; }
    const std::string& getType()      const { return type; }
    double             getPrice()     const { return price; }
    int                getCapacity()  const { return capacity; }
    bool               isBooked()     const { return booked; }
    const std::string& getAmenities() const { return amenities; }
    const std::string& getFloor()     const { return floor; }
    const std::string& getView()      const { return view; }
    double             getRating()    const { return rating; }
    const std::string& getBedType()   const { return bedType; }
    const std::string& getAcType()    const { return acType; }

    // ---- Setters ----
    void setId(const std::string& v)        { id = v; }
    void setType(const std::string& v)      { type = v; }
    void setPrice(double v)                 { price = v; }
    void setCapacity(int v)                 { capacity = v; }
    void setBooked(bool v)                  { booked = v; }
    void setAmenities(const std::string& v) { amenities = v; }
    void setFloor(const std::string& v)     { floor = v; }
    void setView(const std::string& v)      { view = v; }
    void setRating(double v)                { rating = v; }
    void setBedType(const std::string& v)   { bedType = v; }
    void setAcType(const std::string& v)    { acType = v; }

    // Phase 11: date-range availability
    bool isAvailableForDates(const std::string& checkIn, const std::string& checkOut) const;
    void addBookingSlot(const std::string& checkIn, const std::string& checkOut);
    void removeBookingSlot(const std::string& checkIn, const std::string& checkOut);

    void displayRow() const;
    void displayFull() const;
};
