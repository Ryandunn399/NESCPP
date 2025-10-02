#include <bit>
#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, ASL_Accumulator)
{
    SetupMemory(Memory6502::kRomStart, { 0x0A });
    cpu.A = 0x10;
    uint8_t expectedResult = 0x10 << 1;

    cpu.ExecuteInstruction();
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, ASL_AccumulatorNegativeCarry)
{
    SetupMemory(Memory6502::kRomStart, { 0x0A });
    cpu.A = 0xFF;
    uint8_t expectedResult = 0xFF;
    expectedResult <<= 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, ASL_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x06, 0x20 });
    SetupMemory(0x20, { 0x30 });
    uint8_t expectedResult = 0x30 << 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x20));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ASL_ZeroPageNegativeCarry)
{
    SetupMemory(Memory6502::kRomStart, { 0x06, 0x20 });
    SetupMemory(0x20, { 0xFF });
    uint8_t expectedResult = 0xFF;
    expectedResult <<= 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x20));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ASL_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0x16, 0x20 });
    SetupMemory(0x20 + 0x5, { 0x30 });
    cpu.X = 0x5;
    uint8_t expectedResult = 0x30 << 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x20 + 0x5));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ASL_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x0E, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x36 });
    uint8_t expectedResult = 0x36 << 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x1234));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ASL_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0x1E, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x6, { 0x36 });
    cpu.X = 0x6;
    uint8_t expectedResult = 0x36 << 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x1234 + 0x6));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, LSR_Accumulator)
{
    SetupMemory(Memory6502::kRomStart, { 0x4A });
    cpu.A = 0x10;
    uint8_t expectedResult = 0x10 >> 1;
    
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, LSR_AccumulatorCarry)
{
    SetupMemory(Memory6502::kRomStart, { 0x4A });
    cpu.A = 0xFF;
    uint8_t expectedResult = 0xFF >> 1;
    
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, LSR_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x46, 0x30 });
    SetupMemory(0x30, { 0x42 });
    uint8_t expectedResult = 0x42 >> 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x30));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, LSR_ZeroPageCarry)
{
    SetupMemory(Memory6502::kRomStart, { 0x46, 0x30 });
    SetupMemory(0x30, { 0xFF });
    uint8_t expectedResult = 0xFF >> 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x30));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, LSR_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0x56, 0x30 });
    SetupMemory(0x30 + 0x5, { 0x10 });
    cpu.X = 0x5;
    uint8_t expectedResult = 0x10 >> 1;

    cpu.ExecuteInstruction();

    uint8_t memoryValue = memory.ReadByte(0x30 + 0x5);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, LSR_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x4E, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x10 });
    uint8_t expectedResult = 0x10 >> 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x1234));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, LSR_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0x5E, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x5, { 0x10 });
    cpu.X = 0x5;
    uint8_t expectedResult = 0x10 >> 1;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x1234 + 0x5));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ROL_Accumulator)
{
    SetupMemory(Memory6502::kRomStart, { 0x2A });
    cpu.A = 0x20;
    uint8_t expectedResult = std::rotl(cpu.A, 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, ROL_AccumulatorCarryNegative)
{
    SetupMemory(Memory6502::kRomStart, { 0x2A });
    cpu.A = 0xFE;
    cpu.StatusReg.SetCarry(0x1);
    uint8_t expectedResult = std::rotl(cpu.A, 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, ROL_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x26, 0x30 });
    SetupMemory(0x30, { 0x20 });
    uint8_t expectedResult = std::rotl(memory.ReadByte(0x30), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x30));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ROL_ZeroPageNegativeCarry)
{
    SetupMemory(Memory6502::kRomStart, { 0x26, 0x30 });
    SetupMemory(0x30, { 0xFE });
    cpu.StatusReg.SetCarry(0x1);
    uint8_t expectedResult = std::rotl(memory.ReadByte(0x30), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x30));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ROL_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0x36, 0x30 });
    SetupMemory(0x30 + 0x5, { 0x20 });
    cpu.X = 0x5;
    uint8_t expectedResult = std::rotl(memory.ReadByte(0x30 + 0x5), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x30 + 0x5));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ROL_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x2E, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x20 });
    uint8_t expectedResult = std::rotl(memory.ReadByte(0x1234), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x1234));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ROL_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0x3E, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x5, { 0x20 });
    cpu.X = 0x5;
    uint8_t expectedResult = std::rotl(memory.ReadByte(0x1234 + 0x5), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x1234 + 0x5));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ROR_Accumulator)
{
    SetupMemory(Memory6502::kRomStart, { 0x6A });
    cpu.A = 0x40;
    uint8_t expectedResult = std::rotr(cpu.A, 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, ROR_AccumulatorNegativeCarry)
{
    SetupMemory(Memory6502::kRomStart, { 0x6A });
    cpu.A = 0x1;
    cpu.StatusReg.SetCarry(0x1);
    uint8_t expectedResult = std::rotr(cpu.A, 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, ROR_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x66, 0x30 });
    SetupMemory(0x30, { 0x40 });
    uint8_t expectedResult = std::rotr(memory.ReadByte(0x30), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x30));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ROR_ZeroPageNegativeCarry)
{
    SetupMemory(Memory6502::kRomStart, { 0x66, 0x30 });
    SetupMemory(0x30, { 0x1 });
    cpu.StatusReg.SetCarry(0x1);
    uint8_t expectedResult = std::rotr(memory.ReadByte(0x30), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x30));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ROR_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0x76, 0x30 });
    SetupMemory(0x30 + 0x5, { 0x40 });
    cpu.X = 0x5;
    uint8_t expectedResult = std::rotr(memory.ReadByte(0x30 + 0x5), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x30 + 0x5));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ROR_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x6E, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x40 });
    uint8_t expectedResult = std::rotr(memory.ReadByte(0x1234), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x1234));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ROR_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0x7E, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x5, { 0x40 });
    cpu.X = 0x5;
    uint8_t expectedResult = std::rotr(memory.ReadByte(0x1234 + 0x5), 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, memory.ReadByte(0x1234 + 0x5));
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    AssertPCLocation(cpu, 3);
}