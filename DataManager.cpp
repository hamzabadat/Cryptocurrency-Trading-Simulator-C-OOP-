// ==================== DataManager.cpp ====================
/**
 * DataManager.cpp
 * Implementation of data persistence and candlestick generation
 */

#include "DataManager.h"
#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

DataManager::DataManager()
{
    // Ensure CSV files exist
    createFileIfNotExists(USERS_FILE);
    createFileIfNotExists(TRANSACTIONS_FILE);
    createFileIfNotExists(WALLET_FILE);
}

bool DataManager::fileExists(std::string filename)
{
    std::ifstream file(filename);
    return file.good();
}

void DataManager::createFileIfNotExists(std::string filename)
{
    if (!fileExists(filename))
    {
        std::ofstream file(filename);
        file.close();
    }
}

// ==================== USER MANAGEMENT ====================

bool DataManager::saveUser(const User& user)
{
    std::ofstream file(USERS_FILE, std::ios::app);
    if (file.is_open())
    {
        file << user.toCSVString() << std::endl;
        file.close();
        return true;
    }
    return false;
}

User DataManager::loadUser(std::string username)
{
    std::ifstream file(USERS_FILE);
    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            User user = User::fromCSVString(line);
            if (user.getUsername() == username)
            {
                file.close();
                return user;
            }
        }
        file.close();
    }
    return User();
}

std::vector<User> DataManager::loadAllUsers()
{
    std::vector<User> users;
    std::ifstream file(USERS_FILE);
    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (!line.empty())
            {
                users.push_back(User::fromCSVString(line));
            }
        }
        file.close();
    }
    return users;
}

bool DataManager::userExists(std::string email, std::string fullName)
{
    std::vector<User> users = loadAllUsers();
    for (const User& user : users)
    {
        if (user.getEmail() == email && user.getFullName() == fullName)
        {
            return true;
        }
    }
    return false;
}

User DataManager::getUserByEmail(std::string email)
{
    std::vector<User> users = loadAllUsers();
    for (const User& user : users)
    {
        if (user.getEmail() == email)
        {
            return user;
        }
    }
    return User();
}

// ==================== TRANSACTION MANAGEMENT ====================

bool DataManager::saveTransaction(const Transaction& transaction)
{
    std::ofstream file(TRANSACTIONS_FILE, std::ios::app);
    if (file.is_open())
    {
        file << transaction.toCSVString() << std::endl;
        file.close();
        return true;
    }
    return false;
}

std::vector<Transaction> DataManager::loadUserTransactions(std::string username)
{
    std::vector<Transaction> transactions;
    std::ifstream file(TRANSACTIONS_FILE);
    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (!line.empty())
            {
                Transaction trans = Transaction::fromCSVString(line);
                if (trans.getUsername() == username)
                {
                    transactions.push_back(trans);
                }
            }
        }
        file.close();
    }
    return transactions;
}

std::vector<Transaction> DataManager::loadUserTransactions(std::string username, int limit)
{
    std::vector<Transaction> allTransactions = loadUserTransactions(username);
    std::vector<Transaction> recentTransactions;

    // Get last 'limit' transactions
    int start = std::max(0, (int)allTransactions.size() - limit);
    for (size_t i = start; i < allTransactions.size(); i++)
    {
        recentTransactions.push_back(allTransactions[i]);
    }

    return recentTransactions;
}

std::vector<Transaction> DataManager::loadUserTransactionsByProduct(std::string username, std::string product)
{
    std::vector<Transaction> transactions;
    std::ifstream file(TRANSACTIONS_FILE);
    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (!line.empty())
            {
                Transaction trans = Transaction::fromCSVString(line);
                if (trans.getUsername() == username && trans.getProduct() == product)
                {
                    transactions.push_back(trans);
                }
            }
        }
        file.close();
    }
    return transactions;
}

// ==================== WALLET MANAGEMENT ====================

bool DataManager::saveWalletBalance(std::string username, std::string currency, double amount)
{
    std::vector<std::string> lines;
    std::ifstream inFile(WALLET_FILE);
    std::string line;
    bool found = false;

    if (inFile.is_open())
    {
        while (std::getline(inFile, line))
        {
            std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
            if (tokens.size() == 3 && tokens[0] == username && tokens[1] == currency)
            {
                // Update existing entry
                lines.push_back(username + "," + currency + "," + std::to_string(amount));
                found = true;
            }
            else if (!line.empty())
            {
                lines.push_back(line);
            }
        }
        inFile.close();
    }

    // Add new entry if not found
    if (!found)
    {
        lines.push_back(username + "," + currency + "," + std::to_string(amount));
    }

    // Write back to file
    std::ofstream outFile(WALLET_FILE);
    if (outFile.is_open())
    {
        for (const std::string& l : lines)
        {
            outFile << l << std::endl;
        }
        outFile.close();
        return true;
    }

    return false;
}

std::map<std::string, double> DataManager::loadWalletBalance(std::string username)
{
    std::map<std::string, double> wallet;
    std::ifstream file(WALLET_FILE);
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (!line.empty())
            {
                std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
                if (tokens.size() == 3 && tokens[0] == username)
                {
                    wallet[tokens[1]] = std::stod(tokens[2]);
                }
            }
        }
        file.close();
    }

    return wallet;
}

// ==================== TASK 1: CANDLESTICK GENERATION ====================

std::vector<Candlestick> DataManager::generateCandlesticks(
    const std::vector<OrderBookEntry>& orders,
    std::string product,
    std::string period,
    OrderBookType type)
{
    std::vector<Candlestick> candlesticks;

    // Group orders by date period
    std::map<std::string, std::vector<OrderBookEntry>> groupedOrders;

    for (const OrderBookEntry& order : orders)
    {
        if (order.product == product && order.orderType == type)
        {
            std::string dateKey = extractDate(order.timestamp, period);
            groupedOrders[dateKey].push_back(order);
        }
    }

    // Generate candlestick for each period
    for (auto& pair : groupedOrders)
    {
        std::string date = pair.first;
        std::vector<OrderBookEntry>& periodOrders = pair.second;

        if (periodOrders.empty()) continue;

        // Calculate OHLC values
        double open = periodOrders[0].price;
        double close = periodOrders[periodOrders.size() - 1].price;
        double high = periodOrders[0].price;
        double low = periodOrders[0].price;

        for (const OrderBookEntry& order : periodOrders)
        {
            if (order.price > high) high = order.price;
            if (order.price < low) low = order.price;
        }

        std::string typeStr = (type == OrderBookType::ask) ? "ask" : "bid";
        candlesticks.push_back(Candlestick(date, open, high, low, close, product, typeStr));
    }

    // Sort chronologically
    std::sort(candlesticks.begin(), candlesticks.end(), Candlestick::compareByDate);

    return candlesticks;
}

std::string DataManager::extractDate(std::string timestamp, std::string period)
{
    // Timestamp format: 2020/03/17 17:01:24.884492
    if (timestamp.length() < 10) return timestamp;

    std::string year = timestamp.substr(0, 4);
    std::string month = timestamp.substr(5, 2);
    std::string day = timestamp.substr(8, 2);

    if (period == "daily")
    {
        return year + "-" + month + "-" + day;
    }
    else if (period == "monthly")
    {
        return year + "-" + month;
    }
    else if (period == "yearly")
    {
        return year;
    }

    return year + "-" + month + "-" + day;
}