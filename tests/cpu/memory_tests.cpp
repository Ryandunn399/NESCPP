#include <gtest/gtest.h>
#include "cpu/memory.hpp"

class MemoryTest : public ::testing::Test
{
protected:
    Memory6502 memory;

    void SetUp() override
    {
        // Memory is initialized to all zeros by default.
    }
};

TEST_F(MemoryTest, ReadWriteByte)
{
    // Test basic read/write in RAM region
    memory.WriteByte(0x0300, 0x42);
    EXPECT_EQ(0x42, memory.ReadByte(0x0300));
    
    // Test zero page
    memory.WriteZeroPage(0x80, 0xAA);
    EXPECT_EQ(0xAA, memory.ReadZeroPage(0x80));
    EXPECT_EQ(0xAA, memory.ReadByte(0x80));  // Should be same as ReadZeroPage
}

TEST_F(MemoryTest, ReadWriteWord)
{
    // Test little-endian word storage
    memory.WriteWord(0x0300, 0x1234);
    EXPECT_EQ(0x34, memory.ReadByte(0x0300));  // Low byte first
    EXPECT_EQ(0x12, memory.ReadByte(0x0301));  // High byte second
    EXPECT_EQ(0x1234, memory.ReadWord(0x0300));
    
    // Test edge case with 0xFF values
    memory.WriteWord(0x0400, 0xABCD);
    EXPECT_EQ(0xABCD, memory.ReadWord(0x0400));
}

// Stack Operations Tests
TEST_F(MemoryTest, StackOperations)
{
    uint8_t sp = 0xFF;  // Stack pointer starts at top
    
    // Push a byte
    memory.PushByte(sp, 0x42);
    EXPECT_EQ(0xFE, sp);  // SP should decrease
    EXPECT_EQ(0x42, memory.ReadByte(Memory6502::kStackStart + 0xFF));
    
    // Push another byte
    memory.PushByte(sp, 0xAA);
    EXPECT_EQ(0xFD, sp);
    EXPECT_EQ(0xAA, memory.ReadByte(Memory6502::kStackStart + 0xFE));
    
    // Pop the bytes back (should be in reverse order)
    uint8_t popped1 = memory.PopByte(sp);
    EXPECT_EQ(0xAA, popped1);
    EXPECT_EQ(0xFE, sp);
    
    uint8_t popped2 = memory.PopByte(sp);
    EXPECT_EQ(0x42, popped2);
    EXPECT_EQ(0xFF, sp);  // Back to original
}

TEST_F(MemoryTest, StackWordOperations)
{
    uint8_t sp = 0xFF;
    
    // Push a word
    memory.PushWord(sp, 0x1234);
    EXPECT_EQ(0xFD, sp);  // SP decreases by 2
    
    // Check that high byte was pushed first, low byte second
    EXPECT_EQ(0x12, memory.ReadByte(Memory6502::kStackStart + 0xFF));  // High byte
    EXPECT_EQ(0x34, memory.ReadByte(Memory6502::kStackStart + 0xFE));  // Low byte
    
    // Pop the word back
    uint16_t popped = memory.PopWord(sp);
    EXPECT_EQ(0x1234, popped);
    EXPECT_EQ(0xFF, sp);  // Back to original
}

// Exception Tests
TEST_F(MemoryTest, InvalidAddressThrows)
{
    EXPECT_THROW(memory.ReadWord(0xFFFF), std::overflow_error);
}

TEST_F(MemoryTest, RomWriteProtection)
{
    // Should throw when trying to write to ROM region
    EXPECT_THROW(memory.WriteByte(Memory6502::kRomStart, 0x42), std::runtime_error);
    EXPECT_THROW(memory.WriteByte(0xFFFF, 0x42), std::runtime_error);
    EXPECT_THROW(memory.WriteWord(Memory6502::kRomStart, 0x1234), std::runtime_error);
}

TEST_F(MemoryTest, LoadProgram)
{
    uint8_t program[] = {0xA9, 0x42, 0x85, 0x20};
    
    // Load program into ROM
    memory.LoadProgram(program, sizeof(program), Memory6502::kRomStart);
    
    // Verify program was loaded correctly
    EXPECT_EQ(0xA9, memory.ReadByte(Memory6502::kRomStart));
    EXPECT_EQ(0x42, memory.ReadByte(Memory6502::kRomStart + 1));
    EXPECT_EQ(0x85, memory.ReadByte(Memory6502::kRomStart + 2));
    EXPECT_EQ(0x20, memory.ReadByte(Memory6502::kRomStart + 3));
}

TEST_F(MemoryTest, LoadProgramValidation)
{
    uint8_t program[] = {0xA9, 0x42};
    
    // Should throw for null pointer
    EXPECT_THROW(memory.LoadProgram(nullptr, 2), std::invalid_argument);
    
    // Should throw for zero size
    EXPECT_THROW(memory.LoadProgram(program, 0), std::invalid_argument);
    
    // Should throw if program too large
    EXPECT_THROW(memory.LoadProgram(program, Memory6502::kMemorySize + 1), std::invalid_argument);
}
