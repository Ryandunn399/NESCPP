#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, ADC_Immediate)
{
    cpu.StatusReg.SetRegister(0x00);
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
    cpu.StatusReg.SetRegister(0x00);
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
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0x69, 0x32});
    cpu.A = 0x64;

    uint8_t expectedResult = (0x64 + 0x32) & 0xFF;

    cpu.ExecuteInstruction();

    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, Carry_Flag_Persistence)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0x69, 0x01,  
                                       0x69, 0x05});
    cpu.A = 0xFF;
    
    cpu.ExecuteInstruction();
    EXPECT_EQ(0x00, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction();
    EXPECT_EQ(0x06, cpu.A);
}

TEST_F(Cpu6502Test, ADC_ZeroPage)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0x65, 0x15});
    SetupMemory(0x15, {0x34});
    cpu.A = 0x33;

    uint8_t expectedResult = 0x33 + 0x34;

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_ZeroPageX)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0x75, 0x15});
    SetupMemory(0x17, {0x34});
    cpu.X = 0x2;
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_Absolute)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0x6D, 0x34, 0x12});
    SetupMemory(0x1234, {0x34});
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ADC_AbsoluteX)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0x7D, 0x32, 0x12});
    SetupMemory(0x1234, {0x34});
    cpu.X = 0x2;
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ADC_AbsoluteY)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0x79, 0x32, 0x12});
    SetupMemory(0x1234, {0x34});
    cpu.Y = 0x2;
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ADC_IndirectX)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0x61, 0x12});
    SetupMemory(0x16, {0x00});
    SetupMemory(0x17, {0x30});
    SetupMemory(0x3000, {0x34});

    cpu.X = 0x4;
    cpu.A = 0x33;

    uint8_t expectedResult = 0x33 + 0x34;
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_IndirectY)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0x71, 0x12});
    SetupMemory(0x12, {0x00});
    SetupMemory(0x13, {0x30});
    SetupMemory(0x3004, {0x34});

    cpu.Y = 0x4;
    cpu.A = 0x33;

    uint8_t expectedResult = 0x33 + 0x34;
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Immediate_Normal)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, {0xE9, 0x30});
    cpu.A = 0x50;
    cpu.StatusReg.SetCarry(1); // No borrow
    uint8_t expectedResult = 0x50 - 0x30;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Zero_Page)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, { 0xE5, 0x42 });
    SetupMemory(0x42, { 0x5 });
    cpu.A = 0x7A;
    uint8_t expectedResult = 0x7A - 0x5 - (cpu.StatusReg.GetCarry() ? 0 : 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Zero_PageX)
{
    cpu.StatusReg.SetRegister(0x00);
    SetupMemory(Memory6502::kRomStart, { 0xF5, 0x42 });
    SetupMemory(0x42 + 0x8, { 0x5 });
    cpu.X = 0x8;
    cpu.A = 0x7A;
    uint8_t expectedResult = 0x7A - 0x5 - (cpu.StatusReg.GetCarry() ? 0 : 1);

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    AssertPCLocation(cpu, 2);
}