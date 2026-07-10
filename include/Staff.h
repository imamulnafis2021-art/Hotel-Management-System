#pragma once
#include "Person.h"
#include <string>

// ============================================================
// STAFF CLASS  (Phase 2: derived from Person)
// ============================================================
class Staff : public Person {
private:
    int         staffId;
    std::string position;
    double      salary;
    std::string joinDate;
    std::string shift;
    std::string department;
    bool        active;

public:
    Staff();

    // Phase 3: override pure virtual
    void displayInfo() const override;
    void displayRow()  const;

    // ---- Getters ----
    int         getStaffId()     const { return staffId; }
    const std::string& getPosition()   const { return position; }
    double      getSalary()      const { return salary; }
    const std::string& getJoinDate()   const { return joinDate; }
    const std::string& getShift()      const { return shift; }
    const std::string& getDepartment() const { return department; }
    bool        isActive()       const { return active; }

    // ---- Setters ----
    void setStaffId(int v)                      { staffId = v; }
    void setPosition(const std::string& v)      { position = v; }
    void setSalary(double v)                    { salary = v; }
    void setJoinDate(const std::string& v)      { joinDate = v; }
    void setShift(const std::string& v)         { shift = v; }
    void setDepartment(const std::string& v)    { department = v; }
    void setActive(bool v)                      { active = v; }
};
