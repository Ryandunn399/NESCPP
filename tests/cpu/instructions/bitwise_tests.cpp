#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, AND_Immediate)
{
    SetupMemory(Memory6502::kRomStart, { 0x29, 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, AND_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x25, 0x42 });
    SetupMemory(0x42, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, AND_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0x35, 0x42 });
    cpu.X = 0x05;
    SetupMemory(0x47, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, AND_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x2D, 0x10, 0x80 });
    SetupMemory(0x8010, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, AND_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0x3D, 0x00, 0x81 });
    cpu.X = 0x10;
    SetupMemory(0x8110, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, AND_AbsoluteY)
{
    SetupMemory(Memory6502::kRomStart, { 0x39, 0x00, 0x82 });
    cpu.Y = 0x10;
    SetupMemory(0x8210, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, AND_IndirectX)
{
    SetupMemory(Memory6502::kRomStart, { 0x21, 0x40 });
    cpu.X = 0x05;
    SetupMemory(0x45, { 0x10, 0x83 });  // Low and high byte of address
    SetupMemory(0x8310, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, AND_IndirectY)
{
    SetupMemory(Memory6502::kRomStart, { 0x31, 0x40 });
    cpu.Y = 0x10;
    SetupMemory(0x40, { 0x00, 0x84 });  // Low and high byte of address
    SetupMemory(0x8410, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_Immediate)
{
    SetupMemory(Memory6502::kRomStart, { 0x09, 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x05, 0x42 });
    SetupMemory(0x42, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0x15, 0x42 });
    cpu.X = 0x05;
    SetupMemory(0x47, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x0D, 0xB4, 0xA3 });
    SetupMemory(0xA3B4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ORA_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0x1D, 0xB4, 0xA3 });
    cpu.X = 0x10;
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ORA_AbsoluteY)
{
    SetupMemory(Memory6502::kRomStart, { 0x19, 0xB4, 0xA3 });
    cpu.Y = 0x10;
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ORA_IndirectX)
{
    SetupMemory(Memory6502::kRomStart, { 0x01, 0x40 });
    cpu.X = 0x05;
    SetupMemory(0x45, { 0xB4, 0xA3 });  // Low and high byte of address
    SetupMemory(0xA3B4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_IndirectY)
{
    SetupMemory(Memory6502::kRomStart, { 0x11, 0x40 });
    cpu.Y = 0x10;
    SetupMemory(0x40, { 0xB4, 0xA3 });  // Low and high byte of address
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}