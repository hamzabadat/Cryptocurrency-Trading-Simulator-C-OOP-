// ==================== User.h ====================
/**
 * User.h
 * User authentication and profile management
 * TASK 2: Handles user registration, login, and password hashing
 */

#pragma once
#include <string>
#include <vector>

class User
{
public:
    User(std::string _username,
        std::string _fullName,
        std::string _email,
        std::string _passwordHash);

    User();

    // Getters
    std::string getUsername() const { return username; }
    std::string getFullName() const { return fullName; }
    std::string getEmail() const { return email; }
    std::string getPasswordHash() const { return passwordHash; }

    // Setters
    void setPasswordHash(std::string hash) { passwordHash = hash; }

    // CSV persistence
    std::string toCSVString() const;
    static User fromCSVString(std::string csvLine);

    // TASK 2: Authentication functions
    static std::string generateUsername();
    static std::string hashPassword(std::string password);

private:
    std::string username;      // 10-digit unique identifier
    std::string fullName;
    std::string email;
    std::string passwordHash;  // Hashed with std::hash
};