#pragma once
#include <string>
#include <iostream>

// ============================================================
// ABSTRACT BASE CLASS: Person
// Phase 2 - Inheritance base
// Phase 3 - Polymorphism via pure virtual displayInfo()
// ============================================================
class Person {
protected:
    std::string name;
    std::string phone;
    std::string address;
    std::string email;

public:
    Person() = default;
    Person(const std::string& name, const std::string& phone,
           const std::string& address, const std::string& email)
        : name(name), phone(phone), address(address), email(email) {}

    virtual ~Person() = default;

    // Pure virtual - forces subclasses to implement (Phase 3)
    virtual void displayInfo() const = 0;

    // Getters
    const std::string& getName()    const { return name; }
    const std::string& getPhone()   const { return phone; }
    const std::string& getAddress() const { return address; }
    const std::string& getEmail()   const { return email; }

    // Setters
    void setName(const std::string& n)    { name = n; }
    void setPhone(const std::string& p)   { phone = p; }
    void setAddress(const std::string& a) { address = a; }
    void setEmail(const std::string& e)   { email = e; }
};
