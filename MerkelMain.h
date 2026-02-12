// ==================== MerkelMain.h ====================
/**
 * MerkelMain.h
 * Main application controller for cryptocurrency trading simulator
 * Handles user interface, authentication, wallet management, and trading operations
 */

#pragma once
#include <vector>
#include <string>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "User.h"
#include "DataManager.h"
#include "Candlestick.h"
#include "Transaction.h"

class MerkelMain
{
public:
    MerkelMain();
    void init();

private:
    // ===== MENU FUNCTIONS =====
    void printMainMenu();
    void printHelpMenu();
    int getUserOption();
    void processUserOption(int userOption);

    // ===== TASK 1: Candlestick Data =====
    void displayCandlestickData();
    void printCandlestickTable(const std::vector<Candlestick>& candlesticks, std::string type);

    // ===== TASK 2: User Authentication =====
    void loginOrRegister();
    void registerNewUser();
    void loginUser();
    void forgotPassword();
    bool isLoggedIn() const { return !currentUser.getUsername().empty(); }

    // ===== TASK 3: Wallet & Transaction History =====
    void manageWallet();
    void depositFunds();
    void withdrawFunds();
    void viewWalletBalance();
    void viewTransactionHistory();
    void viewUserStatistics();

    // ===== TASK 4: Trading Simulation =====
    void simulateTrading();
    void placeAsk();
    void placeBid();
    double calculateAskPrice(std::string product);
    double calculateBidPrice(std::string product);
    std::string getCurrentTimestamp();

    // ===== TASK 5: Input Validation =====
    std::string getValidatedStringInput(std::string prompt);
    double getValidatedDoubleInput(std::string prompt);
    int getValidatedIntInput(std::string prompt, int min, int max);
    std::string getValidatedProductInput();
    std::string getValidatedCurrencyInput(std::string prompt);
    bool validateEmail(std::string email);
    bool isValidCurrency(std::string currency);

    // ===== Helper Functions =====
    void printMarketStats();
    void gotoNextTimeframe();
    void saveCurrentWalletState();
    void loadUserWallet();
    std::vector<std::string> getKnownCurrencies();

    // ===== Member Variables =====
    std::string currentTime;
    OrderBook orderBook;
    Wallet wallet;
    User currentUser;
    DataManager dataManager;
    bool isAuthenticated;
};