/******************************************************************************
 *
 * Module Name: psutils - Parser miscellaneous utilities (Parser only)
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


#include "acpi.h"
#include "parser.h"
#include "amlcode.h"

#define _COMPONENT          PARSER
        MODULE_NAME         ("psutils");


#define PARSEOP_GENERIC     1
#define PARSEOP_NAMED       2
#define PARSEOP_DEFERRED    3
#define PARSEOP_BYTELIST    4


/*******************************************************************************
 *
 * FUNCTION:    AcpiPsInitOp
 *
 * PARAMETERS:  Op              - A newly allocated Op object
 *              Opcode          - Opcode to store in the Op
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Allocate an acpi_op, choose op type (and thus size) based on opcode
 *
 ******************************************************************************/

void
AcpiPsInitOp (
    ACPI_GENERIC_OP         *Op,
    UINT16                  Opcode)
{
    ACPI_OP_INFO             *AmlOp;


    Op->DataType = DESC_TYPE_PARSER;
    Op->Opcode = Opcode;


    AmlOp = AcpiPsGetOpcodeInfo (Opcode);
    if (AmlOp)
    {
        DEBUG_ONLY_MEMBERS (STRNCPY (Op->OpName, AmlOp->Name, sizeof (Op->OpName)));
    }
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiPsAllocOp
 *
 * PARAMETERS:  Opcode          - Opcode that will be stored in the new Op
 *
 * RETURN:      Pointer to the new Op.
 *
 * DESCRIPTION: Allocate an acpi_op, choose op type (and thus size) based on opcode
 *              A cache of opcodes is available for the pure GENERIC_OP, since this
 *              is by far the most commonly used.
 *
 ******************************************************************************/

ACPI_GENERIC_OP*
AcpiPsAllocOp (
    UINT16                  Opcode)
{
    ACPI_GENERIC_OP         *Op = NULL;
    UINT32                  Size;
    UINT8                   Flags;


    /* Allocate the minimum required size object */

    if (AcpiPsIsDeferredOp (Opcode))
    {
        Size = sizeof (ACPI_DEFERRED_OP);
        Flags = PARSEOP_DEFERRED;
    }

    else if (AcpiPsIsNamedOp (Opcode))
    {
        Size = sizeof (ACPI_NAMED_OP);
        Flags = PARSEOP_NAMED;
    }

    else if (AcpiPsIsBytelistOp (Opcode))
    {
        Size = sizeof (ACPI_BYTELIST_OP);
        Flags = PARSEOP_BYTELIST;
    }

    else
    {
        Size = sizeof (ACPI_GENERIC_OP);
        Flags = PARSEOP_GENERIC;

        /*
         * The generic op is by far the most common (16 to 1), and therefore the op cache is
         * implemented with this type.
         *
         * Check if there is an Op already available in the cache
         */

        AcpiCmAcquireMutex (MTX_CACHES);
        Acpi_GblParseCacheRequests++;
        if (Acpi_GblParseCache)
        {
            /* Extract an op from the front of the cache list */

            Acpi_GblParseCacheDepth--;
            Acpi_GblParseCacheHits++;

            Op = Acpi_GblParseCache;
            Acpi_GblParseCache = Op->Next;

            /* Clear the previously used Op */

            MEMSET (Op, 0, sizeof (ACPI_GENERIC_OP));
        }
        AcpiCmReleaseMutex (MTX_CACHES);
    }

    /* Allocate a new Op if necessary */

    if (!Op)
    {
        Op = AcpiCmCallocate (Size);
    }

    /* Initialize the Op */
    if (Op)
    {
        AcpiPsInitOp (Op, Opcode);
        Op->Flags = Flags;
    }

    return Op;
}



/*******************************************************************************
 *
 * FUNCTION:    AcpiPsFreeOp
 *
 * PARAMETERS:  Op              - Op to be freed
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Free an Op object.  Either put it on the GENERIC_OP cache list
 *              or actually free it.
 *
 ******************************************************************************/

void
AcpiPsFreeOp (
    ACPI_GENERIC_OP         *Op)
{


    if (Op->Flags == PARSEOP_GENERIC)
    {
        /* Is the cache full? */

        if (Acpi_GblParseCacheDepth < MAX_PARSE_CACHE_DEPTH)
        {
            /* Put a GENERIC_OP back into the cache */

            AcpiCmAcquireMutex (MTX_CACHES);
            Acpi_GblParseCacheDepth++;

            Op->Next = Acpi_GblParseCache;
            Acpi_GblParseCache = Op;

            AcpiCmReleaseMutex (MTX_CACHES);
            return;
        }
    }

    /*
     * Not a GENERIC OP, or the cache is full, just free the Op
     */

    AcpiCmFree (Op);
}



/*******************************************************************************
 *
 * FUNCTION:    AcpiPsDeleteParseCache
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Free all objects that are on the parse cache list.
 *
 ******************************************************************************/

void
AcpiPsDeleteParseCache (
    void)
{
    ACPI_GENERIC_OP         *Next;


    FUNCTION_TRACE ("PsDeleteParseCache");


    /* Traverse the global cache list */

    while (Acpi_GblParseCache)
    {
        /* Delete one cached state object */

        Next = Acpi_GblParseCache->Next;
        AcpiCmFree (Acpi_GblParseCache);
        Acpi_GblParseCache = Next;
    }

    return_VOID;
}





/*******************************************************************************
 *
 * FUNCTION:    Utility functions
 *
 * DESCRIPTION: Low level functions
 *
 * TBD: [Restructure]
 * 1) Some of these functions should be macros
 * 2) Some can be simplified
 *
 ******************************************************************************/



/*
 * Is "c" a namestring lead character?
 */


BOOLEAN
AcpiPsIsLeadingChar (
    INT32                   c)
{
    return ((BOOLEAN) (c == '_' || (c >= 'A' && c <= 'Z')));
}


/*
 * Is "c" a namestring prefix character?
 */
BOOLEAN
AcpiPsIsPrefixChar (
    INT32                   c)
{
    return ((BOOLEAN) (c == '\\' || c == '^'));
}


BOOLEAN
AcpiPsIsNamespaceObjectOp (
    UINT16                  Opcode)
{
    return ((BOOLEAN)
           (Opcode == AML_SCOPE_OP          ||
            Opcode == AML_DEVICE_OP         ||
            Opcode == AML_THERMAL_ZONE_OP   ||
            Opcode == AML_METHOD_OP         ||
            Opcode == AML_POWER_RES_OP      ||
            Opcode == AML_PROCESSOR_OP      ||
            Opcode == AML_DEF_FIELD_OP      ||
            Opcode == AML_INDEX_FIELD_OP    ||
            Opcode == AML_BANK_FIELD_OP     ||
            Opcode == AML_NAMEDFIELD_OP     ||
            Opcode == AML_NAME_OP           ||
            Opcode == AML_ALIAS_OP          ||
            Opcode == AML_MUTEX_OP          ||
            Opcode == AML_EVENT_OP          ||
            Opcode == AML_REGION_OP         ||
            Opcode == AML_CREATE_FIELD_OP   ||
            Opcode == AML_BIT_FIELD_OP      ||
            Opcode == AML_BYTE_FIELD_OP     ||
            Opcode == AML_WORD_FIELD_OP     ||
            Opcode == AML_DWORD_FIELD_OP    ||
            Opcode == AML_METHODCALL_OP     ||
            Opcode == AML_NAMEPATH_OP));
}

BOOLEAN
AcpiPsIsNamespaceOp (
    UINT16                  Opcode)
{
    return ((BOOLEAN)
           (Opcode == AML_SCOPE_OP          ||
            Opcode == AML_DEVICE_OP         ||
            Opcode == AML_THERMAL_ZONE_OP   ||
            Opcode == AML_METHOD_OP         ||
            Opcode == AML_POWER_RES_OP      ||
            Opcode == AML_PROCESSOR_OP      ||
            Opcode == AML_DEF_FIELD_OP      ||
            Opcode == AML_INDEX_FIELD_OP    ||
            Opcode == AML_BANK_FIELD_OP     ||
            Opcode == AML_NAME_OP           ||
            Opcode == AML_ALIAS_OP          ||
            Opcode == AML_MUTEX_OP          ||
            Opcode == AML_EVENT_OP          ||
            Opcode == AML_REGION_OP         ||
            Opcode == AML_NAMEDFIELD_OP));
}




/*
 * Is opcode for a named object Op?
 * (Includes all named object opcodes)
 *
 * TBD: [Restructure] Need a better way than this brute force approach!
 */
BOOLEAN
AcpiPsIsNamedObjectOp (
    UINT16                  Opcode)
{
    return ((BOOLEAN)
           (Opcode == AML_SCOPE_OP          ||
            Opcode == AML_DEVICE_OP         ||
            Opcode == AML_THERMAL_ZONE_OP   ||
            Opcode == AML_METHOD_OP         ||
            Opcode == AML_POWER_RES_OP      ||
            Opcode == AML_PROCESSOR_OP      ||
            Opcode == AML_NAMEDFIELD_OP     ||
            Opcode == AML_NAME_OP           ||
            Opcode == AML_ALIAS_OP          ||
            Opcode == AML_MUTEX_OP          ||
            Opcode == AML_EVENT_OP          ||
            Opcode == AML_REGION_OP         ||


            Opcode == AML_CREATE_FIELD_OP   ||
            Opcode == AML_BIT_FIELD_OP      ||
            Opcode == AML_BYTE_FIELD_OP     ||
            Opcode == AML_WORD_FIELD_OP     ||
            Opcode == AML_DWORD_FIELD_OP    ||
            Opcode == AML_METHODCALL_OP     ||
            Opcode == AML_NAMEPATH_OP));
}


/*
 * Is opcode for a named Op?
 */
BOOLEAN
AcpiPsIsNamedOp (
    UINT16                  Opcode)
{
    return ((BOOLEAN)
           (Opcode == AML_SCOPE_OP          ||
            Opcode == AML_DEVICE_OP         ||
            Opcode == AML_THERMAL_ZONE_OP   ||
            Opcode == AML_METHOD_OP         ||
            Opcode == AML_POWER_RES_OP      ||
            Opcode == AML_PROCESSOR_OP      ||
            Opcode == AML_NAME_OP           ||
            Opcode == AML_ALIAS_OP          ||
            Opcode == AML_MUTEX_OP          ||
            Opcode == AML_EVENT_OP          ||
            Opcode == AML_REGION_OP         ||
            Opcode == AML_NAMEDFIELD_OP));
}


BOOLEAN
AcpiPsIsDeferredOp (
    UINT16                  Opcode)
{
    return ((BOOLEAN)
           (Opcode == AML_METHOD_OP ||
            Opcode == AML_REGION_OP));
}


/*
 * Is opcode for a bytelist?
 */
BOOLEAN
AcpiPsIsBytelistOp (
    UINT16                  Opcode)
{
    return ((BOOLEAN) (Opcode == AML_BYTELIST_OP));
}


/*
 * Is opcode for a Field, IndexField, or BankField
 */
BOOLEAN
AcpiPsIsFieldOp (
    UINT16                  Opcode)
{
    return ((BOOLEAN)
              (Opcode == AML_CREATE_FIELD_OP
            || Opcode == AML_DEF_FIELD_OP
            || Opcode == AML_INDEX_FIELD_OP
            || Opcode == AML_BANK_FIELD_OP));
}


/*
 * Is field creation op
 */
BOOLEAN
AcpiPsIsCreateFieldOp (
    UINT16                  Opcode)
{
    return ((BOOLEAN)
           (Opcode == AML_CREATE_FIELD_OP   ||
            Opcode == AML_BIT_FIELD_OP      ||
            Opcode == AML_BYTE_FIELD_OP     ||
            Opcode == AML_WORD_FIELD_OP     ||
            Opcode == AML_DWORD_FIELD_OP));
}



/*
 * Cast an acpi_op to an acpi_deferred_op if possible
 */
ACPI_DEFERRED_OP *
AcpiPsToDeferredOp (
    ACPI_GENERIC_OP         *Op)
{
    return (AcpiPsIsDeferredOp (Op->Opcode)
            ? ( (ACPI_DEFERRED_OP *) Op) : NULL);
}


/*
 * Cast an acpi_op to an acpi_named_op if possible
 */
ACPI_NAMED_OP*
AcpiPsToNamedOp (
    ACPI_GENERIC_OP         *Op)
{
    return (AcpiPsIsNamedOp (Op->Opcode)
            ? ( (ACPI_NAMED_OP *) Op) : NULL);
}


/*
 * Cast an acpi_op to an acpi_bytelist_op if possible
 */
ACPI_BYTELIST_OP*
AcpiPsToBytelistOp (
    ACPI_GENERIC_OP         *Op)
{
    return (AcpiPsIsBytelistOp (Op->Opcode)
            ? ( (ACPI_BYTELIST_OP*) Op) : NULL);
}


/*
 * Get op's name (4-byte name segment) or 0 if unnamed
 */
UINT32
AcpiPsGetName (
    ACPI_GENERIC_OP         *Op)
{
    ACPI_NAMED_OP               *Named = AcpiPsToNamedOp (Op);

    return (Named ? Named->Name : 0);
}


/*
 * Set op's name
 */
void
AcpiPsSetName (
    ACPI_GENERIC_OP         *Op,
    UINT32                  name)
{
    ACPI_NAMED_OP           *Named = AcpiPsToNamedOp (Op);

    if (Named)
    {
        Named->Name = name;
    }
}

