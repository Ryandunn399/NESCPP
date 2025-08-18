#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, ADC_Immediate)
{
    SetupMemory(Memory6502::kRomStart, {0x69, 0x23});
    cpu.A = 0x8A;

    uint8_t expectedResult = (0x8A + 0x23) & 0xFF;

    cpu.ExecuteInstruction();

    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_Immediate_Carry)
{
    SetupMemory(Memory6502::kRomStart, {0x69, 0x5});
    cpu.A = 0xFF;

    uint8_t expectedResult = (0xFF + 0x5) & 0xFF;

    cpu.ExecuteInstruction();

    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_Immediate_Overflow)
{
    SetupMemory(Memory6502::kRomStart, {0x69, 0x32});
    cpu.A = 0x64;

    uint8_t expectedResult = (0x64 + 0x32) & 0xFF;

    cpu.ExecuteInstruction();

    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 2);
}
