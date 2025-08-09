#pragma once

#include <cstdint>

/**
 * @brief Class that organizes the various values of the status register.
 * 
 */
class StatusRegister
{
private:
    union
    {
        uint8_t reg;
        struct
        {
            uint8_t carry       : 1;
            uint8_t zero        : 1;
            uint8_t interrupt   : 1;
            uint8_t decimal     : 1;
            uint8_t brk         : 1;
            uint8_t unused      : 1;
            uint8_t overflow    : 1;
            uint8_t negative    : 1;
        } flags;
    };
    
public:
    /**
    * @brief Get the Carry bit
    * 
    * @return int 
    */
    int GetCarry() const { return flags.carry; }

    /**
     * @brief Set the Carry bit
     * 
     * @param value 
     */
    void SetCarry(bool value) { flags.carry = value; }

    /**
     * @brief Get the Zero bit
     * 
     * @return int 
     */
    int GetZero() const { return flags.zero; }

    /**
     * @brief Set the Zero bit
     * 
     * @param value 
     * @return int 
     */
    void SetZero(bool value) { flags.zero = value; }

    /**
     * @brief Get the Interrupt bit
     * 
     * @return int 
     */
    int GetInterrupt() const { return flags.interrupt; }

    /**
     * @brief Set the Interrupt bit
     * 
     * @param value 
     */
    void SetInterrupt(bool value) { flags.interrupt = value; }

    /**
     * @brief Get the Decimal bit
     * 
     * @return int 
     */
    int GetDecimal() const { return flags.decimal; }

    /**
     * @brief Set the Decimal bit
     * 
     * @param value 
     */
    void SetDecimal(bool value) { flags.decimal = value; }

    /**
     * @brief Get the Break bit
     * 
     * @return int 
     */
    int GetBreak() const { return flags.brk; }

    /**
     * @brief Set the Break bit
     * 
     * @param value 
     */
    void SetBreak(bool value) { flags.brk = value; }

    /**
     * @brief Get the Overflow bit
     * 
     * @return int 
     */
    int GetOverflow() const { return flags.overflow; }

    /**
     * @brief Set the Overflow bit
     * 
     * @param value 
     */
    void SetOverflow(bool value) { flags.overflow = value; }

    /**
     * @brief Get the Negative bit
     * 
     * @return int 
     */
    int GetNegative() const { return flags.negative; }

    /**
     * @brief Set the Negative bit
     * 
     * @param value 
     */
    void SetNegative(bool value) { flags.negative = value; }

    /**
     * @brief Get the entire value of the status register.
     * 
     * @return int 
     */
    uint8_t GetRegister() const { return reg; }

    /**
     * @brief Set the entire value of the status register.
     * 
     * @param value 
     */
    void SetRegister(uint8_t value) { reg = value; }
};