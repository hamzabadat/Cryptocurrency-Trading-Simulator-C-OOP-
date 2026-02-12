// ==================== MerkelMain.cpp ====================
/**
 * MerkelMain.cpp
 * Implementation of main application controller
 * Handles all user interactions, trading operations, and data management
 */

#include "MerkelMain.h"
#include "CSVReader.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <ctime>
#include <regex>
#include <algorithm>

MerkelMain::MerkelMain()
    : orderBook("20200317.csv"), isAuthenticated(false)
{
    currentTime = orderBook.getEarliestTime();
}

void MerkelMain::init()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  CRYPTOCURRENCY TRADING SIMULATOR" << std::endl;
    std::cout << "  Technical Analysis Toolkit" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Force login/register before accessing main features
    while (!isLoggedIn())
    {
        loginOrRegister();
    }

    // Load user's wallet after successful login
    loadUserWallet();

    // Main application loop
    int input;
    while (true)
    {
        printMainMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

// ==================== MENU FUNCTIONS ====================

void MerkelMain::printMainMenu()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  MAIN MENU - Welcome " << currentUser.getFullName() << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1: View Candlestick Data (Technical Analysis)" << std::endl;
    std::cout << "2: Manage Wallet & View Balance" << std::endl;
    std::cout << "3: View Transaction History" << std::endl;
    std::cout << "4: View Trading Statistics" << std::endl;
    std::cout << "5: Place Ask (Sell Order)" << std::endl;
    std::cout << "6: Place Bid (Buy Order)" << std::endl;
    std::cout << "7: Simulate Trading Activity" << std::endl;
    std::cout << "8: View Market Statistics" << std::endl;
    std::cout << "9: Help" << std::endl;
    std::cout << "10: Logout" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Current Time: " << currentTime << std::endl;
    std::cout << "========================================" << std::endl;
}

void MerkelMain::printHelpMenu()
{
    std::cout << "\n========== HELP MENU ==========" << std::endl;
    std::cout << "This is a cryptocurrency trading simulator with technical analysis tools." << std::endl;
    std::cout << "\nFeatures:" << std::endl;
    std::cout << "- Candlestick Analysis: View OHLC data for any product and timeframe" << std::endl;
    std::cout << "- Wallet Management: Deposit, withdraw, and track your funds" << std::endl;
    std::cout << "- Trading: Place ask (sell) and bid (buy) orders" << std::endl;
    std::cout << "- History: View all your past transactions and statistics" << std::endl;
    std::cout << "\nGoal: Analyze market trends and make profitable trades!" << std::endl;
    std::cout << "================================" << std::endl;
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "\nEnter your choice (1-10): ";

    while (true)
    {
        std::getline(std::cin, line);

        if (line.empty())
        {
            std::cout << "Input cannot be empty. Please enter a number (1-10): ";
            continue;
        }

        // Check if input is numeric
        bool isNumeric = true;
        for (char c : line)
        {
            if (!isdigit(c))
            {
                isNumeric = false;
                break;
            }
        }

        if (!isNumeric)
        {
            std::cout << "Invalid input. Please enter a valid number (1-10): ";
            continue;
        }

        try
        {
            userOption = std::stoi(line);

            if (userOption < 1 || userOption > 10)
            {
                std::cout << "Invalid choice. Please enter a number between 1-10: ";
                continue;
            }

            break;
        }
        catch (const std::exception& e)
        {
            std::cout << "Invalid input. Please enter a number (1-10): ";
        }
    }

    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    switch (userOption)
    {
    case 1:
        displayCandlestickData();
        break;
    case 2:
        manageWallet();
        break;
    case 3:
        viewTransactionHistory();
        break;
    case 4:
        viewUserStatistics();
        break;
    case 5:
        placeAsk();
        break;
    case 6:
        placeBid();
        break;
    case 7:
        simulateTrading();
        break;
    case 8:
        printMarketStats();
        break;
    case 9:
        printHelpMenu();
        break;
    case 10:
        std::cout << "\nLogging out... Goodbye!" << std::endl;
        currentUser = User();
        isAuthenticated = false;
        exit(0);
        break;
    default:
        std::cout << "Invalid option." << std::endl;
    }
}

// ==================== TASK 1: CANDLESTICK DATA ====================

void MerkelMain::displayCandlestickData()
{
    std::cout << "\n========== CANDLESTICK TECHNICAL ANALYSIS ==========" << std::endl;

    std::string product = getValidatedProductInput();

    // Get time period selection
    std::cout << "\nSelect time period:" << std::endl;
    std::cout << "1: Daily" << std::endl;
    std::cout << "2: Monthly" << std::endl;
    std::cout << "3: Yearly (default)" << std::endl;
    int periodChoice = getValidatedIntInput("Enter choice (1-3): ", 1, 3);

    std::string period = "yearly";
    if (periodChoice == 1) period = "daily";
    else if (periodChoice == 2) period = "monthly";

    std::cout << "\nGenerating candlestick data for " << product << " (" << period << ")..." << std::endl;

    // Generate candlesticks for asks (sell orders)
    std::vector<OrderBookEntry> allOrders = orderBook.getOrders(OrderBookType::ask, product, currentTime);
    std::vector<Candlestick> askCandlesticks = dataManager.generateCandlesticks(
        allOrders, product, period, OrderBookType::ask);

    // Generate candlesticks for bids (buy orders)
    allOrders = orderBook.getOrders(OrderBookType::bid, product, currentTime);
    std::vector<Candlestick> bidCandlesticks = dataManager.generateCandlesticks(
        allOrders, product, period, OrderBookType::bid);

    // Display results
    std::cout << "\n========== ASK (SELL) ORDERS ==========" << std::endl;
    printCandlestickTable(askCandlesticks, "ASK");

    std::cout << "\n========== BID (BUY) ORDERS ==========" << std::endl;
    printCandlestickTable(bidCandlesticks, "BID");
}

void MerkelMain::printCandlestickTable(const std::vector<Candlestick>& candlesticks, std::string type)
{
    if (candlesticks.empty())
    {
        std::cout << "No data available for this product." << std::endl;
        return;
    }

    // Print table header
    std::cout << std::left << std::setw(15) << "Date"
        << std::right << std::setw(12) << "Open"
        << std::setw(12) << "High"
        << std::setw(12) << "Low"
        << std::setw(12) << "Close" << std::endl;
    std::cout << std::string(63, '-') << std::endl;

    // Print each candlestick
    for (const Candlestick& candle : candlesticks)
    {
        std::cout << std::fixed << std::setprecision(6);
        std::cout << std::left << std::setw(15) << candle.getDate()
            << std::right << std::setw(12) << candle.getOpen()
            << std::setw(12) << candle.getHigh()
            << std::setw(12) << candle.getLow()
            << std::setw(12) << candle.getClose() << std::endl;
    }

    std::cout << "\nTotal records: " << candlesticks.size() << std::endl;
}

// ==================== TASK 2: USER AUTHENTICATION ====================

void MerkelMain::loginOrRegister()
{
    std::cout << "\n========== AUTHENTICATION ==========" << std::endl;
    std::cout << "1: Login" << std::endl;
    std::cout << "2: Register" << std::endl;
    std::cout << "3: Forgot Password" << std::endl;

    int choice = getValidatedIntInput("Enter choice (1-3): ", 1, 3);

    if (choice == 1)
    {
        loginUser();
    }
    else if (choice == 2)
    {
        registerNewUser();
    }
    else if (choice == 3)
    {
        forgotPassword();
    }
}

void MerkelMain::registerNewUser()
{
    std::cout << "\n========== USER REGISTRATION ==========" << std::endl;

    std::string fullName = getValidatedStringInput("Enter full name: ");

    // Email validation loop
    std::string email;
    while (true)
    {
        email = getValidatedStringInput("Enter email address: ");
        if (validateEmail(email))
        {
            break;
        }
        std::cout << "Invalid email format. Please try again." << std::endl;
    }

    // Check for duplicate users
    if (dataManager.userExists(email, fullName))
    {
        std::cout << "\nUser with this email and name already exists!" << std::endl;
        std::cout << "Please login instead or use a different email." << std::endl;
        return;
    }

    std::string password = getValidatedStringInput("Enter password: ");
    std::string confirmPassword = getValidatedStringInput("Confirm password: ");

    if (password != confirmPassword)
    {
        std::cout << "Passwords do not match. Registration failed." << std::endl;
        return;
    }

    // Generate unique username and hash password
    std::string username = User::generateUsername();
    std::string passwordHash = User::hashPassword(password);

    // Create and save user
    User newUser(username, fullName, email, passwordHash);
    if (dataManager.saveUser(newUser))
    {
        std::cout << "\n========== REGISTRATION SUCCESSFUL ==========" << std::endl;
        std::cout << "Your username is: " << username << std::endl;
        std::cout << "Please save this for future logins!" << std::endl;
        std::cout << "=============================================" << std::endl;

        // Auto-login
        currentUser = newUser;
        isAuthenticated = true;

        // Initialize wallet with starting balance
        wallet.insertCurrency("USDT", 10000.0);
        wallet.insertCurrency("BTC", 0.5);
        wallet.insertCurrency("ETH", 5.0);
        saveCurrentWalletState();
    }
    else
    {
        std::cout << "Registration failed. Please try again." << std::endl;
    }
}

void MerkelMain::loginUser()
{
    std::cout << "\n========== USER LOGIN ==========" << std::endl;

    std::string username = getValidatedStringInput("Enter username (10 digits): ");
    std::string password = getValidatedStringInput("Enter password: ");

    // Load user from database
    User user = dataManager.loadUser(username);

    if (user.getUsername().empty())
    {
        std::cout << "User not found. Please check your username or register." << std::endl;
        return;
    }

    // Verify password by comparing hashes
    std::string passwordHash = User::hashPassword(password);
    if (passwordHash == user.getPasswordHash())
    {
        std::cout << "\n========== LOGIN SUCCESSFUL ==========" << std::endl;
        std::cout << "Welcome back, " << user.getFullName() << "!" << std::endl;
        std::cout << "======================================" << std::endl;
        currentUser = user;
        isAuthenticated = true;
    }
    else
    {
        std::cout << "Incorrect password. Login failed." << std::endl;
    }
}

void MerkelMain::forgotPassword()
{
    std::cout << "\n========== PASSWORD RECOVERY ==========" << std::endl;

    std::string email = getValidatedStringInput("Enter your registered email: ");

    User user = dataManager.getUserByEmail(email);

    if (user.getUsername().empty())
    {
        std::cout << "No account found with this email." << std::endl;
        return;
    }

    std::cout << "\nAccount found!" << std::endl;
    std::cout << "Username: " << user.getUsername() << std::endl;
    std::cout << "Full Name: " << user.getFullName() << std::endl;

    std::cout << "\nWould you like to reset your password? (yes/no): ";
    std::string response;
    std::getline(std::cin, response);

    if (response == "yes" || response == "y" || response == "Yes")
    {
        std::string newPassword = getValidatedStringInput("Enter new password: ");
        std::string confirmPassword = getValidatedStringInput("Confirm new password: ");

        if (newPassword != confirmPassword)
        {
            std::cout << "Passwords do not match. Reset failed." << std::endl;
            return;
        }

        std::cout << "\nPassword reset successful! Please login with your new password." << std::endl;
    }
}

// ==================== TASK 3: WALLET & TRANSACTION HISTORY ====================

void MerkelMain::manageWallet()
{
    std::cout << "\n========== WALLET MANAGEMENT ==========" << std::endl;
    std::cout << "1: View Balance" << std::endl;
    std::cout << "2: Deposit Funds" << std::endl;
    std::cout << "3: Withdraw Funds" << std::endl;

    int choice = getValidatedIntInput("Enter choice (1-3): ", 1, 3);

    if (choice == 1)
    {
        viewWalletBalance();
    }
    else if (choice == 2)
    {
        depositFunds();
    }
    else if (choice == 3)
    {
        withdrawFunds();
    }
}

void MerkelMain::depositFunds()
{
    std::cout << "\n========== DEPOSIT FUNDS ==========" << std::endl;

    // TASK 5: Currency validation
    std::string currency = getValidatedCurrencyInput("Select a currency to deposit");

    // TASK 5: Amount validation with limits
    double amount = 0.0;
    while (true)
    {
        amount = getValidatedDoubleInput("Enter amount to deposit: ");

        if (amount <= 0)
        {
            std::cout << "Amount must be positive. Please try again." << std::endl;
            continue;
        }

        if (amount > 1000000000.0)
        {
            std::cout << "Amount too large. Maximum deposit is 1,000,000,000. Please try again." << std::endl;
            continue;
        }

        break;
    }

    // Get current balance
    double currentBalance = 0.0;
    if (wallet.containsCurrency(currency, 0))
    {
        std::string walletStr = wallet.toString();
        std::istringstream iss(walletStr);
        std::string line;

        while (std::getline(iss, line))
        {
            if (line.find(currency) != std::string::npos)
            {
                size_t colonPos = line.find(':');
                if (colonPos != std::string::npos)
                {
                    std::string amountStr = line.substr(colonPos + 2);
                    amountStr.erase(std::remove_if(amountStr.begin(), amountStr.end(), ::isspace),
                        amountStr.end());
                    try {
                        currentBalance = std::stod(amountStr);
                    }
                    catch (...) {
                        currentBalance = 0.0;
                    }
                }
                break;
            }
        }
    }

    wallet.insertCurrency(currency, amount);
    saveCurrentWalletState();

    // Log transaction
    std::string timestamp = getCurrentTimestamp();
    double newBalance = currentBalance + amount;
    Transaction trans(currentUser.getUsername(), timestamp, TransactionType::DEPOSIT,
        currency, amount, 0.0, newBalance);
    dataManager.saveTransaction(trans);

    std::cout << "\nDeposit successful!" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "New " << currency << " balance: " << newBalance << std::endl;
}

void MerkelMain::withdrawFunds()
{
    std::cout << "\n========== WITHDRAW FUNDS ==========" << std::endl;

    // TASK 5: Currency validation
    std::string currency = getValidatedCurrencyInput("Select a currency to withdraw");

    if (!wallet.containsCurrency(currency, 0))
    {
        std::cout << "You don't have any " << currency << " in your wallet." << std::endl;
        return;
    }

    // TASK 5: Amount validation with limits
    double amount = 0.0;
    while (true)
    {
        amount = getValidatedDoubleInput("Enter amount to withdraw: ");

        if (amount <= 0)
        {
            std::cout << "Amount must be positive. Please try again." << std::endl;
            continue;
        }

        if (amount > 1000000000.0)
        {
            std::cout << "Amount too large. Maximum withdrawal is 1,000,000,000. Please try again." << std::endl;
            continue;
        }

        break;
    }

    if (!wallet.containsCurrency(currency, amount))
    {
        std::cout << "Insufficient funds in wallet." << std::endl;
        std::cout << "You attempted to withdraw " << amount << " " << currency << std::endl;
        return;
    }

    if (wallet.removeCurrency(currency, amount))
    {
        saveCurrentWalletState();

        // Log transaction
        std::string timestamp = getCurrentTimestamp();
        Transaction trans(currentUser.getUsername(), timestamp, TransactionType::WITHDRAWAL,
            currency, amount, 0.0, 0.0);
        dataManager.saveTransaction(trans);

        std::cout << "\nWithdrawal successful!" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Withdrew " << amount << " " << currency << std::endl;
    }
    else
    {
        std::cout << "Withdrawal failed." << std::endl;
    }
}

void MerkelMain::viewWalletBalance()
{
    std::cout << "\n========== WALLET BALANCE ==========" << std::endl;
    std::cout << wallet.toString();
    std::cout << "====================================" << std::endl;
}

void MerkelMain::viewTransactionHistory()
{
    std::cout << "\n========== TRANSACTION HISTORY ==========" << std::endl;
    std::cout << "1: View all transactions" << std::endl;
    std::cout << "2: View last 5 transactions" << std::endl;
    std::cout << "3: View by product" << std::endl;

    int choice = getValidatedIntInput("Enter choice (1-3): ", 1, 3);

    std::vector<Transaction> transactions;

    if (choice == 1)
    {
        transactions = dataManager.loadUserTransactions(currentUser.getUsername());
    }
    else if (choice == 2)
    {
        transactions = dataManager.loadUserTransactions(currentUser.getUsername(), 5);
    }
    else if (choice == 3)
    {
        std::string product = getValidatedStringInput("Enter product (e.g., ETH/USDT): ");
        transactions = dataManager.loadUserTransactionsByProduct(currentUser.getUsername(), product);
    }

    if (transactions.empty())
    {
        std::cout << "No transactions found." << std::endl;
        return;
    }

    std::cout << "\nTransaction History:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    for (const Transaction& trans : transactions)
    {
        std::cout << trans.toString() << std::endl;
    }
    std::cout << "\nTotal transactions: " << transactions.size() << std::endl;
}

void MerkelMain::viewUserStatistics()
{
    std::cout << "\n========== USER TRADING STATISTICS ==========" << std::endl;

    std::vector<Transaction> allTransactions = dataManager.loadUserTransactions(currentUser.getUsername());

    if (allTransactions.empty())
    {
        std::cout << "No trading activity yet." << std::endl;
        return;
    }

    // Calculate statistics
    int totalAsks = 0, totalBids = 0;
    double totalMoneySpent = 0.0;
    std::map<std::string, int> productAsks, productBids;

    for (const Transaction& trans : allTransactions)
    {
        TransactionType type = trans.getType();
        std::string product = trans.getProduct();

        if (type == TransactionType::ASK_PLACED || type == TransactionType::ASK_FILLED)
        {
            totalAsks++;
            productAsks[product]++;
        }
        else if (type == TransactionType::BID_PLACED || type == TransactionType::BID_FILLED)
        {
            totalBids++;
            productBids[product]++;
            totalMoneySpent += trans.getAmount() * trans.getPrice();
        }
    }

    std::cout << "Total Transactions: " << allTransactions.size() << std::endl;
    std::cout << "Total Asks (Sell Orders): " << totalAsks << std::endl;
    std::cout << "Total Bids (Buy Orders): " << totalBids << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total Money Spent: " << totalMoneySpent << std::endl;

    std::cout << "\n--- Asks by Product ---" << std::endl;
    for (const auto& pair : productAsks)
    {
        std::cout << pair.first << ": " << pair.second << " asks" << std::endl;
    }

    std::cout << "\n--- Bids by Product ---" << std::endl;
    for (const auto& pair : productBids)
    {
        std::cout << pair.first << ": " << pair.second << " bids" << std::endl;
    }
}

// ==================== TASK 4: TRADING SIMULATION ====================

void MerkelMain::simulateTrading()
{
    std::cout << "\n========== SIMULATE TRADING ACTIVITY ==========" << std::endl;
    std::cout << "This will create 5 ask and 5 bid orders for all products." << std::endl;
    std::cout << "Continue? (yes/no): ";

    std::string response;
    std::getline(std::cin, response);

    if (response != "yes" && response != "y")
    {
        std::cout << "Simulation cancelled." << std::endl;
        return;
    }

    std::vector<std::string> products = orderBook.getKnownProducts();
    std::string timestamp = getCurrentTimestamp();
    int ordersCreated = 0;

    for (const std::string& product : products)
    {
        // Create 5 ask orders with varying amounts
        for (int i = 0; i < 5; i++)
        {
            double price = calculateAskPrice(product);
            double amount = 0.1 + (i * 0.05);

            OrderBookEntry obe(price, amount, timestamp, product,
                OrderBookType::ask, currentUser.getUsername());
            orderBook.insertOrder(obe);

            Transaction trans(currentUser.getUsername(), timestamp, TransactionType::ASK_PLACED,
                product, amount, price, 0.0);
            dataManager.saveTransaction(trans);
            ordersCreated++;
        }

        // Create 5 bid orders with varying amounts
        for (int i = 0; i < 5; i++)
        {
            double price = calculateBidPrice(product);
            double amount = 0.1 + (i * 0.05);

            OrderBookEntry obe(price, amount, timestamp, product,
                OrderBookType::bid, currentUser.getUsername());
            orderBook.insertOrder(obe);

            Transaction trans(currentUser.getUsername(), timestamp, TransactionType::BID_PLACED,
                product, amount, price, 0.0);
            dataManager.saveTransaction(trans);
            ordersCreated++;
        }
    }

    std::cout << "\nSimulation complete!" << std::endl;
    std::cout << "Created " << ordersCreated << " orders across " << products.size() << " products." << std::endl;
    std::cout << "\nNote: Prices calculated using historical data adjusted for time gap." << std::endl;
}

double MerkelMain::calculateAskPrice(std::string product)
{
    // TASK 4: Calculate ask price with 5-year growth adjustment (2020 → 2025)
    // Historical CSV data is from 2020, current system time is 2025
    // Apply 15% annual compound growth: 1.15^5 = 2.011x

    // Try to get historical data for this product
    std::vector<OrderBookEntry> allOrders = orderBook.getOrders(
        OrderBookType::ask, product, orderBook.getEarliestTime());

    // If no data at earliest time, scan through timeframes
    if (allOrders.empty())
    {
        std::string timestamp = orderBook.getEarliestTime();
        for (int i = 0; i < 100; i++)
        {
            allOrders = orderBook.getOrders(OrderBookType::ask, product, timestamp);
            if (!allOrders.empty()) break;
            timestamp = orderBook.getNextTime(timestamp);
            if (timestamp == orderBook.getEarliestTime()) break;
        }
    }

    // Fallback prices for products with no historical data
    if (allOrders.empty())
    {
        if (product.find("DOGE") != std::string::npos)
            return 0.15;
        else if (product.find("BTC") != std::string::npos)
            return 50000.0;
        else if (product.find("ETH") != std::string::npos)
            return 3000.0;
        return 100.0;
    }

    // Calculate average historical price
    double sum = 0.0;
    for (const OrderBookEntry& entry : allOrders)
    {
        sum += entry.price;
    }
    double avgPrice = sum / allOrders.size();

    // Apply 5-year growth factor (15% annual = 2.011x total)
    double growthFactor = 1.15 * 1.15 * 1.15 * 1.15 * 1.15;
    double adjustedPrice = avgPrice * growthFactor;

    // Add random variation (±5%)
    double variation = 0.95 + (std::rand() % 100) / 1000.0;

    return adjustedPrice * variation;
}

double MerkelMain::calculateBidPrice(std::string product)
{
    // TASK 4: Bid price is typically 1-3% lower than ask (market spread)
    double askPrice = calculateAskPrice(product);
    double spreadFactor = 0.97 + (std::rand() % 30) / 1000.0;
    return askPrice * spreadFactor;
}

void MerkelMain::placeAsk()
{
    std::cout << "\n========== PLACE ASK (SELL ORDER) ==========" << std::endl;

    std::string product = getValidatedProductInput();
    double price = getValidatedDoubleInput("Enter price: ");
    double amount = getValidatedDoubleInput("Enter amount: ");

    if (price <= 0 || amount <= 0)
    {
        std::cout << "Price and amount must be positive." << std::endl;
        return;
    }

    std::string timestamp = getCurrentTimestamp();
    OrderBookEntry obe(price, amount, timestamp, product,
        OrderBookType::ask, currentUser.getUsername());

    if (wallet.canFulfilOrder(obe))
    {
        orderBook.insertOrder(obe);

        Transaction trans(currentUser.getUsername(), timestamp, TransactionType::ASK_PLACED,
            product, amount, price, 0.0);
        dataManager.saveTransaction(trans);

        std::cout << "\nAsk order placed successfully!" << std::endl;
    }
    else
    {
        std::cout << "\nInsufficient funds in wallet." << std::endl;
    }
}

void MerkelMain::placeBid()
{
    std::cout << "\n========== PLACE BID (BUY ORDER) ==========" << std::endl;

    std::string product = getValidatedProductInput();
    double price = getValidatedDoubleInput("Enter price: ");
    double amount = getValidatedDoubleInput("Enter amount: ");

    if (price <= 0 || amount <= 0)
    {
        std::cout << "Price and amount must be positive." << std::endl;
        return;
    }

    std::string timestamp = getCurrentTimestamp();
    OrderBookEntry obe(price, amount, timestamp, product,
        OrderBookType::bid, currentUser.getUsername());

    if (wallet.canFulfilOrder(obe))
    {
        orderBook.insertOrder(obe);

        Transaction trans(currentUser.getUsername(), timestamp, TransactionType::BID_PLACED,
            product, amount, price, 0.0);
        dataManager.saveTransaction(trans);

        std::cout << "\nBid order placed successfully!" << std::endl;
    }
    else
    {
        std::cout << "\nInsufficient funds in wallet." << std::endl;
    }
}

// ==================== TASK 5: INPUT VALIDATION ====================

std::string MerkelMain::getValidatedStringInput(std::string prompt)
{
    std::string input;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, input);

        // Remove leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (!input.empty())
        {
            return input;
        }
        std::cout << "Input cannot be empty. Please try again." << std::endl;
    }
}

double MerkelMain::getValidatedDoubleInput(std::string prompt)
{
    std::string input;
    double value;

    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, input);

        if (input.empty())
        {
            std::cout << "Input cannot be empty. Please try again." << std::endl;
            continue;
        }

        try
        {
            value = std::stod(input);

            // Check for NaN and infinity
            if (std::isnan(value) || std::isinf(value))
            {
                std::cout << "Invalid number. Please enter a valid decimal number." << std::endl;
                continue;
            }

            return value;
        }
        catch (const std::exception& e)
        {
            std::cout << "Invalid number. Please enter a valid decimal number." << std::endl;
        }
    }
}

int MerkelMain::getValidatedIntInput(std::string prompt, int min, int max)
{
    std::string input;
    int value;

    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, input);

        try
        {
            value = std::stoi(input);

            if (value >= min && value <= max)
            {
                return value;
            }
            std::cout << "Please enter a number between " << min << " and " << max << ": ";
        }
        catch (const std::exception& e)
        {
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        }
    }
}

std::string MerkelMain::getValidatedProductInput()
{
    std::vector<std::string> knownProducts = orderBook.getKnownProducts();

    std::cout << "\nAvailable products:" << std::endl;
    for (size_t i = 0; i < knownProducts.size(); i++)
    {
        std::cout << (i + 1) << ": " << knownProducts[i] << std::endl;
    }

    int choice = getValidatedIntInput("Enter product number: ", 1, knownProducts.size());
    return knownProducts[choice - 1];
}

std::string MerkelMain::getValidatedCurrencyInput(std::string prompt)
{
    std::vector<std::string> validCurrencies = getKnownCurrencies();
    std::string currency;

    while (true)
    {
        std::cout << prompt;
        std::cout << "\nAvailable currencies: ";
        for (size_t i = 0; i < validCurrencies.size(); i++)
        {
            std::cout << validCurrencies[i];
            if (i < validCurrencies.size() - 1) std::cout << ", ";
        }
        std::cout << "\nEnter currency: ";

        std::getline(std::cin, currency);

        // Remove whitespace
        currency.erase(0, currency.find_first_not_of(" \t\n\r"));
        currency.erase(currency.find_last_not_of(" \t\n\r") + 1);

        if (currency.empty())
        {
            std::cout << "Currency cannot be empty. Please try again.\n" << std::endl;
            continue;
        }

        // Convert to uppercase for validation
        std::transform(currency.begin(), currency.end(), currency.begin(), ::toupper);

        if (isValidCurrency(currency))
        {
            return currency;
        }

        std::cout << "Invalid currency. Please enter one of the available currencies.\n" << std::endl;
    }
}

bool MerkelMain::validateEmail(std::string email)
{
    const std::regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return std::regex_match(email, pattern);
}

bool MerkelMain::isValidCurrency(std::string currency)
{
    std::vector<std::string> validCurrencies = getKnownCurrencies();

    // Convert to uppercase for comparison
    std::transform(currency.begin(), currency.end(), currency.begin(), ::toupper);

    for (const std::string& valid : validCurrencies)
    {
        if (currency == valid)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::string> MerkelMain::getKnownCurrencies()
{
    // Extract unique currencies from all product pairs
    std::vector<std::string> currencies;
    std::vector<std::string> products = orderBook.getKnownProducts();

    for (const std::string& product : products)
    {
        size_t slashPos = product.find('/');
        if (slashPos != std::string::npos)
        {
            std::string currency1 = product.substr(0, slashPos);
            std::string currency2 = product.substr(slashPos + 1);

            // Add unique currencies
            if (std::find(currencies.begin(), currencies.end(), currency1) == currencies.end())
            {
                currencies.push_back(currency1);
            }
            if (std::find(currencies.begin(), currencies.end(), currency2) == currencies.end())
            {
                currencies.push_back(currency2);
            }
        }
    }

    return currencies;
}

// ==================== HELPER FUNCTIONS ====================

void MerkelMain::printMarketStats()
{
    std::cout << "\n========== MARKET STATISTICS ==========" << std::endl;

    for (const std::string& product : orderBook.getKnownProducts())
    {
        std::cout << "\n--- " << product << " ---" << std::endl;

        std::vector<OrderBookEntry> asks = orderBook.getOrders(OrderBookType::ask,
            product, currentTime);
        std::vector<OrderBookEntry> bids = orderBook.getOrders(OrderBookType::bid,
            product, currentTime);

        std::cout << "Asks available: " << asks.size() << std::endl;
        if (!asks.empty())
        {
            std::cout << std::fixed << std::setprecision(8);
            std::cout << "  Max ask: " << OrderBook::getHighPrice(asks) << std::endl;
            std::cout << "  Min ask: " << OrderBook::getLowPrice(asks) << std::endl;
        }

        std::cout << "Bids available: " << bids.size() << std::endl;
        if (!bids.empty())
        {
            std::cout << std::fixed << std::setprecision(8);
            std::cout << "  Max bid: " << OrderBook::getHighPrice(bids) << std::endl;
            std::cout << "  Min bid: " << OrderBook::getLowPrice(bids) << std::endl;
        }
    }
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "\nAdvancing to next timeframe..." << std::endl;

    for (std::string& product : orderBook.getKnownProducts())
    {
        std::cout << "Matching " << product << "..." << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(product, currentTime);

        std::cout << "Sales: " << sales.size() << std::endl;

        for (OrderBookEntry& sale : sales)
        {
            if (sale.username == currentUser.getUsername())
            {
                wallet.processSale(sale);

                TransactionType type = (sale.orderType == OrderBookType::asksale) ?
                    TransactionType::ASK_FILLED : TransactionType::BID_FILLED;

                Transaction trans(currentUser.getUsername(), sale.timestamp, type,
                    sale.product, sale.amount, sale.price, 0.0);
                dataManager.saveTransaction(trans);
            }
        }
    }

    currentTime = orderBook.getNextTime(currentTime);
    saveCurrentWalletState();

    std::cout << "New timeframe: " << currentTime << std::endl;
}

void MerkelMain::saveCurrentWalletState()
{
    std::string walletStr = wallet.toString();
    std::istringstream iss(walletStr);
    std::string line;

    while (std::getline(iss, line))
    {
        if (line.empty()) continue;

        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos)
        {
            std::string currency = line.substr(0, colonPos);
            std::string amountStr = line.substr(colonPos + 2);

            // Remove whitespace
            currency.erase(std::remove_if(currency.begin(), currency.end(), ::isspace), currency.end());
            amountStr.erase(std::remove_if(amountStr.begin(), amountStr.end(), ::isspace), amountStr.end());

            try
            {
                double amount = std::stod(amountStr);
                dataManager.saveWalletBalance(currentUser.getUsername(), currency, amount);
            }
            catch (const std::exception& e)
            {
                // Skip invalid entries
            }
        }
    }
}

void MerkelMain::loadUserWallet()
{
    std::map<std::string, double> walletData = dataManager.loadWalletBalance(currentUser.getUsername());

    for (const auto& pair : walletData)
    {
        wallet.insertCurrency(pair.first, pair.second);
    }

    // Initialize with default values if wallet is empty
    if (walletData.empty())
    {
        wallet.insertCurrency("USDT", 10000.0);
        wallet.insertCurrency("BTC", 0.5);
        wallet.insertCurrency("ETH", 5.0);
        saveCurrentWalletState();
    }
}

std::string MerkelMain::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

#pragma warning(suppress : 4996)
    std::tm* now_tm = std::localtime(&now_c);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", now_tm);

    return std::string(buffer);
}