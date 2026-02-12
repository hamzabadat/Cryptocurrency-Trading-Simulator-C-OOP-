// ==================== Candlestick.h ====================
/**
 * Candlestick.h
 * OHLC (Open, High, Low, Close) data structure for technical analysis
 * TASK 1: Represents aggregated price data for time periods
 */

#pragma once
#include <string>
#include <vector>

class Candlestick
{
public:
    Candlestick(std::string _date,
        double _open,
        double _high,
        double _low,
        double _close,
        std::string _product,
        std::string _type);

    Candlestick();

    // Getters
    std::string getDate() const { return date; }
    double getOpen() const { return open; }
    double getHigh() const { return high; }
    double getLow() const { return low; }
    double getClose() const { return close; }
    std::string getProduct() const { return product; }
    std::string getType() const { return type; }

    std::string toString() const;

    // Comparator for chronological sorting
    static bool compareByDate(const Candlestick& c1, const Candlestick& c2)
    {
        return c1.date < c2.date;
    }

private:
    std::string date;      // YYYY-MM-DD format
    double open;           // First price in period
    double high;           // Highest price in period
    double low;            // Lowest price in period
    double close;          // Last price in period
    std::string product;   // e.g., ETH/USDT
    std::string type;      // "ask" or "bid"
};