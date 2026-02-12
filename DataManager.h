// ==================== DataManager.h ====================
/**
 * DataManager.h
 * Handles all file I/O operations for persistent data storage
 * TASKS 1, 2, 3: Manages users, transactions, wallet, and candlestick generation
 */

#pragma once
#include <string>
#include <vector>
#include <map>
#include "User.h"
#include "Transaction.h"
#include "Candlestick.h"
#include "OrderBookEntry.h"

class DataManager
{
public:
    DataManager();

    // TASK 2: User management
    bool saveUser(const User& user);
    User loadUser(std::string username);
    std::vector<User> loadAllUsers();
    bool userExists(std::string email, std::string fullName);
    User getUserByEmail(std::string email);

    // TASK 3: Transaction management
    bool saveTransaction(const Transaction& transaction);
    std::vector<Transaction> loadUserTransactions(std::string username);
    std::vector<Transaction> loadUserTransactions(std::string username, int limit);
    std::vector<Transaction> loadUserTransactionsByProduct(std::string username, std::string product);

    // TASK 3: Wallet balance management
    bool saveWalletBalance(std::string username, std::string currency, double amount);
    std::map<std::string, double> loadWalletBalance(std::string username);

    // TASK 1: Candlestick data generation
    std::vector<Candlestick> generateCandlesticks(
        const std::vector<OrderBookEntry>& orders,
        std::string product,
        std::string period,  // "daily", "monthly", "yearly"
        OrderBookType type); // ask or bid

    static std::string extractDate(std::string timestamp, std::string period);

private:
    const std::string USERS_FILE = "users.csv";
    const std::string TRANSACTIONS_FILE = "transactions.csv";
    const std::string WALLET_FILE = "wallet.csv";

    bool fileExists(std::string filename);
    void createFileIfNotExists(std::string filename);
};