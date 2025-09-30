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

TEST_F(Cpu6502Test, Carry_Flag_Persistence)
{
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
    SetupMemory(Memory6502::kRomStart, { 0xE5, 0x42 });
    SetupMemory(0x42, { 0x5 });
    cpu.A = 0x7A;
    uint8_t expectedResult = 0x7A - 0x5 - GetSbcCarry();

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
    SetupMemory(Memory6502::kRomStart, { 0xF5, 0x42 });
    SetupMemory(0x42 + 0x8, { 0x5 });
    cpu.X = 0x8;
    cpu.A = 0x7A;
    uint8_t expectedResult = 0x7A - 0x5 - GetSbcCarry();

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0xED, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x5 });
    cpu.A = 0x3D;
    uint8_t expectedResult = 0x3D - 0x5 - GetSbcCarry();

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, SBC_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0xFD, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x1B, { 0x9 });
    cpu.X = 0x1B;
    cpu.A = 0xAA;
    uint8_t expectedResult = 0xAA - 0x9 - GetSbcCarry();
    
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, SBC_AbsoluteY)
{
    SetupMemory(Memory6502::kRomStart, { 0xF9, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x83, { 0x9 });
    cpu.Y = 0x83;
    cpu.A = 0x72;
    uint8_t expectedResult = 0x72 - 0x9 - GetSbcCarry();
    
    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, SBC_IndirectX)
{
    SetupMemory(Memory6502::kRomStart, { 0xE1, 0x40 });
    SetupMemory(0x40 + 0x4, { 0x34, 0x12 });
    SetupMemory(0x1234, { 0x11 });

    cpu.X = 0x4;
    cpu.A = 0x4C;
    uint8_t expectedResult = 0x4C - 0x11 - GetSbcCarry();

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_IndirectY)
{
    SetupMemory(Memory6502::kRomStart, { 0xF1, 0x40 });
    SetupMemory(0x40, { 0x45, 0x34 });
    SetupMemory(0x3445 + 0x11, { 0x3 });

    cpu.Y = 0x11;
    cpu.A = 0x4C;
    uint8_t expectedResult = 0x4C - 0x3 - GetSbcCarry();

    cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Absolute_AllFlags)
{
    // SBC Absolute: opcode 0xED
    SetupMemory(Memory6502::kRomStart, { 0xED, 0x00, 0x20 }); // SBC $2000
    SetupMemory(0x2000, { 0x50 }); // Operand at $2000
    
    cpu.A = 0x50;        // A = 0x50 (80 in decimal, positive in signed)
    cpu.StatusReg.SetCarry(1); // Set carry (no borrow)
    
    // 0x50 - 0x50 = 0x00
    // This should trigger:
    // Z = 1 (result is zero)
    // C = 1 (no borrow needed, A >= M)
    // N = 0 (bit 7 is 0)
    // V = 0 (no signed overflow)
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x00, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetZero());     // Result is zero
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());    // No borrow
    EXPECT_EQ(0, cpu.StatusReg.GetNegative()); // Positive result
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, SBC_Absolute_NegativeAndOverflow)
{
    // Test that triggers Negative and Overflow flags
    SetupMemory(Memory6502::kRomStart, { 0xED, 0x00, 0x20 }); // SBC $2000
    SetupMemory(0x2000, { 0x01 }); // Subtract 1
    
    cpu.A = 0x80;        // A = 0x80 (-128 in signed two's complement)
    uint8_t expectedResult = 0x80 - 0x1 - GetSbcCarry();
    
    // 0x80 - 0x01 = 0x7F
    // In signed terms: -128 - 1 = -129, but result is +127 (overflow!)
    // This should trigger:
    // V = 1 (signed overflow: negative - positive = positive)
    // N = 0 (bit 7 is 0, result is 0x7F)
    // C = 1 (no borrow, 0x80 >= 0x01)
    // Z = 0 (result is not zero)
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow()); // Signed overflow occurred
    EXPECT_EQ(0, cpu.StatusReg.GetNegative()); // Result is positive
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());    // No borrow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());     // Not zero
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, SBC_Absolute_NegativeAndBorrow)
{
    // Test that triggers Negative flag and borrow (C=0)
    SetupMemory(Memory6502::kRomStart, { 0xED, 0x00, 0x20 }); // SBC $2000
    SetupMemory(0x2000, { 0x10 }); // Subtract 0x10
    
    cpu.A = 0x05;        // A = 0x05
    uint8_t expectedResult = 0x05 - 0x10 - GetSbcCarry();
    
    // 0x05 - 0x10 = 0xF5 (wraps around, -11 in signed)
    // This should trigger:
    // N = 1 (bit 7 is 1, result is 0xF5)
    // C = 0 (borrow occurred, A < M)
    // Z = 0 (result is not zero)
    // V = 0 (no signed overflow)
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative()); // Negative result
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());    // Borrow occurred
    EXPECT_EQ(0, cpu.StatusReg.GetZero());     // Not zero
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, INC_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0xE6, 0x60 });
    SetupMemory(0x60, { 0x5A });
    uint8_t expectedResult = 0x5A + 1;
    
    cpu.ExecuteInstruction();

    int memoryValue = memory.ReadByte(0x60);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, INC_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0xF6, 0x30 });
    SetupMemory(0x30 + 0x40, { 0x11 });
    cpu.X = 0x40;
    uint8_t expectedResult = 0x11 + 1;

    cpu.ExecuteInstruction();

    int memoryValue = memory.ReadByte(0x40 + 0x30);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, INC_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0xEE, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x21 });
    uint8_t expectedResult = 0x21 + 1;

    cpu.ExecuteInstruction();

    int memoryValue = memory.ReadWord(0x1234);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, INC_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0xFE, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x3, { 0x12 });
    cpu.X = 0x3;
    uint8_t expectedResult = 0x12 + 1;
    
    cpu.ExecuteInstruction();

    int memoryValue = memory.ReadWord(0x1234 + 0x3);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, DEC_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0xC6, 0x60 });
    SetupMemory(0x60, { 0x5A });
    uint8_t expectedResult = 0x5A - 1;
    
    cpu.ExecuteInstruction();

    int memoryValue = memory.ReadByte(0x60);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, DEC_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0xD6, 0x30 });
    SetupMemory(0x30 + 0x40, { 0x11 });
    cpu.X = 0x40;
    uint8_t expectedResult = 0x11 - 1;

    cpu.ExecuteInstruction();

    int memoryValue = memory.ReadByte(0x40 + 0x30);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, DEC_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0xCE, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x21 });
    uint8_t expectedResult = 0x21 - 1;

    cpu.ExecuteInstruction();

    int memoryValue = memory.ReadWord(0x1234);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, DEC_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0xDE, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x3, { 0x12 });
    cpu.X = 0x3;
    uint8_t expectedResult = 0x12 - 1;
    
    cpu.ExecuteInstruction();

    int memoryValue = memory.ReadWord(0x1234 + 0x3);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    AssertPCLocation(cpu, 3);
}