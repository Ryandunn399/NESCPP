#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, BCC_BranchNotTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x90, 0x32 });
    cpu.StatusReg.SetCarry(1);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2, cpu.PC);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, BCC_BranchTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x90, 0x32 });
    cpu.StatusReg.SetCarry(0);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, BCC_BranchTakenPageCross)
{
    SetupMemory(Memory6502::kRomStart + 0xFD, { 0x90, 0x32 });
    cpu.StatusReg.SetCarry(0);
    cpu.PC += 0xFD;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(4, cycles);
}