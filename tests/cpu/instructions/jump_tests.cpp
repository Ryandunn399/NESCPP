#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, JMPAbsolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x4C, 0x34, 0x12 });

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x1234, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, JMPIndirect)
{
    SetupMemory(Memory6502::kRomStart, { 0x6C, 0x34, 0x12 });
    SetupMemory(0x1234, { 0xCD, 0xAB });

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xABCD, cpu.PC);
    EXPECT_EQ(5, cycles);
}

TEST_F(Cpu6502Test, JMPIndirectBug)
{
    SetupMemory(Memory6502::kRomStart, { 0x6C, 0xFF, 0x12 });
    SetupMemory(0x12FF, { 0xCD });
    SetupMemory(0x1300, { 0xAB });
    SetupMemory(0x1200, { 0xDE });

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xDECD, cpu.PC);
    EXPECT_EQ(5, cycles);
}

TEST_F(Cpu6502Test, JMPIndirectBug_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x6C, 0xFF, 0x00 });
    SetupMemory(0x00FF, { 0x34 });
    SetupMemory(0x0100, { 0x12 }); 
    SetupMemory(0x0000, { 0x56 }); 

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x5634, cpu.PC);
    EXPECT_EQ(5, cycles);
}

TEST_F(Cpu6502Test, JMPIndirect_BeforeBoundary)
{
    SetupMemory(Memory6502::kRomStart, { 0x6C, 0xFE, 0x12 });
    SetupMemory(0x12FE, { 0xCD, 0xAB });

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xABCD, cpu.PC);
    EXPECT_EQ(5, cycles);
}

TEST_F(Cpu6502Test, JSRAbsolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x20, 0x34, 0x12 });
    uint16_t expectedPc = cpu.PC + 2;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x1234, cpu.PC);
    EXPECT_EQ(0xFF - 2, cpu.GetStackPointer());
    EXPECT_EQ(expectedPc, memory.ReadWord(0x01FE));
    EXPECT_EQ(6, cycles);
}

TEST_F(Cpu6502Test, JSR_StackWrap)
{
    SetupMemory(Memory6502::kRomStart, { 0x20, 0x34, 0x12 });
    memory.StackPointer = 0x01;  // Near bottom of stack

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x1234, cpu.PC);
    EXPECT_EQ(0xFF, cpu.GetStackPointer());  // Wraps around
    // Return address stored at $0100 and $0101
}