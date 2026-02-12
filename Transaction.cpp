// ==================== Transaction.cpp ====================
/**
 * Transaction.cpp
 * Implementation of transaction logging
 */

#include "Transaction.h"
#include "CSVReader.h"
#include <sstream>
#include <iomanip>

Transaction::Transaction(std::string _username,
    std::string _timestamp,
    TransactionType _type,
    std::string _product,
    double _amount,
    double _price,
    double _balanceAfter)
    : username(_username),
    timestamp(_timestamp),
    type(_type),
    product(_product),
    amount(_amount),
    price(_price),
    balanceAfter(_balanceAfter)
{
}

Transaction::Transaction()
    : username(""),
    timestamp(""),
    type(TransactionType::DEPOSIT),
    product(""),
    amount(0.0),
    price(0.0),
    balanceAfter(0.0)
{
}

std::string Transaction::toCSVString() const
{
    std::ostringstream oss;
    oss << username << "," << timestamp << "," << typeToString(type) << ","
        << product << "," << amount << "," << price << "," << balanceAfter;
    return oss.str();
}

Transaction Transaction::fromCSVString(std::string csvLine)
{
    std::vector<std::string> tokens = CSVReader::tokenise(csvLine, ',');
    if (tokens.size() == 7)
    {
        return Transaction(
            tokens[0],
            tokens[1],
            stringToType(tokens[2]),
            tokens[3],
            std::stod(tokens[4]),
            std::stod(tokens[5]),
            std::stod(tokens[6])
        );
    }
    return Transaction();
}

std::string Transaction::typeToString(TransactionType type)
{
    switch (type)
    {
    case TransactionType::DEPOSIT: return "DEPOSIT";
    case TransactionType::WITHDRAWAL: return "WITHDRAWAL";
    case TransactionType::ASK_PLACED: return "ASK_PLACED";
    case TransactionType::BID_PLACED: return "BID_PLACED";
    case TransactionType::ASK_FILLED: return "ASK_FILLED";
    case TransactionType::BID_FILLED: return "BID_FILLED";
    default: return "UNKNOWN";
    }
}

TransactionType Transaction::stringToType(std::string str)
{
    if (str == "DEPOSIT") return TransactionType::DEPOSIT;
    if (str == "WITHDRAWAL") return TransactionType::WITHDRAWAL;
    if (str == "ASK_PLACED") return TransactionType::ASK_PLACED;
    if (str == "BID_PLACED") return TransactionType::BID_PLACED;
    if (str == "ASK_FILLED") return TransactionType::ASK_FILLED;
    if (str == "BID_FILLED") return TransactionType::BID_FILLED;
    return TransactionType::DEPOSIT;
}

std::string Transaction::toString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << timestamp << " | " << typeToString(type) << " | "
        << product << " | Amt: " << amount << " | Price: " << price
        << " | Balance: " << balanceAfter;
    return oss.str();
}