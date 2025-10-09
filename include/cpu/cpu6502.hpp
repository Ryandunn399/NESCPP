#pragma once

#include <array>
#include <cstdint>

#include "status_register.hpp"
#include "memory.hpp"

#ifndef MEM_SIZE
#define MEM_SIZE 65536
#endif

#define OPCODE_TABLE_BUFFER 256

#define ZERO_PAGE_START 0x0000;

#define STACK_START 0x0100;
#define STACK_END 0x01FF;

#define RAM_START 0x0200;
#define ROM_START 0x8000;

/**
 * @brief Class that handles the execution of instructions.
 * 
 */
class Cpu6502 
{
public:

    using OpcodeHandler = void(Cpu6502::*)();

    /**
     * @brief Construct a new Cpu6502 object
     * 
     */
    Cpu6502(Memory6502& mem);

    /**
     * @brief Program Counter.
     * 
     */
    uint16_t PC = Memory6502::kRomStart;

    /**
     * @brief Accumulator.
     * 
     */
    uint8_t A = 0;

    /**
     * @brief X register.
     * 
     */
    uint8_t X = 0;

    /**
     * @brief Y register.
     * 
     */
    uint8_t Y = 0;

    /**
     * @brief Stack pointer.  It will be added to the start address for the stack
     * and grow downwards as more things get pushed onto the stack.
     * 
     */
    uint8_t SP = 0xFF;

    /**
     * @brief Class that handles functionality of the status register.
     * 
     */
    StatusRegister StatusReg;

    /**
     * @brief Executes the next instruction in memory.
     * 
     */
    uint8_t ExecuteInstruction();

    /// @brief Retrieves total number of cycles 
    uint64_t GetTotalCycles() const { return totalCycles; }

    /**
     * @brief Resets the CPU information.
     * 
     */
    void Reset();

    /**
     * @brief Retrieves the raw memory for debugging/testing.
     * 
     * @return uint8_t* 
     */
    uint8_t* GetRawMemory() { return memory.GetRawMemory(); }

private:

    /**
     * @brief Array that maps Opcode values to the corresponding CPU 6502 method call.
     * 
     */
    std::array<OpcodeHandler, OPCODE_TABLE_BUFFER> opcodeTable {};

    /**
     * @brief Memory manager class.
     * 
     */
    Memory6502& memory;

    /**
     * @brief Keeps track of the stack pointer.
     * 
     */
    uint8_t stackPointer;

    /// @brief Total cycles since power-on/reset.
    uint64_t totalCycles;

    /// @brief Cycles for current instruction.
    uint8_t currentInstructionCycles;

    /// @brief Cycle lookup table for base instruction timing.
    uint8_t cycleTable[256];

    /**
     * @brief Will initialize the opcode table by pointing to the 
     * 
     */
    void initOpcodeTable();

    /// @brief Will initialize the instruction cycle table.
    void initInstructionCycleTable();

    /**
     * @brief Immediate addressing mode.
     *
     * In Immediate mode, the operand is specified directly in the byte
     * following the opcode. This function returns the current program counter (PC),
     * then advances PC by 1 so that the CPU can read the next instruction afterward.
     *
     * Example: LDA #$05  ; Loads the value 0x05 directly into A.
     *
     * @return uint16_t Address in memory where the immediate value is stored.
     */
    uint16_t AddressingImmediate();

    /**
     * @brief Zero Page addressing mode.
     *
     * Fetches an 8-bit address from the next byte after the opcode.
     * This address refers to memory in the range 0x0000–0x00FF (the zero page).
     * This mode is faster because it only uses one byte for the address.
     *
     * Example: LDA $42  ; Loads the value from address 0x0042.
     *
     * @return uint16_t Effective 16-bit address in zero page memory.
     */
    uint16_t AddressingZeroPage();

    /**
     * @brief Zero Page,X addressing mode.
     *
     * Fetches an 8-bit zero page address from the next byte after the opcode,
     * then adds the X register to it (wrapping within 0x00–0xFF).
     *
     * Example: LDA $42,X ; Loads from address (0x0042 + X) & 0xFF.
     *
     * @return uint16_t Effective 16-bit address in zero page memory.
     */
    uint16_t AddressingZeroPageX();

    /**
     * @brief Zero Page,Y addressing mode.
     *
     * Fetches an 8-bit zero page address from the next byte after the opcode,
     * then adds the Y register to it (wrapping within 0x00–0xFF).
     *
     * Example: LDX $42,Y ; Loads from address (0x0042 + Y) & 0xFF.
     *
     * @return uint16_t Effective 16-bit address in zero page memory.
     */
    uint16_t AddressingZeroPageY();

    /**
     * @brief Absolute addressing mode.
     *
     * Fetches a full 16-bit address from the next two bytes after the opcode
     * (low byte first, then high byte). This address points directly to the operand.
     *
     * Example: LDA $1234 ; Loads from address 0x1234.
     *
     * @return uint16_t Effective 16-bit address in memory.
     */
    uint16_t AddressingAbsolute();

    /**
     * @brief Absolute,X addressing mode.
     *
     * Fetches a full 16-bit base address from the next two bytes after the opcode,
     * then adds the X register to it. May cross a page boundary, which costs
     * an extra CPU cycle for certain instructions.
     *
     * Example: LDA $1234,X ; Loads from 0x1234 + X.
     *
     * @return uint16_t Effective 16-bit address in memory.
     */
    uint16_t AddressingAbsoluteX(bool handlePageCross);

    /**
     * @brief Absolute,Y addressing mode.
     *
     * Fetches a full 16-bit base address from the next two bytes after the opcode,
     * then adds the Y register to it. May cross a page boundary, which costs
     * an extra CPU cycle for certain instructions.
     *
     * Example: LDA $1234,Y ; Loads from 0x1234 + Y.
     *
     * @return uint16_t Effective 16-bit address in memory.
     */
    uint16_t AddressingAbsoluteY(bool handlePageCross);

    /**
     * @brief Indexed Indirect (Indirect,X) addressing mode.
     *
     * Fetches an 8-bit zero page address from the next byte after the opcode,
     * adds the X register to it (wrapping within zero page), and uses the
     * resulting address to fetch a 16-bit pointer (low byte first, high byte next).
     * That pointer is the effective address.
     *
     * Example: LDA ($20,X) ; Takes address at (0x0020 + X) & 0xFF, then dereferences it.
     *
     * @return uint16_t Effective 16-bit address in memory.
     */
    uint16_t AddressingIndirectX(bool handlePageCross);

    /**
     * @brief Indirect Indexed (Indirect),Y addressing mode.
     *
     * Fetches an 8-bit zero page address from the next byte after the opcode,
     * reads a 16-bit pointer from that zero page address (low byte first, high byte next),
     * then adds the Y register to it. May cross a page boundary, costing an extra cycle.
     *
     * Example: LDA ($20),Y ; Dereferences address at 0x0020, then adds Y.
     *
     * @return uint16_t Effective 16-bit address in memory.
     */
    uint16_t AddressingIndirectY(bool handlePageCross);
    
    /**
     * @brief Loads memory address value into the accumulator register.
     * 
     */
    void LDA(uint16_t address);

    /**
     * @brief Loads a memory value into the accumulator using immediate addressing. 
     * 
     */
    void LDAImmediate();

    /**
     * @brief Loads a memory value into the accumulator using zero page addressing. 
     * 
     */
    void LDAZeroPage();

    /**
     * @brief Loads a memory value into the accumulator using zero page x addressing. 
     * 
     */
    void LDAZeroPageX();

    /**
     * @brief Loads a memory value into the accumulator using absolute addressing. 
     * 
     */
    void LDAAbsolute();

    /**
     * @brief Loads a memory value into the accumulator using absolute x addressing. 
     * 
     */
    void LDAAbsoluteX();

    /**
     * @brief Loads a memory value into the accumulator using absolute y addressing. 
     * 
     */
    void LDAAbsoluteY();

    /**
     * @brief Loads a memory value into the accumulator using indirect x addressing. 
     * 
     */
    void LDAIndirectX();

    /**
     * @brief Loads a memory value into the accumulator using indirect y addressing. 
     * 
     */
    void LDAIndirectY();

    /**
     * @brief Stores the accumulator value into memory.
     * 
     * @param address 
     */
    void STA(uint16_t address);

    /**
     * @brief Stores accumulator value into memory using zero page addressing mode.
     * 
     */
    void STAZeroPage();

    /**
     * @brief Stores accumulator value into the memory using zero page x addressing mode.
     * 
     */
    void STAZeroPageX();

    /**
     * @brief Stores accumulator value into the memory using absolute addressing mode.
     * 
     */
    void STAAbsolute();

    /**
     * @brief Stores accumulator value into the memory using absolute x addressing mode.
     * 
     */
    void STAAbsoluteX();

    /**
     * @brief Stores accumulator value into the memory using absolute y addressing mode.
     * 
     */
    void STAAbsoluteY();

    /**
     * @brief Stores accumulator value into the memory using indirect x addressing mode.
     * 
     */
    void STAIndirectX();

    /**
     * @brief Stores accumulator value into the memory using indirect y addressing mode.
     * 
     */
    void STAIndirectY();

    /**
     * @brief Stores the X register value into memory.
     * 
     * @param address memory address to store the value into.
     */
    void STX(uint16_t address);

    /**
     * @brief Stores the x register value into memory using zero page addressing mode.
     * 
     */
    void STXZeroPage();

    /**
     * @brief Stores the x register value into memory using zero page y addressing mode.
     * 
     */
    void STXZeroPageY();

    /**
     * @brief Stores the x register value into memory using absolute addressing mode.
     * 
     */
    void STXAbsolute();

    /**
     * @brief Stores the Y register value into memory.
     * 
     * @param address  memory address to store the value into.
     */
    void STY(uint16_t address);

    /**
     * @brief Stores the y register value into memory using zero page addressing mode.
     * 
     */
    void STYZeroPage();

    /**
     * @brief Stores the y register value into memory using zero page x addressing mode.
     * 
     */
    void STYZeroPageX();

    /**
     * @brief Stores the y register value into memory using absolute addressing mode.
     * 
     */
    void STYAbsolute();

    /**
     * @brief Loads a memory value into the X register.
     * 
     * @param address Address to load value from.
     */
    void LDX(uint16_t address);

    /**
     * @brief Loads a memory value into the X register using immediate addressing mode.
     * 
     */
    void LDXImmediate();

    /**
     * @brief Lodas a memory value into the X register using immediate zero page addressing mode.
     * 
     */
    void LDXZeroPage();

    /**
     * @brief Lodas a memory value into the X register using immediate zero page y addressing mode.
     * 
     */
    void LDXZeroPageY();

    /**
     * @brief Lodas a memory value into the X register using immediate absolute addressing mode.
     * 
     */
    void LDXAbsolute();

    /**
     * @brief Lodas a memory value into the X register using immediate absolute y addressing mode.
     * 
     */
    void LDXAbsoluteY();

    /**
     * @brief Loads a memory value into the Y register
     * 
     * @param address sss
     */
    void LDY(uint16_t address);

    /**
     * @brief Loads a memory value into the Y register using immediate addressing mode.
     * 
     */
    void LDYImmediate();

    /**
     * @brief Loads a memory value into the Y register using zero page addressing mode.
     * 
     */
    void LDYZeroPage();

    /**
     * @brief Loads a memory value into the Y register using zero page x addressing mode.
     * 
     */
    void LDYZeroPageX();

    /**
     * @brief Loads a memory value into the Y register using absolute addressing mode.
     * 
     */
    void LDYAbsolute();

    /**
     * @brief Loads a memory value int othe Y register using absolute x addressing mode.
     * 
     */
    void LDYAbsoluteX();

    /**
     * @brief Copies accumulator value to the X register.
     * 
     */
    void TAX();

    /**
     * @brief Copies the X register value to the accumulator.
     * 
     */
    void TXA();

    /**
     * @brief Copies accumulator value to the Y register.
     * 
     */
    void TAY();

    /**
     * @brief Copies the Y register value to the accumulator.
     * 
     */
    void TYA();
   

    /**
     * @brief Performs the ADC instruction.
     *
     * @param address 
     */
    void ADC(uint16_t address);

    /**
     * @brief Performs ADC instruction using immediate addressing mode.
     */
    void ADCImmediate();

    /**
     * @brief Performs ADC instruction using zero page addressing mode.
     */
    void ADCZeroPage();

    /**
     * @brief Performs ADC instruction using zero page x addressing mode.
     */
    void ADCZeroPageX();

    /**
     * @brief Performs ADC instruction using absolute addressing mode.
     */
    void ADCAbsolute();

    /**
     * @brief Performs ADC instruction using absolute x addressing mode.
     */
    void ADCAbsoluteX();
    
    /**
     * @brief Performs ADC instruction using absolute y addressing mode.
     */
    void ADCAbsoluteY();

    /**
     * @brief Performs ADC instruction using indirect x addressing mode.
     */
    void ADCIndirectX();

    /**
     * @brief Performs ADC instruction using indirect y addressing mode.
     */
    void ADCIndirectY();

    /**
     * @brief Performs SBC instruction using the value at that memory location.
     *
     * @param address address where our value is stored.
     */
    void SBC(uint16_t address);

    /**
     * @brief Performs SBC instruction using immediate addressing mode.
     */
    void SBCImmediate();

    /**
     * @brief Performs SBC instruction using zero page addressing mode.
     */
    void SBCZeroPage();
    
    /**
     * @brief Performs SBC instruction using zero page x addressing mode.
     */
    void SBCZeroPageX();

    /**
     * @brief Performs SBC instruction using absolute addressing mode.
     */
    void SBCAbsolute();

    /**
     * @brief Performs SBC instruction using absolute x addressing mode.
     */
    void SBCAbsoluteX();

    /**
     * @brief Performs SBC instruction using absolute y addressing mode.
     */
    void SBCAbsoluteY();

    /**
     * @brief Performs SBC instruction using indirect x addressing mode.
     */
    void SBCIndirectX();

    /**
     * @brief Performs SBC instruction using indirect y addressing mode.
     */
    void SBCIndirectY();

    /// @brief Performs INC instruction using value at that address.
    void INC(uint16_t addres);

    /// @brief Performs INC instruction using zero page addressing mode.
    void INCZeroPage();

    /// @brief Performs INC instruction using zero page x addressing mode.
    void INCZeroPageX();

    /// @brief Performs INC instruction using absolute addressing mode.
    void INCAbsolute();

    /// @brief Performs INC instruction using absolute x addressing mode.
    void INCAbsoluteX();

    /// @brief Performs DEC instruction using value at that address.
    void DEC(uint16_t address);

    /// @brief Performs DEC instruction using zero page addressing mode.
    void DECZeroPage();

    /// @brief Performs DEC instruction using zero page x addressing mode.
    void DECZeroPageX();

    /// @brief Performs DEC instruction using absolute addressing mode.
    void DECAbsolute();

    /// @brief Performs DEC instruction using absolute x addressing mode.
    void DECAbsoluteX();

    /// @brief Performs INX instruction, addressing mode is implied.
    void INX();

    /// @brief Performs DEX instruction, addressing mode is implied.
    void DEX();

    /// @brief Performs INY instruction, addressing mode is implied.
    void INY();

    /// @brief Performs DEY instruction, addressing mode is implied.
    void DEY();

    /// @brief Performs ASL instruction using the passed address.
    void ASL(uint16_t address);

    /// @brief Performs ASL instruction using accumulator addressing mode.
    void ASLAccumulator();

    /// @brief Performs ASL instruction using zero page addressing mode.
    void ASLZeroPage();

    /// @brief Performs ASL instruction using zero page x addressing mode.
    void ASLZeroPageX();

    /// @brief Performs ASL instruction using absolute addressing mode.
    void ASLAbsolute();

    /// @brief Performs ASL instruction using absolute x addressing mode.
    void ASLAbsoluteX();

    /// @brief Performs LSR instruction using the passed address.
    void LSR(uint16_t address);

    /// @brief Performs LSR using accumulator addressing mode.
    void LSRAccumulator();

    /// @brief Performs LSR using zero page addressing mode.
    void LSRZeroPage();

    /// @brief Performs LSR using zero page x addressing mode.
    void LSRZeroPageX();

    /// @brief Performs LSR using absolute addressing mode.
    void LSRAbsolute();

    /// @brief Performs LSR using absolute x addressing mode.
    void LSRAbsoluteX();

    /// @brief Performs the ROL instruction using the value at that address.
    void ROL(uint16_t address);

    /// @brief Performs ROL using accumulator addressing mode.
    void ROLAccumulator();

    /// @brief Performs ROL using zero page addressing mode.
    void ROLZeroPage();

    /// @brief Performs ROL using zero page x addressing mode.
    void ROLZeroPageX();

    /// @brief Performs ORL using absolute addressing mode.
    void ROLAbsolute();

    /// @brief Performs ROL using absolute x addressing mode.
    void ROLAbsoluteX();

    /// @brief Performs the ROR instruction using the value at that address.
    void ROR(uint16_t address);

    /// @brief Performs ROR using accumulator addressing mode.
    void RORAccumulator();

    /// @brief Performs ROR using zero page addressing mode.
    void RORZeroPage();

    /// @brief Performs ROR using zero page x addressing mode.
    void RORZeroPageX();

    /// @brief Performs ROR using absolute addressing mode.
    void RORAbsolute();

    /// @brief Performs ROR using absolute x addressing mode.
    void RORAbsoluteX();

    /// @brief Performs AND instruction using value at that address.
    void AND(uint16_t address);

    /// @brief Performs AND using immediate addressing mode.
    void ANDImmediate();

    /// @brief Performs AND using zero page addressing mode.
    void ANDZeroPage();

    /// @brief Performs AND using zero page x addressing mode.
    void ANDZeroPageX();

    /// @brief Performs AND using absolute addressing mode.
    void ANDAbsolute();

    /// @brief Performs AND using absolute x addressing mode.
    void ANDAbsoluteX();

    /// @brief Performs AND using absolute y addressing mode.
    void ANDAbsoluteY();

    /// @brief Performs AND using indirect x addressing mode.
    void ANDIndirectX();

    /// @brief Performs AND using indirect y addressing mode.
    void ANDIndirectY();

    /// @brief Performs ORA instruction using value at that address.
    void ORA(uint16_t address);

    /// @brief Performs ORA using immediate addressing mode.
    void ORAImmediate();

    /// @brief Performs ORA using zero page addressing mode.
    void ORAZeroPage();

    /// @brief Performs ORA using zero page x addressing mode.
    void ORAZeroPageX();

    /// @brief Performs ORA using absolute addressing mode.
    void ORAAbsolute();

    /// @brief Performs ORA using absolute x addressing mode.
    void ORAAbsoluteX();

    /// @brief Performs ORA using absolute y addressing mode.
    void ORAAbsoluteY();

    /// @brief Performs ORA using indirect x addressing mode.
    void ORAIndirectX();

    /// @brief Performs ORA using indirect y addressing mode.
    void ORAIndirectY();

    /// @brief Performs EOR instruction using value at the address.
    void EOR(uint16_t address);
    
    /// @brief Performs EOR using immediate addressing mode.
    void EORImmediate();

    /// @brief Performs EOR using zero page addressing mode.
    void EORZeroPage();

    /// @brief Performs EOR using zero page x addressing mode.
    void EORZeroPageX();

    /// @brief Performs EOR using absolute addressing mode.
    void EORAbsolute();

    /// @brief Performs EOR using absolute x addressing mode.
    void EORAbsoluteX();

    /// @brief Performs EOR using absolute y addressing mode.
    void EORAbsoluteY();

    /// @brief Performs EOR using indirect x addressing mode.
    void EORIndirectX();

    /// @brief Performs EOR using indirect y addressing mode.
    void EORIndirectY();

    /// @brief Performs BIT using value at address.
    void BIT(uint16_t address);

    /// @brief Performs BIT using zero page addressing mode.
    void BITZeroPage();

    /// @brief Performs BIT using absolute addressing mode.
    void BITAbsolute();

    /// @brief Performs CMP using value at address.
    void CMP(uint16_t address);

    /// @brief Performs CMP using immediate addressing mode.
    void CMPImmediate();

    /// @brief Performs CMP using zero page addressing mode.
    void CMPZeroPage();

    /// @brief Performs CMP using zero page x addressing mode.
    void CMPZeroPageX();

    /// @brief Performs CMP using absolute addressing mode.
    void CMPAbsolute();
    
    /// @brief Performs CMP using absolute x addressing mode.
    void CMPAbsoluteX();

    /// @brief Performs CMP using absolute y addressing mode.
    void CMPAbsoluteY();

    /// @brief Performs CMP using indirect x addressing mode.
    void CMPIndirectX();

    /// @brief Performs CMP using indirect y addressing mode.
    void CMPIndirectY();

    /// @brief Performs CPX using value at address.
    void CPX(uint16_t address);

    /// @brief Performs CPX using immediate addressing mode.
    void CPXImmediate();

    /// @brief Performs CPX using zero page addressing mode.
    void CPXZeroPage();

    /// @brief Performs CPX using absolute addressing mode.
    void CPXAbsolute();

    /// @brief Performs CPY using value at address.
    void CPY(uint16_t address);

    /// @brief Performs CPY using immediate addressing mode.
    void CPYImmediate();

    /// @brief Performs CPY using zero page addressing mode.
    void CPYZeroPage();

    /// @brief Performs CPY using absolute addressing mode.
    void CPYAbsolute();

    /// @brief Performs the relative BCC instruction.
    void BCC();

    /// @brief Performs the relative BCS instruction.
    void BCS();

    /// @brief Performs the relative BEQ instruction.
    void BEQ();

    /// @brief Performs the relative BNE instruction.
    void BNE();

    /// @brief Performs the relative BPL instruction.
    void BPL();

    /// @brief Performs the relative BMI instruction.
    void BMI();

    /// @brief Performs the relative BVC instruction.
    void BVC();

    /// @brief Performs the relative BVS instruction.
    void BVS();

    /**
     * @brief Helper method that will evaluate a value and set the zero
     * and negative bit flags in our status register.
     * 
     * @param value 
     */
    void SetNZFlags(uint8_t value);

    /**
     * @brief Updates correct cycle count in the event of a page cross.
     * Page cross takes place in the event that high byte changed when updating register values.
     * Ex - $12FF + 0x01 = $1300, the high byte changes from 12 to 13.
     * 
     */
    void HandlePageCross(uint16_t baseAddress, uint16_t effectiveAddress);
};
