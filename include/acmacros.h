/******************************************************************************
 *
 * Name: acmacros.h - C macros for the entire subsystem.
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2013, Intel Corp.
 * All rights reserved.
 *
 * 2. License
 *
 * 2.1. This is your license from Intel Corp. under its intellectual property
 * rights. You may have additional license terms from the party that provided
 * you this software, covering your right to use that party's intellectual
 * property rights.
 *
 * 2.2. Intel grants, free of charge, to any person ("Licensee") obtaining a
 * copy of the source code appearing in this file ("Covered Code") an
 * irrevocable, perpetual, worldwide license under Intel's copyrights in the
 * base code distributed originally by Intel ("Original Intel Code") to copy,
 * make derivatives, distribute, use and display any portion of the Covered
 * Code in any form, with the right to sublicense such rights; and
 *
 * 2.3. Intel grants Licensee a non-exclusive and non-transferable patent
 * license (with the right to sublicense), under only those claims of Intel
 * patents that are infringed by the Original Intel Code, to make, use, sell,
 * offer to sell, and import the Covered Code and derivative works thereof
 * solely to the minimum extent necessary to exercise the above copyright
 * license, and in no event shall the patent license extend to any additions
 * to or modifications of the Original Intel Code. No other license or right
 * is granted directly or by implication, estoppel or otherwise;
 *
 * The above copyright and patent license is granted only if the following
 * conditions are met:
 *
 * 3. Conditions
 *
 * 3.1. Redistribution of Source with Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification with rights to further distribute source must include
 * the above Copyright Notice, the above License, this list of Conditions,
 * and the following Disclaimer and Export Compliance provision. In addition,
 * Licensee must cause all Covered Code to which Licensee contributes to
 * contain a file documenting the changes Licensee made to create that Covered
 * Code and the date of any change. Licensee must include in that file the
 * documentation of any changes made by any predecessor Licensee. Licensee
 * must include a prominent statement that the modification is derived,
 * directly or indirectly, from Original Intel Code.
 *
 * 3.2. Redistribution of Source with no Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification without rights to further distribute source must
 * include the following Disclaimer and Export Compliance provision in the
 * documentation and/or other materials provided with distribution. In
 * addition, Licensee may not authorize further sublicense of source of any
 * portion of the Covered Code, and must include terms to the effect that the
 * license from Licensee to its licensee is limited to the intellectual
 * property embodied in the software Licensee provides to its licensee, and
 * not to intellectual property embodied in modifications its licensee may
 * make.
 *
 * 3.3. Redistribution of Executable. Redistribution in executable form of any
 * substantial portion of the Covered Code or modification must reproduce the
 * above Copyright Notice, and the following Disclaimer and Export Compliance
 * provision in the documentation and/or other materials provided with the
 * distribution.
 *
 * 3.4. Intel retains all right, title, and interest in and to the Original
 * Intel Code.
 *
 * 3.5. Neither the name Intel nor any other trademark owned or controlled by
 * Intel shall be used in advertising or otherwise to promote the sale, use or
 * other dealings in products derived from or relating to the Covered Code
 * without prior written authorization from Intel.
 *
 * 4. Disclaimer and Export Compliance
 *
 * 4.1. INTEL MAKES NO WARRANTY OF ANY KIND REGARDING ANY SOFTWARE PROVIDED
 * HERE. ANY SOFTWARE ORIGINATING FROM INTEL OR DERIVED FROM INTEL SOFTWARE
 * IS PROVIDED "AS IS," AND INTEL WILL NOT PROVIDE ANY SUPPORT, ASSISTANCE,
 * INSTALLATION, TRAINING OR OTHER SERVICES. INTEL WILL NOT PROVIDE ANY
 * UPDATES, ENHANCEMENTS OR EXTENSIONS. INTEL SPECIFICALLY DISCLAIMS ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * 4.2. IN NO EVENT SHALL INTEL HAVE ANY LIABILITY TO LICENSEE, ITS LICENSEES
 * OR ANY OTHER THIRD PARTY, FOR ANY LOST PROFITS, LOST DATA, LOSS OF USE OR
 * COSTS OF PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, OR FOR ANY INDIRECT,
 * SPECIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THIS AGREEMENT, UNDER ANY
 * CAUSE OF ACTION OR THEORY OF LIABILITY, AND IRRESPECTIVE OF WHETHER INTEL
 * HAS ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES. THESE LIMITATIONS
 * SHALL APPLY NOTWITHSTANDING THE FAILURE OF THE ESSENTIAL PURPOSE OF ANY
 * LIMITED REMEDY.
 *
 * 4.3. Licensee shall not export, either directly or indirectly, any of this
 * software or system incorporating such software without first obtaining any
 * required license or other approval from the U. S. Department of Commerce or
 * any other agency or department of the United States Government. In the
 * event Licensee exports any such software from the United States or
 * re-exports any such software from a foreign destination, Licensee shall
 * ensure that the distribution and export/re-export of the software is in
 * compliance with all laws, regulations, orders, or other restrictions of the
 * U.S. Export Administration Regulations. Licensee agrees that neither it nor
 * any of its subsidiaries will export/re-export any technical data, process,
 * software, or service, directly or indirectly, to any country for which the
 * United States government or any agency thereof requires an export license,
 * other governmental approval, or letter of assurance, without first obtaining
 * such license, approval or letter.
 *
 *****************************************************************************/

#ifndef __ACMACROS_H__
#define __ACMACROS_H__


/*
 * Extract data using a pointer. Any more than a byte and we
 * get into potential aligment issues -- see the STORE macros below.
 * Use with care.
 */
#define ACPI_CAST8(ptr)                 ACPI_CAST_PTR (UINT8, (ptr))
#define ACPI_CAST16(ptr)                ACPI_CAST_PTR (UINT16, (ptr))
#define ACPI_CAST32(ptr)                ACPI_CAST_PTR (UINT32, (ptr))
#define ACPI_CAST64(ptr)                ACPI_CAST_PTR (UINT64, (ptr))
#define ACPI_GET8(ptr)                  (*ACPI_CAST8 (ptr))
#define ACPI_GET16(ptr)                 (*ACPI_CAST16 (ptr))
#define ACPI_GET32(ptr)                 (*ACPI_CAST32 (ptr))
#define ACPI_GET64(ptr)                 (*ACPI_CAST64 (ptr))
#define ACPI_SET8(ptr, val)             (*ACPI_CAST8 (ptr) = (UINT8) (val))
#define ACPI_SET16(ptr, val)            (*ACPI_CAST16 (ptr) = (UINT16) (val))
#define ACPI_SET32(ptr, val)            (*ACPI_CAST32 (ptr) = (UINT32) (val))
#define ACPI_SET64(ptr, val)            (*ACPI_CAST64 (ptr) = (UINT64) (val))

/*
 * printf() format helpers
 */

/* Split 64-bit integer into two 32-bit values. Use with %8.8X%8.8X */

#define ACPI_FORMAT_UINT64(i)           ACPI_HIDWORD(i), ACPI_LODWORD(i)

#if ACPI_MACHINE_WIDTH == 64
#define ACPI_FORMAT_NATIVE_UINT(i)      ACPI_FORMAT_UINT64(i)
#else
#define ACPI_FORMAT_NATIVE_UINT(i)      0, (i)
#endif


/*
 * Macros for moving data around to/from buffers that are possibly unaligned.
 * If the hardware supports the transfer of unaligned data, just do the store.
 * Otherwise, we have to move one byte at a time.
 */
#ifdef ACPI_BIG_ENDIAN
/*
 * Macros for big-endian machines
 */

/* These macros reverse the bytes during the move, converting little-endian to big endian */

                                                     /* Big Endian      <==        Little Endian */
                                                     /*  Hi...Lo                     Lo...Hi     */
/* 16-bit source, 16/32/64 destination */

#define ACPI_MOVE_16_TO_16(d, s)        {((  UINT8 *)(void *)(d))[0] = ((UINT8 *)(void *)(s))[1];\
                                         ((  UINT8 *)(void *)(d))[1] = ((UINT8 *)(void *)(s))[0];}

#define ACPI_MOVE_16_TO_32(d, s)        {(*(UINT32 *)(void *)(d))=0;\
                                           ((UINT8 *)(void *)(d))[2] = ((UINT8 *)(void *)(s))[1];\
                                           ((UINT8 *)(void *)(d))[3] = ((UINT8 *)(void *)(s))[0];}

#define ACPI_MOVE_16_TO_64(d, s)        {(*(UINT64 *)(void *)(d))=0;\
                                           ((UINT8 *)(void *)(d))[6] = ((UINT8 *)(void *)(s))[1];\
                                           ((UINT8 *)(void *)(d))[7] = ((UINT8 *)(void *)(s))[0];}

/* 32-bit source, 16/32/64 destination */

#define ACPI_MOVE_32_TO_16(d, s)        ACPI_MOVE_16_TO_16(d, s)    /* Truncate to 16 */

#define ACPI_MOVE_32_TO_32(d, s)        {((  UINT8 *)(void *)(d))[0] = ((UINT8 *)(void *)(s))[3];\
                                         ((  UINT8 *)(void *)(d))[1] = ((UINT8 *)(void *)(s))[2];\
                                         ((  UINT8 *)(void *)(d))[2] = ((UINT8 *)(void *)(s))[1];\
                                         ((  UINT8 *)(void *)(d))[3] = ((UINT8 *)(void *)(s))[0];}

#define ACPI_MOVE_32_TO_64(d, s)        {(*(UINT64 *)(void *)(d))=0;\
                                           ((UINT8 *)(void *)(d))[4] = ((UINT8 *)(void *)(s))[3];\
                                           ((UINT8 *)(void *)(d))[5] = ((UINT8 *)(void *)(s))[2];\
                                           ((UINT8 *)(void *)(d))[6] = ((UINT8 *)(void *)(s))[1];\
                                           ((UINT8 *)(void *)(d))[7] = ((UINT8 *)(void *)(s))[0];}

/* 64-bit source, 16/32/64 destination */

#define ACPI_MOVE_64_TO_16(d, s)        ACPI_MOVE_16_TO_16(d, s)    /* Truncate to 16 */

#define ACPI_MOVE_64_TO_32(d, s)        ACPI_MOVE_32_TO_32(d, s)    /* Truncate to 32 */

#define ACPI_MOVE_64_TO_64(d, s)        {((  UINT8 *)(void *)(d))[0] = ((UINT8 *)(void *)(s))[7];\
                                         ((  UINT8 *)(void *)(d))[1] = ((UINT8 *)(void *)(s))[6];\
                                         ((  UINT8 *)(void *)(d))[2] = ((UINT8 *)(void *)(s))[5];\
                                         ((  UINT8 *)(void *)(d))[3] = ((UINT8 *)(void *)(s))[4];\
                                         ((  UINT8 *)(void *)(d))[4] = ((UINT8 *)(void *)(s))[3];\
                                         ((  UINT8 *)(void *)(d))[5] = ((UINT8 *)(void *)(s))[2];\
                                         ((  UINT8 *)(void *)(d))[6] = ((UINT8 *)(void *)(s))[1];\
                                         ((  UINT8 *)(void *)(d))[7] = ((UINT8 *)(void *)(s))[0];}
#else
/*
 * Macros for little-endian machines
 */

#ifndef ACPI_MISALIGNMENT_NOT_SUPPORTED

/* The hardware supports unaligned transfers, just do the little-endian move */

/* 16-bit source, 16/32/64 destination */

#define ACPI_MOVE_16_TO_16(d, s)        *(UINT16 *)(void *)(d) = *(UINT16 *)(void *)(s)
#define ACPI_MOVE_16_TO_32(d, s)        *(UINT32 *)(void *)(d) = *(UINT16 *)(void *)(s)
#define ACPI_MOVE_16_TO_64(d, s)        *(UINT64 *)(void *)(d) = *(UINT16 *)(void *)(s)

/* 32-bit source, 16/32/64 destination */

#define ACPI_MOVE_32_TO_16(d, s)        ACPI_MOVE_16_TO_16(d, s)    /* Truncate to 16 */
#define ACPI_MOVE_32_TO_32(d, s)        *(UINT32 *)(void *)(d) = *(UINT32 *)(void *)(s)
#define ACPI_MOVE_32_TO_64(d, s)        *(UINT64 *)(void *)(d) = *(UINT32 *)(void *)(s)

/* 64-bit source, 16/32/64 destination */

#define ACPI_MOVE_64_TO_16(d, s)        ACPI_MOVE_16_TO_16(d, s)    /* Truncate to 16 */
#define ACPI_MOVE_64_TO_32(d, s)        ACPI_MOVE_32_TO_32(d, s)    /* Truncate to 32 */
#define ACPI_MOVE_64_TO_64(d, s)        *(UINT64 *)(void *)(d) = *(UINT64 *)(void *)(s)

#else
/*
 * The hardware does not support unaligned transfers. We must move the
 * data one byte at a time. These macros work whether the source or
 * the destination (or both) is/are unaligned. (Little-endian move)
 */

/* 16-bit source, 16/32/64 destination */

#define ACPI_MOVE_16_TO_16(d, s)        {((  UINT8 *)(void *)(d))[0] = ((UINT8 *)(void *)(s))[0];\
                                         ((  UINT8 *)(void *)(d))[1] = ((UINT8 *)(void *)(s))[1];}

#define ACPI_MOVE_16_TO_32(d, s)        {(*(UINT32 *)(void *)(d)) = 0; ACPI_MOVE_16_TO_16(d, s);}
#define ACPI_MOVE_16_TO_64(d, s)        {(*(UINT64 *)(void *)(d)) = 0; ACPI_MOVE_16_TO_16(d, s);}

/* 32-bit source, 16/32/64 destination */

#define ACPI_MOVE_32_TO_16(d, s)        ACPI_MOVE_16_TO_16(d, s)    /* Truncate to 16 */

#define ACPI_MOVE_32_TO_32(d, s)        {((  UINT8 *)(void *)(d))[0] = ((UINT8 *)(void *)(s))[0];\
                                         ((  UINT8 *)(void *)(d))[1] = ((UINT8 *)(void *)(s))[1];\
                                         ((  UINT8 *)(void *)(d))[2] = ((UINT8 *)(void *)(s))[2];\
                                         ((  UINT8 *)(void *)(d))[3] = ((UINT8 *)(void *)(s))[3];}

#define ACPI_MOVE_32_TO_64(d, s)        {(*(UINT64 *)(void *)(d)) = 0; ACPI_MOVE_32_TO_32(d, s);}

/* 64-bit source, 16/32/64 destination */

#define ACPI_MOVE_64_TO_16(d, s)        ACPI_MOVE_16_TO_16(d, s)    /* Truncate to 16 */
#define ACPI_MOVE_64_TO_32(d, s)        ACPI_MOVE_32_TO_32(d, s)    /* Truncate to 32 */
#define ACPI_MOVE_64_TO_64(d, s)        {((  UINT8 *)(void *)(d))[0] = ((UINT8 *)(void *)(s))[0];\
                                         ((  UINT8 *)(void *)(d))[1] = ((UINT8 *)(void *)(s))[1];\
                                         ((  UINT8 *)(void *)(d))[2] = ((UINT8 *)(void *)(s))[2];\
                                         ((  UINT8 *)(void *)(d))[3] = ((UINT8 *)(void *)(s))[3];\
                                         ((  UINT8 *)(void *)(d))[4] = ((UINT8 *)(void *)(s))[4];\
                                         ((  UINT8 *)(void *)(d))[5] = ((UINT8 *)(void *)(s))[5];\
                                         ((  UINT8 *)(void *)(d))[6] = ((UINT8 *)(void *)(s))[6];\
                                         ((  UINT8 *)(void *)(d))[7] = ((UINT8 *)(void *)(s))[7];}
#endif
#endif


/*
 * Fast power-of-two math macros for non-optimized compilers
 */
#define _ACPI_DIV(value, PowerOf2)      ((UINT32) ((value) >> (PowerOf2)))
#define _ACPI_MUL(value, PowerOf2)      ((UINT32) ((value) << (PowerOf2)))
#define _ACPI_MOD(value, Divisor)       ((UINT32) ((value) & ((Divisor) -1)))

#define ACPI_DIV_2(a)                   _ACPI_DIV(a, 1)
#define ACPI_MUL_2(a)                   _ACPI_MUL(a, 1)
#define ACPI_MOD_2(a)                   _ACPI_MOD(a, 2)

#define ACPI_DIV_4(a)                   _ACPI_DIV(a, 2)
#define ACPI_MUL_4(a)                   _ACPI_MUL(a, 2)
#define ACPI_MOD_4(a)                   _ACPI_MOD(a, 4)

#define ACPI_DIV_8(a)                   _ACPI_DIV(a, 3)
#define ACPI_MUL_8(a)                   _ACPI_MUL(a, 3)
#define ACPI_MOD_8(a)                   _ACPI_MOD(a, 8)

#define ACPI_DIV_16(a)                  _ACPI_DIV(a, 4)
#define ACPI_MUL_16(a)                  _ACPI_MUL(a, 4)
#define ACPI_MOD_16(a)                  _ACPI_MOD(a, 16)

#define ACPI_DIV_32(a)                  _ACPI_DIV(a, 5)
#define ACPI_MUL_32(a)                  _ACPI_MUL(a, 5)
#define ACPI_MOD_32(a)                  _ACPI_MOD(a, 32)

/*
 * Rounding macros (Power of two boundaries only)
 */
#define ACPI_ROUND_DOWN(value, boundary)    (((ACPI_SIZE)(value)) & \
                                                (~(((ACPI_SIZE) boundary)-1)))

#define ACPI_ROUND_UP(value, boundary)      ((((ACPI_SIZE)(value)) + \
                                                (((ACPI_SIZE) boundary)-1)) & \
                                                (~(((ACPI_SIZE) boundary)-1)))

/* Note: sizeof(ACPI_SIZE) evaluates to either 4 or 8 (32- vs 64-bit mode) */

#define ACPI_ROUND_DOWN_TO_32BIT(a)         ACPI_ROUND_DOWN(a, 4)
#define ACPI_ROUND_DOWN_TO_64BIT(a)         ACPI_ROUND_DOWN(a, 8)
#define ACPI_ROUND_DOWN_TO_NATIVE_WORD(a)   ACPI_ROUND_DOWN(a, sizeof(ACPI_SIZE))

#define ACPI_ROUND_UP_TO_32BIT(a)           ACPI_ROUND_UP(a, 4)
#define ACPI_ROUND_UP_TO_64BIT(a)           ACPI_ROUND_UP(a, 8)
#define ACPI_ROUND_UP_TO_NATIVE_WORD(a)     ACPI_ROUND_UP(a, sizeof(ACPI_SIZE))

#define ACPI_ROUND_BITS_UP_TO_BYTES(a)      ACPI_DIV_8((a) + 7)
#define ACPI_ROUND_BITS_DOWN_TO_BYTES(a)    ACPI_DIV_8((a))

#define ACPI_ROUND_UP_TO_1K(a)              (((a) + 1023) >> 10)

/* Generic (non-power-of-two) rounding */

#define ACPI_ROUND_UP_TO(value, boundary)   (((value) + ((boundary)-1)) / (boundary))

#define ACPI_IS_MISALIGNED(value)           (((ACPI_SIZE) value) & (sizeof(ACPI_SIZE)-1))

/*
 * Bitmask creation
 * Bit positions start at zero.
 * MASK_BITS_ABOVE creates a mask starting AT the position and above
 * MASK_BITS_BELOW creates a mask starting one bit BELOW the position
 */
#define ACPI_MASK_BITS_ABOVE(position)      (~((ACPI_UINT64_MAX) << ((UINT32) (position))))
#define ACPI_MASK_BITS_BELOW(position)      ((ACPI_UINT64_MAX) << ((UINT32) (position)))

/* Bitfields within ACPI registers */

#define ACPI_REGISTER_PREPARE_BITS(Val, Pos, Mask) \
    ((Val << Pos) & Mask)

#define ACPI_REGISTER_INSERT_VALUE(Reg, Pos, Mask, Val) \
    Reg = (Reg & (~(Mask))) | ACPI_REGISTER_PREPARE_BITS(Val, Pos, Mask)

#define ACPI_INSERT_BITS(Target, Mask, Source) \
    Target = ((Target & (~(Mask))) | (Source & Mask))

/* Generic bitfield macros and masks */

#define ACPI_GET_BITS(SourcePtr, Position, Mask) \
    ((*SourcePtr >> Position) & Mask)

#define ACPI_SET_BITS(TargetPtr, Position, Mask, Value) \
    (*TargetPtr |= ((Value & Mask) << Position))

#define ACPI_1BIT_MASK      0x00000001
#define ACPI_2BIT_MASK      0x00000003
#define ACPI_3BIT_MASK      0x00000007
#define ACPI_4BIT_MASK      0x0000000F
#define ACPI_5BIT_MASK      0x0000001F
#define ACPI_6BIT_MASK      0x0000003F
#define ACPI_7BIT_MASK      0x0000007F
#define ACPI_8BIT_MASK      0x000000FF
#define ACPI_16BIT_MASK     0x0000FFFF
#define ACPI_24BIT_MASK     0x00FFFFFF

/* Macros to extract flag bits from position zero */

#define ACPI_GET_1BIT_FLAG(Value)                   ((Value) & ACPI_1BIT_MASK)
#define ACPI_GET_2BIT_FLAG(Value)                   ((Value) & ACPI_2BIT_MASK)
#define ACPI_GET_3BIT_FLAG(Value)                   ((Value) & ACPI_3BIT_MASK)
#define ACPI_GET_4BIT_FLAG(Value)                   ((Value) & ACPI_4BIT_MASK)

/* Macros to extract flag bits from position one and above */

#define ACPI_EXTRACT_1BIT_FLAG(Field, Position)     (ACPI_GET_1BIT_FLAG ((Field) >> Position))
#define ACPI_EXTRACT_2BIT_FLAG(Field, Position)     (ACPI_GET_2BIT_FLAG ((Field) >> Position))
#define ACPI_EXTRACT_3BIT_FLAG(Field, Position)     (ACPI_GET_3BIT_FLAG ((Field) >> Position))
#define ACPI_EXTRACT_4BIT_FLAG(Field, Position)     (ACPI_GET_4BIT_FLAG ((Field) >> Position))

/* ACPI Pathname helpers */

#define ACPI_IS_ROOT_PREFIX(c)      ((c) == (UINT8) 0x5C) /* Backslash */
#define ACPI_IS_PARENT_PREFIX(c)    ((c) == (UINT8) 0x5E) /* Carat */
#define ACPI_IS_PATH_SEPARATOR(c)   ((c) == (UINT8) 0x2E) /* Period (dot) */

/*
 * An object of type ACPI_NAMESPACE_NODE can appear in some contexts
 * where a pointer to an object of type ACPI_OPERAND_OBJECT can also
 * appear. This macro is used to distinguish them.
 *
 * The "Descriptor" field is the first field in both structures.
 */
#define ACPI_GET_DESCRIPTOR_TYPE(d)     (((ACPI_DESCRIPTOR *)(void *)(d))->Common.DescriptorType)
#define ACPI_SET_DESCRIPTOR_TYPE(d, t)  (((ACPI_DESCRIPTOR *)(void *)(d))->Common.DescriptorType = t)

/*
 * Macros for the master AML opcode table
 */
#if defined (ACPI_DISASSEMBLER) || defined (ACPI_DEBUG_OUTPUT)
#define ACPI_OP(Name, PArgs, IArgs, ObjType, Class, Type, Flags) \
    {Name, (UINT32)(PArgs), (UINT32)(IArgs), (UINT32)(Flags), ObjType, Class, Type}
#else
#define ACPI_OP(Name, PArgs, IArgs, ObjType, Class, Type, Flags) \
    {(UINT32)(PArgs), (UINT32)(IArgs), (UINT32)(Flags), ObjType, Class, Type}
#endif

#define ARG_TYPE_WIDTH                  5
#define ARG_1(x)                        ((UINT32)(x))
#define ARG_2(x)                        ((UINT32)(x) << (1 * ARG_TYPE_WIDTH))
#define ARG_3(x)                        ((UINT32)(x) << (2 * ARG_TYPE_WIDTH))
#define ARG_4(x)                        ((UINT32)(x) << (3 * ARG_TYPE_WIDTH))
#define ARG_5(x)                        ((UINT32)(x) << (4 * ARG_TYPE_WIDTH))
#define ARG_6(x)                        ((UINT32)(x) << (5 * ARG_TYPE_WIDTH))

#define ARGI_LIST1(a)                   (ARG_1(a))
#define ARGI_LIST2(a, b)                (ARG_1(b)|ARG_2(a))
#define ARGI_LIST3(a, b, c)             (ARG_1(c)|ARG_2(b)|ARG_3(a))
#define ARGI_LIST4(a, b, c, d)          (ARG_1(d)|ARG_2(c)|ARG_3(b)|ARG_4(a))
#define ARGI_LIST5(a, b, c, d, e)       (ARG_1(e)|ARG_2(d)|ARG_3(c)|ARG_4(b)|ARG_5(a))
#define ARGI_LIST6(a, b, c, d, e, f)    (ARG_1(f)|ARG_2(e)|ARG_3(d)|ARG_4(c)|ARG_5(b)|ARG_6(a))

#define ARGP_LIST1(a)                   (ARG_1(a))
#define ARGP_LIST2(a, b)                (ARG_1(a)|ARG_2(b))
#define ARGP_LIST3(a, b, c)             (ARG_1(a)|ARG_2(b)|ARG_3(c))
#define ARGP_LIST4(a, b, c, d)          (ARG_1(a)|ARG_2(b)|ARG_3(c)|ARG_4(d))
#define ARGP_LIST5(a, b, c, d, e)       (ARG_1(a)|ARG_2(b)|ARG_3(c)|ARG_4(d)|ARG_5(e))
#define ARGP_LIST6(a, b, c, d, e, f)    (ARG_1(a)|ARG_2(b)|ARG_3(c)|ARG_4(d)|ARG_5(e)|ARG_6(f))

#define GET_CURRENT_ARG_TYPE(List)      (List & ((UINT32) 0x1F))
#define INCREMENT_ARG_LIST(List)        (List >>= ((UINT32) ARG_TYPE_WIDTH))

/*
 * Ascii error messages can be configured out
 */
#ifndef ACPI_NO_ERROR_MESSAGES
/*
 * Error reporting. Callers module and line number are inserted by AE_INFO,
 * the plist contains a set of parens to allow variable-length lists.
 * These macros are used for both the debug and non-debug versions of the code.
 */
#define ACPI_ERROR_NAMESPACE(s, e)      AcpiUtNamespaceError (AE_INFO, s, e);
#define ACPI_ERROR_METHOD(s, n, p, e)   AcpiUtMethodError (AE_INFO, s, n, p, e);
#define ACPI_WARN_PREDEFINED(plist)     AcpiUtPredefinedWarning plist
#define ACPI_INFO_PREDEFINED(plist)     AcpiUtPredefinedInfo plist

#else

/* No error messages */

#define ACPI_ERROR_NAMESPACE(s, e)
#define ACPI_ERROR_METHOD(s, n, p, e)
#define ACPI_WARN_PREDEFINED(plist)
#define ACPI_INFO_PREDEFINED(plist)

#endif /* ACPI_NO_ERROR_MESSAGES */

#if (!ACPI_REDUCED_HARDWARE)
#define ACPI_HW_OPTIONAL_FUNCTION(addr)     addr
#else
#define ACPI_HW_OPTIONAL_FUNCTION(addr)     NULL
#endif


/*
 * Some code only gets executed when the debugger is built in.
 * Note that this is entirely independent of whether the
 * DEBUG_PRINT stuff (set by ACPI_DEBUG_OUTPUT) is on, or not.
 */
#ifdef ACPI_DEBUGGER
#define ACPI_DEBUGGER_EXEC(a)           a
#else
#define ACPI_DEBUGGER_EXEC(a)
#endif


/*
 * Memory allocation tracking (DEBUG ONLY)
 */
#define ACPI_MEM_PARAMETERS         _COMPONENT, _AcpiModuleName, __LINE__

#ifndef ACPI_DBG_TRACK_ALLOCATIONS

/* Memory allocation */

#define ACPI_ALLOCATE(a)            AcpiUtAllocate((ACPI_SIZE) (a), ACPI_MEM_PARAMETERS)
#define ACPI_ALLOCATE_ZEROED(a)     AcpiUtAllocateZeroed((ACPI_SIZE) (a), ACPI_MEM_PARAMETERS)
#define ACPI_FREE(a)                AcpiOsFree(a)
#define ACPI_MEM_TRACKING(a)

#else

/* Memory allocation */

#define ACPI_ALLOCATE(a)            AcpiUtAllocateAndTrack((ACPI_SIZE) (a), ACPI_MEM_PARAMETERS)
#define ACPI_ALLOCATE_ZEROED(a)     AcpiUtAllocateZeroedAndTrack((ACPI_SIZE) (a), ACPI_MEM_PARAMETERS)
#define ACPI_FREE(a)                AcpiUtFreeAndTrack(a, ACPI_MEM_PARAMETERS)
#define ACPI_MEM_TRACKING(a)        a

#endif /* ACPI_DBG_TRACK_ALLOCATIONS */


/*
 * Macros used for ACPICA utilities only
 */

/* Generate a UUID */

#define ACPI_INIT_UUID(a, b, c, d0, d1, d2, d3, d4, d5, d6, d7) \
    (a) & 0xFF, ((a) >> 8) & 0xFF, ((a) >> 16) & 0xFF, ((a) >> 24) & 0xFF, \
    (b) & 0xFF, ((b) >> 8) & 0xFF, \
    (c) & 0xFF, ((c) >> 8) & 0xFF, \
    (d0), (d1), (d2), (d3), (d4), (d5), (d6), (d7)

#define ACPI_IS_OCTAL_DIGIT(d)              (((char)(d) >= '0') && ((char)(d) <= '7'))


#endif /* ACMACROS_H */
