// ==================== Wallet.cpp ====================
/**
 * Wallet.cpp
 * Implementation of wallet operations
 */

#include "Wallet.h"
#include "CSVReader.h"
#include <iostream>

Wallet::Wallet()
{
}

void Wallet::insertCurrency(std::string type, double amount)
{
    if (amount < 0)
    {
        throw std::exception{};
    }

    double balance = 0;
    if (currencies.count(type) > 0)
    {
        balance = currencies[type];
    }

    balance += amount;
    currencies[type] = balance;
}

bool Wallet::removeCurrency(std::string type, double amount)
{
    if (amount < 0 || currencies.count(type) == 0 || currencies[type] < amount)
    {
        return false;
    }

    currencies[type] -= amount;
    return true;
}

bool Wallet::containsCurrency(std::string type, double amount)
{
    if (currencies.count(type) == 0)
    {
        return false;
    }
    return currencies[type] >= amount;
}

bool Wallet::canFulfilOrder(OrderBookEntry order)
{
    // Product format: Currency1/Currency2
    // Ask: need Currency1 to sell
    // Bid: need Currency2 to buy
    std::vector<std::string> currencies = CSVReader::tokenise(order.product, '/');

    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount;
        std::string currency = currencies[0];
        std::cout << "Wallet::canFulfilOrder: currency = " << currency << ", amount = " << amount << std::endl;
        return containsCurrency(currency, amount);
    }

    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        std::string currency = currencies[1];
        return containsCurrency(currency, amount);
    }

    return false;
}

std::string Wallet::toString()
{
    std::string s;
    for (std::pair<std::string, double> pair : currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + ": " + std::to_string(amount) + "\n";
    }
    return s;
}

void Wallet::processSale(OrderBookEntry& sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');

    if (sale.orderType == OrderBookType::asksale)
    {
        // Sold Currency1, received Currency2
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];

        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }

    if (sale.orderType == OrderBookType::bidsale)
    {
        // Bought Currency1, spent Currency2
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];

        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}