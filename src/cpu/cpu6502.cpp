#include "cpu/cpu6502.hpp"
#include "cpu/opcode.hpp"
#include "utils.hpp"

Cpu6502::Cpu6502(Memory& mem) : memory(mem)
{
    initOpcodeTable();
}

void Cpu6502::initOpcodeTable()
{
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_IMM)]          = &Cpu6502::LDAImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ZEROPAGE)]     = &Cpu6502::LDAZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ZEROPAGEX)]    = &Cpu6502::LDAZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ABSOLUTE)]     = &Cpu6502::LDAAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ABSOLUTEX)]    = &Cpu6502::LDAAbsoluteX;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ABSOLUTEY)]    = &Cpu6502::LDAAbsoluteY;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_INDIRECTX)]    = &Cpu6502::LDAIndirectX;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_INDIRECTY)]    = &Cpu6502::LDAIndirectY;
}

void Cpu6502::Reset()
{
    StatusRegister.SetRegister(0x00);
    PC = Memory::kRomStart;
    A = 0;
    X = 0;
    Y = 0;
    SP = 0xFF;
}

void Cpu6502::ExecuteInstruction()
{
    uint8_t opcode = memory.ReadByte(PC);
    PC++;

    OpcodeHandler handler = opcodeTable[opcode];
    if (handler == nullptr)
        throw std::runtime_error(Utils::to_hex(opcode) + " is an unknown opcode.");

    (this->*handler)();
}

uint16_t Cpu6502::AddressingImmediate()
{
    uint16_t address = PC;
    PC++;
    return address;
}

uint16_t Cpu6502::AddressingZeroPage()
{
    uint8_t address = memory.ReadByte(PC);
    PC++;
    return static_cast<uint16_t>(address);
}

uint16_t Cpu6502::AddressingZeroPageX()
{
    uint8_t baseAddress = memory.ReadByte(PC);
    PC++;
    // Zero page wraps around (0xFF + 1 = 0x00)
    return static_cast<uint16_t>((baseAddress + X) & 0xFF);
}

uint16_t Cpu6502::AddressingZeroPageY()
{
    uint8_t baseAddress = memory.ReadByte(PC);
    PC++;
    return static_cast<uint16_t>((baseAddress + Y) & 0xFF);
}

uint16_t Cpu6502::AddressingAbsolute()
{
    uint16_t address = memory.ReadWord(PC);
    PC += 2;
    return address;
}

uint16_t Cpu6502::AddressingAbsoluteX()
{
    uint16_t baseAddress = memory.ReadWord(PC);
    PC += 2;
    uint16_t effectiveAddress = baseAddress + X;
    HandlePageCross(baseAddress, effectiveAddress);
    return effectiveAddress;
}

uint16_t Cpu6502::AddressingAbsoluteY()
{
    uint16_t baseAddress = memory.ReadWord(PC);
    PC += 2;
    uint16_t effectiveAddress = baseAddress + Y;
    HandlePageCross(baseAddress, effectiveAddress);
    return effectiveAddress;
}

uint16_t Cpu6502::AddressingIndirectX()
{
    uint8_t baseAddress = memory.ReadByte(PC);
    PC++;
    uint8_t effectiveAddress = (baseAddress + X) & 0xFF;
    return memory.ReadWord(effectiveAddress);
}

uint16_t Cpu6502::AddressingIndirectY()
{
    uint8_t indirectAddress = memory.ReadByte(PC);
    PC++;
    uint16_t baseAddress = memory.ReadWord(indirectAddress);
    uint16_t effectiveAddress = baseAddress + Y;
    HandlePageCross(baseAddress, effectiveAddress);
    return effectiveAddress;
}

void Cpu6502::LDA(uint16_t address)
{
    A = memory.ReadByte(address);
    SetNZFlags(A);
}

void Cpu6502::LDAImmediate()
{
    LDA(AddressingImmediate());
}

void Cpu6502::LDAZeroPage()
{
    LDA(AddressingZeroPage());
}

void Cpu6502::LDAZeroPageX()
{
    LDA(AddressingZeroPageX());
}

void Cpu6502::LDAAbsolute()
{
    LDA(AddressingAbsolute());
}

void Cpu6502::LDAAbsoluteX()
{
    LDA(AddressingAbsoluteX());
}

void Cpu6502::LDAAbsoluteY()
{
    LDA(AddressingAbsoluteY());
}

void Cpu6502::LDAIndirectX()
{
    LDA(AddressingIndirectX());
}

void Cpu6502::LDAIndirectY()
{
    LDA(AddressingIndirectY());
}

void Cpu6502::SetNZFlags(uint8_t value)
{
    StatusRegister.SetZero(value == 0);
    StatusRegister.SetNegative((value & 0x80) != 0);
}

void Cpu6502::HandlePageCross(uint16_t baseAddress, uint16_t effectiveAddress)
{
    if ((baseAddress & 0xFF00) != (effectiveAddress & 0xFF00))
    {
        // TODO update cycles.
    }
}