#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, ADC_Immediate)
{
    SetupMemory(Memory6502::kRomStart, {0x69, 0x23});
    cpu.A = 0x8A;

    uint8_t expectedResult = (0x8A + 0x23) & 0xFF;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(2, cycles);  // ADC Immediate = 2 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_Immediate_Carry)
{
    SetupMemory(Memory6502::kRomStart, {0x69, 0x5});
    cpu.A = 0xFF;

    uint8_t expectedResult = (0xFF + 0x5) & 0xFF;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(2, cycles);  // ADC Immediate = 2 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_Immediate_Overflow)
{
    SetupMemory(Memory6502::kRomStart, {0x69, 0x32});
    cpu.A = 0x64;

    uint8_t expectedResult = (0x64 + 0x32) & 0xFF;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(2, cycles);  // ADC Immediate = 2 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, Carry_Flag_Persistence)
{
    SetupMemory(Memory6502::kRomStart, {0x69, 0x01,  
                                       0x69, 0x05});
    cpu.A = 0xFF;
    
    uint8_t cycles1 = cpu.ExecuteInstruction();
    EXPECT_EQ(0x00, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(2, cycles1);
    
    uint8_t cycles2 = cpu.ExecuteInstruction();
    EXPECT_EQ(0x06, cpu.A);
    EXPECT_EQ(2, cycles2);
}

TEST_F(Cpu6502Test, ADC_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, {0x65, 0x15});
    SetupMemory(0x15, {0x34});
    cpu.A = 0x33;

    uint8_t expectedResult = 0x33 + 0x34;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(3, cycles);  // ADC Zero Page = 3 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, {0x75, 0x15});
    SetupMemory(0x17, {0x34});
    cpu.X = 0x2;
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(4, cycles);  // ADC Zero Page,X = 4 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_Absolute)
{
    SetupMemory(Memory6502::kRomStart, {0x6D, 0x34, 0x12});
    SetupMemory(0x1234, {0x34});
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(4, cycles);  // ADC Absolute = 4 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ADC_AbsoluteX_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, {0x7D, 0x32, 0x12});
    SetupMemory(0x1234, {0x34});
    cpu.X = 0x2;  // 0x1232 + 0x02 = 0x1234 (no page cross)
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(4, cycles);  // ADC Absolute,X = 4 cycles (no page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ADC_AbsoluteX_PageCross)
{
    SetupMemory(Memory6502::kRomStart, {0x7D, 0xFF, 0x12});
    SetupMemory(0x1305, {0x34});  // 0x12FF + 0x06 = 0x1305
    cpu.X = 0x6;  // Crosses page boundary
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(5, cycles);  // ADC Absolute,X = 5 cycles (page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ADC_AbsoluteY_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, {0x79, 0x32, 0x12});
    SetupMemory(0x1234, {0x34});
    cpu.Y = 0x2;  // No page cross
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(4, cycles);  // ADC Absolute,Y = 4 cycles (no page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, ADC_AbsoluteY_PageCross)
{
    SetupMemory(Memory6502::kRomStart, {0x79, 0xFE, 0x12});
    SetupMemory(0x1308, {0x34});  // 0x12FE + 0x0A = 0x1308
    cpu.Y = 0xA;  // Crosses page boundary
    cpu.A = 0x33;
    
    uint8_t expectedResult = 0x33 + 0x34;
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(5, cycles);  // ADC Absolute,Y = 5 cycles (page cross)
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
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(6, cycles);  // ADC Indirect,X = 6 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_IndirectY_NoPageCross)
{
    SetupMemory(Memory6502::kRomStart, {0x71, 0x12});
    SetupMemory(0x12, {0x00});
    SetupMemory(0x13, {0x30});
    SetupMemory(0x3004, {0x34});

    cpu.Y = 0x4;  // No page cross
    cpu.A = 0x33;

    uint8_t expectedResult = 0x33 + 0x34;
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(5, cycles);  // ADC Indirect,Y = 5 cycles (no page cross)
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, ADC_IndirectY_PageCross)
{
    SetupMemory(Memory6502::kRomStart, {0x71, 0x12});
    SetupMemory(0x12, {0xFE});
    SetupMemory(0x13, {0x30});
    SetupMemory(0x3108, {0x34});  // 0x30FE + 0x0A = 0x3108

    cpu.Y = 0xA;  // Crosses page boundary
    cpu.A = 0x33;

    uint8_t expectedResult = 0x33 + 0x34;
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(6, cycles);  // ADC Indirect,Y = 6 cycles (page cross)
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Immediate_Normal)
{
    SetupMemory(Memory6502::kRomStart, {0xE9, 0x30});
    cpu.A = 0x50;
    cpu.StatusReg.SetCarry(1); // No borrow
    uint8_t expectedResult = 0x50 - 0x30;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(2, cycles);  // SBC Immediate = 2 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Zero_Page)
{
    SetupMemory(Memory6502::kRomStart, { 0xE5, 0x42 });
    SetupMemory(0x42, { 0x5 });
    cpu.A = 0x7A;
    uint8_t expectedResult = 0x7A - 0x5 - GetSbcCarry();

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(3, cycles);  // SBC Zero Page = 3 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Zero_PageX)
{
    SetupMemory(Memory6502::kRomStart, { 0xF5, 0x42 });
    SetupMemory(0x42 + 0x8, { 0x5 });
    cpu.X = 0x8;
    cpu.A = 0x7A;
    uint8_t expectedResult = 0x7A - 0x5 - GetSbcCarry();

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(4, cycles);  // SBC Zero Page,X = 4 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0xED, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x5 });
    cpu.A = 0x3D;
    uint8_t expectedResult = 0x3D - 0x5 - GetSbcCarry();

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(4, cycles);  // SBC Absolute = 4 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, SBC_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0xFD, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x1B, { 0x9 });
    cpu.X = 0x1B;
    cpu.A = 0xAA;
    uint8_t expectedResult = 0xAA - 0x9 - GetSbcCarry();
    
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(4, cycles);  // SBC Absolute,X = 4 cycles (no page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, SBC_AbsoluteY)
{
    SetupMemory(Memory6502::kRomStart, { 0xF9, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x83, { 0x9 });
    cpu.Y = 0x83;
    cpu.A = 0x72;
    uint8_t expectedResult = 0x72 - 0x9 - GetSbcCarry();
    
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(4, cycles);  // SBC Absolute,Y = 4 cycles (no page cross)
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

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(6, cycles);  // SBC Indirect,X = 6 cycles
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

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry()); // No underflow
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow()); // No overflow
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(5, cycles);  // SBC Indirect,Y = 5 cycles (no page cross)
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, SBC_Absolute_AllFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0xED, 0x00, 0x20 });
    SetupMemory(0x2000, { 0x50 });
    
    cpu.A = 0x50;
    cpu.StatusReg.SetCarry(1);
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x00, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(4, cycles);  // SBC Absolute = 4 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, SBC_Absolute_NegativeAndOverflow)
{
    SetupMemory(Memory6502::kRomStart, { 0xED, 0x00, 0x20 });
    SetupMemory(0x2000, { 0x01 });
    
    cpu.A = 0x80;
    uint8_t expectedResult = 0x80 - 0x1 - GetSbcCarry();
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);  // SBC Absolute = 4 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, SBC_Absolute_NegativeAndBorrow)
{
    SetupMemory(Memory6502::kRomStart, { 0xED, 0x00, 0x20 });
    SetupMemory(0x2000, { 0x10 });
    
    cpu.A = 0x05;
    uint8_t expectedResult = 0x05 - 0x10 - GetSbcCarry();
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(expectedResult, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(4, cycles);  // SBC Absolute = 4 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, INC_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0xE6, 0x60 });
    SetupMemory(0x60, { 0x5A });
    uint8_t expectedResult = 0x5A + 1;
    
    uint8_t cycles = cpu.ExecuteInstruction();

    int memoryValue = memory.ReadByte(0x60);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // INC Zero Page = 5 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, INC_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0xF6, 0x30 });
    SetupMemory(0x30 + 0x40, { 0x11 });
    cpu.X = 0x40;
    uint8_t expectedResult = 0x11 + 1;

    uint8_t cycles = cpu.ExecuteInstruction();

    int memoryValue = memory.ReadByte(0x40 + 0x30);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // INC Zero Page,X = 6 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, INC_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0xEE, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x21 });
    uint8_t expectedResult = 0x21 + 1;

    uint8_t cycles = cpu.ExecuteInstruction();

    int memoryValue = memory.ReadWord(0x1234);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // INC Absolute = 6 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, INC_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0xFE, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x3, { 0x12 });
    cpu.X = 0x3;
    uint8_t expectedResult = 0x12 + 1;
    
    uint8_t cycles = cpu.ExecuteInstruction();

    int memoryValue = memory.ReadWord(0x1234 + 0x3);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(7, cycles);  // INC Absolute,X = 7 cycles (always, even without page cross)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, DEC_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0xC6, 0x60 });
    SetupMemory(0x60, { 0x5A });
    uint8_t expectedResult = 0x5A - 1;
    
    uint8_t cycles = cpu.ExecuteInstruction();

    int memoryValue = memory.ReadByte(0x60);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(5, cycles);  // DEC Zero Page = 5 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, DEC_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, { 0xD6, 0x30 });
    SetupMemory(0x30 + 0x40, { 0x11 });
    cpu.X = 0x40;
    uint8_t expectedResult = 0x11 - 1;

    uint8_t cycles = cpu.ExecuteInstruction();

    int memoryValue = memory.ReadByte(0x40 + 0x30);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // DEC Zero Page,X = 6 cycles
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, DEC_Absolute)
{
    SetupMemory(Memory6502::kRomStart, { 0xCE, 0x34, 0x12 });
    SetupMemory(0x1234, { 0x21 });
    uint8_t expectedResult = 0x21 - 1;

    uint8_t cycles = cpu.ExecuteInstruction();

    int memoryValue = memory.ReadWord(0x1234);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(6, cycles);  // DEC Absolute = 6 cycles
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, DEC_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, { 0xDE, 0x34, 0x12 });
    SetupMemory(0x1234 + 0x3, { 0x12 });
    cpu.X = 0x3;
    uint8_t expectedResult = 0x12 - 1;
    
    uint8_t cycles = cpu.ExecuteInstruction();

    int memoryValue = memory.ReadWord(0x1234 + 0x3);
    EXPECT_EQ(expectedResult, memoryValue);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(7, cycles);  // DEC Absolute,X = 7 cycles (always)
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, INX)
{
    SetupMemory(Memory6502::kRomStart, { 0xE8 });
    cpu.X = 0x32;
    uint8_t expectedResult = 0x32 + 1;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.X);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(2, cycles); 
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, DEX)
{
    SetupMemory(Memory6502::kRomStart, { 0xCA });
    cpu.X = 0x32;
    uint8_t expectedResult = 0x32 - 1;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.X);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(2, cycles);
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, INY)
{
    SetupMemory(Memory6502::kRomStart, { 0xC8 });
    cpu.Y = 0x32;
    uint8_t expectedResult = 0x32 + 1;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.Y);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(2, cycles);
    AssertPCLocation(cpu, 1);
}

TEST_F(Cpu6502Test, DEY)
{
    SetupMemory(Memory6502::kRomStart, { 0x88 });
    cpu.Y = 0x32;
    uint8_t expectedResult = 0x32 - 1;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(expectedResult, cpu.Y);
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(2, cycles);
    AssertPCLocation(cpu, 1);
}
