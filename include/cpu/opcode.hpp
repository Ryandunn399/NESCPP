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
    TYA_IMPLIED = 0x98
};