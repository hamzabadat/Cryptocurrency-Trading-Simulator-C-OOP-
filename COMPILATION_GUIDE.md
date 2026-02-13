# 🔧 Compilation & Usage Guide

## Quick Start (2 minutes)

### Method 1: Using g++ (Simplest)

```bash
# Navigate to project directory
cd crypto-trading-system

# Compile all files
g++ -std=c++17 *.cpp -o trading_system

# Run
./trading_system
```

---

### Method 2: Using CMake (Recommended)

```bash
# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Compile
make

# Run
./trading_system
```

---

## System Requirements

**Minimum:**
- C++ compiler with C++17 support
  - g++ 7.0 or later
  - Clang 5.0 or later  
  - MSVC 2017 or later
- Make or CMake (optional)

**Tested On:**
- Ubuntu 20.04+ (g++)
- macOS 11+ (Clang)
- Windows 10+ (MinGW / MSVC)

---

## Platform-Specific Instructions

### Linux (Ubuntu/Debian)

```bash
# Install compiler if needed
sudo apt update
sudo apt install g++ make cmake

# Clone/download project
cd crypto-trading-system

# Compile
g++ -std=c++17 *.cpp -o trading_system

# Run
./trading_system
```

---

### macOS

```bash
# Install Xcode command line tools
xcode-select --install

# Navigate to project
cd crypto-trading-system

# Compile
clang++ -std=c++17 *.cpp -o trading_system

# Run
./trading_system
```

---

### Windows (MinGW)

```batch
# Using MinGW g++
g++ -std=c++17 *.cpp -o trading_system.exe

# Run
trading_system.exe
```

---

### Windows (Visual Studio)

1. Open Visual Studio
2. Create new "Empty C++ Project"
3. Add all `.cpp` and `.h` files to project
4. Set C++ Language Standard to C++17:
   - Project → Properties → C/C++ → Language → C++ Language Standard → ISO C++17
5. Build → Build Solution (Ctrl+Shift+B)
6. Run (F5)

---

## Usage Instructions

### First Time Setup

When you run the program, you'll see:

```
==================================================
   CRYPTOCURRENCY TRADING SIMULATOR
==================================================

Please select an option:
1. Login/Register
2. View Candlestick Data
3. Exit
```

**Choose Option 1** to create your account.

---

### Creating an Account

```
Enter your full name: John Doe
Enter your email: john@example.com

Registration successful!
Your username is: 1234567890 (auto-generated)

You are now logged in.
```

---

### Main Menu

After login, you'll see the main menu:

```
Current Time: 2020/03/17 17:01:24

Main Menu:
1. Display Candlestick Data
2. Manage Wallet
3. Trade (Place Ask/Bid)
4. View Transaction History
5. User Statistics
6. Next Time Frame
7. Logout
8. Exit
```

---

### Example Workflow

**Step 1: Deposit Funds**
```
Choose option 2: Manage Wallet
Choose option 1: Deposit Funds
Enter currency (e.g., USDT, ETH, BTC): USDT
Enter amount: 10000
✓ Deposit successful! Balance: 10000 USDT
```

**Step 2: View Market Data**
```
Choose option 1: Display Candlestick Data
Choose product (e.g., ETH/USDT): ETH/USDT
Choose period (daily/monthly/yearly): daily

ETH/USDT - Ask Orders (Daily)
Date       | Open     | High     | Low      | Close
─────────────────────────────────────────────────────
2020-03-17 | 131.58   | 134.72   | 128.94   | 133.21
```

**Step 3: Place a Buy Order (Bid)**
```
Choose option 3: Trade
Choose option 2: Place Bid
Choose product: ETH/USDT
Enter amount of ETH to buy: 10
Calculated price: 1325.50 USDT (10 ETH × 132.55)
✓ Order placed!
```

**Step 4: View Transaction History**
```
Choose option 4: View Transaction History

Recent Transactions:
2020/03/17 17:01:24 | DEPOSIT     | USDT      | 10000.00
2020/03/17 17:05:12 | BID_PLACED  | ETH/USDT  | 10 ETH @ 132.55
```

---

## Sample Data

The project includes `20200317.csv` with **53,407 real market transactions** from March 17, 2020.

**Products available:**
- BTC/USD
- ETH/USDT
- DOGE/USDT
- BTC/USDT
- ETH/BTC

---

## Troubleshooting

### Compilation Errors

**Error: "C++17 required"**
```bash
# Add -std=c++17 flag
g++ -std=c++17 *.cpp -o trading_system
```

**Error: "undefined reference to..."**
```bash
# Make sure you compiled ALL .cpp files
g++ -std=c++17 *.cpp -o trading_system
# (note the *.cpp, not just one file)
```

**Error: "No such file or directory"**
```bash
# Make sure you're in the project directory
ls *.cpp  # Should show all .cpp files
```

---

### Runtime Errors

**Error: "Could not open CSV file"**
- Make sure `20200317.csv` is in the same directory as the executable
- Check file permissions

**Error: "Invalid email format"**
- Email must contain '@' and '.'
- Example: `user@example.com`

---

## Advanced Usage

### Custom CSV Data

You can use your own market data:

**Format required:**
```csv
timestamp,product,orderType,price,amount
2020/03/17 17:01:24.884492,BTC/USD,ask,5000.50,0.1
2020/03/17 17:01:25.123456,BTC/USD,bid,4999.00,0.2
```

**Fields:**
- `timestamp`: YYYY/MM/DD HH:MM:SS.microseconds
- `product`: CURRENCY1/CURRENCY2
- `orderType`: "ask" or "bid"
- `price`: Decimal number
- `amount`: Decimal number

Save as `.csv` and update filename in code if needed.

---

### Batch Testing

```bash
# Run with predefined inputs (Linux/macOS)
./trading_system < test_inputs.txt
```

Create `test_inputs.txt`:
```
1
John Doe
john@example.com
2
1
USDT
10000
8
```

---

## Performance Notes

**Speed:**
- Loads 50,000+ orders in <1 second
- Generates candlesticks in <100ms
- Order matching in <50ms

**Memory:**
- Typical usage: ~50MB RAM
- Scales linearly with data size

---

## File Structure After Running

```
crypto-trading-system/
├── trading_system          # Executable
├── 20200317.csv            # Market data (input)
├── users.csv               # Generated: User accounts
├── transactions.csv        # Generated: Transaction log
├── wallet.csv              # Generated: Wallet balances
└── *.cpp, *.h              # Source code
```

---

## Tips for Best Experience

1. **Start with deposits** - Fund your wallet before trading
2. **Check candlesticks first** - Understand market before placing orders
3. **Use realistic amounts** - System validates you have sufficient funds
4. **Review transaction history** - Track all your activities

---

## Getting Help

**Common Questions:**

Q: How do I reset my account?  
A: Delete `users.csv`, `transactions.csv`, and `wallet.csv`

Q: Can I use real money?  
A: No! This is a SIMULATOR using historical data

Q: Why do my orders not execute?  
A: Orders execute only when there's a matching ask/bid in the dataset

Q: How do I exit?  
A: Choose option 8 from main menu

---

## Next Steps

After running the simulator:

1. Try different trading strategies
2. Analyze candlestick patterns
3. Review the architecture documentation
4. Explore the source code
5. Extend with new features!

---

## Support

For issues or questions about the code:
1. Check this guide's troubleshooting section
2. Review ARCHITECTURE.md for design decisions
3. Read inline code comments
4. Open an issue on GitHub

---

**Happy Trading! 📈**
