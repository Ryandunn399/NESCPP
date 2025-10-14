#include <gtest/gtest.h>
#include "tests/cpu_fixture.hpp"

TEST_F(Cpu6502Test, BCC_BranchNotTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x90, 0x32 });
    cpu.StatusReg.SetCarry(1);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2, cpu.PC);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, BCC_BranchTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x90, 0x32 });
    cpu.StatusReg.SetCarry(0);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, BCC_BranchTakenNegative)
{
    uint16_t initOffset = 0xA000;
    SetupMemory(Memory6502::kRomStart + initOffset, { 0x90, 0x9E });
    cpu.StatusReg.SetCarry(0);

    int8_t bcOffset = static_cast<int8_t>(0x9E);
    cpu.PC += initOffset;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 +  bcOffset, cpu.PC);
    EXPECT_GT(oldPc, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BCC_BranchTakenPageCross)
{
    SetupMemory(Memory6502::kRomStart + 0xFD, { 0x90, 0x32 });
    cpu.StatusReg.SetCarry(0);
    cpu.PC += 0xFD;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BCS_BranchNotTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0xB0, 0x32 });
    cpu.StatusReg.SetCarry(0);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2, cpu.PC);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, BCS_BranchTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0xB0, 0x32 });
    cpu.StatusReg.SetCarry(1);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, BCS_BranchTakenNegative)
{
    uint16_t initOffset = 0xA000;
    SetupMemory(Memory6502::kRomStart + initOffset, { 0xB0, 0x9E });
    cpu.StatusReg.SetCarry(1);

    int8_t bcOffset = static_cast<int8_t>(0x9E);
    cpu.PC += initOffset;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 +  bcOffset, cpu.PC);
    EXPECT_GT(oldPc, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BCS_BranchTakenPageCross)
{
    SetupMemory(Memory6502::kRomStart + 0xFD, { 0xB0, 0x32 });
    cpu.StatusReg.SetCarry(1);
    cpu.PC += 0xFD;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BEQ_BranchNotTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0xF0, 0x32 });
    cpu.StatusReg.SetZero(0);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2, cpu.PC);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, BEQ_BranchTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0xF0, 0x32 });
    cpu.StatusReg.SetZero(1);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, BEQ_BranchTakenNegative)
{
    uint16_t initOffset = 0xA000;
    SetupMemory(Memory6502::kRomStart + initOffset, { 0xF0, 0x9E });
    cpu.StatusReg.SetZero(1);

    int8_t bcOffset = static_cast<int8_t>(0x9E);
    cpu.PC += initOffset;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 +  bcOffset, cpu.PC);
    EXPECT_GT(oldPc, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BEQ_BranchTakenPageCross)
{
    SetupMemory(Memory6502::kRomStart + 0xFD, { 0xF0, 0x32 });
    cpu.StatusReg.SetZero(1);
    cpu.PC += 0xFD;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BNE_BranchNotTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0xD0, 0x32 });
    cpu.StatusReg.SetZero(1);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2, cpu.PC);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, BNE_BranchTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0xD0, 0x32 });
    cpu.StatusReg.SetZero(0);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, BNE_BranchTakenNegative)
{
    uint16_t initOffset = 0xA000;
    SetupMemory(Memory6502::kRomStart + initOffset, { 0xD0, 0x9E });
    cpu.StatusReg.SetZero(0);

    int8_t bcOffset = static_cast<int8_t>(0x9E);
    cpu.PC += initOffset;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 +  bcOffset, cpu.PC);
    EXPECT_GT(oldPc, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BNE_BranchTakenPageCross)
{
    SetupMemory(Memory6502::kRomStart + 0xFD, { 0xD0, 0x32 });
    cpu.StatusReg.SetZero(0);
    cpu.PC += 0xFD;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BPL_BranchNotTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x10, 0x32 });
    cpu.StatusReg.SetNegative(1);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2, cpu.PC);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, BPL_BranchTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x10, 0x32 });
    cpu.StatusReg.SetNegative(0);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, BPL_BranchTakenNegative)
{
    uint16_t initOffset = 0xA000;
    SetupMemory(Memory6502::kRomStart + initOffset, { 0x10, 0x9E });
    cpu.StatusReg.SetNegative(0);

    int8_t bcOffset = static_cast<int8_t>(0x9E);
    cpu.PC += initOffset;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 +  bcOffset, cpu.PC);
    EXPECT_GT(oldPc, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BPL_BranchTakenPageCross)
{
    SetupMemory(Memory6502::kRomStart + 0xFD, { 0x10, 0x32 });
    cpu.StatusReg.SetNegative(0);
    cpu.PC += 0xFD;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BMI_BranchNotTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x30, 0x32 });
    cpu.StatusReg.SetNegative(0);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2, cpu.PC);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, BMI_BranchTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x30, 0x32 });
    cpu.StatusReg.SetNegative(1);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, BMI_BranchTakenNegative)
{
    uint16_t initOffset = 0xA000;
    SetupMemory(Memory6502::kRomStart + initOffset, { 0x30, 0x9E });
    cpu.StatusReg.SetNegative(1);

    int8_t bcOffset = static_cast<int8_t>(0x9E);
    cpu.PC += initOffset;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 +  bcOffset, cpu.PC);
    EXPECT_GT(oldPc, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BMI_BranchTakenPageCross)
{
    SetupMemory(Memory6502::kRomStart + 0xFD, { 0x30, 0x32 });
    cpu.StatusReg.SetNegative(1);
    cpu.PC += 0xFD;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BVC_BranchNotTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x50, 0x32 });
    cpu.StatusReg.SetOverflow(1);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2, cpu.PC);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, BVC_BranchTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x50, 0x32 });
    cpu.StatusReg.SetOverflow(0);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, BVC_BranchTakenNegative)
{
    uint16_t initOffset = 0xA000;
    SetupMemory(Memory6502::kRomStart + initOffset, { 0x50, 0x9E });
    cpu.StatusReg.SetOverflow(0);

    int8_t bcOffset = static_cast<int8_t>(0x9E);
    cpu.PC += initOffset;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 +  bcOffset, cpu.PC);
    EXPECT_GT(oldPc, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BVC_BranchTakenPageCross)
{
    SetupMemory(Memory6502::kRomStart + 0xFD, { 0x50, 0x32 });
    cpu.StatusReg.SetOverflow(0);
    cpu.PC += 0xFD;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BVS_BranchNotTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x70, 0x32 });
    cpu.StatusReg.SetOverflow(0);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2, cpu.PC);
    EXPECT_EQ(2, cycles);
}

TEST_F(Cpu6502Test, BVS_BranchTaken)
{
    SetupMemory(Memory6502::kRomStart, { 0x70, 0x32 });
    cpu.StatusReg.SetOverflow(1);

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(3, cycles);
}

TEST_F(Cpu6502Test, BVS_BranchTakenNegative)
{
    uint16_t initOffset = 0xA000;
    SetupMemory(Memory6502::kRomStart + initOffset, { 0x70, 0x9E });
    cpu.StatusReg.SetOverflow(1);

    int8_t bcOffset = static_cast<int8_t>(0x9E);
    cpu.PC += initOffset;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 +  bcOffset, cpu.PC);
    EXPECT_GT(oldPc, cpu.PC);
    EXPECT_EQ(4, cycles);
}

TEST_F(Cpu6502Test, BVS_BranchTakenPageCross)
{
    SetupMemory(Memory6502::kRomStart + 0xFD, { 0x70, 0x32 });
    cpu.StatusReg.SetOverflow(1);
    cpu.PC += 0xFD;

    uint16_t oldPc = cpu.PC;

    uint8_t cycles = cpu.ExecuteInstruction();

    EXPECT_EQ(oldPc + 2 + 0x32, cpu.PC);
    EXPECT_EQ(4, cycles);
}