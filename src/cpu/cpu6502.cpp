#include "cpu/cpu6502.hpp"
#include "cpu/opcode.hpp"
#include "utils.hpp"

Cpu6502::Cpu6502(Memory6502& mem) : memory(mem)
{
    initOpcodeTable();
}

void Cpu6502::initOpcodeTable()
{
    // LDA
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_IMM)]          = &Cpu6502::LDAImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ZEROPAGE)]     = &Cpu6502::LDAZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ZEROPAGEX)]    = &Cpu6502::LDAZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ABSOLUTE)]     = &Cpu6502::LDAAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ABSOLUTEX)]    = &Cpu6502::LDAAbsoluteX;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_ABSOLUTEY)]    = &Cpu6502::LDAAbsoluteY;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_INDIRECTX)]    = &Cpu6502::LDAIndirectX;
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_INDIRECTY)]    = &Cpu6502::LDAIndirectY;
    
    // LDX
    opcodeTable[static_cast<uint8_t>(Opcode::LDX_IMMEDIATE)]    = &Cpu6502::LDXImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::LDX_ZEROPAGE)]     = &Cpu6502::LDXZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::LDX_ZEROPAGEY)]    = &Cpu6502::LDXZeroPageY;
    opcodeTable[static_cast<uint8_t>(Opcode::LDX_ABSOLUTE)]     = &Cpu6502::LDXAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::LDX_ABSOLUTEY)]    = &Cpu6502::LDXAbsoluteY;

    // LDY
    opcodeTable[static_cast<uint8_t>(Opcode::LDY_IMMEDIATE)]    = &Cpu6502::LDYImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::LDY_ZEROPAGE)]     = &Cpu6502::LDYZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::LDY_ZEROPAGEX)]    = &Cpu6502::LDYZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::LDY_ABSOLUTE)]     = &Cpu6502::LDYAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::LDY_ABSOLUTEX)]    = &Cpu6502::LDYAbsoluteX;

    // STA
    opcodeTable[static_cast<uint8_t>(Opcode::STA_ZEROPAGE)]     = &Cpu6502::STAZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::STA_ZEROPAGEX)]    = &Cpu6502::STAZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::STA_ABSOLUTE)]     = &Cpu6502::STAAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::STA_ABSOLUTEX)]    = &Cpu6502::STAAbsoluteX;
    opcodeTable[static_cast<uint8_t>(Opcode::STA_ABSOLUTEY)]    = &Cpu6502::STAAbsoluteY;
    opcodeTable[static_cast<uint8_t>(Opcode::STA_INDIRECTX)]    = &Cpu6502::STAIndirectX;
    opcodeTable[static_cast<uint8_t>(Opcode::STA_INDIRECTY)]    = &Cpu6502::STAIndirectY;

    // STX
    opcodeTable[static_cast<uint8_t>(Opcode::STX_ZEROPAGE)]     = &Cpu6502::STXZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::STX_ZEROPAGEY)]    = &Cpu6502::STXZeroPageY;
    opcodeTable[static_cast<uint8_t>(Opcode::STX_ABSOLUTE)]     = &Cpu6502::STXAbsolute;

    // STY
    opcodeTable[static_cast<uint8_t>(Opcode::STY_ZEROPAGE)]     = &Cpu6502::STYZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::STY_ZEROPAGEX)]    = &Cpu6502::STYZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::STY_ABSOLUTE)]     = &Cpu6502::STYAbsolute;

    // Transfer
    opcodeTable[static_cast<uint8_t>(Opcode::TAX_IMPLIED)]      = &Cpu6502::TAX;
    opcodeTable[static_cast<uint8_t>(Opcode::TXA_IMPLIED)]      = &Cpu6502::TXA;
    opcodeTable[static_cast<uint8_t>(Opcode::TAY_IMPLIED)]      = &Cpu6502::TAY;
    opcodeTable[static_cast<uint8_t>(Opcode::TYA_IMPLIED)]      = &Cpu6502::TYA;

    // ADC
    opcodeTable[static_cast<uint8_t>(Opcode::ADC_IMMEDIATE)]    = &Cpu6502::ADCImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::ADC_ZEROPAGE)]     = &Cpu6502::ADCZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::ADC_ZEROPAGEX)]    = &Cpu6502::ADCZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::ADC_ABSOLUTE)]     = &Cpu6502::ADCAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::ADC_ABSOLUTEX)]    = &Cpu6502::ADCAbsoluteX;
    opcodeTable[static_cast<uint8_t>(Opcode::ADC_ABSOLUTEY)]    = &Cpu6502::ADCAbsoluteY;
    opcodeTable[static_cast<uint8_t>(Opcode::ADC_INDIRECTX)]    = &Cpu6502::ADCIndirectX;
    opcodeTable[static_cast<uint8_t>(Opcode::ADC_INDIRECTY)]    = &Cpu6502::ADCIndirectY;

    // SBC
    opcodeTable[static_cast<uint8_t>(Opcode::SBC_IMMEDIATE)]    = &Cpu6502::SBCImmediate;
}

void Cpu6502::Reset()
{
    StatusReg.SetRegister(0x00);
    PC = Memory6502::kRomStart;
    A = 0;
    X = 0;
    Y = 0;
    SP = 0xFF;
}

void Cpu6502::ExecuteInstruction()
{
    StatusReg.SetRegister(0x00);
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

void Cpu6502::LDX(uint16_t address)
{
    X = memory.ReadByte(address);
    SetNZFlags(X);
}

void Cpu6502::LDXImmediate()
{
    LDX(AddressingImmediate());
}

void Cpu6502::LDXZeroPage()
{
    LDX(AddressingZeroPage());
}

void Cpu6502::LDXZeroPageY()
{
    LDX(AddressingZeroPageY());
}

void Cpu6502::LDXAbsolute()
{
    LDX(AddressingAbsolute());
}

void Cpu6502::LDXAbsoluteY()
{
    LDX(AddressingAbsoluteY());
}

void Cpu6502::LDY(uint16_t address)
{
    Y = memory.ReadByte(address);
    SetNZFlags(Y);
}

void Cpu6502::LDYImmediate()
{
    LDY(AddressingImmediate());
}

void Cpu6502::LDYZeroPage()
{
    LDY(AddressingZeroPage());
}

void Cpu6502::LDYZeroPageX()
{
    LDY(AddressingZeroPageX());
}

void Cpu6502::LDYAbsolute()
{
    LDY(AddressingAbsolute());
}

void Cpu6502::LDYAbsoluteX()
{
    LDY(AddressingAbsoluteX());
}

void Cpu6502::STA(uint16_t address)
{
    memory.WriteByte(address, A);
    SetNZFlags(A);
}

void Cpu6502::STAZeroPage()
{
    STA(AddressingZeroPage());
}

void Cpu6502::STAZeroPageX()
{
    STA(AddressingZeroPageX());
}

void Cpu6502::STAAbsolute()
{
    STA(AddressingAbsolute());
}

void Cpu6502::STAAbsoluteX()
{
    STA(AddressingAbsoluteX());
}

void Cpu6502::STAAbsoluteY()
{
    STA(AddressingAbsoluteY());
}

void Cpu6502::STAIndirectX()
{
    STA(AddressingIndirectX());
}

void Cpu6502::STAIndirectY()
{
    STA(AddressingIndirectY());
}

void Cpu6502::STX(uint16_t address)
{
    memory.WriteByte(address, X);
    SetNZFlags(X);
}

void Cpu6502::STXZeroPage()
{
    STX(AddressingZeroPage());
}

void Cpu6502::STXZeroPageY()
{
    STX(AddressingZeroPageY());
}

void Cpu6502::STXAbsolute()
{
    STX(AddressingAbsolute());
}

void Cpu6502::STY(uint16_t address)
{
    memory.WriteByte(address, Y);
    SetNZFlags(Y);
}

void Cpu6502::STYZeroPage()
{
    STY(AddressingZeroPage());
}

void Cpu6502::STYZeroPageX()
{
    STY(AddressingZeroPageX());
}

void Cpu6502::STYAbsolute()
{
    STY(AddressingAbsolute());
}

void Cpu6502::TAX()
{
    X = A;
    SetNZFlags(X);
}

void Cpu6502::TXA()
{
    A = X;
    SetNZFlags(A);
}

void Cpu6502::TAY()
{
    Y = A;
    SetNZFlags(Y);
}

void Cpu6502::TYA()
{
    A = Y;
    SetNZFlags(A);
}

void Cpu6502::ADC(uint16_t address)
{
    uint8_t memoryValue = memory.ReadByte(address);
    uint16_t result = A + memoryValue + (StatusReg.GetCarry() ? 1 : 0);

    StatusReg.SetCarry(result > 0xFF);

    bool overflow = ((A ^ result) & (memoryValue ^ result) & 0x80) != 0;
    StatusReg.SetOverflow(overflow);

    A = static_cast<uint8_t>(result & 0xFF);

    SetNZFlags(A);
}

void Cpu6502::ADCImmediate()
{
    ADC(AddressingImmediate());
}

void Cpu6502::ADCZeroPage()
{
    ADC(AddressingZeroPage());
}

void Cpu6502::ADCZeroPageX()
{
    ADC(AddressingZeroPageX());
}

void Cpu6502::ADCAbsolute()
{
    ADC(AddressingAbsolute());
}

void Cpu6502::ADCAbsoluteX()
{
    ADC(AddressingAbsoluteX());
}

void Cpu6502::ADCAbsoluteY()
{
    ADC(AddressingAbsoluteY());
}

void Cpu6502::ADCIndirectX()
{
    ADC(AddressingIndirectX());
}

void Cpu6502::ADCIndirectY()
{
    ADC(AddressingIndirectY());
}

void Cpu6502::SBC(uint16_t address)
{
    uint8_t memoryValue = memory.ReadByte(address);
    
    uint16_t result = A - memoryValue - !StatusReg.GetCarry(); 
    
    StatusReg.SetCarry(!(result > 0xFF));
    StatusReg.SetOverflow((result ^ A) & (result ^ memoryValue) & 0x80);
    
    A = static_cast<uint8_t>(result & 0xFF);
    SetNZFlags(A);
}

void Cpu6502::SBCImmediate()
{
    SBC(AddressingImmediate());
}

void Cpu6502::SetNZFlags(uint8_t value)
{
    StatusReg.SetZero(value == 0);
    StatusReg.SetNegative((value & 0x80) != 0);
}

void Cpu6502::HandlePageCross(uint16_t baseAddress, uint16_t effectiveAddress)
{
    if ((baseAddress & 0xFF00) != (effectiveAddress & 0xFF00))
    {
        // TODO update cycles.
    }
}
