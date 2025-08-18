#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, TAX)
{
    SetupMemory(Memory6502::kRomStart, {0xAA});
    cpu.A = 0x9;
    cpu.X = 0x1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(0x9, cpu.A);
    EXPECT_EQ(0x9, cpu.X);
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, TXA)
{
    SetupMemory(Memory6502::kRomStart, {0x8A});
    cpu.A = 0x9;
    cpu.X = 0x1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(0x1, cpu.A);
    EXPECT_EQ(0x1, cpu.X);
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, TAY)
{
    SetupMemory(Memory6502::kRomStart, {0xA8});
    cpu.A = 0x11;
    cpu.Y = 0xBB;

    cpu.ExecuteInstruction();

    EXPECT_EQ(0x11, cpu.A);
    EXPECT_EQ(0x11, cpu.Y);
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, TYA)
{
    SetupMemory(Memory6502::kRomStart, {0x98});
    cpu.A = 0x11;
    cpu.Y = 0xBB;

    cpu.ExecuteInstruction();

    EXPECT_EQ(0xBB, cpu.A);
    EXPECT_EQ(0xBB, cpu.Y);
    AssertPCLocation(cpu, 1);
}
