#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

class Utils
{
public:
    // Delete constructor and destructor to prevent instantiation
    Utils() = delete;
    ~Utils() = delete;

    // Converts an unsigned integer type to a hex string with "0x" prefix, uppercase, 2 digits by default
    template <typename T>
    static std::string to_hex(T value, int width = 2)
    {
        static_assert(std::is_integral<T>::value, "to_hex requires integral type");
        std::stringstream ss;
        ss << "0x"
           << std::hex << std::uppercase << std::setfill('0') << std::setw(width)
           << static_cast<uint64_t>(value);
        return ss.str();
    }
};