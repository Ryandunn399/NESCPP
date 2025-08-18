#include <gtest/gtest.h>
#include "cpu/cpu6502.hpp"
#include "nes.hpp"
#include "tests/cpu_fixture.hpp"

// Basic CPU State Tests
TEST_F(Cpu6502Test, InitialState)
{
    EXPECT_EQ(Memory6502::kRomStart, cpu.PC);
    EXPECT_EQ(0, cpu.A);
    EXPECT_EQ(0, cpu.X);
    EXPECT_EQ(0, cpu.Y);
    EXPECT_EQ(0xFF, cpu.SP);
}

TEST_F(Cpu6502Test, Reset)
{
    // Modify some state
    cpu.A = 0x42;
    cpu.X = 0x33;
    cpu.PC = 0x1234;
    cpu.SP = 0x80;
    
    // Reset should restore initial state
    cpu.Reset();
    
    EXPECT_EQ(0, cpu.A);
    EXPECT_EQ(0, cpu.X);
    EXPECT_EQ(0, cpu.Y);
    EXPECT_EQ(Memory6502::kRomStart, cpu.PC);
    EXPECT_EQ(0xFF, cpu.SP);
}

// Addressing Mode Tests (using public memory access methods for verification)
TEST_F(Cpu6502Test, AddressingModeImmediate)
{
    // Test that immediate addressing uses the byte right after the opcode
    SetupMemory(Memory6502::kRomStart, {0xA9, 0x55, 0x66}); // LDA #$55, then 0x66
    cpu.PC = Memory6502::kRomStart;
    
    cpu.ExecuteInstruction();  // Execute LDA #$55
    
    EXPECT_EQ(0x55, cpu.A);  // Should load 0x55, not 0x66
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);  // PC should point to byte after immediate value
}

TEST_F(Cpu6502Test, AddressingModeZeroPage)
{
    // Set up zero page memory
    memory.ForceWriteByte(0x80, 0xAB);  // Put test value in zero page
    
    // Set up program: LDA $80 (zero page addressing)
    SetupMemory(Memory6502::kRomStart, {0xA5, 0x80});
    cpu.PC = Memory6502::kRomStart;
    
    cpu.ExecuteInstruction();  // Execute LDA $80
    
    EXPECT_EQ(0xAB, cpu.A);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, AddressingModeZeroPageX)
{
    // Set up zero page memory and X register
    cpu.X = 0x05;
    memory.ForceWriteByte(0x85, 0xCD);  // Put test value at 0x80 + 0x05 = 0x85
    
    // Set up program: LDA $80,X
    SetupMemory(Memory6502::kRomStart, {0xB5, 0x80});
    cpu.PC = Memory6502::kRomStart;
    
    cpu.ExecuteInstruction();  // Execute LDA $80,X
    
    EXPECT_EQ(0xCD, cpu.A);
    EXPECT_EQ(Memory6502::kRomStart + 2, cpu.PC);
}

TEST_F(Cpu6502Test, AddressingModeZeroPageX_Wraparound)
{
    // Test zero page wraparound: 0xFF + 0x02 should = 0x01
    cpu.X = 0x02;
    memory.ForceWriteByte(0x01, 0xEF);  // Put test value at wraparound address
    
    // Set up program: LDA $FF,X (should access 0x01)
    SetupMemory(Memory6502::kRomStart, {0xB5, 0xFF});
    cpu.PC = Memory6502::kRomStart;
    
    cpu.ExecuteInstruction();  // Execute LDA $FF,X
    
    EXPECT_EQ(0xEF, cpu.A);
}

TEST_F(Cpu6502Test, AddressingModeAbsolute)
{
    // Set up absolute address memory
    memory.ForceWriteByte(0x1234, 0x99);
    
    // Set up program: LDA $1234 (little-endian: 34 12)
    SetupMemory(Memory6502::kRomStart, {0xAD, 0x34, 0x12});
    cpu.PC = Memory6502::kRomStart;
    
    cpu.ExecuteInstruction();  // Execute LDA $1234
    
    EXPECT_EQ(0x99, cpu.A);
    EXPECT_EQ(Memory6502::kRomStart + 3, cpu.PC);
}

// Program Loading and Execution Tests
TEST_F(Cpu6502Test, LoadAndExecuteProgram)
{
    // Create a simple program: LDA #$42, LDA #$00
    uint8_t program[] = {
        0xA9, 0x42,  // LDA #$42
        0xA9, 0x00   // LDA #$00
    };
    
    memory.LoadProgram(program, sizeof(program));
    
    // Execute first instruction
    cpu.ExecuteInstruction();
    EXPECT_EQ(0x42, cpu.A);
    EXPECT_FALSE(cpu.StatusReg.GetZero());
    
    // Execute second instruction
    cpu.ExecuteInstruction();
    EXPECT_EQ(0x00, cpu.A);
    EXPECT_TRUE(cpu.StatusReg.GetZero());
}

TEST_F(Cpu6502Test, UnimplementedOpcodeThrows)
{
    // Use an opcode that shouldn't be implemented
    SetupMemory(Memory6502::kRomStart, {0x02});  // Assuming 0x02 is not implemented
    cpu.PC = Memory6502::kRomStart;
    
    EXPECT_THROW(cpu.ExecuteInstruction(), std::runtime_error);
}

// Integration test with memory system
TEST_F(Cpu6502Test, MemoryIntegration)
{
    // Test that CPU properly uses memory system
    memory.ForceWriteByte(0x0200, 0x77);
    EXPECT_EQ(0x77, memory.ReadByte(0x0200));
    
    // Test word operations
    memory.WriteWord(0x0300, 0x1234);
    EXPECT_EQ(0x1234, memory.ReadWord(0x0300));
    
    // Test that ROM protection works through CPU
    EXPECT_THROW(memory.WriteWord(Memory6502::kRomStart, 0x42), std::runtime_error);
}
