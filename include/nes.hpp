#pragma once

#include "cpu/cpu6502.hpp"
#include "cpu/memory.hpp"

/**
 * @brief Wrapper class that contains all the emulated hardware
 * used to run NES ROM data.
 * 
 */
class NES
{
public:
    NES();

    Memory6502 Memory;
    Cpu6502 CPU;
};
