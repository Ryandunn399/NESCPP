#pragma once

#include <cstdint>

/**
 * @brief Contains opcode definitions.
 * 
 */
enum class Opcode: uint8_t
{
    /**
     * @brief Load Accumulator with Immediate Value
     * 
     * Loads the accumulator with a constant value provided immediately after the opcode.
     * PC is advanced by 2 bytes.
     */
    LDA_IMM = 0xA9,

    /**
     * @brief Load Accumulator from Zero Page Address
     * 
     * Loads the accumulator with the value stored in zero page memory at the given address.
     * The address is one byte following the opcode.
     */
    LDA_ZEROPAGE = 0xA5,

    /**
     * @brief Load Accumulator from Zero Page Address Offset by X Register
     * 
     * Loads the accumulator with the value stored in zero page memory at address plus X.
     * The address wraps around within zero page.
     */
    LDA_ZEROPAGEX = 0xB5,

    /**
     * @brief Load Accumulator from Absolute Address
     * 
     * Loads the accumulator with the value stored at the two-byte absolute address following the opcode.
     */
    LDA_ABSOLUTE = 0xAD,

    /**
     * @brief Load Accumulator from Absolute Address Offset by X Register
     * 
     * Loads the accumulator with the value at the absolute address plus X.
     * If the indexing crosses a page boundary, extra cycles may be required.
     */
    LDA_ABSOLUTEX = 0xBD,

    /**
     * @brief Load Accumulator from Absolute Address Offset by Y Register
     * 
     * Loads the accumulator with the value at the absolute address plus Y.
     * Page crossing may add extra cycles.
     */
    LDA_ABSOLUTEY = 0xB9,

    /**
     * @brief Load Accumulator from Address Indirect Indexed by X
     * 
     * The zero page address following the opcode is added to X, then the effective address
     * is read from that zero page location and the next byte.
     * Accumulator is loaded from this effective address.
     */
    LDA_INDIRECTX = 0xA1,

    /**
     * @brief Load Accumulator from Address Indirect Indexed by Y
     * 
     * The zero page address following the opcode is used to read an address from zero page,
     * then Y is added to that address to get the effective address.
     * Accumulator is loaded from this effective address.
     * Page crossing may add extra cycles.
     */
    LDA_INDIRECTY = 0xB1,
     
    /**
     * @brief Load X Register with Immediate Value
     * 
     * The value following the opcode is loaded directly into the X register.
     */
    LDX_IMMEDIATE = 0xA2,

    /**
     * @brief Load X Register from Zero Page
     * 
     * The byte at the zero page address following the opcode is loaded into the X register.
     */
    LDX_ZEROPAGE = 0xA6,

    /**
     * @brief Load X Register from Zero Page Indexed by Y
     * 
     * The zero page address following the opcode is offset by the Y register to get the effective address.
     * The byte at this effective address is loaded into the X register.
     */
    LDX_ZEROPAGEY = 0xB6,

    /**
     * @brief Load X Register from Absolute Address
     * 
     * The two bytes following the opcode form a 16-bit absolute address.
     * The byte at this address is loaded into the X register.
     */
    LDX_ABSOLUTE = 0xAE,

    /**
     * @brief Load X Register from Absolute Address Indexed by Y
     * 
     * The two bytes following the opcode form a 16-bit base address, then Y is added to get the effective address.
     * The byte at this effective address is loaded into the X register.
     * Page crossing may add extra cycles.
     */
    LDX_ABSOLUTEY = 0xBE,

    /**
     * @brief Load Y Register with Immediate Value
     * 
     * The value following the opcode is loaded directly into the Y register.
     */
    LDY_IMMEDIATE = 0xA0,

    /**
     * @brief Load Y Register from Zero Page
     * 
     * The byte at the zero page address following the opcode is loaded into the Y register.
     */
    LDY_ZEROPAGE = 0xA4,

    /**
     * @brief Load Y Register from Zero Page Indexed by X
     * 
     * The zero page address following the opcode is offset by the X register to get the effective address.
     * The byte at this effective address is loaded into the Y register.
     */
    LDY_ZEROPAGEX = 0xB4,

    /**
     * @brief Load Y Register from Absolute Address
     * 
     * The two bytes following the opcode form a 16-bit absolute address.
     * The byte at this address is loaded into the Y register.
     */
    LDY_ABSOLUTE = 0xAC,

    /**
     * @brief Load Y Register from Absolute Address Indexed by X
     * 
     * The two bytes following the opcode form a 16-bit base address, then X is added to get the effective address.
     * The byte at this effective address is loaded into the Y register.
     * Page crossing may add extra cycles.
     */
    LDY_ABSOLUTEX = 0xBC,

    /**
     * @brief Store Y Register into Zero Page
     * 
     * The Y register value is stored at the zero page address following the opcode.
     */
    STY_ZEROPAGE = 0x84,

    /**
     * @brief Store Y Register into Zero Page Indexed by X
     * 
     * The zero page address following the opcode is offset by the X register to get the effective address.
     * The Y register value is stored at this address.
     */
    STY_ZEROPAGEX = 0x94,

    /**
     * @brief Store Y Register into Absolute Address
     * 
     * The two bytes following the opcode form a 16-bit absolute address.
     * The Y register value is stored at this address.
     */
    STY_ABSOLUTE = 0x8C,

    /**
     * @brief Store X Register into Zero Page
     * 
     * The X register value is stored at the zero page address following the opcode.
     */
    STX_ZEROPAGE = 0x86,

    /**
     * @brief Store X Register into Zero Page Indexed by Y
     * 
     * The zero page address following the opcode is offset by the Y register to get the effective address.
     * The X register value is stored at this address.
     */
    STX_ZEROPAGEY = 0x96,

    /**
     * @brief Load X Register from Absolute Address
     * 
     * The two bytes following the opcode form a 16-bit absolute address.
     * The byte at this address is loaded into the X register.
     */
    STX_ABSOLUTE = 0x8E,

    /**
     * @brief Store Accumulator to Zero Page Address
     * 
     * The zero page address following the opcode specifies where the accumulator value is stored.
     * Only the lower 8 bits of the address are used.
     */
    STA_ZEROPAGE = 0x85,

    /**
     * @brief Store Accumulator to Zero Page Address Indexed by X
     * 
     * The zero page base address following the opcode is added to the X register (with wraparound on zero page),
     * and the accumulator is stored at this effective zero page address.
     */
    STA_ZEROPAGEX = 0x95,

    /**
     * @brief Store Accumulator to Absolute Address
     * 
     * The two bytes following the opcode specify the 16-bit absolute address where the accumulator is stored.
     */
    STA_ABSOLUTE = 0x8D,

    /**
     * @brief Store Accumulator to Absolute Address Indexed by X
     * 
     * The absolute base address from the following two bytes is added to the X register to get the effective address.
     * The accumulator is stored at this effective address.
     */
    STA_ABSOLUTEX = 0x9D,

    /**
     * @brief Store Accumulator to Absolute Address Indexed by Y
     * 
     * The absolute base address from the following two bytes is added to the Y register to get the effective address.
     * The accumulator is stored at this effective address.
     */
    STA_ABSOLUTEY = 0x99,

    /**
     * @brief Store Accumulator Indirect Indexed by X (Indexed Indirect)
     * 
     * The zero page base address following the opcode is added to the X register (with zero page wraparound) to get a zero page address.
     * The 16-bit effective address is read from this zero page address and the next byte.
     * The accumulator is stored at this effective address.
     */
    STA_INDIRECTX = 0x81,

    /**
     * @brief Store Accumulator Indirect Indexed by Y (Indirect Indexed)
     * 
     * The zero page base address following the opcode is used to read a 16-bit address from zero page,
     * then Y is added to that address to get the effective address.
     * The accumulator is stored at this effective address.
     */
    STA_INDIRECTY = 0x91,

    /**
     * @brief Transfer Accumulator to X Register (Implied)
     * 
     * Copies the value currently in the accumulator into the X register.
     * No memory is accessed. Processor flags affected:
     * - Negative (N): Set if the result in X is negative.
     * - Zero (Z): Set if the result in X is zero.
     */
    TAX_IMPLIED = 0xAA,

    /**
     * @brief Transfer Accumulator to Y Register (Implied)
     * 
     * Copies the value currently in the accumulator into the Y register.
     * No memory is accessed. Processor flags affected:
     * - Negative (N): Set if the result in Y is negative.
     * - Zero (Z): Set if the result in Y is zero.
     */
    TAY_IMPLIED = 0xA8,

    /**
     * @brief Transfer Stack Pointer to X Register (Implied)
     * 
     * Copies the current value of the stack pointer into the X register.
     * No memory is accessed. Processor flags affected:
     * - Negative (N): Set if the result in X is negative.
     * - Zero (Z): Set if the result in X is zero.
     */
    TSX_IMPLIED = 0xBA,

    /**
     * @brief Transfer X Register to Accumulator (Implied)
     * 
     * Copies the value currently in the X register into the accumulator.
     * No memory is accessed. Processor flags affected:
     * - Negative (N): Set if the result in A is negative.
     * - Zero (Z): Set if the result in A is zero.
     */
    TXA_IMPLIED = 0x8A,

    /**
     * @brief Transfer X Register to Stack Pointer (Implied)
     * 
     * Copies the value currently in the X register into the stack pointer.
     * No processor flags are affected.
     */
    TXS_IMPLIED = 0x9A,

    /**
     * @brief Transfer Y Register to Accumulator (Implied)
     * 
     * Copies the value currently in the Y register into the accumulator.
     * No memory is accessed. Processor flags affected:
     * - Negative (N): Set if the result in A is negative.
     * - Zero (Z): Set if the result in A is zero.
     */
    TYA_IMPLIED = 0x98,
    
    /**
     * @brief Opcode for the ADC Immediate instruction.
     * 
     */
    ADC_IMMEDIATE = 0x69,

    /**
     * @brief Opcode for ADC Zero Page instruction.
     * 
     */
    ADC_ZEROPAGE = 0x65,

    /**
     * @brief Opcode for ADC Zero Page X instruction.
     * 
     */
    ADC_ZEROPAGEX = 0x75,

    /**
     * @brief Opcode for ADC Absolute instruction.
     * 
     */
    ADC_ABSOLUTE = 0x6D,

    /**
     * @brief Opcode for ADC Absolute X instruction.
     * 
     */
    ADC_ABSOLUTEX = 0x7D,

    /**
     * @brief Opcode for ADC Absolute Y instruction.
     * 
     */
    ADC_ABSOLUTEY = 0x79,

    /**
     * @brief Opcode for ADC Indirect X instruction.
     * 
     */
    ADC_INDIRECTX = 0x61,

    /**
     * @brief Opcode for ADC Indirect Y instruction.
     * 
     */
    ADC_INDIRECTY = 0x71,

    /// @brief Opcode for SBC Immediate instruction.
    SBC_IMMEDIATE = 0xE9,

    /// @brief Opcode for SBC Zero Page instruction.
    SBC_ZEROPAGE = 0xE5,

    /// @brief Opcode for SBC Zero Page X instruction.
    SBC_ZEROPAGEX = 0xF5,

    /// @brief Opcode for SBC Absolute instruction.
    SBC_ABSOLUTE = 0xED,

    /// @brief Opcode for SBC Absolute X instruction.
    SBC_ABSOLUTEX = 0xFD,

    /// @brief Opcode for SBC Absolute Y instruction.
    SBC_ABSOLUTEY = 0xF9,

    /// @brief Opcode for SBC Indirect X instruction.
    SBC_INDIRECTX = 0xE1,

    /// @brief Opcode for SBC Indirect Y instruction.
    SBC_INDIRECTY = 0xF1,

    /// @brief Opcode for INC Zero Page instruction.
    INC_ZEROPAGE = 0xE6,

    /// @brief Opcode for INC Zero Page X instruction.
    INC_ZEROPAGEX = 0xF6,

    /// @brief Opcode for INC Absolute instruction.
    INC_ABSOLUTE = 0xEE,

    /// @brief Opcode for INC Absolute X instruction.
    INC_ABSOLUTEX = 0xFE,
};
