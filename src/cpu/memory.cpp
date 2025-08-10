#include "cpu/memory.hpp"
#include <string>
#include <stdexcept>

Memory::Memory()
{
    memory.fill(0);
}

uint8_t Memory::ReadByte(uint16_t address)
{
    return memory[address];
}

void Memory::WriteByte(uint16_t address, uint8_t value)
{
    // Attempting to write in ROM region.
    if (address >= kRomStart)
        throw std::runtime_error("An attempt was made to write into ROM memory.");

    memory[address] = value;
}

void Memory::ForceWriteByte(uint16_t address, uint8_t value)
{
    memory[address] = value;
}

uint16_t Memory::ReadWord(uint16_t address)
{
    if (address + 1 >= kMemorySize)
        throw std::overflow_error("Overflow occurred when attempting to call Memory::WriteWord");

    uint8_t lowByte = memory[address];
    uint8_t highByte = memory[address + 1];
    return lowByte | (highByte << 8);
}

void Memory::WriteWord(uint16_t address, uint16_t value)
{
    if (address + 1 >= kMemorySize)
        throw std::overflow_error("Overflow occurred when attempting to call Memory::WriteWord");

    if (address >= kRomStart)
        throw std::runtime_error("An attempt was made to write into ROM memory.");

    uint8_t lowByte = value & 0xFF;
    uint8_t highByte = (value >> 8) & 0xFF;
    
    memory[address] = lowByte;
    memory[address + 1] = highByte;
}

void Memory::PushByte(uint8_t& sp, uint8_t value)
{
    uint16_t stackAddress = kStackStart + sp;

    memory[stackAddress] = value;
    sp--; // Stack grows downwards
}

uint8_t Memory::PopByte(uint8_t& sp)
{
    sp++;
    uint16_t stackAddress = kStackStart + sp;
    return memory[stackAddress];
}

void Memory::PushWord(uint8_t& sp, uint16_t value)
{
    // Push high byte first, then low byte
    PushByte(sp, (value >> 8) & 0xFF);
    PushByte(sp, value & 0xFF);
}

uint16_t Memory::PopWord(uint8_t& sp)
{
    uint8_t lowByte = PopByte(sp);
    uint8_t highByte = PopByte(sp);
    return lowByte | (highByte << 8);
}

uint8_t Memory::ReadZeroPage(uint8_t address)
{
    return memory[address];
}

void Memory::WriteZeroPage(uint8_t address, uint8_t value)
{
    memory[address] = value;
}

void Memory::LoadProgram(const uint8_t* program, size_t size, uint16_t startAddress)
{
    if (program == nullptr)
        throw std::invalid_argument("Program pointer cannot be null");

    if (size == 0)
        throw std::invalid_argument("Program size cannot be zero");

    if (startAddress + size >= kMemorySize)
        throw std::invalid_argument("Program too large for memory space");

    std::memcpy(&memory[startAddress], program, size);
}