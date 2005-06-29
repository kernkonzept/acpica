/******************************************************************************
 *
 * Module Name: exdump - Interpreter debug output routines
 *              $Revision: 1.139 $
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999, 2000, 2001, Intel Corp.
 * All rights reserved.
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
 * license (with the right to sublicense), under only those claims of Intel
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

#define __EXDUMP_C__

#include "acpi.h"
#include "acinterp.h"
#include "amlcode.h"
#include "acnamesp.h"
#include "actables.h"
#include "acparser.h"

#define _COMPONENT          ACPI_EXECUTER
        MODULE_NAME         ("exdump")


/*
 * The following routines are used for debug output only
 */

#if defined(ACPI_DEBUG) || defined(ENABLE_DEBUGGER)

/*****************************************************************************
 *
 * FUNCTION:    AcpiExShowHexValue
 *
 * PARAMETERS:  ByteCount           - Number of bytes to print (1, 2, or 4)
 *              *AmlStart             - Address in AML stream of bytes to print
 *              InterpreterMode     - Current running mode (load1/Load2/Exec)
 *              LeadSpace           - # of spaces to print ahead of value
 *                                    0 => none ahead but one behind
 *
 * DESCRIPTION: Print ByteCount byte(s) starting at AmlStart as a single
 *              value, in hex.  If ByteCount > 1 or the value printed is > 9, also
 *              print in decimal.
 *
 ****************************************************************************/

void
AcpiExShowHexValue (
    UINT32                  ByteCount,
    UINT8                   *AmlStart,
    UINT32                  LeadSpace)
{
    UINT32                  Value;                  /*  Value retrieved from AML stream */
    UINT32                  ShowDecimalValue;
    UINT32                  Length;                 /*  Length of printed field */
    UINT8                   *CurrentAmlPtr = NULL;  /*  Pointer to current byte of AML value    */


    FUNCTION_TRACE ("ExShowHexValue");


    if (!((ACPI_LV_LOAD & AcpiDbgLevel) && (_COMPONENT & AcpiDbgLayer)))
    {
        return;
    }

    if (!AmlStart)
    {
        REPORT_ERROR (("ExShowHexValue: null pointer\n"));
        return;
    }

    /*
     * AML numbers are always stored little-endian,
     * even if the processor is big-endian.
     */
    for (CurrentAmlPtr = AmlStart + ByteCount,
            Value = 0;
            CurrentAmlPtr > AmlStart; )
    {
        Value = (Value << 8) + (UINT32)* --CurrentAmlPtr;
    }

    Length = LeadSpace * ByteCount + 2;
    if (ByteCount > 1)
    {
        Length += (ByteCount - 1);
    }

    ShowDecimalValue = (ByteCount > 1 || Value > 9);
    if (ShowDecimalValue)
    {
        Length += 3 + AcpiExDigitsNeeded (Value, 10);
    }

    for (Length = LeadSpace; Length; --Length )
    {
        AcpiOsPrintf (" ");
    }

    while (ByteCount--)
    {
        AcpiOsPrintf ("%02x", *AmlStart++);
        if (ByteCount)
        {
            AcpiOsPrintf (" ");
        }
    }

    if (ShowDecimalValue)
    {
        AcpiOsPrintf (" [%d]", Value);
    }

    if (0 == LeadSpace)
    {
        AcpiOsPrintf (" ");
    }

    AcpiOsPrintf ("\n");
    return_VOID;
}


/*****************************************************************************
 *
 * FUNCTION:    AcpiExDumpOperand
 *
 * PARAMETERS:  *ObjDesc          - Pointer to entry to be dumped
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Dump an operand object
 *
 ****************************************************************************/

ACPI_STATUS
AcpiExDumpOperand (
    ACPI_OPERAND_OBJECT     *ObjDesc)
{
    UINT8                   *Buf = NULL;
    UINT32                  Length;
    UINT32                  i;


    PROC_NAME ("ExDumpOperand")


    if (!((ACPI_LV_INFO & AcpiDbgLevel) && (_COMPONENT & AcpiDbgLayer)))
    {
        return (AE_OK);
    }

    if (!ObjDesc)
    {
        /*
         * This usually indicates that something serious is wrong --
         * since most (if not all)
         * code that dumps the stack expects something to be there!
         */
        ACPI_DEBUG_PRINT ((ACPI_DB_INFO, "Null stack entry ptr\n"));
        return (AE_OK);
    }

    if (ACPI_GET_DESCRIPTOR_TYPE (ObjDesc) == ACPI_DESC_TYPE_NAMED)
    {
        ACPI_DEBUG_PRINT ((ACPI_DB_INFO, "%p NS Node: ", ObjDesc));
        DUMP_ENTRY (ObjDesc, ACPI_LV_INFO);
        return (AE_OK);
    }

    if (ACPI_GET_DESCRIPTOR_TYPE (ObjDesc) != ACPI_DESC_TYPE_INTERNAL)
    {
        ACPI_DEBUG_PRINT ((ACPI_DB_INFO, "%p Is not a local object \n", ObjDesc));
        DUMP_BUFFER (ObjDesc, sizeof (ACPI_OPERAND_OBJECT));
        return (AE_OK);
    }

    /*  ObjDesc is a valid object  */

    ACPI_DEBUG_PRINT ((ACPI_DB_INFO, "%p ", ObjDesc));

    switch (ObjDesc->Common.Type)
    {
    case INTERNAL_TYPE_REFERENCE:

        switch (ObjDesc->Reference.Opcode)
        {
        case AML_ZERO_OP:

            AcpiOsPrintf ("Reference: Zero\n");
            break;


        case AML_ONE_OP:

            AcpiOsPrintf ("Reference: One\n");
            break;


        case AML_ONES_OP:

            AcpiOsPrintf ("Reference: Ones\n");
            break;


        case AML_REVISION_OP:

            AcpiOsPrintf ("Reference: Revision\n");
            break;


        case AML_DEBUG_OP:

            AcpiOsPrintf ("Reference: Debug\n");
            break;


        case AML_NAME_OP:

            DUMP_PATHNAME (ObjDesc->Reference.Object, "Reference: Name: ",
                            ACPI_LV_INFO, _COMPONENT);
            DUMP_ENTRY (ObjDesc->Reference.Object, ACPI_LV_INFO);
            break;


        case AML_INDEX_OP:

            AcpiOsPrintf ("Reference: Index %p\n",
                        ObjDesc->Reference.Object);
            break;


        case AML_ARG_OP:

            AcpiOsPrintf ("Reference: Arg%d",
                        ObjDesc->Reference.Offset);

            if (ACPI_TYPE_INTEGER == ObjDesc->Common.Type)
            {
                /* Value is a Number */

                AcpiOsPrintf (" value is [%8.8X%8.8x]",
                                            HIDWORD(ObjDesc->Integer.Value),
                                            LODWORD(ObjDesc->Integer.Value));
            }

            AcpiOsPrintf ("\n");
            break;


        case AML_LOCAL_OP:

            AcpiOsPrintf ("Reference: Local%d",
                        ObjDesc->Reference.Offset);

            if (ACPI_TYPE_INTEGER == ObjDesc->Common.Type)
            {

                /* Value is a Number */

                AcpiOsPrintf (" value is [%8.8X%8.8x]",
                                            HIDWORD(ObjDesc->Integer.Value),
                                            LODWORD(ObjDesc->Integer.Value));
            }

            AcpiOsPrintf ("\n");
            break;


        case AML_INT_NAMEPATH_OP:
            AcpiOsPrintf ("Reference.Node->Name %X\n",
                        ObjDesc->Reference.Node->Name);
            break;

        default:

            /*  unknown opcode  */

            AcpiOsPrintf ("Unknown opcode=%X\n",
                ObjDesc->Reference.Opcode);
            break;

        }

        break;


    case ACPI_TYPE_BUFFER:

        AcpiOsPrintf ("Buffer len %X @ %p \n",
                    ObjDesc->Buffer.Length,
                    ObjDesc->Buffer.Pointer);

        Length = ObjDesc->Buffer.Length;

        if (Length > 64)
        {
            Length = 64;
        }

        /* Debug only -- dump the buffer contents */

        if (ObjDesc->Buffer.Pointer)
        {
            AcpiOsPrintf ("Buffer Contents: ");

            for (Buf = ObjDesc->Buffer.Pointer; Length--; ++Buf)
            {
                AcpiOsPrintf (" %02x", *Buf);
            }
            AcpiOsPrintf ("\n");
        }

        break;


    case ACPI_TYPE_INTEGER:

        AcpiOsPrintf ("Integer %8.8X%8.8X\n",
                    HIDWORD (ObjDesc->Integer.Value),
                    LODWORD (ObjDesc->Integer.Value));
        break;


    case INTERNAL_TYPE_IF:

        AcpiOsPrintf ("If [Integer] %8.8X%8.8X\n",
                    HIDWORD (ObjDesc->Integer.Value),
                    LODWORD (ObjDesc->Integer.Value));
        break;


    case INTERNAL_TYPE_WHILE:

        AcpiOsPrintf ("While [Integer] %8.8X%8.8X\n",
                    HIDWORD (ObjDesc->Integer.Value),
                    LODWORD (ObjDesc->Integer.Value));
        break;


    case ACPI_TYPE_PACKAGE:

        AcpiOsPrintf ("Package count %X @ %p\n",
                    ObjDesc->Package.Count, ObjDesc->Package.Elements);

        /*
         * If elements exist, package vector pointer is valid,
         * and debug_level exceeds 1, dump package's elements.
         */
        if (ObjDesc->Package.Count &&
            ObjDesc->Package.Elements &&
            AcpiDbgLevel > 1)
        {
            ACPI_OPERAND_OBJECT**Element;
            UINT16              ElementIndex;

            for (ElementIndex = 0, Element = ObjDesc->Package.Elements;
                  ElementIndex < ObjDesc->Package.Count;
                  ++ElementIndex, ++Element)
            {
                AcpiExDumpOperand (*Element);
            }
        }

        AcpiOsPrintf ("\n");

        break;


    case ACPI_TYPE_REGION:

        AcpiOsPrintf ("Region %s (%X)",
            AcpiUtGetRegionName (ObjDesc->Region.SpaceId),
            ObjDesc->Region.SpaceId);

        /*
         * If the address and length have not been evaluated,
         * don't print them.
         */
        if (!(ObjDesc->Region.Flags & AOPOBJ_DATA_VALID))
        {
            AcpiOsPrintf ("\n");
        }
        else
        {
            AcpiOsPrintf (" base %8.8X%8.8X Length %X\n",
                HIDWORD(ObjDesc->Region.Address),
                LODWORD(ObjDesc->Region.Address),
                ObjDesc->Region.Length);
        }
        break;


    case ACPI_TYPE_STRING:

        AcpiOsPrintf ("String length %X @ %p \"",
                    ObjDesc->String.Length, ObjDesc->String.Pointer);

        for (i = 0; i < ObjDesc->String.Length; i++)
        {
            AcpiOsPrintf ("%c",
                        ObjDesc->String.Pointer[i]);
        }

        AcpiOsPrintf ("\"\n");
        break;


    case INTERNAL_TYPE_BANK_FIELD:

        AcpiOsPrintf ("BankField\n");
        break;


    case INTERNAL_TYPE_REGION_FIELD:

        AcpiOsPrintf (
            "RegionField: Bits=%X  BitAccWidth=%X Lock=%X Update=%X at byte=%X bit=%X of below:\n",
            ObjDesc->Field.BitLength,      ObjDesc->Field.AccessBitWidth,
            ObjDesc->Field.FieldFlags & AML_FIELD_LOCK_RULE_MASK,       
            ObjDesc->Field.FieldFlags & AML_FIELD_UPDATE_RULE_MASK,
            ObjDesc->Field.BaseByteOffset, ObjDesc->Field.StartFieldBitOffset);
        DUMP_STACK_ENTRY (ObjDesc->Field.RegionObj);
        break;


    case INTERNAL_TYPE_INDEX_FIELD:

        AcpiOsPrintf ("IndexField\n");
        break;


    case ACPI_TYPE_BUFFER_FIELD:

        AcpiOsPrintf (
            "BufferField: %X bits at byte %X bit %X of \n",
            ObjDesc->BufferField.BitLength, ObjDesc->BufferField.BaseByteOffset,
            ObjDesc->BufferField.StartFieldBitOffset);

        if (!ObjDesc->BufferField.BufferObj)
        {
            ACPI_DEBUG_PRINT ((ACPI_DB_INFO, "*NULL* \n"));
        }

        else if (ACPI_TYPE_BUFFER !=
                     ObjDesc->BufferField.BufferObj->Common.Type)
        {
            AcpiOsPrintf ("*not a Buffer* \n");
        }

        else
        {
            DUMP_STACK_ENTRY (ObjDesc->BufferField.BufferObj);
        }

        break;


    case ACPI_TYPE_EVENT:

        AcpiOsPrintf ("Event\n");
        break;


    case ACPI_TYPE_METHOD:

        AcpiOsPrintf (
            "Method(%X) @ %p:%X\n",
            ObjDesc->Method.ParamCount,
            ObjDesc->Method.AmlStart, ObjDesc->Method.AmlLength);
        break;


    case ACPI_TYPE_MUTEX:

        AcpiOsPrintf ("Mutex\n");
        break;


    case ACPI_TYPE_DEVICE:

        AcpiOsPrintf ("Device\n");
        break;


    case ACPI_TYPE_POWER:

        AcpiOsPrintf ("Power\n");
        break;


    case ACPI_TYPE_PROCESSOR:

        AcpiOsPrintf ("Processor\n");
        break;


    case ACPI_TYPE_THERMAL:

        AcpiOsPrintf ("Thermal\n");
        break;


    default:
        /*  unknown ObjDesc->Common.Type value    */

        AcpiOsPrintf ("Unknown Type %X\n", ObjDesc->Common.Type);
        break;
    }

    return (AE_OK);
}


/*****************************************************************************
 *
 * FUNCTION:    AcpiExDumpOperands
 *
 * PARAMETERS:  InterpreterMode      - Load or Exec
 *              *Ident              - Identification
 *              NumLevels           - # of stack entries to dump above line
 *              *Note               - Output notation
 *
 * DESCRIPTION: Dump the object stack
 *
 ****************************************************************************/

void
AcpiExDumpOperands (
    ACPI_OPERAND_OBJECT     **Operands,
    OPERATING_MODE          InterpreterMode,
    NATIVE_CHAR             *Ident,
    UINT32                  NumLevels,
    NATIVE_CHAR             *Note,
    NATIVE_CHAR             *ModuleName,
    UINT32                  LineNumber)
{
    NATIVE_UINT             i;
    ACPI_OPERAND_OBJECT     **ObjDesc;


    PROC_NAME ("ExDumpOperands");


    if (!Ident)
    {
        Ident = "?";
    }

    if (!Note)
    {
        Note = "?";
    }


    ACPI_DEBUG_PRINT ((ACPI_DB_INFO,
        "************* Operand Stack Contents (Opcode [%s], %d Operands)\n",
        Ident, NumLevels));

    if (NumLevels == 0)
    {
        NumLevels = 1;
    }

    /* Dump the stack starting at the top, working down */

    for (i = 0; NumLevels > 0; i--, NumLevels--)
    {
        ObjDesc = &Operands[i];

        if (ACPI_FAILURE (AcpiExDumpOperand (*ObjDesc)))
        {
            break;
        }
    }

    ACPI_DEBUG_PRINT ((ACPI_DB_INFO,
        "************* Stack dump from %s(%d), %s\n",
        ModuleName, LineNumber, Note));
    return;
}


/*****************************************************************************
 *
 * FUNCTION:    AcpiExOut*
 *
 * PARAMETERS:  Title               - Descriptive text
 *              Value               - Value to be displayed
 *
 * DESCRIPTION: Object dump output formatting functions.  These functions
 *              reduce the number of format strings required and keeps them
 *              all in one place for easy modification.
 *
 ****************************************************************************/

void
AcpiExOutString (
    char                    *Title,
    char                    *Value)
{
    AcpiOsPrintf ("%20s : %s\n", Title, Value);
}

void
AcpiExOutPointer (
    char                    *Title,
    void                    *Value)
{
    AcpiOsPrintf ("%20s : %p\n", Title, Value);
}

void
AcpiExOutInteger (
    char                    *Title,
    UINT32                  Value)
{
    AcpiOsPrintf ("%20s : %X\n", Title, Value);
}

void
AcpiExOutAddress (
    char                    *Title,
    ACPI_PHYSICAL_ADDRESS   Value)
{
#ifdef _IA16
    AcpiOsPrintf ("%20s : %p\n", Title, Value);
#else
    AcpiOsPrintf ("%20s : %8.8X%8.8X\n", Title,
                HIDWORD (Value), LODWORD (Value));
#endif
}


/*****************************************************************************
 *
 * FUNCTION:    AcpiExDumpNode
 *
 * PARAMETERS:  *Node           - Descriptor to dump
 *              Flags               - Force display
 *
 * DESCRIPTION: Dumps the members of the given.Node
 *
 ****************************************************************************/

void
AcpiExDumpNode (
    ACPI_NAMESPACE_NODE     *Node,
    UINT32                  Flags)
{

    FUNCTION_ENTRY ();


    if (!Flags)
    {
        if (!((ACPI_LV_OBJECTS & AcpiDbgLevel) && (_COMPONENT & AcpiDbgLayer)))
        {
            return;
        }
    }


    AcpiOsPrintf ("%20s : %4.4s\n", "Name",             (char*)&Node->Name);
    AcpiExOutString ("Type",             AcpiUtGetTypeName (Node->Type));
    AcpiExOutInteger ("Flags",            Node->Flags);
    AcpiExOutInteger ("Owner Id",         Node->OwnerId);
    AcpiExOutInteger ("Reference Count",  Node->ReferenceCount);
    AcpiExOutPointer ("Attached Object",  AcpiNsGetAttachedObject (Node));
    AcpiExOutPointer ("ChildList",        Node->Child);
    AcpiExOutPointer ("NextPeer",         Node->Peer);
    AcpiExOutPointer ("Parent",           AcpiNsGetParentObject (Node));
}


/*****************************************************************************
 *
 * FUNCTION:    AcpiExDumpObjectDescriptor
 *
 * PARAMETERS:  *Object             - Descriptor to dump
 *              Flags               - Force display
 *
 * DESCRIPTION: Dumps the members of the object descriptor given.
 *
 ****************************************************************************/

void
AcpiExDumpObjectDescriptor (
    ACPI_OPERAND_OBJECT     *ObjDesc,
    UINT32                  Flags)
{
    UINT32                  i;


    FUNCTION_TRACE ("ExDumpObjectDescriptor");


    if (!Flags)
    {
        if (!((ACPI_LV_OBJECTS & AcpiDbgLevel) && (_COMPONENT & AcpiDbgLayer)))
        {
            return;
        }
    }

    if (ACPI_GET_DESCRIPTOR_TYPE (ObjDesc) != ACPI_DESC_TYPE_INTERNAL)
    {
        AcpiOsPrintf ("ExDumpObjectDescriptor: %p is not a valid ACPI object\n", ObjDesc);
        return;
    }

    /* Common Fields */

    AcpiExOutString  ("Type",            AcpiUtGetTypeName (ObjDesc->Common.Type));
    AcpiExOutInteger ("Reference Count", ObjDesc->Common.ReferenceCount);
    AcpiExOutInteger ("Flags",           ObjDesc->Common.Flags);

    /* Object-specific Fields */

    switch (ObjDesc->Common.Type)
    {
    case ACPI_TYPE_INTEGER:

        AcpiOsPrintf ("%20s : %X%8.8X\n", "Value", HIDWORD (ObjDesc->Integer.Value),
                                                   LODWORD (ObjDesc->Integer.Value));
        break;


    case ACPI_TYPE_STRING:

        AcpiExOutInteger ("Length",          ObjDesc->String.Length);
        AcpiExOutPointer ("Pointer",         ObjDesc->String.Pointer);
        break;


    case ACPI_TYPE_BUFFER:

        AcpiExOutInteger ("Length",          ObjDesc->Buffer.Length);
        AcpiExOutPointer ("Pointer",         ObjDesc->Buffer.Pointer);
        break;


    case ACPI_TYPE_PACKAGE:

        AcpiExOutInteger ("Flags",           ObjDesc->Package.Flags);
        AcpiExOutInteger ("Count",           ObjDesc->Package.Count);
        AcpiExOutPointer ("Elements",        ObjDesc->Package.Elements);
        AcpiExOutPointer ("NextElement",     ObjDesc->Package.NextElement);

        /* Dump the package contents */

        if (ObjDesc->Package.Count > 0)
        {
            AcpiOsPrintf ("\nPackage Contents:\n");
            for (i = 0; i < ObjDesc->Package.Count; i++)
            {
                AcpiOsPrintf ("[%.3d] %p", i, ObjDesc->Package.Elements[i]);
                if (ObjDesc->Package.Elements[i])
                {
                    AcpiOsPrintf (" %s", AcpiUtGetTypeName ((ObjDesc->Package.Elements[i])->Common.Type));
                }
                AcpiOsPrintf ("\n");
            }
        }
        break;


    case ACPI_TYPE_DEVICE:

        AcpiExOutPointer ("AddrHandler",     ObjDesc->Device.AddrHandler);
        AcpiExOutPointer ("SysHandler",      ObjDesc->Device.SysHandler);
        AcpiExOutPointer ("DrvHandler",      ObjDesc->Device.DrvHandler);
        break;


    case ACPI_TYPE_EVENT:

        AcpiExOutPointer ("Semaphore",       ObjDesc->Event.Semaphore);
        break;


    case ACPI_TYPE_METHOD:

        AcpiExOutInteger ("ParamCount",      ObjDesc->Method.ParamCount);
        AcpiExOutInteger ("Concurrency",     ObjDesc->Method.Concurrency);
        AcpiExOutPointer ("Semaphore",       ObjDesc->Method.Semaphore);
        AcpiExOutInteger ("AmlLength",       ObjDesc->Method.AmlLength);
        AcpiExOutPointer ("AmlStart",        ObjDesc->Method.AmlStart);
        break;


    case ACPI_TYPE_MUTEX:

        AcpiExOutInteger ("SyncLevel",       ObjDesc->Mutex.SyncLevel);
        AcpiExOutPointer ("OwnerThread",     ObjDesc->Mutex.OwnerThread);
        AcpiExOutInteger ("AcquisitionDepth",ObjDesc->Mutex.AcquisitionDepth);
        AcpiExOutPointer ("Semaphore",       ObjDesc->Mutex.Semaphore);
        break;


    case ACPI_TYPE_REGION:

        AcpiExOutInteger ("SpaceId",         ObjDesc->Region.SpaceId);
        AcpiExOutInteger ("Flags",           ObjDesc->Region.Flags);
        AcpiExOutAddress ("Address",         ObjDesc->Region.Address);
        AcpiExOutInteger ("Length",          ObjDesc->Region.Length);
        AcpiExOutPointer ("AddrHandler",     ObjDesc->Region.AddrHandler);
        AcpiExOutPointer ("Next",            ObjDesc->Region.Next);
        break;


    case ACPI_TYPE_POWER:

        AcpiExOutInteger ("SystemLevel",     ObjDesc->PowerResource.SystemLevel);
        AcpiExOutInteger ("ResourceOrder",   ObjDesc->PowerResource.ResourceOrder);
        AcpiExOutPointer ("SysHandler",      ObjDesc->PowerResource.SysHandler);
        AcpiExOutPointer ("DrvHandler",      ObjDesc->PowerResource.DrvHandler);
        break;


    case ACPI_TYPE_PROCESSOR:

        AcpiExOutInteger ("Processor ID",    ObjDesc->Processor.ProcId);
        AcpiExOutInteger ("Length",          ObjDesc->Processor.Length);
        AcpiExOutInteger ("Address",         ObjDesc->Processor.Address);
        AcpiExOutPointer ("SysHandler",      ObjDesc->Processor.SysHandler);
        AcpiExOutPointer ("DrvHandler",      ObjDesc->Processor.DrvHandler);
        AcpiExOutPointer ("AddrHandler",     ObjDesc->Processor.AddrHandler);
        break;


    case ACPI_TYPE_THERMAL:

        AcpiExOutPointer ("SysHandler",      ObjDesc->ThermalZone.SysHandler);
        AcpiExOutPointer ("DrvHandler",      ObjDesc->ThermalZone.DrvHandler);
        AcpiExOutPointer ("AddrHandler",     ObjDesc->ThermalZone.AddrHandler);
        break;


    case ACPI_TYPE_BUFFER_FIELD:
    case INTERNAL_TYPE_REGION_FIELD:
    case INTERNAL_TYPE_BANK_FIELD:
    case INTERNAL_TYPE_INDEX_FIELD:

        AcpiExOutInteger ("FieldFlags",      ObjDesc->CommonField.FieldFlags);
        AcpiExOutInteger ("AccessBitWidth",  ObjDesc->CommonField.AccessBitWidth);
        AcpiExOutInteger ("AccessByteWidth", ObjDesc->CommonField.AccessByteWidth);
        AcpiExOutInteger ("BitLength",       ObjDesc->CommonField.BitLength);
        AcpiExOutInteger ("FldBitOffset",    ObjDesc->CommonField.StartFieldBitOffset);
        AcpiExOutInteger ("BaseByteOffset",  ObjDesc->CommonField.BaseByteOffset);
        AcpiExOutInteger ("DatumValidBits",  ObjDesc->CommonField.DatumValidBits);
        AcpiExOutInteger ("EndFldValidBits", ObjDesc->CommonField.EndFieldValidBits);
        AcpiExOutInteger ("EndBufValidBits", ObjDesc->CommonField.EndBufferValidBits);
        AcpiExOutPointer ("ParentNode",      ObjDesc->CommonField.Node);

        switch (ObjDesc->Common.Type)
        {
        case ACPI_TYPE_BUFFER_FIELD:
            AcpiExOutPointer ("BufferObj",       ObjDesc->BufferField.BufferObj);
            break;

        case INTERNAL_TYPE_REGION_FIELD:
            AcpiExOutPointer ("RegionObj",       ObjDesc->Field.RegionObj);
            break;

        case INTERNAL_TYPE_BANK_FIELD:
            AcpiExOutInteger ("Value",           ObjDesc->BankField.Value);
            AcpiExOutPointer ("RegionObj",       ObjDesc->BankField.RegionObj);
            AcpiExOutPointer ("BankObj",         ObjDesc->BankField.BankObj);
            break;

        case INTERNAL_TYPE_INDEX_FIELD:
            AcpiExOutInteger ("Value",           ObjDesc->IndexField.Value);
            AcpiExOutPointer ("Index",           ObjDesc->IndexField.IndexObj);
            AcpiExOutPointer ("Data",            ObjDesc->IndexField.DataObj);
            break;
        }
        break;


    case INTERNAL_TYPE_REFERENCE:

        AcpiExOutInteger ("TargetType",      ObjDesc->Reference.TargetType);
        AcpiExOutString  ("Opcode",          (AcpiPsGetOpcodeInfo (ObjDesc->Reference.Opcode))->Name);
        AcpiExOutInteger ("Offset",          ObjDesc->Reference.Offset);
        AcpiExOutPointer ("ObjDesc",         ObjDesc->Reference.Object);
        AcpiExOutPointer ("Node",            ObjDesc->Reference.Node);
        AcpiExOutPointer ("Where",           ObjDesc->Reference.Where);
        break;


    case INTERNAL_TYPE_ADDRESS_HANDLER:

        AcpiExOutInteger ("SpaceId",         ObjDesc->AddrHandler.SpaceId);
        AcpiExOutPointer ("Next",            ObjDesc->AddrHandler.Next);
        AcpiExOutPointer ("RegionList",      ObjDesc->AddrHandler.RegionList);
        AcpiExOutPointer ("Node",            ObjDesc->AddrHandler.Node);
        AcpiExOutPointer ("Context",         ObjDesc->AddrHandler.Context);
        break;


    case INTERNAL_TYPE_NOTIFY:

        AcpiExOutPointer ("Node",            ObjDesc->NotifyHandler.Node);
        AcpiExOutPointer ("Context",         ObjDesc->NotifyHandler.Context);
        break;


    case INTERNAL_TYPE_ALIAS:
    case INTERNAL_TYPE_FIELD_DEFN:
    case INTERNAL_TYPE_BANK_FIELD_DEFN:
    case INTERNAL_TYPE_INDEX_FIELD_DEFN:
    case INTERNAL_TYPE_IF:
    case INTERNAL_TYPE_ELSE:
    case INTERNAL_TYPE_WHILE:
    case INTERNAL_TYPE_SCOPE:
    case INTERNAL_TYPE_DEF_ANY:
    case INTERNAL_TYPE_EXTRA:
    case INTERNAL_TYPE_DATA:

        AcpiOsPrintf ("ExDumpObjectDescriptor: Display not implemented for object type %X\n",
            ObjDesc->Common.Type);
        break;


    default:

        AcpiOsPrintf ("ExDumpObjectDescriptor: Unknown object type %X\n", ObjDesc->Common.Type);
        break;
    }

    return_VOID;
}

#endif

