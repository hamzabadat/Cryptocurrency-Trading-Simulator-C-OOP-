// ==================== Transaction.h ====================
/**
 * Transaction.h
 * Transaction logging for wallet and trading operations
 * TASK 3: Tracks all user wallet activities and order placements
 */

#pragma once
#include <string>

enum class TransactionType
{
    DEPOSIT,
    WITHDRAWAL,
    ASK_PLACED,
    BID_PLACED,
    ASK_FILLED,
    BID_FILLED
};

class Transaction
{
public:
    Transaction(std::string _username,
        std::string _timestamp,
        TransactionType _type,
        std::string _product,
        double _amount,
        double _price,
        double _balanceAfter);

    Transaction();

    // Getters
    std::string getUsername() const { return username; }
    std::string getTimestamp() const { return timestamp; }
    TransactionType getType() const { return type; }
    std::string getProduct() const { return product; }
    double getAmount() const { return amount; }
    double getPrice() const { return price; }
    double getBalanceAfter() const { return balanceAfter; }

    // CSV persistence
    std::string toCSVString() const;
    static Transaction fromCSVString(std::string csvLine);

    // Type conversion utilities
    static std::string typeToString(TransactionType type);
    static TransactionType stringToType(std::string str);

    std::string toString() const;

private:
    std::string username;
    std::string timestamp;
    TransactionType type;
    std::string product;
    double amount;
    double price;
    double balanceAfter;
};