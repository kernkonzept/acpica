
/******************************************************************************
 * 
 * Name: acpiobj.h - Definition of ACPI_OBJECT_INTERNAL (Internal object only)
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999, Intel Corp.  All rights
 * reserved.
 *
 * 2. License
 *
 * 2.1. This is your license from Intel Corp. under its intellectual property
 * rights.  You may have additional license terms from the party that provided
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
 * license (without the right to sublicense), under only those claims of Intel
 * patents that are infringed by the Original Intel Code, to make, use, sell,
 * offer to sell, and import the Covered Code and derivative works thereof
 * solely to the minimum extent necessary to exercise the above copyright
 * license, and in no event shall the patent license extend to any additions
 * to or modifications of the Original Intel Code.  No other license or right
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
 * and the following Disclaimer and Export Compliance provision.  In addition,
 * Licensee must cause all Covered Code to which Licensee contributes to
 * contain a file documenting the changes Licensee made to create that Covered
 * Code and the date of any change.  Licensee must include in that file the
 * documentation of any changes made by any predecessor Licensee.  Licensee 
 * must include a prominent statement that the modification is derived,
 * directly or indirectly, from Original Intel Code.
 *
 * 3.2. Redistribution of Source with no Rights to Further Distribute Source.  
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification without rights to further distribute source must
 * include the following Disclaimer and Export Compliance provision in the
 * documentation and/or other materials provided with distribution.  In
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
 * HERE.  ANY SOFTWARE ORIGINATING FROM INTEL OR DERIVED FROM INTEL SOFTWARE
 * IS PROVIDED "AS IS," AND INTEL WILL NOT PROVIDE ANY SUPPORT,  ASSISTANCE,
 * INSTALLATION, TRAINING OR OTHER SERVICES.  INTEL WILL NOT PROVIDE ANY
 * UPDATES, ENHANCEMENTS OR EXTENSIONS.  INTEL SPECIFICALLY DISCLAIMS ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE. 
 *
 * 4.2. IN NO EVENT SHALL INTEL HAVE ANY LIABILITY TO LICENSEE, ITS LICENSEES
 * OR ANY OTHER THIRD PARTY, FOR ANY LOST PROFITS, LOST DATA, LOSS OF USE OR
 * COSTS OF PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, OR FOR ANY INDIRECT,
 * SPECIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THIS AGREEMENT, UNDER ANY
 * CAUSE OF ACTION OR THEORY OF LIABILITY, AND IRRESPECTIVE OF WHETHER INTEL
 * HAS ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.  THESE LIMITATIONS
 * SHALL APPLY NOTWITHSTANDING THE FAILURE OF THE ESSENTIAL PURPOSE OF ANY
 * LIMITED REMEDY.
 *
 * 4.3. Licensee shall not export, either directly or indirectly, any of this
 * software or system incorporating such software without first obtaining any
 * required license or other approval from the U. S. Department of Commerce or
 * any other agency or department of the United States Government.  In the
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

#ifndef _ACPIOBJ_H
#define _ACPIOBJ_H

#include <acpitypes.h>

/* 
 * All variants of the ACPI_OBJECT_INTERNAL are defined with the same
 * sequence of field types, with fields that are not used in a particular
 * variant being named "Reserved".  This is not strictly necessary, but
 * may in some circumstances simplify understanding if these structures
 * need to be displayed in a debugger having limited (or no) support for
 * union types.  It also simplifies some debug code in NsDumpTable() which
 * dumps multi-level values: fetching Buffer.Pointer suffices to pick up
 * the value or next level for any of several types.
 */

/*
 * The basic union is
 *   UINT32
 *   UINT32
 *   UINT32
 *   Pointer
 *   Pointer
 *   Pointer
 */

typedef union AcpiObjInternal           /* ACPI_OBJECT_INTERNAL DESCRIPTOR */
{
    UINT8                   Type;           /* See definition of NsType for values */

    struct /* NUMBER - has value */
    {
        UINT8                   Type;
        UINT8                   Reserved1;
        UINT16                  Reserved2;
        UINT32                  Value;
        UINT32                  Reserved3;
        void                    *Reserved_p1;
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } Number;


    struct /* STRING - has length and pointer */
    {
        UINT8                   Type;
        UINT8                   Reserved1;
        UINT16                  Length;         /* # of bytes in string, excluding trailing null */
        UINT32                  Reserved2;
        UINT32                  Reserved3;
        UINT8                   *Pointer;        /* points to the string value in the AML stream
                                                 * or in allocated space
                                                 */
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } String;


    struct /* BUFFER - has length, sequence, and pointer */
    {
        UINT8                   Type;
        UINT8                   Reserved1;
        UINT16                  Length;         /* # of bytes in buffer */
        UINT32                  Reserved2;
        UINT32                  Sequence;       /* Sequential count of buffers created */
        UINT8                   *Pointer;       /* points to the buffer in allocated space */
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } Buffer;


    struct /* PACKAGE - has count, elements, next element */
    {
        UINT8                   Type;
        UINT8                   Reserved1;
        UINT16                  Count;          /* # of elements in package */
        UINT32                  Reserved2;
        UINT32                  Reserved3;
        union AcpiObjInternal   **Elements;     /* Addr of an allocated array of pointers
                                                 * to the OBJECT_DESCRIPTORs representing
                                                 * the elements
                                                 */
        union AcpiObjInternal   **NextElement;  /* used only while initializing */
        void                    *Reserved_p3;

    } Package;


    struct /* FIELD UNIT */
    {
        /* Using WORD_BIT instead of BYTE_BIT here because the Length field
         * is larger than a UINT8.  It is possible that some implementations
         * may map this in an unexpected way -- see code and comments in
         * amlexec.c:iPrep*FieldValue() -- but it works properly in IC386
         * and in MS Visual C++
         */
        UINT16_BIT              Type        : 8;
        UINT16_BIT              Access      : 4;
        UINT16_BIT              LockRule    : 1;
        UINT16_BIT              UpdateRule  : 2;
        UINT16_BIT              Reserved1   : 1;
        UINT16_BIT              Length      :13;    /* # of bits in buffer */
        UINT16_BIT              BitOffset   : 3;
        UINT32                  Offset;             /* Byte offset within containing object */
        UINT32                  Sequence;           /* Container's sequence number */
        union AcpiObjInternal   *Container;         /* Containing object (Buffer) */
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } FieldUnit;


    struct /* DEVICE - has handle and notification handler/context */
    {
        UINT8                   Type;
        UINT8                   Reserved1;
        UINT16                  Reserved2;
        UINT32                  Reserved3;
        UINT32                  Reserved4;
        ACPI_HANDLE             Handle;
        NOTIFY_HANDLER          Handler;
        void                    *Context;

    } Device;


    struct /* EVENT */
    {
        UINT8                   Type;
        UINT8                   Reserved1;
        UINT16                  SignalCount;
        UINT32                  Semaphore;
        UINT16                  LockCount;
        UINT16                  ThreadId;
        void                    *Reserved_p1;
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } Event;


    struct /* METHOD */
    {
        UINT8                   Type;
        UINT8                   ParamCount;
        UINT16                  Length;
        UINT32                  AmlOffset;
        UINT32                  Reserved3;
        UINT8                   *AmlBase;
        void                    *Reserved4;

    } Method;


    struct /* MUTEX */
    {
        UINT8                   Type;
        UINT8                   SyncLevel;
        UINT16                  Reserved2;
        UINT32                  Semaphore;
        UINT16                  LockCount;
        UINT16                  ThreadId;
        void                    *Reserved_p1;
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } Mutex;


    struct /* REGION */
    {
        UINT8                   Type;
        UINT8                   SpaceId;
        UINT16                  DataValid;      /* 1 => Address & Length have been set
                                                 * 0 => Address & Length have not been set,
                                                 *        and should be obtained via AdrLoc
                                                 */
        UINT32                  Address;
        UINT32                  Length;
        METHOD_INFO             AddressLocation;/* Loc of 1st (address) OpCode in AML stream */

    } Region;


    struct /* POWER RESOURCE - has Handle */
    {
        UINT8                   Type;
        UINT8                   Reserved1;
        UINT16                  Reserved2;
        UINT32                  Reserved3;
        UINT32                  Reserved4;
        ACPI_HANDLE             Handle;
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } PowerResource;


    struct /* PROCESSOR - has Handle */
    {
        UINT8                   Type;
        UINT8                   Reserved1;
        UINT16                  Reserved2;
        UINT32                  Reserved3;
        UINT32                  Reserved4;
        ACPI_HANDLE             Handle;
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } Processor;


    struct /* THERMAL ZONE - has Handle and Handler/Context */
    {
        UINT8                   Type;
        UINT8                   Reserved1;
        UINT16                  Reserved2;
        UINT32                  Reserved3;
        UINT32                  Reserved4;
        ACPI_HANDLE             Handle;
        NOTIFY_HANDLER          Handler;
        void                    *Context;

    } ThermalZone;


    struct /* FIELD */
    {
        /* See comments in FieldUnit about use of WORD_BIT */

        UINT16_BIT              Type        : 8;
        UINT16_BIT              Access      : 4;
        UINT16_BIT              LockRule    : 1;
        UINT16_BIT              UpdateRule  : 2;
        UINT16_BIT              Reserved1   : 1;
        UINT16_BIT              Length      :13;    /* # of bits in buffer */
        UINT16_BIT              BitOffset   : 3;
        UINT32                  Offset;             /* Byte offset within containing object */
        UINT32                  Reserved2;
        union AcpiObjInternal   *Container;         /* Containing object */
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } Field;


    struct /* BANK FIELD */
    {
        /* See comments in FieldUnit about use of WORD_BIT */

        UINT16_BIT              Type        : 8;
        UINT16_BIT              Access      : 4;
        UINT16_BIT              LockRule    : 1;
        UINT16_BIT              UpdateRule  : 2;
        UINT16_BIT              Reserved1   : 1;
        UINT16_BIT              Length      :13;    /* # of bits in buffer */
        UINT16_BIT              BitOffset   : 3;
        UINT32                  Offset;             /* Byte offset within containing object */
        UINT32                  Value;              /* Value to store into BankSelect */
        union AcpiObjInternal   *Container;         /* Containing object */
        ACPI_HANDLE             BankSelect;         /* Bank select register */
        void                    *Reserved_p3;

    } BankField;


    struct /* INDEX FIELD */
    {
        /* See comments in FieldUnit about use of WORD_BIT */

        UINT16_BIT              Type        : 8;
        UINT16_BIT              Access      : 4;
        UINT16_BIT              LockRule    : 1;
        UINT16_BIT              UpdateRule  : 2;
        UINT16_BIT              Reserved1   : 1;
        UINT16_BIT              Length      :13;    /* # of bits in buffer */
        UINT16_BIT              BitOffset   : 3;
        UINT32                  Value;              /* Value to store into Index register */
        UINT32                  Reserved2;          /* No container pointer needed since the index
                                                     * and data register definitions will define
                                                     * how to access the respective registers
                                                     */
        ACPI_HANDLE             Index;              /* Index register */
        ACPI_HANDLE             Data;               /* Data register */
        void                    *Reserved_p3;

    } IndexField;


    struct /* Lvalue - Local object type */
    {
        UINT8                   Type;
        UINT8                   OpCode;             /* Arg#, Local#, IndexOp, NameOp,
                                                     * ZeroOp, OneOp, OnesOp, Debug1 => DebugOp
                                                     */
        UINT16                  Reserved1;
        UINT32                  Reserved2;
        UINT32                  Reserved3;
        void                    *Object;            /* OpCode   Use of Object field
                                                     * -------  ----------------------------
                                                     * NameOp   ACPI_HANDLE for referenced name
                                                     * IndexOp  ACPI_OBJECT_INTERNAL **
                                                     */
        void                    *Reserved_p2;
        void                    *Reserved_p3;

    } Lvalue;

} ACPI_OBJECT_INTERNAL;


/* 
 * The sLvalue case of ACPI_OBJECT_INTERNAL includes a one-byte field which
 * contains an AML opcode identifying the type of lvalue.  Debug1 is used
 * in this field as a stand-in for the (two-byte) AML encoding of DebugOp.
 * TBD: Obsolete?
 */

#define Debug1          0x31



#endif

 