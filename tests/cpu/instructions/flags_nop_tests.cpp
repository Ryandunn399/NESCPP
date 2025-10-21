#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, CLCBasic)
{
    SetupMemory(Memory6502::kRomStart, { 0x18 });
    
    cpu.StatusReg.SetCarry(1);
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, CLCWhenAlreadyClear)
{
    SetupMemory(Memory6502::kRomStart, { 0x18 });
    
    cpu.StatusReg.SetCarry(0);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
}

TEST_F(Cpu6502Test, CLCDoesNotAffectOtherFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0x18 });
    
    cpu.StatusReg.SetCarry(1);
    cpu.StatusReg.SetZero(1);
    cpu.StatusReg.SetNegative(1);
    cpu.StatusReg.SetOverflow(1);
    cpu.StatusReg.SetInterruptDisable(1);
    cpu.StatusReg.SetDecimal(1);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    EXPECT_EQ(1, cpu.StatusReg.GetDecimal());
}

TEST_F(Cpu6502Test, SECBasic)
{
    SetupMemory(Memory6502::kRomStart, { 0x38 });
    
    cpu.StatusReg.SetCarry(0);
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, SECWhenAlreadySet)
{
    SetupMemory(Memory6502::kRomStart, { 0x38 });
    
    cpu.StatusReg.SetCarry(1);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
}

TEST_F(Cpu6502Test, SECDoesNotAffectOtherFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0x38 });
    
    cpu.StatusReg.SetRegister(0x00);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
}

TEST_F(Cpu6502Test, CLCSECRoundTrip)
{
    SetupMemory(Memory6502::kRomStart, { 0x38, 0x18, 0x38 });
    
    cpu.StatusReg.SetCarry(0);
    
    cpu.ExecuteInstruction(); // SEC
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // CLC
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // SEC
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
}

TEST_F(Cpu6502Test, CLDBasic)
{
    SetupMemory(Memory6502::kRomStart, { 0xD8 });
    
    cpu.StatusReg.SetDecimal(1);
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetDecimal());
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, CLDWhenAlreadyClear)
{
    SetupMemory(Memory6502::kRomStart, { 0xD8 });
    
    cpu.StatusReg.SetDecimal(0);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetDecimal());
}

TEST_F(Cpu6502Test, CLDDoesNotAffectOtherFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0xD8 });
    
    cpu.StatusReg.SetRegister(0xFF);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetDecimal());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
}

TEST_F(Cpu6502Test, SEDBasic)
{
    SetupMemory(Memory6502::kRomStart, { 0xF8 });
    
    cpu.StatusReg.SetDecimal(0);
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(1, cpu.StatusReg.GetDecimal());
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, SEDWhenAlreadySet)
{
    SetupMemory(Memory6502::kRomStart, { 0xF8 });
    
    cpu.StatusReg.SetDecimal(1);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(1, cpu.StatusReg.GetDecimal());
}

TEST_F(Cpu6502Test, SEDDoesNotAffectOtherFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0xF8 });
    
    cpu.StatusReg.SetRegister(0x00);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(1, cpu.StatusReg.GetDecimal());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
}

TEST_F(Cpu6502Test, CLIBasic)
{
    SetupMemory(Memory6502::kRomStart, { 0x58 });
    
    cpu.StatusReg.SetInterruptDisable(1);
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetInterruptDisable());
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, CLIWhenAlreadyClear)
{
    SetupMemory(Memory6502::kRomStart, { 0x58 });
    
    cpu.StatusReg.SetInterruptDisable(0);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetInterruptDisable());
}

TEST_F(Cpu6502Test, CLIDoesNotAffectOtherFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0x58 });
    
    cpu.StatusReg.SetRegister(0xFF);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetInterruptDisable());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
}


TEST_F(Cpu6502Test, SEIBasic)
{
    SetupMemory(Memory6502::kRomStart, { 0x78 });
    
    cpu.StatusReg.SetInterruptDisable(0);
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, SEIWhenAlreadySet)
{
    SetupMemory(Memory6502::kRomStart, { 0x78 });
    
    cpu.StatusReg.SetInterruptDisable(1);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
}

TEST_F(Cpu6502Test, SEIDoesNotAffectOtherFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0x78 });
    
    cpu.StatusReg.SetRegister(0x00);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
}

TEST_F(Cpu6502Test, CLVBasic)
{
    SetupMemory(Memory6502::kRomStart, { 0xB8 });
    
    cpu.StatusReg.SetOverflow(1);
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, CLVWhenAlreadyClear)
{
    SetupMemory(Memory6502::kRomStart, { 0xB8 });
    
    cpu.StatusReg.SetOverflow(0);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
}

TEST_F(Cpu6502Test, CLVDoesNotAffectOtherFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0xB8 });
    
    cpu.StatusReg.SetRegister(0xFF);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
}

TEST_F(Cpu6502Test, CLVNoSetCounterpart)
{
    // NOTE: There is no SEV instruction in the 6502!
    // Overflow flag can only be set by arithmetic operations (ADC, SBC)
    // or by pulling from stack (PLP, RTI)
    SetupMemory(Memory6502::kRomStart, { 0xB8 });
    
    cpu.StatusReg.SetOverflow(1);
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
}

TEST_F(Cpu6502Test, NOPBasic)
{
    SetupMemory(Memory6502::kRomStart, { 0xEA });
    
    uint8_t initialSP = cpu.GetStackPointer();
    cpu.A = 0x42;
    cpu.X = 0x11;
    cpu.Y = 0x22;
    cpu.StatusReg.SetRegister(0xFF);
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0x42, cpu.A);
    EXPECT_EQ(0x11, cpu.X);
    EXPECT_EQ(0x22, cpu.Y);
    EXPECT_EQ(initialSP, cpu.GetStackPointer());
    EXPECT_EQ(0xFF, cpu.StatusReg.GetRegister());
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, NOPDoesNothing)
{
    SetupMemory(Memory6502::kRomStart, { 0xEA, 0xEA, 0xEA });
    
    uint16_t initialPC = cpu.PC;
    uint8_t initialA = cpu.A;
    uint8_t initialStatus = cpu.StatusReg.GetRegister();
    
    cpu.ExecuteInstruction();
    cpu.ExecuteInstruction();
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 3);
    EXPECT_EQ(initialA, cpu.A);
    EXPECT_EQ(initialStatus, cpu.StatusReg.GetRegister());
}

TEST_F(Cpu6502Test, NOPWithAllFlagsSet)
{
    SetupMemory(Memory6502::kRomStart, { 0xEA });
    
    cpu.StatusReg.SetRegister(0xFF);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0xFF, cpu.StatusReg.GetRegister());
}

TEST_F(Cpu6502Test, NOPWithAllFlagsClear)
{
    SetupMemory(Memory6502::kRomStart, { 0xEA });
    
    cpu.StatusReg.SetRegister(0x00);
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0x00, cpu.StatusReg.GetRegister());
}

TEST_F(Cpu6502Test, AllFlagsClearSequence)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x18, // CLC
        0xD8, // CLD
        0x58, // CLI
        0xB8  // CLV
    });
    
    cpu.StatusReg.SetRegister(0xFF);
    
    cpu.ExecuteInstruction(); // CLC
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // CLD
    EXPECT_EQ(0, cpu.StatusReg.GetDecimal());
    
    cpu.ExecuteInstruction(); // CLI
    EXPECT_EQ(0, cpu.StatusReg.GetInterruptDisable());
    
    cpu.ExecuteInstruction(); // CLV
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    
    // N and Z should still be set
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
}

TEST_F(Cpu6502Test, AllFlagsSetSequence)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x38, // SEC
        0xF8, // SED
        0x78  // SEI
    });
    
    cpu.StatusReg.SetRegister(0x00);
    
    cpu.ExecuteInstruction(); // SEC
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // SED
    EXPECT_EQ(1, cpu.StatusReg.GetDecimal());
    
    cpu.ExecuteInstruction(); // SEI
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    
    // Other flags should still be clear
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
}

TEST_F(Cpu6502Test, FlagToggling)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x38, // SEC
        0x18, // CLC
        0x38, // SEC
        0xF8, // SED
        0xD8, // CLD
        0x78, // SEI
        0x58  // CLI
    });
    
    cpu.StatusReg.SetRegister(0x00);
    
    cpu.ExecuteInstruction(); // SEC
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // CLC
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // SEC
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // SED
    EXPECT_EQ(1, cpu.StatusReg.GetDecimal());
    
    cpu.ExecuteInstruction(); // CLD
    EXPECT_EQ(0, cpu.StatusReg.GetDecimal());
    
    cpu.ExecuteInstruction(); // SEI
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    
    cpu.ExecuteInstruction(); // CLI
    EXPECT_EQ(0, cpu.StatusReg.GetInterruptDisable());
}

TEST_F(Cpu6502Test, NOPBetweenFlagOperations)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x38, // SEC
        0xEA, // NOP
        0xEA, // NOP
        0x18  // CLC
    });
    
    cpu.StatusReg.SetCarry(0);
    
    cpu.ExecuteInstruction(); // SEC
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // NOP
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // NOP
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    
    cpu.ExecuteInstruction(); // CLC
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
}

TEST_F(Cpu6502Test, InterruptDisableCommonPattern)
{
    // Common pattern: SEI at start, CLI at end
    SetupMemory(Memory6502::kRomStart, { 
        0x78, // SEI - disable interrupts
        0xEA, // NOP - do some work
        0xEA, // NOP
        0x58  // CLI - re-enable interrupts
    });
    
    cpu.StatusReg.SetInterruptDisable(0);
    
    cpu.ExecuteInstruction(); // SEI
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    
    cpu.ExecuteInstruction(); // NOP
    cpu.ExecuteInstruction(); // NOP
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    
    cpu.ExecuteInstruction(); // CLI
    EXPECT_EQ(0, cpu.StatusReg.GetInterruptDisable());
}

TEST_F(Cpu6502Test, DecimalModePattern)
{
    // Common pattern for BCD arithmetic
    SetupMemory(Memory6502::kRomStart, { 
        0xF8, // SED - set decimal mode
        0xEA, // NOP - BCD operations would go here
        0xD8  // CLD - clear decimal mode
    });
    
    cpu.StatusReg.SetDecimal(0);
    
    cpu.ExecuteInstruction(); // SED
    EXPECT_EQ(1, cpu.StatusReg.GetDecimal());
    
    cpu.ExecuteInstruction(); // NOP
    EXPECT_EQ(1, cpu.StatusReg.GetDecimal());
    
    cpu.ExecuteInstruction(); // CLD
    EXPECT_EQ(0, cpu.StatusReg.GetDecimal());
}