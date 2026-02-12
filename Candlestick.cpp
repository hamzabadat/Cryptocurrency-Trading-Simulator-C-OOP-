// ==================== Candlestick.cpp ====================
/**
 * Candlestick.cpp
 * Implementation of candlestick OHLC data structure
 */

#include "Candlestick.h"
#include <sstream>
#include <iomanip>

Candlestick::Candlestick(std::string _date,
    double _open,
    double _high,
    double _low,
    double _close,
    std::string _product,
    std::string _type)
    : date(_date),
    open(_open),
    high(_high),
    low(_low),
    close(_close),
    product(_product),
    type(_type)
{
}

Candlestick::Candlestick()
    : date(""),
    open(0.0),
    high(0.0),
    low(0.0),
    close(0.0),
    product(""),
    type("")
{
}

std::string Candlestick::toString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    oss << date << " | "
        << "O: " << open << " | "
        << "H: " << high << " | "
        << "L: " << low << " | "
        << "C: " << close;
    return oss.str();
}