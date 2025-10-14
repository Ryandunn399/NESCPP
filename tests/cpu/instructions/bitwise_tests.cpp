#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, AND_Immediate)
{
    SetupMemory(Memory6502::kRomStart, { 0x29, 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(2, cycles);  // AND Immediate = 2 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, AND_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x25, 0x42 });
    SetupMemory(0x42, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(3, cycles);  // AND Zero Page = 3 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, AND_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0x35, 0x42 });
    cpu.X = 0x05;
    SetupMemory(0x47, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // AND Zero Page,X = 4 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, AND_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x2D, 0x10, 0x80 });
    SetupMemory(0x8010, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // AND Absolute = 4 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, AND_AbsoluteX_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x3D, 0x00, 0x81 });
    cpu.X = 0x10;
    SetupMemory(0x8110, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // AND Absolute,X = 4 cycles (no page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, AND_AbsoluteX_PageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x3D, 0xFF, 0x80 });
    cpu.X = 0x11;  // 0x80FF + 0x11 = 0x8110 (page cross)
    SetupMemory(0x8110, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // AND Absolute,X = 5 cycles (page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, AND_AbsoluteY_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x39, 0x00, 0x82 });
    cpu.Y = 0x10;
    SetupMemory(0x8210, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // AND Absolute,Y = 4 cycles (no page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, AND_AbsoluteY_PageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x39, 0xFF, 0x81 });
    cpu.Y = 0x11;  // 0x81FF + 0x11 = 0x8210 (page cross)
    SetupMemory(0x8210, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // AND Absolute,Y = 5 cycles (page cross)
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

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // AND Indirect,X = 6 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, AND_IndirectY_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x31, 0x40 });
    cpu.Y = 0x10;
    SetupMemory(0x40, { 0x00, 0x84 });  // Low and high byte of address
    SetupMemory(0x8410, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // AND Indirect,Y = 5 cycles (no page cross)
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, AND_IndirectY_PageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x31, 0x40 });
    cpu.Y = 0x11;
    SetupMemory(0x40, { 0xFF, 0x83 });  // Low and high byte of address
    SetupMemory(0x8410, { 0b10101010 });  // 0x83FF + 0x11 = 0x8410 (page cross)
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10001010;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // AND Indirect,Y = 6 cycles (page cross)
    AssertPCLocation(cpu, 2);
}

// ORA Instructions
TEST_F(Cpu6502Test, ORA_Immediate)
{
    SetupMemory(Memory6502::kRomStart, { 0x09, 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(2, cycles);  // ORA Immediate = 2 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x05, 0x42 });
    SetupMemory(0x42, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(3, cycles);  // ORA Zero Page = 3 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0x15, 0x42 });
    cpu.X = 0x05;
    SetupMemory(0x47, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // ORA Zero Page,X = 4 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x0D, 0xB4, 0xA3 });
    SetupMemory(0xA3B4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // ORA Absolute = 4 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ORA_AbsoluteX_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x1D, 0xB4, 0xA3 });
    cpu.X = 0x10;
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // ORA Absolute,X = 4 cycles (no page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ORA_AbsoluteX_PageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x1D, 0xFF, 0xA2 });
    cpu.X = 0xC5;  // 0xA2FF + 0xC5 = 0xA3C4 (page cross)
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // ORA Absolute,X = 5 cycles (page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ORA_AbsoluteY_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x19, 0xB4, 0xA3 });
    cpu.Y = 0x10;
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // ORA Absolute,Y = 4 cycles (no page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ORA_AbsoluteY_PageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x19, 0xFF, 0xA2 });
    cpu.Y = 0xC5;  // 0xA2FF + 0xC5 = 0xA3C4 (page cross)
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // ORA Absolute,Y = 5 cycles (page cross)
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

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // ORA Indirect,X = 6 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_IndirectY_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x11, 0x40 });
    cpu.Y = 0x10;
    SetupMemory(0x40, { 0xB4, 0xA3 });  // Low and high byte of address
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // ORA Indirect,Y = 5 cycles (no page cross)
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ORA_IndirectY_PageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x11, 0x40 });
    cpu.Y = 0xC5;
    SetupMemory(0x40, { 0xFF, 0xA2 });  // Low and high byte of address
    SetupMemory(0xA3C4, { 0b10101010 });  // 0xA2FF + 0xC5 = 0xA3C4 (page cross)
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b10101111;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // ORA Indirect,Y = 6 cycles (page cross)
    AssertPCLocation(cpu, 2);
}

// EOR Instructions
TEST_F(Cpu6502Test, EOR_Immediate)
{
    SetupMemory(Memory6502::kRomStart, { 0x49, 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(2, cycles);  // EOR Immediate = 2 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, EOR_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x45, 0x40 });
    SetupMemory(0x40, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(3, cycles);  // EOR Zero Page = 3 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, EOR_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0x55, 0x40 });
    cpu.X = 0x10;
    SetupMemory(0x50, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // EOR Zero Page,X = 4 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, EOR_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x4D, 0xC4, 0xA3 });
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // EOR Absolute = 4 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, EOR_AbsoluteX_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x5D, 0xB4, 0xA3 });
    cpu.X = 0x10;
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // EOR Absolute,X = 4 cycles (no page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, EOR_AbsoluteX_PageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x5D, 0xFF, 0xA2 });
    cpu.X = 0xC5;  // 0xA2FF + 0xC5 = 0xA3C4 (page cross)
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // EOR Absolute,X = 5 cycles (page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, EOR_AbsoluteY_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x59, 0xB4, 0xA3 });
    cpu.Y = 0x10;
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // EOR Absolute,Y = 4 cycles (no page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, EOR_AbsoluteY_PageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x59, 0xFF, 0xA2 });
    cpu.Y = 0xC5;  // 0xA2FF + 0xC5 = 0xA3C4 (page cross)
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // EOR Absolute,Y = 5 cycles (page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, EOR_IndirectX)
{
    SetupMemory(Memory6502::kRomStart, { 0x41, 0x30 });
    cpu.X = 0x10;
    SetupMemory(0x40, { 0xC4, 0xA3 });  // Low and high byte of address
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // EOR Indirect,X = 6 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, EOR_IndirectY_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x51, 0x40 });
    cpu.Y = 0x10;
    SetupMemory(0x40, { 0xB4, 0xA3 });  // Low and high byte of address
    SetupMemory(0xA3C4, { 0b10101010 });
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // EOR Indirect,Y = 5 cycles (no page cross)
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, EOR_IndirectY_PageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0x51, 0x40 });
    cpu.Y = 0xC5;
    SetupMemory(0x40, { 0xFF, 0xA2 });  // Low and high byte of address
    SetupMemory(0xA3C4, { 0b10101010 });  // 0xA2FF + 0xC5 = 0xA3C4 (page cross)
    cpu.A = 0b10001111;
    uint8_t expectedOutput = 0b00100101;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // EOR Indirect,Y = 6 cycles (page cross)
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, EOR_SetsZeroFlag)
{
    SetupMemory(Memory6502::kRomStart, { 0x49, 0b10101010 });
    cpu.A = 0b10101010;  // XOR with itself = 0
    uint8_t expectedOutput = 0b00000000;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, EOR_SetsNegativeFlag)
{
    SetupMemory(Memory6502::kRomStart, { 0x49, 0b11110000 });
    cpu.A = 0b01110000;
    uint8_t expectedOutput = 0b10000000;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedOutput, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, BIT_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x24, 0x40 });
    SetupMemory(0x40, { 0b11000000 });
    cpu.A = 0b10000000;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0b10000000, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());  // Bit 7 of memory
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());  // Bit 6 of memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A & memory != 0
    EXPECT_EQ(3, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, BIT_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x2C, 0xC4, 0xA3 });
    SetupMemory(0xA3C4, { 0b11000000 });
    cpu.A = 0b10000000;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0b10000000, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());  // Bit 7 of memory
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());  // Bit 6 of memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A & memory != 0
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, BIT_SetsZeroFlag)
{
    SetupMemory(Memory6502::kRomStart, { 0x24, 0x40 });
    SetupMemory(0x40, { 0b11000000 });
    cpu.A = 0b00111111;  // No bits overlap with memory

    cpu.ExecuteInstruction();

    EXPECT_EQ(0b00111111, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());  // Bit 7 of memory
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());  // Bit 6 of memory
    EXPECT_EQ(1, cpu.StatusReg.GetZero());      // A & memory == 0
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, BIT_NegativeClearOverflowSet)
{
    SetupMemory(Memory6502::kRomStart, { 0x24, 0x40 });
    SetupMemory(0x40, { 0b01000000 });
    cpu.A = 0b11111111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(0b11111111, cpu.A);  // A should remain unchanged
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Bit 7 of memory is 0
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());  // Bit 6 of memory is 1
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A & memory != 0
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, BIT_NegativeSetOverflowClear)
{
    SetupMemory(Memory6502::kRomStart, { 0x24, 0x40 });
    SetupMemory(0x40, { 0b10000000 });
    cpu.A = 0b11111111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(0b11111111, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());  // Bit 7 of memory is 1
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());  // Bit 6 of memory is 0
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A & memory != 0
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, BIT_BothFlagsClear)
{
    SetupMemory(Memory6502::kRomStart, { 0x24, 0x40 });
    SetupMemory(0x40, { 0b00111111 });
    cpu.A = 0b00111111;

    cpu.ExecuteInstruction();

    EXPECT_EQ(0b00111111, cpu.A);  // A should remain unchanged
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Bit 7 of memory is 0
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());  // Bit 6 of memory is 0
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A & memory != 0
    AssertPCLocation(cpu, 2);
}
