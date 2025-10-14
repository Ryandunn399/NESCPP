#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, CMP_Immediate)
{
    SetupMemory(Memory6502::kRomStart, { 0xC9, 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(2, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CMP_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0xC5, 0x40 });
    SetupMemory(0x40, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(3, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CMP_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0xD5, 0x40 });
    cpu.X = 0x10;
    SetupMemory(0x50, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CMP_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0xCD, 0xC4, 0xA3 });
    SetupMemory(0xA3C4, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, CMP_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0xDD, 0xB4, 0xA3 });
    cpu.X = 0x10;
    SetupMemory(0xA3C4, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, CMP_AbsoluteXPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0xDD, 0xB4, 0xA3 });
    cpu.X = 0xFF;
    SetupMemory(0xA3B4 + 0xFF, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(5, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, CMP_AbsoluteY)
{
    SetupMemory(Memory6502::kRomStart, { 0xD9, 0xB4, 0xA3 });
    cpu.Y = 0x10;
    SetupMemory(0xA3C4, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, CMP_AbsoluteYPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0xD9, 0xB4, 0xA3 });
    cpu.Y = 0xFF;
    SetupMemory(0xA3B4 + 0xFF, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(5, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, CMP_IndirectX)
{
    SetupMemory(Memory6502::kRomStart, { 0xC1, 0x30 });
    cpu.X = 0x10;
    SetupMemory(0x40, { 0xC4, 0xA3 });  // Low and high byte of address
    SetupMemory(0xA3C4, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(6, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CMP_IndirectY)
{
    SetupMemory(Memory6502::kRomStart, { 0xD1, 0x40 });
    cpu.Y = 0x10;
    SetupMemory(0x40, { 0xB4, 0xA3 });  // Low and high byte of address
    SetupMemory(0xA3C4, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(5, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CMP_IndirectYPageCross)
{
    SetupMemory(Memory6502::kRomStart, { 0xD1, 0x40 });
    cpu.Y = 0xFF;
    SetupMemory(0x40, { 0xB4, 0xA3 });  // Low and high byte of address
    SetupMemory(0xA3B4 + 0xFF, { 0x50 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(6, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CMP_Equal)
{
    SetupMemory(Memory6502::kRomStart, { 0xC9, 0x80 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(1, cpu.StatusReg.GetZero());      // A == memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result is 0
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CMP_LessThan)
{
    SetupMemory(Memory6502::kRomStart, { 0xC9, 0x80 });
    cpu.A = 0x50;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x50, cpu.A);  // A should remain unchanged
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());     // A < memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());  // Result bit 7 is 1
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CMP_GreaterThan)
{
    SetupMemory(Memory6502::kRomStart, { 0xC9, 0x30 });
    cpu.A = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.A);  // A should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // A >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // A != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPX_Immediate)
{
    SetupMemory(Memory6502::kRomStart, { 0xE0, 0x50 });
    cpu.X = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.X);  // X should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // X >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // X != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(2, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPX_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0xE4, 0x40 });
    SetupMemory(0x40, { 0x50 });
    cpu.X = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.X);  // X should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // X >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // X != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(3, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPX_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0xEC, 0xC4, 0xA3 });
    SetupMemory(0xA3C4, { 0x50 });
    cpu.X = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.X);  // X should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // X >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // X != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, CPX_Equal)
{
    SetupMemory(Memory6502::kRomStart, { 0xE0, 0x80 });
    cpu.X = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.X);  // X should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // X >= memory
    EXPECT_EQ(1, cpu.StatusReg.GetZero());      // X == memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result is 0
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPX_LessThan)
{
    SetupMemory(Memory6502::kRomStart, { 0xE0, 0x80 });
    cpu.X = 0x50;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x50, cpu.X);  // X should remain unchanged
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());     // X < memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // X != memory
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());  // Result bit 7 is 1
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPX_GreaterThan)
{
    SetupMemory(Memory6502::kRomStart, { 0xE0, 0x30 });
    cpu.X = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.X);  // X should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // X >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // X != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPY_Immediate)
{
    SetupMemory(Memory6502::kRomStart, { 0xC0, 0x50 });
    cpu.Y = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.Y);  // Y should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // Y >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // Y != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(2, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPY_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0xC4, 0x40 });
    SetupMemory(0x40, { 0x50 });
    cpu.Y = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.Y);  // Y should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // Y >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // Y != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(3, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPY_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0xCC, 0xC4, 0xA3 });
    SetupMemory(0xA3C4, { 0x50 });
    cpu.Y = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.Y);  // Y should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // Y >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // Y != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, CPY_Equal)
{
    SetupMemory(Memory6502::kRomStart, { 0xC0, 0x80 });
    cpu.Y = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.Y);  // Y should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // Y >= memory
    EXPECT_EQ(1, cpu.StatusReg.GetZero());      // Y == memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result is 0
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPY_LessThan)
{
    SetupMemory(Memory6502::kRomStart, { 0xC0, 0x80 });
    cpu.Y = 0x50;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x50, cpu.Y);  // Y should remain unchanged
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());     // Y < memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // Y != memory
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());  // Result bit 7 is 1
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, CPY_GreaterThan)
{
    SetupMemory(Memory6502::kRomStart, { 0xC0, 0x30 });
    cpu.Y = 0x80;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x80, cpu.Y);  // Y should remain unchanged
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());     // Y >= memory
    EXPECT_EQ(0, cpu.StatusReg.GetZero());      // Y != memory
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());  // Result bit 7 is 0
    AssertPCLocation(cpu, 2);
}