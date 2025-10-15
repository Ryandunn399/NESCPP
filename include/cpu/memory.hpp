#pragma once
#include <array>
#include <cstdint>
#include <cstddef>

#ifndef MEM_SIZE
#define MEM_SIZE 65536
#endif

/**
 * @brief Memory class that attempts to simplify memory abstractions.
 * 
 */
class Memory6502
{
public:
    static constexpr size_t kMemorySize = 65536;
    static constexpr size_t kOpcodeTableSize = 256;
    
    static constexpr uint16_t kZeroPageStart = 0x0000;
    static constexpr uint16_t kZeroPageEnd = 0x00FF;
    static constexpr uint16_t kStackStart = 0x0100;
    static constexpr uint16_t kStackEnd = 0x01FF;
    static constexpr uint16_t kRamStart = 0x0200;
    static constexpr uint16_t kRamEnd = 0x7FFF;
    static constexpr uint16_t kRomStart = 0x8000;
    static constexpr uint16_t kRomEnd = 0xFFFF;
    static constexpr uint16_t kInterruptVector = 0xFFFE;


    /**
     * @brief Keeps track of the stack pointer.
     * 
     */
    uint8_t StackPointer;

    /**
     * @brief Construct a new Memory object.
     * 
     */
    Memory6502();

    /**
     * @brief Reads a byte from main memory.
     * 
     * @param address 
     * @return uint8_t 
     */
    uint8_t ReadByte(uint16_t address);

    /**
     * @brief Writes a byte into main memory.
     * 
     * @param address 
     * @param value 
     */
    void WriteByte(uint16_t address, uint8_t value);

    /**
     * @brief Writes a byte into main memory without validating it first.
     * 
     * Should only be used for things like setting up test environments.
     * 
     * @param address 
     * @param value 
     */
    void ForceWriteByte(uint16_t address, uint8_t value);

    /**
     * @brief Reads a word from main memory.
     * 
     * @param address 
     * @return uint16_t 
     */
    uint16_t ReadWord(uint16_t address);

    /**
     * @brief Writes a word into main memory.
     * 
     * @param address 
     * @param value 
     */
    void WriteWord(uint16_t address, uint16_t value);

    /**
     * @brief Pushes byte onto stack.
     * 
     * @param sp 
     * @param value 
     */
    void PushByte(uint8_t value);

    /**
     * @brief Pops a byte off the stack.
     * 
     * @param sp 
     * @return uint8_t 
     */
    uint8_t PopByte();

    /**
     * @brief Pushes a word onto the stack.
     * 
     * @param sp 
     * @param value 
     */
    void PushWord(uint16_t value);

    /**
     * @brief Pops a word off the stack.
     * 
     * @param sp 
     * @return uint16_t 
     */
    uint16_t PopWord();

    /**
     * @brief Reads from zero page.
     * 
     * @param address 
     * @return uint8_t 
     */
    uint8_t ReadZeroPage(uint8_t address);

    /**
     * @brief Writes into zero page.
     * 
     * @param address 
     * @param value 
     */
    void WriteZeroPage(uint8_t address, uint8_t value);

    /**
     * @brief Loads a program into main memory.
     * 
     * @param program 
     * @param size 
     * @param startAddress 
     */
    void LoadProgram(const uint8_t* program, size_t size, uint16_t startAddress = kRomStart);

    /**
     * @brief Retrieves raw memory data.
     * 
     * @return uint8_t* 
     */
    uint8_t* GetRawMemory() { return memory.data(); }

    /// @brief Resets the stack pointer.
    void ResetStackPointer();
private:
    /**
     * @brief Memory array for storing our bytes.
     * 
     */
    std::array<uint8_t, kMemorySize> memory;
};
