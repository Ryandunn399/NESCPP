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
    LDA_IMMEDIATE = 0xA9,

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

    /// @brief Opcode for DEC Zero Page instruction.
    DEC_ZEROPAGE = 0xC6,

    /// @brief Opcode for DEC Zero Page X instruction.
    DEC_ZEROPAGEX = 0xD6,

    /// @brief Opcode for DEC Absolute instruction.
    DEC_ABSOLUTE = 0xCE,

    /// @brief Opcode for DEC Absolute X instruction.
    DEC_ABSOLUTEX = 0xDE,

    /// @brief Opcode for implied INX instruction.
    INX = 0xE8,

    /// @brief Opcode for implied DEX instruction.
    DEX = 0xCA,

    /// @brief Opcode for implied INY instruction.
    INY = 0xC8,

    /// @brief Opcode for implied DEY instruction.
    DEY = 0x88,

    /// @brief Opcode for ASL Accumulator instruction.
    ASL_ACCUMULATOR = 0x0A,

    /// @brief Opcode for ASL Zero Page instruction.
    ASL_ZEROPAGE = 0x06,

    /// @brief Opcode for ASL Zero Page X instruction.
    ASL_ZEROPAGEX = 0x16,

    /// @brief Opcode for ASL Absolute instruction.
    ASL_ABSOLUTE = 0x0E,

    /// @brief Opcode for ASL Absolute X instruction.
    ASL_ABSOLUTEX = 0x1E,

    /// @brief Opcode for LSR Accumulator instruction.
    LSR_ACCUMULATOR = 0x4A,

    /// @brief Opcode for LSR Zero Page instruction.
    LSR_ZEROPAGE = 0x46,

    /// @brief Opcode for LSR Zero Page X instruction.
    LSR_ZEROPAGEX = 0x56,

    /// @brief Opcode for LSR Absolute instruction.
    LSR_ABSOLUTE = 0x4E,

    /// @brief Opcode for LSR Absolute X instruction.
    LSR_ABSOLUTEX = 0x5E,

    /// @brief Opcode for ROL Accumulator instruction.
    ROL_ACCUMULATOR = 0x2A,
    
    /// @brief Opcode for ROL Zero Page instruction.
    ROL_ZEROPAGE = 0x26,

    /// @brief Opcode for ROL Zero Page X instruction.
    ROL_ZEROPAGEX = 0x36,

    /// @brief Opcode for ROL Absolute instruction.
    ROL_ABSOLUTE = 0x2E,

    /// @brief Opcode for ROL Absolute X instruction.
    ROL_ABSOLUTEX = 0x3E,

    /// @brief Opcode for ROR Accumulator instruction.
    ROR_ACCUMULATOR = 0x6A,

    /// @brief Opcode for ROR Zero Page instruction.
    ROR_ZEROPAGE = 0x66,

    /// @brief Opcode for ROR Zero Page X instruction.
    ROR_ZEROPAGEX = 0x76,

    /// @brief Opcode for ROR Absolute instruction.
    ROR_ABSOLUTE = 0x6E,

    /// @brief Opcode for ROR Absolute X instruction.
    ROR_ABSOLUTEX = 0x7E,

    /// @brief Opcode for AND Immediate instruction
    AND_IMMEDIATE = 0x29,

    /// @brief Opcode for AND Zero Page instruction
    AND_ZEROPAGE = 0x25,

    /// @brief Opcode for AND Zero Page X instruction
    AND_ZEROPAGEX = 0x35,

    /// @brief Opcode for AND Absolute instruction
    AND_ABSOLUTE = 0x2D,

    /// @brief Opcode for AND Absolute X instruction
    AND_ABSOLUTEX = 0x3D,

    /// @brief Opcode for AND Absolute Y instruction
    AND_ABSOLUTEY = 0x39,

    /// @brief Opcode for AND Indirect X instruction
    AND_INDIRECTX = 0x21,

    /// @brief Opcode for AND Indirect Y instruction
    AND_INDIRECTY = 0x31,

    /// @brief Opcode for ORA Immediate instruction.
    ORA_IMMEDIATE = 0x09,

    /// @brief Opcode for ORA Zero Page instruction.
    ORA_ZEROPAGE = 0x05,

    /// @brief Opcode for ORA Zero Page X instruction.
    ORA_ZEROPAGEX = 0x15,

    /// @brief Opcode for ORA Absolute instruction.
    ORA_ABSOLUTE = 0x0D,

    /// @brief Opcode for ORA Absolute X instruction.
    ORA_ABSOLUTEX = 0x1D,

    /// @brief Opcode for ORA Absolute Y instruction.
    ORA_ABSOLUTEY = 0x19,

    /// @brief Opcode for ORA Indirect X instruction.
    ORA_INDIRECTX = 0x01,

    /// @brief Opcode for ORA Indirect Y instruction.
    ORA_INDIRECTY = 0x11,

    /// @brief Opcode for EOR Immediate instruction.
    EOR_IMMEDIATE = 0x49,

    /// @brief Opcode for EOR Zero Page instruction.
    EOR_ZEROPAGE = 0x45,

    /// @brief Opcode for EOR Zero Page X instruction.
    EOR_ZEROPAGEX = 0x55,

    /// @brief Opcode for EOR Absolute instruction.
    EOR_ABSOLUTE = 0x4D,

    /// @brief Opcode for EOR Absolute X instruction.
    EOR_ABSOLUTEX = 0x5D,

    /// @brief Opcode for EOR Absolute Y instruction.
    EOR_ABSOLUTEY = 0x59,

    /// @brief Opcode for EOR Indirect X instruction.
    EOR_INDIRECTX = 0x41,

    /// @brief Opcode for EOR Indirect Y instruction.
    EOR_INDIRECTY = 0x51,

    /// @brief Opcode for BIT Zero Page instruction.
    BIT_ZEROPAGE = 0x24,

    /// @brief Opcode for BIT Absolute instruction.
    BIT_ABSOLUTE = 0x2C,

    /// @brief Opcode for CMP Immediate instruction.
    CMP_IMMEDIATE = 0xC9,

    /// @brief Opcode for CMP Zero Page instruction.
    CMP_ZEROPAGE = 0xC5,

    /// @brief Opcode for CMP Zero Page X instruction.
    CMP_ZEROPAGEX = 0xD5,

    /// @brief Opcode for CMP Absolute instruction.
    CMP_ABSOLUTE = 0xCD,

    /// @brief Opcode for CMP Absolute X instruction.
    CMP_ABSOLUTEX = 0xDD,

    /// @brief Opcode for CMP Absolute Y instruction.
    CMP_ABSOLUTEY = 0xD9,

    /// @brief Opcode for CMP Indirect X instruction.
    CMP_INDIRECTX = 0xC1,

    /// @brief Opcode for CMP Indirect Y instruction.
    CMP_INDIRECTY = 0xD1,

    /// @brief Opcode for CPX Immediate instruction.
    CPX_IMMEDIATE = 0xE0,

    /// @brief Opcode for CPX Zero Page instruction.
    CPX_ZEROPAGE = 0xE4,

    /// @brief Opcode for CPX Absolute instruction.
    CPX_ABSOLUTE = 0xEC,

    /// @brief Opcode for CPY Immediate instruction.
    CPY_IMMEDIATE = 0xC0,

    /// @brief Opcode for CPY Zero Page instruction.
    CPY_ZEROPAGE = 0xC4,

    /// @brief Opcode for CPY Absolute instruction.
    CPY_ABSOLUTE = 0xCC,

    /// @brief Opcode for BCC Relative instruction.
    BCC_RELATIVE = 0x90,

    /// @brief Opcode for BCS Relative instruction.
    BCS_RELATIVE = 0xB0,

    /// @brief Opcode for BEQ Relative instruction.
    BEQ_RELATIVE = 0xF0,

    /// @brief Opcode for BNE Relative instruction.
    BNE_RELATIVE = 0xD0,

    /// @brief Opcode for BPL Relative instruction.
    BPL_RELATIVE = 0x10,

    /// @brief Opcode for BMI Relative instruction.
    BMI_RELATIVE = 0x30,

    /// @brief Opcode for BNE Relative instruction.
    BVC_RELATIVE = 0x50,

    /// @brief Opcode for BPL Relative instruction.
    BVS_RELATIVE = 0x70,

    /// @brief Opcode for JMP Absolute instruction.
    JMP_ABSOLUTE = 0x4C,

    /// @brief Opcode for JMP Indirect instruction.
    JMP_INDIRECT = 0x6C,

    /// @brief Opcode for JSR Absolute instruction.
    JSR_ABSOLUTE = 0x20,

    /// @brief Opcode for RTS Implied instruction.
    RTS_IMPLIED = 0x60,

    /// @brief Opcode for BRK instruction.
    BRK = 0x00,

    /// @brief Opcode for RTI Implied instruction.
    RTI_IMPLIED = 0x40,

    /// @brief Opcode for PHA Implied instruction.
    PHA_IMPLIED = 0X48,

    /// @brief Opcode for PLA Implied instruction.
    PLA_IMPLIED = 0X68,

    /// @brief Opcode for PHP Implied instruction.
    PHP_IMPLIED = 0X08,

    /// @brief Opcode for PLP Implied instruction.
    PLP_IMPLIED = 0X28,

    /// @brief Opcode for TXS Implied instruction.
    TXS_IMPLIED = 0X9A,

    /// @brief Opcode for TSX Implied instruction.
    TSX_IMPLIED = 0XBA,
};
