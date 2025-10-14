#include "cpu/cpu6502.hpp"
#include "cpu/opcode.hpp"
#include "utils.hpp"

Cpu6502::Cpu6502(Memory6502& mem) : memory(mem)
{
    initOpcodeTable();
    initInstructionCycleTable();
}

void Cpu6502::initOpcodeTable()
{
    // LDA
    opcodeTable[static_cast<uint8_t>(Opcode::LDA_IMMEDIATE)]    = &Cpu6502::LDAImmediate;
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
    opcodeTable[static_cast<uint8_t>(Opcode::SBC_ZEROPAGE)]     = &Cpu6502::SBCZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::SBC_ZEROPAGEX)]    = &Cpu6502::SBCZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::SBC_ABSOLUTE)]     = &Cpu6502::SBCAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::SBC_ABSOLUTEX)]    = &Cpu6502::SBCAbsoluteX;
    opcodeTable[static_cast<uint8_t>(Opcode::SBC_ABSOLUTEY)]    = &Cpu6502::SBCAbsoluteY;
    opcodeTable[static_cast<uint8_t>(Opcode::SBC_INDIRECTX)]    = &Cpu6502::SBCIndirectX;
    opcodeTable[static_cast<uint8_t>(Opcode::SBC_INDIRECTY)]    = &Cpu6502::SBCIndirectY;

    // INC
    opcodeTable[static_cast<uint8_t>(Opcode::INC_ZEROPAGE)]     = &Cpu6502::INCZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::INC_ZEROPAGEX)]    = &Cpu6502::INCZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::INC_ABSOLUTE)]     = &Cpu6502::INCAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::INC_ABSOLUTEX)]    = &Cpu6502::INCAbsoluteX;

    // DEC
    opcodeTable[static_cast<uint8_t>(Opcode::DEC_ZEROPAGE)]     = &Cpu6502::DECZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::DEC_ZEROPAGEX)]    = &Cpu6502::DECZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::DEC_ABSOLUTE)]     = &Cpu6502::DECAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::DEC_ABSOLUTEX)]    = &Cpu6502::DECAbsoluteX;

    // INX
    opcodeTable[static_cast<uint8_t>(Opcode::INX)]              = &Cpu6502::INX;
    
    // DEX
    opcodeTable[static_cast<uint8_t>(Opcode::DEX)]              = &Cpu6502::DEX;

    // INY
    opcodeTable[static_cast<uint8_t>(Opcode::INY)]              = &Cpu6502::INY;
    
    // DEY
    opcodeTable[static_cast<uint8_t>(Opcode::DEY)]              = &Cpu6502::DEY;

    // ASL
    opcodeTable[static_cast<uint8_t>(Opcode::ASL_ACCUMULATOR)]  = &Cpu6502::ASLAccumulator;
    opcodeTable[static_cast<uint8_t>(Opcode::ASL_ZEROPAGE)]     = &Cpu6502::ASLZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::ASL_ZEROPAGEX)]    = &Cpu6502::ASLZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::ASL_ABSOLUTE)]     = &Cpu6502::ASLAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::ASL_ABSOLUTEX)]    = &Cpu6502::ASLAbsoluteX;

    // LSR
    opcodeTable[static_cast<uint8_t>(Opcode::LSR_ACCUMULATOR)]  = &Cpu6502::LSRAccumulator;
    opcodeTable[static_cast<uint8_t>(Opcode::LSR_ZEROPAGE)]     = &Cpu6502::LSRZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::LSR_ZEROPAGEX)]    = &Cpu6502::LSRZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::LSR_ABSOLUTE)]     = &Cpu6502::LSRAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::LSR_ABSOLUTEX)]    = &Cpu6502::LSRAbsoluteX;

    // ROL
    opcodeTable[static_cast<uint8_t>(Opcode::ROL_ACCUMULATOR)]  = &Cpu6502::ROLAccumulator;
    opcodeTable[static_cast<uint8_t>(Opcode::ROL_ZEROPAGE)]     = &Cpu6502::ROLZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::ROL_ZEROPAGEX)]    = &Cpu6502::ROLZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::ROL_ABSOLUTE)]     = &Cpu6502::ROLAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::ROL_ABSOLUTEX)]    = &Cpu6502::ROLAbsoluteX;

    // ROR
    opcodeTable[static_cast<uint8_t>(Opcode::ROR_ACCUMULATOR)]  = &Cpu6502::RORAccumulator;
    opcodeTable[static_cast<uint8_t>(Opcode::ROR_ZEROPAGE)]     = &Cpu6502::RORZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::ROR_ZEROPAGEX)]    = &Cpu6502::RORZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::ROR_ABSOLUTE)]     = &Cpu6502::RORAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::ROR_ABSOLUTEX)]    = &Cpu6502::RORAbsoluteX;

    // AND
    opcodeTable[static_cast<uint8_t>(Opcode::AND_IMMEDIATE)]    = &Cpu6502::ANDImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::AND_ZEROPAGE)]     = &Cpu6502::ANDZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::AND_ZEROPAGEX)]    = &Cpu6502::ANDZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::AND_ABSOLUTE)]     = &Cpu6502::ANDAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::AND_ABSOLUTEX)]    = &Cpu6502::ANDAbsoluteX;
    opcodeTable[static_cast<uint8_t>(Opcode::AND_ABSOLUTEY)]    = &Cpu6502::ANDAbsoluteY;
    opcodeTable[static_cast<uint8_t>(Opcode::AND_INDIRECTX)]    = &Cpu6502::ANDIndirectX;
    opcodeTable[static_cast<uint8_t>(Opcode::AND_INDIRECTY)]    = &Cpu6502::ANDIndirectY;

    // ORA
    opcodeTable[static_cast<uint8_t>(Opcode::ORA_IMMEDIATE)]    = &Cpu6502::ORAImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::ORA_ZEROPAGE)]     = &Cpu6502::ORAZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::ORA_ZEROPAGEX)]    = &Cpu6502::ORAZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::ORA_ABSOLUTE)]     = &Cpu6502::ORAAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::ORA_ABSOLUTEX)]    = &Cpu6502::ORAAbsoluteX;
    opcodeTable[static_cast<uint8_t>(Opcode::ORA_ABSOLUTEY)]    = &Cpu6502::ORAAbsoluteY;
    opcodeTable[static_cast<uint8_t>(Opcode::ORA_INDIRECTX)]    = &Cpu6502::ORAIndirectX;
    opcodeTable[static_cast<uint8_t>(Opcode::ORA_INDIRECTY)]    = &Cpu6502::ORAIndirectY;

    // EOR
    opcodeTable[static_cast<uint8_t>(Opcode::EOR_IMMEDIATE)]    = &Cpu6502::EORImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::EOR_ZEROPAGE)]     = &Cpu6502::EORZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::EOR_ZEROPAGEX)]    = &Cpu6502::EORZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::EOR_ABSOLUTE)]     = &Cpu6502::EORAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::EOR_ABSOLUTEX)]    = &Cpu6502::EORAbsoluteX;
    opcodeTable[static_cast<uint8_t>(Opcode::EOR_ABSOLUTEY)]    = &Cpu6502::EORAbsoluteY;
    opcodeTable[static_cast<uint8_t>(Opcode::EOR_INDIRECTX)]    = &Cpu6502::EORIndirectX;
    opcodeTable[static_cast<uint8_t>(Opcode::EOR_INDIRECTY)]    = &Cpu6502::EORIndirectY;

    // BIT
    opcodeTable[static_cast<uint8_t>(Opcode::BIT_ZEROPAGE)]     = &Cpu6502::BITZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::BIT_ABSOLUTE)]     = &Cpu6502::BITAbsolute;
    
    // CMP
    opcodeTable[static_cast<uint8_t>(Opcode::CMP_IMMEDIATE)]    = &Cpu6502::CMPImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::CMP_ZEROPAGE)]     = &Cpu6502::CMPZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::CMP_ZEROPAGEX)]    = &Cpu6502::CMPZeroPageX;
    opcodeTable[static_cast<uint8_t>(Opcode::CMP_ABSOLUTE)]     = &Cpu6502::CMPAbsolute;
    opcodeTable[static_cast<uint8_t>(Opcode::CMP_ABSOLUTEX)]    = &Cpu6502::CMPAbsoluteX;
    opcodeTable[static_cast<uint8_t>(Opcode::CMP_ABSOLUTEY)]    = &Cpu6502::CMPAbsoluteY;
    opcodeTable[static_cast<uint8_t>(Opcode::CMP_INDIRECTX)]    = &Cpu6502::CMPIndirectX;
    opcodeTable[static_cast<uint8_t>(Opcode::CMP_INDIRECTY)]    = &Cpu6502::CMPIndirectY;

    // CPX
    opcodeTable[static_cast<uint8_t>(Opcode::CPX_IMMEDIATE)]    = &Cpu6502::CPXImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::CPX_ZEROPAGE)]     = &Cpu6502::CPXZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::CPX_ABSOLUTE)]     = &Cpu6502::CPXAbsolute;

    // CPY
    opcodeTable[static_cast<uint8_t>(Opcode::CPY_IMMEDIATE)]    = &Cpu6502::CPYImmediate;
    opcodeTable[static_cast<uint8_t>(Opcode::CPY_ZEROPAGE)]     = &Cpu6502::CPYZeroPage;
    opcodeTable[static_cast<uint8_t>(Opcode::CPY_ABSOLUTE)]     = &Cpu6502::CPYAbsolute;

    // BRANCH
    opcodeTable[static_cast<uint8_t>(Opcode::BCC_RELATIVE)]     = &Cpu6502::BCC;
    opcodeTable[static_cast<uint8_t>(Opcode::BCS_RELATIVE)]     = &Cpu6502::BCS;
    opcodeTable[static_cast<uint8_t>(Opcode::BEQ_RELATIVE)]     = &Cpu6502::BEQ;
    opcodeTable[static_cast<uint8_t>(Opcode::BNE_RELATIVE)]     = &Cpu6502::BNE;
    opcodeTable[static_cast<uint8_t>(Opcode::BPL_RELATIVE)]     = &Cpu6502::BPL;
    opcodeTable[static_cast<uint8_t>(Opcode::BMI_RELATIVE)]     = &Cpu6502::BMI;
    opcodeTable[static_cast<uint8_t>(Opcode::BVC_RELATIVE)]     = &Cpu6502::BVC;
    opcodeTable[static_cast<uint8_t>(Opcode::BVS_RELATIVE)]     = &Cpu6502::BVS;
}

void Cpu6502::initInstructionCycleTable()
{
    std::fill(std::begin(cycleTable), std::end(cycleTable), 0);

    // LDA
    cycleTable[static_cast<uint8_t>(Opcode::LDA_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::LDA_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::LDA_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::LDA_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::LDA_ABSOLUTEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::LDA_ABSOLUTEY)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::LDA_INDIRECTX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::LDA_INDIRECTY)]     = 5;

    // LDX
    cycleTable[static_cast<uint8_t>(Opcode::LDX_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::LDX_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::LDX_ZEROPAGEY)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::LDX_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::LDX_ABSOLUTEY)]     = 4;

    // LDY
    cycleTable[static_cast<uint8_t>(Opcode::LDY_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::LDY_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::LDY_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::LDY_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::LDY_ABSOLUTEX)]     = 4;

    // STA
    cycleTable[static_cast<uint8_t>(Opcode::STA_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::STA_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::STA_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::STA_ABSOLUTEX)]     = 5;
    cycleTable[static_cast<uint8_t>(Opcode::STA_ABSOLUTEY)]     = 5;
    cycleTable[static_cast<uint8_t>(Opcode::STA_INDIRECTX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::STA_INDIRECTY)]     = 6;

    // STX
    cycleTable[static_cast<uint8_t>(Opcode::STX_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::STX_ZEROPAGEY)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::STX_ABSOLUTE)]      = 4;

    // STY
    cycleTable[static_cast<uint8_t>(Opcode::STY_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::STY_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::STY_ABSOLUTE)]      = 4;

    // Transfer (all 2 cycles)
    cycleTable[static_cast<uint8_t>(Opcode::TAX_IMPLIED)]       = 2;
    cycleTable[static_cast<uint8_t>(Opcode::TXA_IMPLIED)]       = 2;
    cycleTable[static_cast<uint8_t>(Opcode::TAY_IMPLIED)]       = 2;
    cycleTable[static_cast<uint8_t>(Opcode::TYA_IMPLIED)]       = 2;

    // ADC
    cycleTable[static_cast<uint8_t>(Opcode::ADC_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::ADC_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::ADC_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::ADC_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::ADC_ABSOLUTEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::ADC_ABSOLUTEY)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::ADC_INDIRECTX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::ADC_INDIRECTY)]     = 5;

    // SBC
    cycleTable[static_cast<uint8_t>(Opcode::SBC_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::SBC_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::SBC_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::SBC_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::SBC_ABSOLUTEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::SBC_ABSOLUTEY)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::SBC_INDIRECTX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::SBC_INDIRECTY)]     = 5;

    // INC
    cycleTable[static_cast<uint8_t>(Opcode::INC_ZEROPAGE)]      = 5;
    cycleTable[static_cast<uint8_t>(Opcode::INC_ZEROPAGEX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::INC_ABSOLUTE)]      = 6;
    cycleTable[static_cast<uint8_t>(Opcode::INC_ABSOLUTEX)]     = 7;

    // DEC
    cycleTable[static_cast<uint8_t>(Opcode::DEC_ZEROPAGE)]      = 5;
    cycleTable[static_cast<uint8_t>(Opcode::DEC_ZEROPAGEX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::DEC_ABSOLUTE)]      = 6;
    cycleTable[static_cast<uint8_t>(Opcode::DEC_ABSOLUTEX)]     = 7;

    // INX, DEX, INY, DEY (all 2 cycles)
    cycleTable[static_cast<uint8_t>(Opcode::INX)]               = 2;
    cycleTable[static_cast<uint8_t>(Opcode::DEX)]               = 2;
    cycleTable[static_cast<uint8_t>(Opcode::INY)]               = 2;
    cycleTable[static_cast<uint8_t>(Opcode::DEY)]               = 2;

    // ASL
    cycleTable[static_cast<uint8_t>(Opcode::ASL_ACCUMULATOR)]   = 2;
    cycleTable[static_cast<uint8_t>(Opcode::ASL_ZEROPAGE)]      = 5;
    cycleTable[static_cast<uint8_t>(Opcode::ASL_ZEROPAGEX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::ASL_ABSOLUTE)]      = 6;
    cycleTable[static_cast<uint8_t>(Opcode::ASL_ABSOLUTEX)]     = 7;

    // LSR
    cycleTable[static_cast<uint8_t>(Opcode::LSR_ACCUMULATOR)]   = 2;
    cycleTable[static_cast<uint8_t>(Opcode::LSR_ZEROPAGE)]      = 5;
    cycleTable[static_cast<uint8_t>(Opcode::LSR_ZEROPAGEX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::LSR_ABSOLUTE)]      = 6;
    cycleTable[static_cast<uint8_t>(Opcode::LSR_ABSOLUTEX)]     = 7;

    // ROL
    cycleTable[static_cast<uint8_t>(Opcode::ROL_ACCUMULATOR)]   = 2;
    cycleTable[static_cast<uint8_t>(Opcode::ROL_ZEROPAGE)]      = 5;
    cycleTable[static_cast<uint8_t>(Opcode::ROL_ZEROPAGEX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::ROL_ABSOLUTE)]      = 6;
    cycleTable[static_cast<uint8_t>(Opcode::ROL_ABSOLUTEX)]     = 7;

    // ROR
    cycleTable[static_cast<uint8_t>(Opcode::ROR_ACCUMULATOR)]   = 2;
    cycleTable[static_cast<uint8_t>(Opcode::ROR_ZEROPAGE)]      = 5;
    cycleTable[static_cast<uint8_t>(Opcode::ROR_ZEROPAGEX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::ROR_ABSOLUTE)]      = 6;
    cycleTable[static_cast<uint8_t>(Opcode::ROR_ABSOLUTEX)]     = 7;

    // AND
    cycleTable[static_cast<uint8_t>(Opcode::AND_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::AND_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::AND_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::AND_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::AND_ABSOLUTEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::AND_ABSOLUTEY)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::AND_INDIRECTX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::AND_INDIRECTY)]     = 5;

    // ORA
    cycleTable[static_cast<uint8_t>(Opcode::ORA_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::ORA_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::ORA_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::ORA_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::ORA_ABSOLUTEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::ORA_ABSOLUTEY)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::ORA_INDIRECTX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::ORA_INDIRECTY)]     = 5;

    // EOR
    cycleTable[static_cast<uint8_t>(Opcode::EOR_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::EOR_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::EOR_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::EOR_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::EOR_ABSOLUTEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::EOR_ABSOLUTEY)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::EOR_INDIRECTX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::EOR_INDIRECTY)]     = 5;

    // BIT
    cycleTable[static_cast<uint8_t>(Opcode::BIT_ZEROPAGE)]       = 3;
    cycleTable[static_cast<uint8_t>(Opcode::BIT_ABSOLUTE)]      = 4;
    
    // CMP
    cycleTable[static_cast<uint8_t>(Opcode::CMP_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::CMP_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::CMP_ZEROPAGEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::CMP_ABSOLUTE)]      = 4;
    cycleTable[static_cast<uint8_t>(Opcode::CMP_ABSOLUTEX)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::CMP_ABSOLUTEY)]     = 4;
    cycleTable[static_cast<uint8_t>(Opcode::CMP_INDIRECTX)]     = 6;
    cycleTable[static_cast<uint8_t>(Opcode::CMP_INDIRECTY)]     = 5;

    // CPX
    cycleTable[static_cast<uint8_t>(Opcode::CPX_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::CPX_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::CPX_ABSOLUTE)]      = 4;

    // CPY
    cycleTable[static_cast<uint8_t>(Opcode::CPY_IMMEDIATE)]     = 2;
    cycleTable[static_cast<uint8_t>(Opcode::CPY_ZEROPAGE)]      = 3;
    cycleTable[static_cast<uint8_t>(Opcode::CPY_ABSOLUTE)]      = 4;

    cycleTable[static_cast<uint8_t>(Opcode::BCC_RELATIVE)]      = 2;
    cycleTable[static_cast<uint8_t>(Opcode::BCS_RELATIVE)]      = 2;
    cycleTable[static_cast<uint8_t>(Opcode::BEQ_RELATIVE)]      = 2;
    cycleTable[static_cast<uint8_t>(Opcode::BNE_RELATIVE)]      = 2;
    cycleTable[static_cast<uint8_t>(Opcode::BPL_RELATIVE)]      = 2;
    cycleTable[static_cast<uint8_t>(Opcode::BMI_RELATIVE)]      = 2;
    cycleTable[static_cast<uint8_t>(Opcode::BVC_RELATIVE)]      = 2;
    cycleTable[static_cast<uint8_t>(Opcode::BVS_RELATIVE)]      = 2;
}

void Cpu6502::Reset()
{
    StatusReg.SetRegister(0x00);
    PC = Memory6502::kRomStart;
    A = 0;
    X = 0;
    Y = 0;
    SP = 0xFF;
    totalCycles = 0;
    currentInstructionCycles = 0;
}

uint8_t Cpu6502::ExecuteInstruction()
{
    uint8_t opcode = memory.ReadByte(PC);
    PC++;

    OpcodeHandler handler = opcodeTable[opcode];
    if (handler == nullptr)
        throw std::runtime_error(Utils::to_hex(opcode) + " is an unknown opcode.");

    currentInstructionCycles = cycleTable[opcode];

    (this->*handler)();

    totalCycles = currentInstructionCycles;
    return currentInstructionCycles;
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

uint16_t Cpu6502::AddressingAbsoluteX(bool handlePageCross = true)
{
    uint16_t baseAddress = memory.ReadWord(PC);
    PC += 2;
    uint16_t effectiveAddress = baseAddress + X;

    if (handlePageCross)
        HandlePageCross(baseAddress, effectiveAddress);

    return effectiveAddress;
}

uint16_t Cpu6502::AddressingAbsoluteY(bool handlePageCross = true)
{
    uint16_t baseAddress = memory.ReadWord(PC);
    PC += 2;
    uint16_t effectiveAddress = baseAddress + Y;

    if (handlePageCross)
        HandlePageCross(baseAddress, effectiveAddress);

    return effectiveAddress;
}

uint16_t Cpu6502::AddressingIndirectX(bool handlePageCross = true)
{
    uint8_t baseAddress = memory.ReadByte(PC);
    PC++;
    uint8_t effectiveAddress = (baseAddress + X) & 0xFF;

    if (handlePageCross)
        HandlePageCross(baseAddress, effectiveAddress);

    return memory.ReadWord(effectiveAddress);
}

uint16_t Cpu6502::AddressingIndirectY(bool handlePageCross = true)
{
    uint8_t indirectAddress = memory.ReadByte(PC);
    PC++;
    uint16_t baseAddress = memory.ReadWord(indirectAddress);
    uint16_t effectiveAddress = baseAddress + Y;

    if (handlePageCross)
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
    STA(AddressingAbsoluteX(false));
}

void Cpu6502::STAAbsoluteY()
{
    STA(AddressingAbsoluteY(false));
}

void Cpu6502::STAIndirectX()
{
    STA(AddressingIndirectX(false));
}

void Cpu6502::STAIndirectY()
{
    STA(AddressingIndirectY(false));
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
    
    uint16_t result = A - memoryValue - (StatusReg.GetCarry() ? 0 : 1); 
    
    StatusReg.SetCarry(!(result > 0xFF));
    StatusReg.SetOverflow((result ^ A) & (result ^ ~memoryValue) & 0x80);
    
    A = static_cast<uint8_t>(result & 0xFF);
    SetNZFlags(A);
}

void Cpu6502::SBCImmediate()
{
    SBC(AddressingImmediate());
}

void Cpu6502::SBCZeroPage()
{
    SBC(AddressingZeroPage());
}

void Cpu6502::SBCZeroPageX()
{
    SBC(AddressingZeroPageX());
}

void Cpu6502::SBCAbsolute()
{
    SBC(AddressingAbsolute());
}

void Cpu6502::SBCAbsoluteX()
{
    SBC(AddressingAbsoluteX());
}

void Cpu6502::SBCAbsoluteY()
{
    SBC(AddressingAbsoluteY());
}

void Cpu6502::SBCIndirectX()
{
    SBC(AddressingIndirectX());
}

void Cpu6502::SBCIndirectY()
{
    SBC(AddressingIndirectY());
}

void Cpu6502::INC(uint16_t address)
{
    uint8_t result = memory.ReadByte(address) + 1;
    memory.WriteByte(address, result);
    SetNZFlags(result);
}

void Cpu6502::INCZeroPage()
{
    INC(AddressingZeroPage());
}

void Cpu6502::INCZeroPageX()
{
    INC(AddressingZeroPageX());
}

void Cpu6502::INCAbsolute()
{
    INC(AddressingAbsolute());
}

void Cpu6502::INCAbsoluteX()
{
    INC(AddressingAbsoluteX(false));
}

void Cpu6502::DEC(uint16_t address)
{
    uint8_t result = memory.ReadByte(address) - 1;
    memory.WriteByte(address, result);
    SetNZFlags(result);
}

void Cpu6502::DECZeroPage()
{
    DEC(AddressingZeroPage());
}

void Cpu6502::DECZeroPageX()
{
    DEC(AddressingZeroPageX());
}

void Cpu6502::DECAbsolute()
{
    DEC(AddressingAbsolute());
}

void Cpu6502::DECAbsoluteX()
{
    DEC(AddressingAbsoluteX(false));
}

void Cpu6502::INX()
{
    X = X + 1;
    SetNZFlags(X);
}

void Cpu6502::DEX()
{
    X = X - 1;
    SetNZFlags(X);
}

void Cpu6502::INY()
{
    Y = Y + 1;
    SetNZFlags(Y);
}

void Cpu6502::DEY()
{
    Y = Y - 1;
    SetNZFlags(Y);
}

void Cpu6502::ASL(uint16_t address)
{
    uint8_t memoryValue = memory.ReadByte(address);
    uint8_t carryValue = memoryValue & 0x80;
    
    uint8_t result = memoryValue << 1;
    memory.WriteByte(address, result);

    StatusReg.SetCarry(carryValue);
    SetNZFlags(result);

}

void Cpu6502::ASLAccumulator()
{
    uint8_t carryValue = A & 0x80;

    A = A << 1;

    StatusReg.SetCarry(carryValue);
    SetNZFlags(A);
}

void Cpu6502::ASLZeroPage()
{
    ASL(AddressingZeroPage());
}

void Cpu6502::ASLZeroPageX()
{
    ASL(AddressingZeroPageX());
}

void Cpu6502::ASLAbsolute()
{
    ASL(AddressingAbsolute());
}

void Cpu6502::ASLAbsoluteX()
{
    ASL(AddressingAbsoluteX(false));
}

void Cpu6502::LSR(uint16_t address)
{
    uint8_t memoryValue = memory.ReadByte(address);
    uint8_t carryValue = memoryValue & 0x1;
    
    uint8_t result = memoryValue >> 1;
    memory.WriteByte(address, result);

    StatusReg.SetCarry(carryValue);
    SetNZFlags(result);
}

void Cpu6502::LSRAccumulator()
{
    uint8_t carryValue = A & 0x1;

    A = A >> 1;

    StatusReg.SetCarry(carryValue);
    SetNZFlags(A);
}

void Cpu6502::LSRZeroPage()
{
    LSR(AddressingZeroPage());
}

void Cpu6502::LSRZeroPageX()
{
    LSR(AddressingZeroPageX());
}

void Cpu6502::LSRAbsolute()
{
    LSR(AddressingAbsolute());
}

void Cpu6502::LSRAbsoluteX()
{
    LSR(AddressingAbsoluteX(false));
}

void Cpu6502::ROL(uint16_t address)
{
    uint8_t memoryValue = memory.ReadByte(address);
    int carryValue = memoryValue & 0x80;

    uint8_t result = (memoryValue << 1) | StatusReg.GetCarry();
    memory.WriteByte(address, result);

    StatusReg.SetCarry(carryValue);
    SetNZFlags(result);
}

void Cpu6502::ROLAccumulator()
{
    int carryValue = A & 0x80;
    
    A = (A << 1) | StatusReg.GetCarry();

    StatusReg.SetCarry(carryValue);
    SetNZFlags(A);
}

void Cpu6502::ROLZeroPage()
{
    ROL(AddressingZeroPage());
}

void Cpu6502::ROLZeroPageX()
{
    ROL(AddressingZeroPageX());
}

void Cpu6502::ROLAbsolute()
{
    ROL(AddressingAbsolute());
}

void Cpu6502::ROLAbsoluteX()
{
    ROL(AddressingAbsoluteX(false));
}

void Cpu6502::ROR(uint16_t address)
{
    uint8_t memoryValue = memory.ReadByte(address);
    int carryValue = memoryValue & 0x1;

    uint8_t result = (memoryValue >> 1) | (StatusReg.GetCarry() << 7);
    memory.WriteByte(address, result);

    StatusReg.SetCarry(carryValue);
    SetNZFlags(result);
}

void Cpu6502::RORAccumulator()
{
    int carryValue = A & 0x1;
    
    A = (A >> 1) | (StatusReg.GetCarry() << 7);

    StatusReg.SetCarry(carryValue);
    SetNZFlags(A);
}

void Cpu6502::RORZeroPage()
{
    ROR(AddressingZeroPage());
}

void Cpu6502::RORZeroPageX()
{
    ROR(AddressingZeroPageX());
}

void Cpu6502::RORAbsolute()
{
    ROR(AddressingAbsolute());
}

void Cpu6502::RORAbsoluteX()
{
    ROR(AddressingAbsoluteX(false));
}

void Cpu6502::AND(uint16_t address)
{
    uint8_t memoryValue = memory.ReadByte(address);
    A = A & memoryValue;
    SetNZFlags(A);
}

void Cpu6502::ANDImmediate()
{
    AND(AddressingImmediate());
}

void Cpu6502::ANDZeroPage()
{
    AND(AddressingZeroPage());
}

void Cpu6502::ANDZeroPageX()
{
    AND(AddressingZeroPageX());
}

void Cpu6502::ANDAbsolute()
{
    AND(AddressingAbsolute());
}

void Cpu6502::ANDAbsoluteX()
{
    AND(AddressingAbsoluteX());
}

void Cpu6502::ANDAbsoluteY()
{
    AND(AddressingAbsoluteY());
}

void Cpu6502::ANDIndirectX()
{
    AND(AddressingIndirectX());
}

void Cpu6502::ANDIndirectY()
{
    AND(AddressingIndirectY());
}

void Cpu6502::ORA(uint16_t address)
{
    A = A | memory.ReadByte(address);
    SetNZFlags(A);
}

void Cpu6502::ORAImmediate()
{
    ORA(AddressingImmediate());
}

void Cpu6502::ORAZeroPage()
{
    ORA(AddressingZeroPage());
}

void Cpu6502::ORAZeroPageX()
{
    ORA(AddressingZeroPageX());
}

void Cpu6502::ORAAbsolute()
{
    ORA(AddressingAbsolute());
}

void Cpu6502::ORAAbsoluteX()
{
    ORA(AddressingAbsoluteX());
}

void Cpu6502::ORAAbsoluteY()
{
    ORA(AddressingAbsoluteY());
}

void Cpu6502::ORAIndirectX()
{
    ORA(AddressingIndirectX());
}

void Cpu6502::ORAIndirectY()
{
    ORA(AddressingIndirectY());
}

void Cpu6502::EOR(uint16_t address)
{
    A = A ^ memory.ReadByte(address);
    SetNZFlags(A);
}

void Cpu6502::EORImmediate()
{
    EOR(AddressingImmediate());
}

void Cpu6502::EORZeroPage()
{
    EOR(AddressingZeroPage());
}

void Cpu6502::EORZeroPageX()
{
    EOR(AddressingZeroPageX());
}

void Cpu6502::EORAbsolute()
{
    EOR(AddressingAbsolute());
}

void Cpu6502::EORAbsoluteX()
{
    EOR(AddressingAbsoluteX());
}

void Cpu6502::EORAbsoluteY()
{
    EOR(AddressingAbsoluteY());
}

void Cpu6502::EORIndirectX()
{
    EOR(AddressingIndirectX());
}

void Cpu6502::EORIndirectY()
{
    EOR(AddressingIndirectY());
}

void Cpu6502::BIT(uint16_t address)
{
    uint8_t memoryValue = memory.ReadByte(address);
    uint8_t zeroResult = A & memoryValue;
    StatusReg.SetZero(zeroResult == 0);
    StatusReg.SetOverflow((memoryValue & 0b01000000) >> 6);
    StatusReg.SetNegative((memoryValue & 0b10000000) >> 7);
}

void Cpu6502::BITZeroPage()
{
    BIT(AddressingZeroPage());
}

void Cpu6502::BITAbsolute()
{
    BIT(AddressingAbsolute());
}

void Cpu6502::CMP(uint16_t address)
{
    uint8_t memValue = memory.ReadByte(address);
    uint8_t cmpResult = A - memValue;
    StatusReg.SetCarry(A >= memValue);
    StatusReg.SetZero(A == memValue);
    StatusReg.SetNegative((cmpResult & 0b10000000) >> 7);
}

void Cpu6502::CMPImmediate()
{
    CMP(AddressingImmediate());
}

void Cpu6502::CMPZeroPage()
{
    CMP(AddressingZeroPage());
}

void Cpu6502::CMPZeroPageX()
{
    CMP(AddressingZeroPageX());
}

void Cpu6502::CMPAbsolute()
{
    CMP(AddressingAbsolute());
}

void Cpu6502::CMPAbsoluteX()
{
    CMP(AddressingAbsoluteX());
}

void Cpu6502::CMPAbsoluteY()
{
    CMP(AddressingAbsoluteY());
}

void Cpu6502::CMPIndirectX()
{
    CMP(AddressingIndirectX());
}

void Cpu6502::CMPIndirectY()
{
    CMP(AddressingIndirectY());
}

void Cpu6502::CPX(uint16_t address)
{
    uint8_t memValue = memory.ReadByte(address);
    uint8_t cmpResult = X - memValue;
    StatusReg.SetCarry(X >= memValue);
    StatusReg.SetZero(X == memValue);
    StatusReg.SetNegative((cmpResult & 0b10000000) >> 7);
}

void Cpu6502::CPXImmediate()
{
    CPX(AddressingImmediate());
}

void Cpu6502::CPXZeroPage()
{
    CPX(AddressingZeroPage());
}

void Cpu6502::CPXAbsolute()
{
    CPX(AddressingAbsolute());
}

void Cpu6502::CPY(uint16_t address)
{
    uint8_t memValue = memory.ReadByte(address);
    uint8_t cmpResult = Y - memValue;
    StatusReg.SetCarry(Y >= memValue);
    StatusReg.SetZero(Y == memValue);
    StatusReg.SetNegative((cmpResult & 0b10000000) >> 7);
}

void Cpu6502::CPYImmediate()
{
    CPY(AddressingImmediate());
}

void Cpu6502::CPYZeroPage()
{
    CPY(AddressingZeroPage());
}

void Cpu6502::CPYAbsolute()
{
    CPY(AddressingAbsolute());
}

void Cpu6502::BCC()
{
    // Grab branch location and increment PC
    uint8_t branchOffset = memory.ReadByte(PC);
    PC++;

    if (StatusReg.GetCarry())
        return;

    uint16_t oldPc = PC;
    PC += branchOffset;

    if ((oldPc & 0xFF00) != (PC & 0xFF00))
    {
        // increment cycles
    }
}

void Cpu6502::BCS()
{
    
}

void Cpu6502::BEQ()
{
    
}

void Cpu6502::BNE()
{
    
}

void Cpu6502::BPL()
{
    
}

void Cpu6502::BMI()
{
    
}

void Cpu6502::BVC()
{
    
}

void Cpu6502::BVS()
{
    
}

void Cpu6502::HandlePageCross(uint16_t baseAddress, uint16_t effectiveAddress)
{
    if ((baseAddress & 0xFF00) != (effectiveAddress & 0xFF00))
        currentInstructionCycles++;
}

void Cpu6502::SetNZFlags(uint8_t value)
{
    StatusReg.SetZero(value == 0);
    StatusReg.SetNegative((value & 0x80) != 0);
}
