#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

// ============================================================================
// PHA - Push Accumulator
// ============================================================================

TEST_F(Cpu6502Test, PHABasic)
{
    SetupMemory(Memory6502::kRomStart, { 0x48 });
    
    cpu.A = 0x42;
    uint8_t initialSP = cpu.GetStackPointer();
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(initialSP - 1, cpu.GetStackPointer());
    EXPECT_EQ(0x42, memory.ReadByte(0x0100 + initialSP));
    EXPECT_EQ(0x42, cpu.A);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, PHAZeroValue)
{
    SetupMemory(Memory6502::kRomStart, { 0x48 });
    
    cpu.A = 0x00;
    uint8_t initialSP = cpu.GetStackPointer();
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0x00, memory.ReadByte(0x0100 + initialSP));
    EXPECT_EQ(initialSP - 1, cpu.GetStackPointer());
}

TEST_F(Cpu6502Test, PHANegativeValue)
{
    SetupMemory(Memory6502::kRomStart, { 0x48 });
    
    cpu.A = 0xFF;
    uint8_t initialSP = cpu.GetStackPointer();
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(0xFF, memory.ReadByte(0x0100 + initialSP));
    EXPECT_EQ(initialSP - 1, cpu.GetStackPointer());
}

TEST_F(Cpu6502Test, PHADoesNotAffectFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0x48 });
    
    cpu.StatusReg.SetNegative(1);
    cpu.StatusReg.SetZero(1);
    cpu.StatusReg.SetCarry(1);
    
    uint8_t statusBefore = cpu.StatusReg.GetRegister();
    cpu.A = 0x42;
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(statusBefore, cpu.StatusReg.GetRegister());
}

// ============================================================================
// PLA - Pull Accumulator
// ============================================================================

TEST_F(Cpu6502Test, PLABasic)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x48, // PHA - push value onto stack first
        0x68  // PLA - pull it back
    });
    
    cpu.A = 0x42;
    uint8_t initialSP = cpu.GetStackPointer();
    
    // Push
    cpu.ExecuteInstruction();
    EXPECT_EQ(initialSP - 1, cpu.GetStackPointer());
    
    // Change A to verify PLA works
    cpu.A = 0x00;
    
    // Pull
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 2);
    EXPECT_EQ(0x42, cpu.A);
    EXPECT_EQ(initialSP, cpu.GetStackPointer());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, PLAZeroSetsZeroFlag)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x48, // PHA
        0x68  // PLA
    });
    
    cpu.A = 0x00;
    uint8_t initialSP = cpu.GetStackPointer();
    
    cpu.ExecuteInstruction(); // Push 0x00
    cpu.A = 0xFF; // Change to something else
    cpu.ExecuteInstruction(); // Pull 0x00
    
    AssertPCLocation(cpu, 2);
    EXPECT_EQ(0x00, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(initialSP, cpu.GetStackPointer());
}

TEST_F(Cpu6502Test, PLANegativeSetsNegativeFlag)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x48, // PHA
        0x68  // PLA
    });
    
    cpu.A = 0x80;
    
    cpu.ExecuteInstruction(); // Push
    cpu.A = 0x00; // Change
    cpu.ExecuteInstruction(); // Pull
    
    AssertPCLocation(cpu, 2);
    EXPECT_EQ(0x80, cpu.A);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
}

TEST_F(Cpu6502Test, PLAClearsZeroFlag)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x48, // PHA
        0x68  // PLA
    });
    
    cpu.A = 0x01;
    cpu.ExecuteInstruction(); // Push
    
    cpu.StatusReg.SetZero(1);
    cpu.A = 0x00;
    cpu.ExecuteInstruction(); // Pull
    
    AssertPCLocation(cpu, 2);
    EXPECT_EQ(0x01, cpu.A);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
}

TEST_F(Cpu6502Test, PHAPLARoundTrip)
{
    SetupMemory(Memory6502::kRomStart, { 0x48, 0x68 });
    
    cpu.A = 0x99;
    uint8_t initialSP = cpu.GetStackPointer();
    
    // Push
    cpu.ExecuteInstruction();
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(initialSP - 1, cpu.GetStackPointer());
    
    // Change A
    cpu.A = 0x00;
    
    // Pull
    cpu.ExecuteInstruction();
    AssertPCLocation(cpu, 2);
    EXPECT_EQ(0x99, cpu.A);
    EXPECT_EQ(initialSP, cpu.GetStackPointer());
}

// ============================================================================
// PHP - Push Processor Status
// ============================================================================

TEST_F(Cpu6502Test, PHPBasic)
{
    SetupMemory(Memory6502::kRomStart, { 0x08 });
    
    cpu.StatusReg.SetCarry(1);
    cpu.StatusReg.SetZero(1);
    uint8_t initialSP = cpu.GetStackPointer();
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(initialSP - 1, cpu.GetStackPointer());
    
    uint8_t pushedStatus = memory.ReadByte(0x0100 + initialSP);
    // Check B and Unused flags are set (bits 4 and 5)
    EXPECT_EQ(1, (pushedStatus >> 4) & 1);
    EXPECT_EQ(1, (pushedStatus >> 5) & 1);
    // Check Carry and Zero were preserved
    EXPECT_EQ(1, pushedStatus & 0x01);
    EXPECT_EQ(1, (pushedStatus >> 1) & 1);
    
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, PHPAlwaysSetsBreakAndUnused)
{
    SetupMemory(Memory6502::kRomStart, { 0x08 });
    
    // Clear all flags
    cpu.StatusReg.SetRegister(0x00);
    uint8_t initialSP = cpu.GetStackPointer();
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    
    uint8_t pushedStatus = memory.ReadByte(0x0100 + initialSP);
    // B and Unused should be set even though status was 0
    EXPECT_EQ(0b00110000, pushedStatus & 0b00110000);
}

TEST_F(Cpu6502Test, PHPWithAllFlagsSet)
{
    SetupMemory(Memory6502::kRomStart, { 0x08 });
    
    cpu.StatusReg.SetRegister(0xFF);
    uint8_t initialSP = cpu.GetStackPointer();
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    
    uint8_t pushedStatus = memory.ReadByte(0x0100 + initialSP);
    EXPECT_EQ(0xFF, pushedStatus);
}

TEST_F(Cpu6502Test, PHPDoesNotModifyStatusRegister)
{
    SetupMemory(Memory6502::kRomStart, { 0x08 });
    
    cpu.StatusReg.SetCarry(1);
    cpu.StatusReg.SetZero(1);
    uint8_t statusBefore = cpu.StatusReg.GetRegister();
    
    cpu.ExecuteInstruction();
    
    AssertPCLocation(cpu, 1);
    EXPECT_EQ(statusBefore, cpu.StatusReg.GetRegister());
}

// ============================================================================
// PLP - Pull Processor Status
// ============================================================================

TEST_F(Cpu6502Test, PLPBasic)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x08, // PHP - push current status
        0x28  // PLP - pull it back
    });
    
    // Set up some flags
    cpu.StatusReg.SetNegative(1);
    cpu.StatusReg.SetOverflow(1);
    cpu.StatusReg.SetCarry(1);
    cpu.StatusReg.SetZero(1);
    uint8_t initialSP = cpu.GetStackPointer();
    
    cpu.ExecuteInstruction(); // PHP
    
    // Clear all flags
    cpu.StatusReg.SetRegister(0x00);
    
    uint8_t cycles = cpu.ExecuteInstruction(); // PLP
    
    AssertPCLocation(cpu, 2);
    EXPECT_EQ(initialSP, cpu.GetStackPointer());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, PLPIgnoresBreakAndUnusedBits)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x08, // PHP - will push with B and Unused set
        0x28  // PLP
    });
    
    // Start with B and Unused clear in status register
    cpu.StatusReg.SetRegister(0b00000000);
    uint8_t initialBandUnused = cpu.StatusReg.GetRegister() & 0b00110000;
    
    cpu.ExecuteInstruction(); // PHP - pushes with B and Unused set
    cpu.ExecuteInstruction(); // PLP
    
    AssertPCLocation(cpu, 2);
    // Bits 4 and 5 should remain as they were before PLP
    uint8_t currentStatus = cpu.StatusReg.GetRegister();
    EXPECT_EQ(initialBandUnused, currentStatus & 0b00110000);
}

TEST_F(Cpu6502Test, PLPClearsAllFlags)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x08, // PHP
        0x28  // PLP
    });
    
    // Push status with all flags clear
    cpu.StatusReg.SetRegister(0x00);
    cpu.ExecuteInstruction(); // PHP
    
    // Set all flags
    cpu.StatusReg.SetRegister(0xFF);
    
    cpu.ExecuteInstruction(); // PLP
    
    AssertPCLocation(cpu, 2);
    // All flags except B and Unused should be cleared
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetInterruptDisable());
    EXPECT_EQ(0, cpu.StatusReg.GetDecimal());
    EXPECT_EQ(0, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
}

TEST_F(Cpu6502Test, PHPPLPRoundTrip)
{
    SetupMemory(Memory6502::kRomStart, { 0x08, 0x28 });
    
    cpu.StatusReg.SetCarry(1);
    cpu.StatusReg.SetOverflow(1);
    cpu.StatusReg.SetNegative(1);
    uint8_t initialSP = cpu.GetStackPointer();
    
    // Push
    cpu.ExecuteInstruction();
    AssertPCLocation(cpu, 1);
    
    // Clear status
    cpu.StatusReg.SetRegister(0x00);
    
    // Pull
    cpu.ExecuteInstruction();
    AssertPCLocation(cpu, 2);
    
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(initialSP, cpu.GetStackPointer());
}

// ============================================================================
// TSX - Transfer Stack Pointer to X
// ============================================================================

TEST_F(Cpu6502Test, TSXBasic)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0x42, // LDX #$42
        0x9A,       // TXS
        0xBA        // TSX
    });
    
    cpu.ExecuteInstruction(); // LDX - X = 0x42
    cpu.ExecuteInstruction(); // TXS - SP = 0x42
    
    cpu.X = 0x00; // Clear X to verify TSX works
    uint8_t cycles = cpu.ExecuteInstruction(); // TSX
    
    AssertPCLocation(cpu, 4);
    EXPECT_EQ(0x42, cpu.X);
    EXPECT_EQ(0x42, cpu.GetStackPointer());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, TSXZeroSetsZeroFlag)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0x00, // LDX #$00
        0x9A,       // TXS
        0xBA        // TSX
    });
    
    cpu.ExecuteInstruction(); // LDX
    cpu.ExecuteInstruction(); // TXS - SP = 0x00
    cpu.ExecuteInstruction(); // TSX
    
    AssertPCLocation(cpu, 4);
    EXPECT_EQ(0x00, cpu.X);
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
}

TEST_F(Cpu6502Test, TSXNegativeSetsNegativeFlag)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0x80, // LDX #$80
        0x9A,       // TXS
        0xBA        // TSX
    });
    
    cpu.ExecuteInstruction(); // LDX
    cpu.ExecuteInstruction(); // TXS - SP = 0x80
    cpu.ExecuteInstruction(); // TSX
    
    AssertPCLocation(cpu, 4);
    EXPECT_EQ(0x80, cpu.X);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
}

TEST_F(Cpu6502Test, TSXClearsFlags)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0x42, // LDX #$42
        0x9A,       // TXS
        0xBA        // TSX
    });
    
    cpu.StatusReg.SetZero(1);
    cpu.StatusReg.SetNegative(1);
    
    cpu.ExecuteInstruction(); // LDX
    cpu.ExecuteInstruction(); // TXS
    cpu.ExecuteInstruction(); // TSX
    
    AssertPCLocation(cpu, 4);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
}

// ============================================================================
// TXS - Transfer X to Stack Pointer
// ============================================================================

TEST_F(Cpu6502Test, TXSBasic)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0x42, // LDX #$42
        0x9A        // TXS
    });
    
    cpu.ExecuteInstruction(); // LDX
    
    uint8_t cycles = cpu.ExecuteInstruction(); // TXS
    
    AssertPCLocation(cpu, 3);
    EXPECT_EQ(0x42, cpu.GetStackPointer());
    EXPECT_EQ(0x42, cpu.X);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, TXSDoesNotAffectFlags)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0x00, // LDX #$00
        0x9A        // TXS
    });
    
    cpu.StatusReg.SetZero(1);
    cpu.StatusReg.SetNegative(1);
    uint8_t statusBefore = cpu.StatusReg.GetRegister();
    
    cpu.ExecuteInstruction(); // LDX (will clear Z flag)
    cpu.StatusReg.SetRegister(statusBefore); // Restore flags
    
    cpu.ExecuteInstruction(); // TXS
    
    AssertPCLocation(cpu, 3);
    EXPECT_EQ(0x00, cpu.GetStackPointer());
    EXPECT_EQ(statusBefore, cpu.StatusReg.GetRegister());
}

TEST_F(Cpu6502Test, TXSWithZeroValue)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0x00, // LDX #$00
        0x9A        // TXS
    });
    
    cpu.ExecuteInstruction(); // LDX
    cpu.StatusReg.SetRegister(0x00); // Clear all flags
    cpu.ExecuteInstruction(); // TXS
    
    AssertPCLocation(cpu, 3);
    EXPECT_EQ(0x00, cpu.GetStackPointer());
    // No flags should be set
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
}

TEST_F(Cpu6502Test, TXSWithNegativeValue)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0xFF, // LDX #$FF
        0x9A        // TXS
    });
    
    cpu.ExecuteInstruction(); // LDX
    cpu.StatusReg.SetRegister(0x00); // Clear all flags
    cpu.ExecuteInstruction(); // TXS
    
    AssertPCLocation(cpu, 3);
    EXPECT_EQ(0xFF, cpu.GetStackPointer());
    // No flags should be set by TXS
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetNegative());
}

TEST_F(Cpu6502Test, TSXTXSRoundTrip)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0x88, // LDX #$88
        0x9A,       // TXS
        0xA2, 0x00, // LDX #$00
        0xBA,       // TSX
        0xA2, 0x00, // LDX #$00
        0x9A        // TXS
    });
    
    cpu.ExecuteInstruction(); // LDX #$88
    cpu.ExecuteInstruction(); // TXS - SP = 0x88
    cpu.ExecuteInstruction(); // LDX #$00
    cpu.ExecuteInstruction(); // TSX - X = 0x88
    EXPECT_EQ(0x88, cpu.X);
    
    cpu.ExecuteInstruction(); // LDX #$00
    cpu.ExecuteInstruction(); // TXS - SP = 0x00
    EXPECT_EQ(0x00, cpu.GetStackPointer());
}

// ============================================================================
// Combined Stack Operation Tests
// ============================================================================

TEST_F(Cpu6502Test, StackMultiplePushesAndPulls)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x48, // PHA
        0x08, // PHP
        0x48, // PHA
        0x68, // PLA
        0x28, // PLP
        0x68  // PLA
    });
    
    uint8_t initialSP = cpu.GetStackPointer();
    
    cpu.A = 0x11;
    cpu.ExecuteInstruction(); // PHA - push 0x11
    EXPECT_EQ(initialSP - 1, cpu.GetStackPointer());
    
    cpu.StatusReg.SetCarry(1);
    cpu.ExecuteInstruction(); // PHP - push status
    EXPECT_EQ(initialSP - 2, cpu.GetStackPointer());
    
    cpu.A = 0x22;
    cpu.ExecuteInstruction(); // PHA - push 0x22
    EXPECT_EQ(initialSP - 3, cpu.GetStackPointer());
    
    cpu.A = 0x00;
    cpu.ExecuteInstruction(); // PLA - pull 0x22
    EXPECT_EQ(0x22, cpu.A);
    EXPECT_EQ(initialSP - 2, cpu.GetStackPointer());
    
    cpu.StatusReg.SetRegister(0x00);
    cpu.ExecuteInstruction(); // PLP - pull status
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(initialSP - 1, cpu.GetStackPointer());
    
    cpu.ExecuteInstruction(); // PLA - pull 0x11
    EXPECT_EQ(0x11, cpu.A);
    EXPECT_EQ(initialSP, cpu.GetStackPointer());
}

TEST_F(Cpu6502Test, StackWrapAround)
{
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0x01, // LDX #$01
        0x9A,       // TXS - set SP to 0x01
        0x48,       // PHA
        0x48        // PHA - should wrap
    });
    
    cpu.ExecuteInstruction(); // LDX
    cpu.ExecuteInstruction(); // TXS - SP = 0x01
    
    cpu.A = 0x42;
    cpu.ExecuteInstruction(); // PHA - SP = 0x00
    EXPECT_EQ(0x00, cpu.GetStackPointer());
    EXPECT_EQ(0x42, memory.ReadByte(0x0101));
    
    cpu.A = 0x99;
    cpu.ExecuteInstruction(); // PHA - SP wraps to 0xFF
    EXPECT_EQ(0xFF, cpu.GetStackPointer());
    EXPECT_EQ(0x99, memory.ReadByte(0x0100));
}

TEST_F(Cpu6502Test, StackInitialization)
{
    // Common initialization sequence
    SetupMemory(Memory6502::kRomStart, { 
        0xA2, 0xFF, // LDX #$FF
        0x9A        // TXS
    });
    
    cpu.ExecuteInstruction(); // LDX
    cpu.ExecuteInstruction(); // TXS
    
    EXPECT_EQ(0xFF, cpu.GetStackPointer());
    EXPECT_EQ(0xFF, cpu.X);
}

TEST_F(Cpu6502Test, StackDeepNesting)
{
    SetupMemory(Memory6502::kRomStart, { 
        0x48, 0x48, 0x48, 0x48, 0x48, // 5x PHA
        0x68, 0x68, 0x68, 0x68, 0x68  // 5x PLA
    });
    
    uint8_t initialSP = cpu.GetStackPointer();
    
    // Push 5 values
    for (int i = 1; i <= 5; i++) {
        cpu.A = i * 0x10;
        cpu.ExecuteInstruction();
    }
    
    EXPECT_EQ(initialSP - 5, cpu.GetStackPointer());
    
    // Pull 5 values (LIFO order)
    for (int i = 5; i >= 1; i--) {
        cpu.ExecuteInstruction();
        EXPECT_EQ(i * 0x10, cpu.A);
    }
    
    EXPECT_EQ(initialSP, cpu.GetStackPointer());
}
