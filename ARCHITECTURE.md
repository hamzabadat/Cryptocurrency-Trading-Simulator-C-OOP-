# 🏗️ System Architecture Documentation

## Overview

This document explains the architectural decisions and design patterns used in the Financial Data Processing System.

---

## Layer Architecture

The system follows a **three-tier architecture** pattern:

### 1. Presentation Layer (User Interface)
**Component:** `MerkelMain.cpp/h`

**Responsibilities:**
- Display menus and prompts
- Collect user input
- Validate input data
- Display results and reports

**Design Pattern:** Model-View-Controller (MVC) - Acts as Controller

---

### 2. Business Logic Layer
**Components:** `OrderBook`, `Wallet`, `DataManager`, `User`, `Transaction`

**Responsibilities:**
- Implement business rules
- Process transactions
- Match orders
- Generate analytics
- Enforce validation rules

**Design Patterns:**
- **Single Responsibility Principle:** Each class has one clear purpose
- **Dependency Injection:** Components receive dependencies via constructor
- **Strategy Pattern:** Different order types handled by same matching algorithm

---

### 3. Data Layer
**Components:** `CSVReader`, `OrderBookEntry`, `Candlestick`

**Responsibilities:**
- Parse CSV files
- Store/retrieve data
- Data structure definitions
- File I/O operations

**Design Pattern:** **Repository Pattern** - DataManager acts as data repository

---

## Data Flow Diagrams

### Candlestick Generation Flow

```
┌────────────┐
│  CSV File  │
│ (Market    │
│   Data)    │
└──────┬─────┘
       │
       │ CSVReader::readCSV()
       ▼
┌──────────────────┐
│ Vector<OrderBook │
│      Entry>      │
└──────┬───────────┘
       │
       │ DataManager::generateCandlesticks()
       │
       │  ┌─────────────────────┐
       ├──► 1. Filter by product│
       │  └─────────────────────┘
       │
       │  ┌─────────────────────┐
       ├──► 2. Group by date    │
       │  └─────────────────────┘
       │
       │  ┌─────────────────────┐
       ├──► 3. Calculate OHLC   │
       │  └─────────────────────┘
       │
       │  ┌─────────────────────┐
       └──► 4. Sort & return    │
          └─────────────────────┘
       │
       ▼
┌──────────────────┐
│Vector<Candlestick│
│     (OHLC)       │
└──────────────────┘
```

---

### Order Matching Flow

```
User Places Ask Order
       │
       ▼
┌─────────────────┐
│ Wallet Check:   │
│ Has currency?   │
└────┬────┬───────┘
     │YES │NO
     │    └────► Return Error
     ▼
┌─────────────────┐
│ Add to OrderBook│
└────┬────────────┘
     │
     ▼
┌─────────────────────┐
│ matchAsksToBids()   │
│ 1. Get all asks     │
│ 2. Get all bids     │
│ 3. Sort asks ASC    │
│ 4. Sort bids DESC   │
│ 5. Match orders     │
└────┬────────────────┘
     │
     ▼
┌─────────────────┐      ┌──────────────────┐
│ Update Wallet   │──────► Log Transaction  │
│ - Remove sold   │      │ - Type: ASK_FILL │
│ - Add received  │      │ - Amount         │
└─────────────────┘      │ - Price          │
                         └──────────────────┘
```

---

### User Authentication Flow

```
User Input: Email + Full Name
       │
       ▼
┌──────────────────┐
│ DataManager:     │
│ userExists()?    │
└────┬────┬────────┘
     │NO  │YES
     │    └────► Load existing user → Login success
     ▼
┌──────────────────┐
│ Generate 10-digit│
│   username       │
│ (timestamp+rand) │
└────┬─────────────┘
     │
     ▼
┌──────────────────┐
│ Hash password    │
│ using std::hash  │
└────┬─────────────┘
     │
     ▼
┌──────────────────┐
│ Save to users.csv│
└────┬─────────────┘
     │
     ▼
  Login Success
```

---

## Class Relationships

### Core Domain Models

```
┌─────────────────┐
│  OrderBookEntry │
│                 │
│ + price         │
│ + amount        │
│ + timestamp     │
│ + product       │
│ + orderType     │
└────────┬────────┘
         │ used by
         ▼
┌─────────────────┐
│   OrderBook     │
│                 │
│ + orders[]      │
│ + matchOrders() │
│ + getHighPrice()│
└────────┬────────┘
         │
         │ creates
         ▼
┌─────────────────┐
│  Candlestick    │
│                 │
│ + open          │
│ + high          │
│ + low           │
│ + close         │
│ + date          │
└─────────────────┘
```

### User & Wallet System

```
┌─────────────────┐
│      User       │
│                 │
│ + username      │
│ + email         │
│ + passwordHash  │
└────────┬────────┘
         │ owns
         ▼
┌─────────────────┐
│     Wallet      │
│                 │
│ + currencies{}  │
│ + insert()      │
│ + remove()      │
│ + canFulfill()  │
└────────┬────────┘
         │
         │ generates
         ▼
┌─────────────────┐
│  Transaction    │
│                 │
│ + type          │
│ + amount        │
│ + timestamp     │
│ + balanceAfter  │
└─────────────────┘
```

---

## Design Patterns Used

### 1. **Single Responsibility Principle**

Each class has one clear purpose:
- `CSVReader` → Parse CSV files ONLY
- `OrderBook` → Manage orders ONLY
- `Wallet` → Track balances ONLY
- `DataManager` → Persistence ONLY

**Benefit:** Easy to test, maintain, and extend

---

### 2. **Static Factory Methods**

```cpp
// User class
User User::fromCSVString(string csvLine);

// Transaction class
Transaction Transaction::fromCSVString(string csvLine);

// OrderBookEntry class
OrderBookType OrderBookEntry::stringToOrderBookType(string s);
```

**Benefit:** Centralized object creation logic

---

### 3. **Strategy Pattern**

Order matching algorithm handles different order types uniformly:

```cpp
// Same algorithm works for:
- OrderBookType::ask
- OrderBookType::bid
- OrderBookType::asksale
- OrderBookType::bidsale
```

**Benefit:** Extensible to new order types

---

### 4. **Repository Pattern**

`DataManager` acts as a data repository:

```cpp
class DataManager {
    // All data access goes through these methods:
    User loadUser(string username);
    vector<Transaction> loadUserTransactions(string username);
    bool saveUser(const User& user);
    bool saveTransaction(const Transaction& t);
};
```

**Benefit:** Centralized data access, easy to swap storage backend

---

## Algorithm Complexity Analysis

### CSVReader::readCSV()
```
Operation: Parse CSV file
Complexity: O(n) where n = number of lines
Memory: O(n) to store all entries
```

### OrderBook::matchAsksToBids()
```
Operation: Match buy/sell orders
Complexity: O(n log n + n²)
  - Sorting: O(n log n)
  - Matching: O(n²) worst case
Memory: O(n) for sorted vectors
```

### DataManager::generateCandlesticks()
```
Operation: Aggregate tick data to OHLC
Complexity: O(n log n)
  - Grouping: O(n) using map
  - Sorting: O(k log k) where k = number of periods
Memory: O(n) to store grouped data
```

### Wallet::containsCurrency()
```
Operation: Check balance
Complexity: O(1) using std::map
Memory: O(k) where k = number of currencies
```

---

## Data Persistence Strategy

### File Format: CSV

**Why CSV?**
- ✅ Human-readable for debugging
- ✅ Easy to import into Excel/Tableau
- ✅ No external database dependencies
- ✅ Git-friendly (text-based)

**Trade-offs:**
- ❌ Slower than binary formats
- ❌ No built-in indexing
- ❌ Manual data integrity management

### Future Improvements:
- Use SQLite for ACID transactions
- Add indexing for faster queries
- Implement caching layer

---

## Error Handling Strategy

### Input Validation Layers

**Layer 1: Type Validation**
```cpp
// Catch invalid conversions
try {
    price = std::stod(tokens[3]);
} catch (const std::exception& e) {
    throw; // Skip invalid lines
}
```

**Layer 2: Business Rule Validation**
```cpp
// Enforce business constraints
if (!wallet.canFulfilOrder(order)) {
    return "Insufficient funds";
}
```

**Layer 3: Data Integrity Validation**
```cpp
// Ensure data consistency
if (tokens.size() != 5) {
    throw std::exception{}; // Invalid CSV format
}
```

---

## Scalability Considerations

### Current Limitations
- Single-threaded (synchronous processing)
- In-memory data structures (limited by RAM)
- File-based storage (I/O bottleneck)

### If Scaling to Production:

**1. Multi-threading**
```cpp
// Process different products in parallel
std::thread t1(processBTC);
std::thread t2(processETH);
```

**2. Database Migration**
```cpp
// Replace CSV with SQLite/PostgreSQL
- Transactions table with indexes
- User authentication with hashing
- Audit logs with foreign keys
```

**3. Caching Layer**
```cpp
// Cache frequently accessed data
std::map<string, Candlestick> candlestickCache;
```

---

## Security Considerations

### Current Implementation

**Password Hashing:**
```cpp
std::hash<std::string> hasher;
size_t hash = hasher(password);
```

⚠️ **Note:** This is educational only. Production systems should use:
- bcrypt or Argon2 for password hashing
- Salt + pepper for additional security
- Secure key management

**Data Storage:**
- Plain text CSV (acceptable for learning project)
- Production would need encryption at rest

---

## Testing Strategy

### Unit Test Coverage (Recommended)

**High Priority:**
- `CSVReader::tokenise()` - String parsing edge cases
- `OrderBook::matchAsksToBids()` - Order matching logic
- `Wallet::canFulfilOrder()` - Balance validation
- `User::hashPassword()` - Authentication

**Medium Priority:**
- `DataManager::generateCandlesticks()` - OHLC calculations
- `Transaction::fromCSVString()` - Deserialization

**Low Priority:**
- UI/Menu functions (manual testing sufficient)

---

## Conclusion

This architecture demonstrates:

✅ **Clean separation of concerns** (3-tier architecture)
✅ **SOLID principles** (especially Single Responsibility)
✅ **Efficient algorithms** (appropriate complexity for operations)
✅ **Extensible design** (easy to add new features)
✅ **Production-ready patterns** (repository, factory, strategy)

For a **Business Analyst role**, this shows:
- Understanding of system architecture
- Ability to translate business requirements into technical design
- Knowledge of design trade-offs and scalability
- Communication of complex technical concepts clearly
