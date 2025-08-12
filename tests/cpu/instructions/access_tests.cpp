#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

// LDA Immediate Tests
TEST_F(Cpu6502Test, LDA_Immediate_BasicOperation)
{
    // Set up program: LDA #$42
    SetupMemory(Memory::kRomStart, {0xA9, 0x42});

    cpu.PC = Memory::kRomStart;
    
    // Execute LDA #$42
    cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x42, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);  // PC should advance by 2
}

// LDA Zero Page Tests
TEST_F(Cpu6502Test, LDA_ZeroPage_BasicOperation)
{
    // Set up memory: LDA $80 (loads from address $0080)
    SetupMemory(Memory::kRomStart, {0xA5, 0x80});
    SetupMemory(0x0080, {0x33});  // Value to load
    cpu.PC = Memory::kRomStart;
    
    // Execute LDA $80
    cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x33, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);  // PC should advance by 2
}

TEST_F(Cpu6502Test, LDA_ZeroPageX_BasicOperation)
{
    // Set up memory: LDA $80,X where X = $05, loads from $0085
    SetupMemory(Memory::kRomStart, {0xB5, 0x80});
    SetupMemory(0x0085, {0x77});
    cpu.PC = Memory::kRomStart;
    cpu.X = 0x05;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x77, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_ZeroPageX_Wraparound)
{
    // Test wraparound: $FF + $05 = $04 (wraps in zero page)
    SetupMemory(Memory::kRomStart, {0xB5, 0xFF});
    SetupMemory(0x0004, {0x88});  // Wrapped address
    cpu.PC = Memory::kRomStart;
    cpu.X = 0x05;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x88, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_Absolute_HighAddress)
{
    // Test with high memory address
    SetupMemory(Memory::kRomStart, {0xAD, 0x00, 0xFF});  // $FF00
    SetupMemory(0xFF00, {0xBB});
    cpu.PC = Memory::kRomStart;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xBB, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 3, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_AbsoluteX_BasicOperation)
{
    // LDA $1234,X where X = $10, loads from $1244
    SetupMemory(Memory::kRomStart, {0xBD, 0x34, 0x12});
    SetupMemory(0x1244, {0xCC});
    cpu.PC = Memory::kRomStart;
    cpu.X = 0x10;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xCC, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 3, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_AbsoluteX_PageCross)
{
    // Test page boundary crossing: $12FF + $01 = $1300
    SetupMemory(Memory::kRomStart, {0xBD, 0xFF, 0x12});
    SetupMemory(0x1300, {0xDD});
    cpu.PC = Memory::kRomStart;
    cpu.X = 0x01;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xDD, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 3, cpu.PC);
    // Note: Page crossing should be handled by HandlePageCross() for cycle counting
}

// LDA Absolute,Y Tests
TEST_F(Cpu6502Test, LDA_AbsoluteY_BasicOperation)
{
    // LDA $1234,Y where Y = $08, loads from $123C
    SetupMemory(Memory::kRomStart, {0xB9, 0x34, 0x12});
    SetupMemory(0x123C, {0xEE});
    cpu.PC = Memory::kRomStart;
    cpu.Y = 0x08;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xEE, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 3, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_AbsoluteY_PageCross)
{
    // Test page boundary crossing: $20FF + $02 = $2101
    SetupMemory(Memory::kRomStart, {0xB9, 0xFF, 0x20});
    SetupMemory(0x2101, {0x11});
    cpu.PC = Memory::kRomStart;
    cpu.Y = 0x02;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x11, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 3, cpu.PC);
}

// LDA Indirect,X Tests (Indexed Indirect)
TEST_F(Cpu6502Test, LDA_IndirectX_BasicOperation)
{
    // LDA ($20,X) where X = $04
    // Pointer at $0024 contains address $1234 (34 12 in little-endian)
    // Load from $1234
    SetupMemory(Memory::kRomStart, {0xA1, 0x20});
    SetupMemory(0x0024, {0x34, 0x12});  // Pointer to $1234
    SetupMemory(0x1234, {0x55});        // Data to load
    cpu.PC = Memory::kRomStart;
    cpu.X = 0x04;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x55, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_IndirectX_ZeroPageWrap)
{
    // Test zero page wraparound: ($FF,X) where X = $02
    // Pointer calculation: $FF + $02 = $01 (wraps in zero page)
    SetupMemory(Memory::kRomStart, {0xA1, 0xFF});
    SetupMemory(0x0001, {0x00, 0x30});  // Pointer to $3000
    SetupMemory(0x3000, {0x66});
    cpu.PC = Memory::kRomStart;
    cpu.X = 0x02;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x66, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);
}

// LDA Indirect,Y Tests (Indirect Indexed)
TEST_F(Cpu6502Test, LDA_IndirectY_BasicOperation)
{
    // LDA ($20),Y where Y = $08
    // Address at $0020 is $1234, final address is $1234 + $08 = $123C
    SetupMemory(Memory::kRomStart, {0xB1, 0x20});
    SetupMemory(0x0020, {0x34, 0x12});  // Base address $1234
    SetupMemory(0x123C, {0x77});        // Data at $1234 + $08
    cpu.PC = Memory::kRomStart;
    cpu.Y = 0x08;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x77, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_IndirectY_PageCross)
{
    // Test page boundary crossing in final address calculation
    // Base address $20FF + Y register $05 = $2104
    SetupMemory(Memory::kRomStart, {0xB1, 0x40});
    SetupMemory(0x0040, {0xFF, 0x20});  // Base address $20FF
    SetupMemory(0x2104, {0x88});        // Data at crossed page
    cpu.PC = Memory::kRomStart;
    cpu.Y = 0x05;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x88, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_ZeroPage_BoundaryCase)
{
    // Test loading from $00FF (high zero page address)
    SetupMemory(Memory::kRomStart, {0xA5, 0xFF});
    SetupMemory(0x00FF, {0xAA});
    cpu.PC = Memory::kRomStart;
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xAA, cpu.A);
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_Immediate_ZeroFlag)
{
    // Set up program: LDA #$00
    SetupMemory(Memory::kRomStart, {0xA9, 0x00});
    cpu.PC = Memory::kRomStart;
    
    // Execute LDA #$00
    cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x00, cpu.A);
    EXPECT_TRUE(true);
    EXPECT_TRUE(cpu.StatusRegister.GetZero());   // Zero flag should be set
    EXPECT_FALSE(cpu.StatusRegister.GetNegative()); // Negative flag should be clear
}

TEST_F(Cpu6502Test, LDA_Immediate_NegativeFlag)
{
    // Set up program: LDA #$80 (negative number in signed interpretation)
    SetupMemory(Memory::kRomStart, {0xA9, 0x80});
    cpu.PC = Memory::kRomStart;
    
    // Execute LDA #$80
    cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x80, cpu.A);
    EXPECT_FALSE(cpu.StatusRegister.GetZero());   // Zero flag should be clear
    EXPECT_TRUE(cpu.StatusRegister.GetNegative()); // Negative flag should be set
}

TEST_F(Cpu6502Test, LDA_Immediate_ClearsFlags)
{
    // Set flags to known state
    cpu.StatusRegister.SetZero(true);
    cpu.StatusRegister.SetNegative(true);
    
    // Set up program: LDA #$42 (should clear both flags)
    SetupMemory(Memory::kRomStart, {0xA9, 0x42});
    cpu.PC = Memory::kRomStart;
    
    // Execute LDA #$42
    cpu.ExecuteInstruction();
    
    // Check that flags were properly updated
    EXPECT_EQ(0x42, cpu.A);
    EXPECT_FALSE(cpu.StatusRegister.GetZero());
    EXPECT_FALSE(cpu.StatusRegister.GetNegative());
}

// STA Zero Page Tests
TEST_F(Cpu6502Test, STA_ZeroPage_BasicOperation)
{
    // Set up program: STA $80 (stores A to address $0080)
    SetupMemory(Memory::kRomStart, {0x85, 0x80});
    SetupMemory(0x0080, {0x00});  // Initialize memory location
    cpu.PC = Memory::kRomStart;
    cpu.A = 0x42;  // Value to store
    
    // Execute STA $80
    cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x42, memory.ReadByte(0x0080));  // Memory should contain A value
    EXPECT_EQ(0x42, cpu.A);                    // A register unchanged
    EXPECT_EQ(Memory::kRomStart + 2, cpu.PC);  // PC should advance by 2
}