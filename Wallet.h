// ==================== Wallet.h ====================
/**
 * Wallet.h
 * Manages user cryptocurrency holdings
 * TASK 3: Handles deposits, withdrawals, and order fulfillment
 */

#pragma once
#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet
{
public:
    Wallet();

    void insertCurrency(std::string type, double amount);
    bool removeCurrency(std::string type, double amount);
    bool containsCurrency(std::string type, double amount);
    bool canFulfilOrder(OrderBookEntry order);
    void processSale(OrderBookEntry& sale);
    std::string toString();

private:
    std::map<std::string, double> currencies;  // Currency -> Amount mapping
};