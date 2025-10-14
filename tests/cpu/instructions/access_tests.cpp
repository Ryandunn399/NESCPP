#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

void AssertPCLocation(Cpu6502& cpu, int cycles);

// LDA Immediate Tests
TEST_F(Cpu6502Test, LDA_Immediate_BasicOperation)
{
    // Set up program: LDA #$42
    SetupMemory(Memory6502::kRomStart, {0xA9, 0x42});

    cpu.PC = Memory6502::kRomStart;
    
    // Execute LDA #$42
    uint8_t cycles = cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x42, cpu.A);
    EXPECT_EQ(2, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);  // PC should advance by 2
}

// LDA Zero Page Tests
TEST_F(Cpu6502Test, LDA_ZeroPage_BasicOperation)
{
    // Set up memory: LDA $80 (loads from address $0080)
    SetupMemory(Memory6502::kRomStart, {0xA5, 0x80});
    SetupMemory(0x0080, {0x33});  // Value to load
    cpu.PC = Memory6502::kRomStart;
    
    // Execute LDA $80
    uint8_t cycles = cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x33, cpu.A);
    EXPECT_EQ(3, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);  // PC should advance by 2
}

TEST_F(Cpu6502Test, LDA_ZeroPageX_BasicOperation)
{
    // Set up memory: LDA $80,X where X = $05, loads from $0085
    SetupMemory(Memory6502::kRomStart, {0xB5, 0x80});
    SetupMemory(0x0085, {0x77});
    cpu.PC = Memory6502::kRomStart;
    cpu.X = 0x05;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x77, cpu.A);
    EXPECT_EQ(4, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_ZeroPageX_Wraparound)
{
    // Test wraparound: $FF + $05 = $04 (wraps in zero page)
    SetupMemory(Memory6502::kRomStart, {0xB5, 0xFF});
    SetupMemory(0x0004, {0x88});  // Wrapped address
    cpu.PC = Memory6502::kRomStart;
    cpu.X = 0x05;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x88, cpu.A);
    EXPECT_EQ(4, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_Absolute_HighAddress)
{
    // Test with high memory address
    SetupMemory(Memory6502::kRomStart, {0xAD, 0x00, 0xFF});  // $FF00
    SetupMemory(0xFF00, {0xBB});
    cpu.PC = Memory6502::kRomStart;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xBB, cpu.A);
    EXPECT_EQ(4, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 3, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_AbsoluteX_BasicOperation)
{
    // LDA $1234,X where X = $10, loads from $1244
    SetupMemory(Memory6502::kRomStart, {0xBD, 0x34, 0x12});
    SetupMemory(0x1244, {0xCC});
    cpu.PC = Memory6502::kRomStart;
    cpu.X = 0x10;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xCC, cpu.A);
    EXPECT_EQ(4, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 3, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_AbsoluteX_PageCross)
{
    // Test page boundary crossing: $12FF + $01 = $1300
    SetupMemory(Memory6502::kRomStart, {0xBD, 0xFF, 0x12});
    SetupMemory(0x1300, {0xDD});
    cpu.PC = Memory6502::kRomStart;
    cpu.X = 0x01;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xDD, cpu.A);
    EXPECT_EQ(5, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 3, cpu.PC);
}

// LDA Absolute,Y Tests
TEST_F(Cpu6502Test, LDA_AbsoluteY_BasicOperation)
{
    // LDA $1234,Y where Y = $08, loads from $123C
    SetupMemory(Memory6502::kRomStart, {0xB9, 0x34, 0x12});
    SetupMemory(0x123C, {0xEE});
    cpu.PC = Memory6502::kRomStart;
    cpu.Y = 0x08;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xEE, cpu.A);
    EXPECT_EQ(4, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 3, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_AbsoluteY_PageCross)
{
    // Test page boundary crossing: $20FF + $02 = $2101
    SetupMemory(Memory6502::kRomStart, {0xB9, 0xFF, 0x20});
    SetupMemory(0x2101, {0x11});
    cpu.PC = Memory6502::kRomStart;
    cpu.Y = 0x02;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x11, cpu.A);
    EXPECT_EQ(5, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 3, cpu.PC);
}

// LDA Indirect,X Tests (Indexed Indirect)
TEST_F(Cpu6502Test, LDA_IndirectX_BasicOperation)
{
    // LDA ($20,X) where X = $04
    // Pointer at $0024 contains address $1234 (34 12 in little-endian)
    // Load from $1234
    SetupMemory(Memory6502::kRomStart, {0xA1, 0x20});
    SetupMemory(0x0024, {0x34, 0x12});  // Pointer to $1234
    SetupMemory(0x1234, {0x55});        // Data to load
    cpu.PC = Memory6502::kRomStart;
    cpu.X = 0x04;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x55, cpu.A);
    EXPECT_EQ(6, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_IndirectX_ZeroPageWrap)
{
    // Test zero page wraparound: ($FF,X) where X = $02
    // Pointer calculation: $FF + $02 = $01 (wraps in zero page)
    SetupMemory(Memory6502::kRomStart, {0xA1, 0xFF});
    SetupMemory(0x0001, {0x00, 0x30});  // Pointer to $3000
    SetupMemory(0x3000, {0x66});
    cpu.PC = Memory6502::kRomStart;
    cpu.X = 0x02;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x66, cpu.A);
    EXPECT_EQ(6, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);
}

// LDA Indirect,Y Tests (Indirect Indexed)
TEST_F(Cpu6502Test, LDA_IndirectY_BasicOperation)
{
    // LDA ($20),Y where Y = $08
    // Address at $0020 is $1234, final address is $1234 + $08 = $123C
    SetupMemory(Memory6502::kRomStart, {0xB1, 0x20});
    SetupMemory(0x0020, {0x34, 0x12});  // Base address $1234
    SetupMemory(0x123C, {0x77});        // Data at $1234 + $08
    cpu.PC = Memory6502::kRomStart;
    cpu.Y = 0x08;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x77, cpu.A);
    EXPECT_EQ(5, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_IndirectY_PageCross)
{
    // Test page boundary crossing in final address calculation
    // Base address $20FF + Y register $05 = $2104
    SetupMemory(Memory6502::kRomStart, {0xB1, 0x40});
    SetupMemory(0x0040, {0xFF, 0x20});  // Base address $20FF
    SetupMemory(0x2104, {0x88});        // Data at crossed page
    cpu.PC = Memory6502::kRomStart;
    cpu.Y = 0x05;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x88, cpu.A);
    EXPECT_EQ(6, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_ZeroPage_BoundaryCase)
{
    // Test loading from $00FF (high zero page address)
    SetupMemory(Memory6502::kRomStart, {0xA5, 0xFF});
    SetupMemory(0x00FF, {0xAA});
    cpu.PC = Memory6502::kRomStart;
    
    uint8_t cycles = cpu.ExecuteInstruction();
    
    EXPECT_EQ(0xAA, cpu.A);
    EXPECT_EQ(3, cycles);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, LDA_Immediate_ZeroFlag)
{
    // Set up program: LDA #$00
    SetupMemory(Memory6502::kRomStart, {0xA9, 0x00});
    cpu.PC = Memory6502::kRomStart;
    
    // Execute LDA #$00
    cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x00, cpu.A);
    EXPECT_TRUE(cpu.StatusReg.GetZero());   // Zero flag should be set
    EXPECT_FALSE(cpu.StatusReg.GetNegative()); // Negative flag should be clear
}

TEST_F(Cpu6502Test, LDA_Immediate_NegativeFlag)
{
    // Set up program: LDA #$80 (negative number in signed interpretation)
    SetupMemory(Memory6502::kRomStart, {0xA9, 0x80});
    cpu.PC = Memory6502::kRomStart;
    
    // Execute LDA #$80
    cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x80, cpu.A);
    EXPECT_FALSE(cpu.StatusReg.GetZero());   // Zero flag should be clear
    EXPECT_TRUE(cpu.StatusReg.GetNegative()); // Negative flag should be set
}

TEST_F(Cpu6502Test, LDA_Immediate_ClearsFlags)
{
    // Set flags to known state
    cpu.StatusReg.SetZero(true);
    cpu.StatusReg.SetNegative(true);
    
    // Set up program: LDA #$42 (should clear both flags)
    SetupMemory(Memory6502::kRomStart, {0xA9, 0x42});
    cpu.PC = Memory6502::kRomStart;
    
    // Execute LDA #$42
    cpu.ExecuteInstruction();
    
    // Check that flags were properly updated
    EXPECT_EQ(0x42, cpu.A);
    EXPECT_FALSE(cpu.StatusReg.GetZero());
    EXPECT_FALSE(cpu.StatusReg.GetNegative());
}

// STA Zero Page Tests
TEST_F(Cpu6502Test, STA_ZeroPage_BasicOperation)
{
    // Set up program: STA $80 (stores A to address $0080)
    SetupMemory(Memory6502::kRomStart, {0x85, 0x80});
    SetupMemory(0x0080, {0x00});  // Initialize memory location
    cpu.PC = Memory6502::kRomStart;
    cpu.A = 0x42;  // Value to store
    
    // Execute STA $80
    uint8_t cycles = cpu.ExecuteInstruction();
    
    // Check results
    EXPECT_EQ(0x42, memory.ReadByte(0x0080));  // Memory should contain A value
    EXPECT_EQ(0x42, cpu.A);                    // A register unchanged
    EXPECT_EQ(3, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, STA_Zero_Page_X)
{
    SetupMemory(Memory6502::kRomStart, {0x95, 0xA});
    cpu.PC = Memory6502::kRomStart;
    cpu.X = 0x33;
    cpu.A = 0x69;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x69, memory.ReadByte(0xA + 0x33));
    EXPECT_EQ(0x69, cpu.A);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, STA_Absolute)
{
    SetupMemory(Memory6502::kRomStart, {0x8D, 0x34, 0x12});
    cpu.PC = Memory6502::kRomStart;
    cpu.A = 0x69;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x69, memory.ReadByte(0x1234));
    EXPECT_EQ(0x69, cpu.A);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, STA_Absolute_X)
{
    SetupMemory(Memory6502::kRomStart, {0x9D, 0x00, 0x06});
    cpu.PC = Memory6502::kRomStart;
    cpu.X = 0x2D;
    cpu.A = 0xDD;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xDD, memory.ReadByte(0x600 + 0x2D));
    EXPECT_EQ(0xDD, cpu.A);
    EXPECT_EQ(5, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, STA_Absolute_Y)
{
    SetupMemory(Memory6502::kRomStart, {0x99, 0x00, 0x06});
    cpu.PC = Memory6502::kRomStart;
    cpu.Y = 0x2D;
    cpu.A = 0xDD;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xDD, memory.ReadByte(0x600 + 0x2D));
    EXPECT_EQ(0xDD, cpu.A);
    EXPECT_EQ(5, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, STA_Indirect_X)
{
    SetupMemory(Memory6502::kRomStart, {0x81, 0xAA});
    memory.WriteByte(0xB2, 0x1A);
    memory.WriteByte(0xB3, 0x00);
    cpu.X = 0x8;
    cpu.A = 0x5;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x5, memory.ReadByte(0x1A));
    EXPECT_EQ(0x5, cpu.A);
    EXPECT_EQ(6, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, STA_Indirect_Y)
{
    SetupMemory(Memory6502::kRomStart, {0x91, 0xAA});
    memory.WriteByte(0xAA, 0x1A);
    memory.WriteByte(0xAB, 0x00);
    cpu.Y = 0x8;
    cpu.A = 0x5;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x5, memory.ReadByte(0x1A + 0x8));
    EXPECT_EQ(0x5, cpu.A);
    EXPECT_EQ(6, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, STX_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, {0x86, 0x73});
    cpu.X = 0xAB;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xAB, memory.ReadByte(0x73));
    EXPECT_EQ(0xAB, cpu.X);
    EXPECT_EQ(3, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, STX_ZeroPageY)
{
    SetupMemory(Memory6502::kRomStart, {0x96, 0x1});
    cpu.Y = 0x2B;
    cpu.X = 0xFA;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xFA, memory.ReadByte(0x1 + 0x2B));
    EXPECT_EQ(0xFA, cpu.X);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, STX_Absolute)
{
    SetupMemory(Memory6502::kRomStart, {0x8E, 0xB3, 0x77});
    cpu.X = 0xFA;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xFA, memory.ReadByte(0x77B3));
    EXPECT_EQ(0xFA, cpu.X);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, STY_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, {0x84, 0x73});
    cpu.Y = 0xAB;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xAB, memory.ReadByte(0x73));
    EXPECT_EQ(0xAB, cpu.Y);
    EXPECT_EQ(3, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, STY_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, {0x94, 0x1});
    cpu.X = 0x2B;
    cpu.Y = 0xFA;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xFA, memory.ReadByte(0x1 + 0x2B));
    EXPECT_EQ(0xFA, cpu.Y);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, STY_Absolute)
{
    SetupMemory(Memory6502::kRomStart, {0x8C, 0xB3, 0x77});
    cpu.Y = 0xFA;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xFA, memory.ReadByte(0x77B3));
    EXPECT_EQ(0xFA, cpu.Y);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, LDX_Immediate)
{
    SetupMemory(Memory6502::kRomStart, {0xA2, 0xD7});

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xD7, cpu.X);
    EXPECT_EQ(2, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, LDX_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, {0xA6, 0xBB});
    memory.WriteByte(0xBB, 0xCC);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xCC, cpu.X);
    EXPECT_EQ(3, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, LDX_ZeroPageY)
{
    SetupMemory(Memory6502::kRomStart, {0xB6, 0x5});
    cpu.Y = 0x7F;
    memory.WriteByte(0x5 + 0x7F, 0xCC);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xCC, cpu.X);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, LDX_Absolute)
{
    SetupMemory(Memory6502::kRomStart, {0xAE, 0x72, 0x22});
    memory.WriteByte(0x2272, 0x1A);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x1A, cpu.X);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, LDX_AbsoluteY)
{
    SetupMemory(Memory6502::kRomStart, {0xBE, 0x11, 0x12});
    cpu.Y = 0x43;
    memory.WriteByte(0x1211 + 0x43, 0x1B);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x1B, cpu.X);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, LDX_AbsoluteYPageCross)
{
    SetupMemory(Memory6502::kRomStart, {0xBE, 0x72, 0x22});
    cpu.Y = 0xFF;
    memory.WriteByte(0x2272 + 0xFF, 0x1B);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x1B, cpu.X);
    EXPECT_EQ(5, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, LDY_Immediate)
{
    SetupMemory(Memory6502::kRomStart, {0xA0, 0x22});
    
    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x22, cpu.Y);
    EXPECT_EQ(2, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, LDY_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, {0xA4, 0x72});
    memory.WriteByte(0x72, 0xCB);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xCB, cpu.Y);
    EXPECT_EQ(3, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, LDY_ZeroPageX)
{
    SetupMemory(Memory6502::kRomStart, {0xB4, 0x7});
    cpu.X = 0xF1;
    memory.WriteByte(0x7 + 0xF1, 0x11);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x11, cpu.Y);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 2);
}

TEST_F(Cpu6502Test, LDY_Absolute)
{
    SetupMemory(Memory6502::kRomStart, {0xAC, 0x11, 0x12});
    memory.WriteByte(0x1211, 0x59);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x59, cpu.Y);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, LDY_AbsoluteX)
{
    SetupMemory(Memory6502::kRomStart, {0xBC, 0x11, 0x12});
    cpu.X = 0xD7;
    memory.WriteByte(0x1211 + 0xD7, 0xAA);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xAA, cpu.Y);
    EXPECT_EQ(4, cycles);
    AssertPCLocation(cpu, 3);
}

TEST_F(Cpu6502Test, LDY_AbsoluteXPageCross)
{
    SetupMemory(Memory6502::kRomStart, {0xBC, 0x11, 0x12});
    cpu.X = 0xFF;
    memory.WriteByte(0x1211 + 0xFF, 0xAA);

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xAA, cpu.Y);
    EXPECT_EQ(5, cycles);
    AssertPCLocation(cpu, 3);
}