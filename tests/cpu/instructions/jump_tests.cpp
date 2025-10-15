#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, JMPAbsolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x4C, 0x34, 0x12 });

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x1234, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, JMPIndirect)
{
    SetupMemory(Memory6502::kRomStart, { 0x6C, 0x34, 0x12 });
    SetupMemory(0x1234, { 0xCD, 0xAB });

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xABCD, cpu.PC);
    EXPECT_EQ(5, cycles);
}

TEST_F(Cpu6502Test, JMPIndirectBug)
{
    SetupMemory(Memory6502::kRomStart, { 0x6C, 0xFF, 0x12 });
    SetupMemory(0x12FF, { 0xCD });
    SetupMemory(0x1300, { 0xAB });
    SetupMemory(0x1200, { 0xDE });

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xDECD, cpu.PC);
    EXPECT_EQ(5, cycles);
}

TEST_F(Cpu6502Test, JMPIndirectBug_ZeroPage)
{
    SetupMemory(Memory6502::kRomStart, { 0x6C, 0xFF, 0x00 });
    SetupMemory(0x00FF, { 0x34 });
    SetupMemory(0x0100, { 0x12 }); 
    SetupMemory(0x0000, { 0x56 }); 

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x5634, cpu.PC);
    EXPECT_EQ(5, cycles);
}

TEST_F(Cpu6502Test, JMPIndirect_BeforeBoundary)
{
    SetupMemory(Memory6502::kRomStart, { 0x6C, 0xFE, 0x12 });
    SetupMemory(0x12FE, { 0xCD, 0xAB });

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0xABCD, cpu.PC);
    EXPECT_EQ(5, cycles);
}

TEST_F(Cpu6502Test, JSRAbsolute)
{
    SetupMemory(Memory6502::kRomStart, { 0x20, 0x34, 0x12 });
    uint16_t expectedPc = cpu.PC + 2;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x1234, cpu.PC);
    EXPECT_EQ(0xFF - 2, cpu.GetStackPointer());
    EXPECT_EQ(expectedPc, memory.ReadWord(0x01FE));
    EXPECT_EQ(6, cycles);
}

TEST_F(Cpu6502Test, JSR_StackWrap)
{
    SetupMemory(Memory6502::kRomStart, { 0x20, 0x34, 0x12 });
    memory.StackPointer = 0x01;  // Near bottom of stack

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(0x1234, cpu.PC);
    EXPECT_EQ(0xFF, cpu.GetStackPointer());  // Wraps around
    // Return address stored at $0100 and $0101
}

TEST_F(Cpu6502Test, RTSImplied)
{
    SetupMemory(Memory6502::kRomStart, { 0x20, 0x34, 0x12, 0xAA });
    SetupMemory(0x1234, { 0x60 });
    
    uint16_t returnAddress = Memory6502::kRomStart + 3;  // Next instruction after JSR

    // Execute JSR
    uint8_t cycles = cpu.ExecuteInstruction();
    EXPECT_EQ(0x1234, cpu.PC);
    EXPECT_EQ(0xFD, cpu.GetStackPointer());
    EXPECT_EQ(6, cycles);

    // Execute RTS
    cycles = cpu.ExecuteInstruction();
    EXPECT_EQ(returnAddress, cpu.PC);  // More direct check
    EXPECT_EQ(0xFF, cpu.GetStackPointer());  // Stack restored
    EXPECT_EQ(6, cycles);
}

TEST_F(Cpu6502Test, BRKBasic)
{
    // Setup BRK instruction with padding
    SetupMemory(Memory6502::kRomStart, { 0x00, 0x00 });

    // Setup IRQ vector
    SetupMemory(0xFFFE, { 0x00, 0xC0 });
    
    uint16_t pcBeforeBrk = cpu.PC;
    uint8_t statusBeforeBrk = cpu.StatusReg.GetRegister();

    uint8_t cycles = cpu.ExecuteInstruction();

    // Check PC jumped to IRQ vector
    EXPECT_EQ(0xC000, cpu.PC);

    // Validate stack pointer and stack value is at PC + 1
    EXPECT_EQ(0xFC, cpu.GetStackPointer());
    EXPECT_EQ(pcBeforeBrk + 2, memory.ReadWord(0x01FE));

    // Validate status register pushed with B and unused flags set
    uint8_t pushedStatus = memory.ReadByte(0x01FD);
    EXPECT_EQ(1, (pushedStatus >> 4) & 1); 
    EXPECT_EQ(1, (pushedStatus >> 5) & 1);

    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    
    EXPECT_EQ(7, cycles);
}

TEST_F(Cpu6502Test, BRKReservesStatusFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0x00, 0x00 });
    SetupMemory(0xFFFE, { 0x00, 0xC0 });
    
    // Set some flags
    cpu.StatusReg.SetCarry(1);
    cpu.StatusReg.SetZero(1);
    cpu.StatusReg.SetOverflow(1);
    cpu.StatusReg.SetNegative(1);
    
    cpu.ExecuteInstruction();
    
    // Check flags preserved on stack (with B flag added)
    uint8_t pushedStatus = memory.ReadByte(0x01FD);
    EXPECT_EQ(1, (pushedStatus >> 0) & 1);  // Carry
    EXPECT_EQ(1, (pushedStatus >> 1) & 1);  // Zero
    EXPECT_EQ(1, (pushedStatus >> 4) & 1);  // B flag
    EXPECT_EQ(1, (pushedStatus >> 6) & 1);  // Overflow
    EXPECT_EQ(1, (pushedStatus >> 7) & 1);  // Negative
}

TEST_F(Cpu6502Test, BRK_NMIHijack)
{
    SetupMemory(Memory6502::kRomStart, { 0x00, 0x00 });
    SetupMemory(0xFFFA, { 0x00, 0xD0 });
    SetupMemory(0xFFFE, { 0x00, 0xC0 });

    uint16_t pcBeforeBrk = cpu.PC;

    cpu.TriggerNMI();

    uint16_t cycles = cpu.ExecuteInstruction();

    // Bug should jump to NMI vector, not IRQ vector
    EXPECT_EQ(0xD000, cpu.PC);

    // Stack still has return address
    uint16_t pushedPc = memory.ReadWord(0x01FE);
    EXPECT_EQ(pcBeforeBrk + 2, pushedPc);

    // B flag is STILL SET even though NMI handled
    uint8_t pushedStatus = memory.ReadByte(0x01FD);
    EXPECT_EQ(1, (pushedStatus >> 4) & 1);

    EXPECT_EQ(7, cycles);
}

TEST_F(Cpu6502Test, BRK_StackWrap)
{
    SetupMemory(Memory6502::kRomStart, { 0x00, 0x00 });
    SetupMemory(0xFFFE, { 0x00, 0xC0 });
    
    // Set stack pointer near bottom
    memory.StackPointer = 0x02;
    
    cpu.ExecuteInstruction();
    
    // Stack should wrap: 0x02 -> 0x01 -> 0x00 -> 0xFF
    EXPECT_EQ(0xFF, cpu.GetStackPointer());
    
    // Data wraps around stack page
    EXPECT_EQ(0xC000, cpu.PC);  // Still executes correctly
}

TEST_F(Cpu6502Test, RTI_Basic)
{
    // Setup RTI instruction
    SetupMemory(Memory6502::kRomStart, { 0x40 });
    
    // Manually setup stack as if interrupt occurred
    memory.PushWord(0x8050);
    memory.PushByte(0b11100101);

    uint8_t cycles = cpu.ExecuteInstruction();
    
    // Check PC restored
    EXPECT_EQ(0x8050, cpu.PC);
    
    // Check status flags restored
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(0, cpu.StatusReg.GetDecimal());
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    
    // Check stack pointer restored
    EXPECT_EQ(0xFF, cpu.GetStackPointer());
    
    EXPECT_EQ(6, cycles);
}

TEST_F(Cpu6502Test, RTI_IgnoresBFlag)
{
    SetupMemory(Memory6502::kRomStart, { 0x40 });
    
    memory.PushWord(0x8000);

    // Push status with B flag set (as BRK would)
    memory.PushByte(0b00110000);
    
    cpu.ExecuteInstruction();
    
    // B flag should not affect status register
    uint8_t status = cpu.StatusReg.GetRegister();

    // Check that bit 4 (B flag) is not set in actual register
    EXPECT_EQ(0, (status >> 4) & 1);
    EXPECT_EQ(1, (status >> 5) & 1);
}

TEST_F(Cpu6502Test, BRK_RTI_RoundTrip)
{
    // Setup BRK and RTI
    SetupMemory(Memory6502::kRomStart, { 0x00, 0x00, 0xA9, 0x42 });
    SetupMemory(0xFFFE, { 0x00, 0xC0 });
    SetupMemory(0xC000, { 0x40 });
    
    // Set some flags before BRK
    cpu.StatusReg.SetCarry(1);
    cpu.StatusReg.SetZero(1);
    cpu.StatusReg.SetOverflow(1);
    
    uint16_t originalPC = cpu.PC;
    
    // Execute BRK
    cpu.ExecuteInstruction();
    EXPECT_EQ(0xC000, cpu.PC);
    EXPECT_EQ(0xFC, cpu.GetStackPointer());
    
    // Execute RTI
    cpu.ExecuteInstruction();
    EXPECT_EQ(originalPC + 2, cpu.PC);
    EXPECT_EQ(0xFF, cpu.GetStackPointer());
    
    // Flags restored (except I flag which BRK set)
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
}

TEST_F(Cpu6502Test, NMI_RTI_RoundTrip)
{
    // Setup code and NMI handler
    SetupMemory(Memory6502::kRomStart, { 0xA9, 0x42 });
    SetupMemory(0xFFFA, { 0x00, 0xD0 });
    SetupMemory(0xD000, { 0x40 });
    
    // Set some flags
    cpu.StatusReg.SetNegative(1);
    cpu.StatusReg.SetCarry(1);
    
    uint16_t originalPC = cpu.PC;
    uint8_t originalStatus = cpu.StatusReg.GetRegister();
    
    // Trigger and handle NMI
    cpu.TriggerNMI();
    cpu.ExecuteInstruction();
    EXPECT_EQ(0xD000, cpu.PC);
    
    // Execute RTI
    cpu.ExecuteInstruction();
    EXPECT_EQ(originalPC + 2, cpu.PC);
    
    // Flags restored (NMI doesn't set I flag)
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
}

TEST_F(Cpu6502Test, RTI_RestoresAllFlags)
{
    SetupMemory(Memory6502::kRomStart, { 0x40 });
    
    memory.PushWord(0x9000);

    // Push all flags set
    memory.PushByte(0b11111111);
    
    cpu.ExecuteInstruction();
    
    EXPECT_EQ(0x9000, cpu.PC);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetOverflow());

    // Bit 5 (unused) - depends on implementation
    // Bit 4 (B) should be ignored
    EXPECT_EQ(1, cpu.StatusReg.GetDecimal());
    EXPECT_EQ(1, cpu.StatusReg.GetInterruptDisable());
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(1, cpu.StatusReg.GetUnused());
    EXPECT_EQ(0, cpu.StatusReg.GetBreak());
}

TEST_F(Cpu6502Test, RTI_StackNearWrap)
{
    SetupMemory(Memory6502::kRomStart, { 0x40 });
    
    // Set stack pointer near top (wrapping scenario)
    memory.StackPointer = 0xFD;
    
    // Manually setup stack (wraps around)
    memory.WriteByte(0x01FE, 0b10000001);
    memory.WriteByte(0x01FF, 0x34);
    memory.WriteByte(0x0100, 0x12);
    
    cpu.ExecuteInstruction();
    
    // Should handle wrap correctly
    EXPECT_EQ(0x1234, cpu.PC);
    EXPECT_EQ(1, cpu.StatusReg.GetNegative());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
    EXPECT_EQ(0x00, cpu.GetStackPointer());
}

TEST_F(Cpu6502Test, RTI_CanClearInterruptFlag)
{
    SetupMemory(Memory6502::kRomStart, { 0x40 });
    
    // Set I flag in CPU initially
    cpu.StatusReg.SetInterruptDisable(1);
    
    memory.PushWord(0x8000);

    // Push status with I=0
    memory.PushByte(0b00000000);
    
    cpu.ExecuteInstruction();
    
    // I flag should be cleared (restored from stack)
    EXPECT_EQ(0, cpu.StatusReg.GetInterruptDisable());
}

TEST_F(Cpu6502Test, RTI_NestedInterrupts)
{
    // First interrupt pushes to stack
    memory.PushWord(0x8000);
    memory.PushByte(0b00000001);
    
    // Second interrupt pushes to stack
    memory.PushWord(0x9000);
    memory.PushByte(0b00000010);
    
    SetupMemory(Memory6502::kRomStart, { 0x40, 0x40 });
    
    // First RTI - returns from second interrupt
    cpu.ExecuteInstruction();
    EXPECT_EQ(0x9000, cpu.PC);
    EXPECT_EQ(1, cpu.StatusReg.GetZero());
    EXPECT_EQ(0, cpu.StatusReg.GetCarry());
    
    // Second RTI - returns from first interrupt
    cpu.PC = Memory6502::kRomStart + 1;
    cpu.ExecuteInstruction();
    EXPECT_EQ(0x8000, cpu.PC);
    EXPECT_EQ(0, cpu.StatusReg.GetZero());
    EXPECT_EQ(1, cpu.StatusReg.GetCarry());
}