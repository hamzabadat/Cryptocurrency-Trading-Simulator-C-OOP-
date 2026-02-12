// ==================== User.cpp ====================
/**
 * User.cpp
 * Implementation of user authentication and profile management
 */

#include "User.h"
#include "CSVReader.h"
#include <random>
#include <chrono>
#include <functional>
#include <sstream>

User::User(std::string _username,
    std::string _fullName,
    std::string _email,
    std::string _passwordHash)
    : username(_username),
    fullName(_fullName),
    email(_email),
    passwordHash(_passwordHash)
{
}

User::User()
    : username(""),
    fullName(""),
    email(""),
    passwordHash("")
{
}

std::string User::toCSVString() const
{
    return username + "," + fullName + "," + email + "," + passwordHash;
}

User User::fromCSVString(std::string csvLine)
{
    std::vector<std::string> tokens = CSVReader::tokenise(csvLine, ',');
    if (tokens.size() == 4)
    {
        return User(tokens[0], tokens[1], tokens[2], tokens[3]);
    }
    return User();
}

std::string User::generateUsername()
{
    // TASK 2: Generate unique 10-digit username
    // Combines timestamp with random number for uniqueness
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000000000, 9999999999);

    // Combine timestamp with random number
    long long combined = (millis % 100000) * 100000 + (dis(gen) % 100000);

    std::string username = std::to_string(combined);

    // Ensure exactly 10 digits
    while (username.length() < 10)
    {
        username = "0" + username;
    }
    if (username.length() > 10)
    {
        username = username.substr(username.length() - 10);
    }

    return username;
}

std::string User::hashPassword(std::string password)
{
    // TASK 2: Hash password using std::hash (one-way function)
    std::hash<std::string> hasher;
    size_t hash = hasher(password);
    return std::to_string(hash);
}