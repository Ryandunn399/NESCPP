#ifndef CPU_FIXTURE_HPP
#define CPU_FIXTURE_HPP

#include <gtest/gtest.h>
#include "cpu/cpu6502.hpp"
#include "nes.hpp"

/**
 * @brief Fixture to be used to test CPU functionality.
 * 
 */
class Cpu6502Test : public ::testing::Test
{
protected:
    NES nes;
    Cpu6502& cpu;
    Memory& memory;
    
    Cpu6502Test() : nes(), cpu(nes.CPU), memory(nes.Memory)
    {
    }

    void SetUp() override
    {
        // CPU is initialized with fresh state
        // PC starts at ROM area, all registers are 0
    }
    
    // Helper to set up memory with test data
    void SetupMemory(uint16_t address, const std::vector<uint8_t>& data)
    {
        for (uint16_t i = 0; i < data.size(); ++i)
        {
            memory.ForceWriteByte(address + i, data[i]);
        }
    }
};

#endif