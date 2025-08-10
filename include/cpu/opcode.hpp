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
};