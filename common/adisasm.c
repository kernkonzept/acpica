/******************************************************************************
 *
 * Module Name: adexec - AcpiDump utility, top level parse and execute routines
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


#include "acpi.h"
#include "acparser.h"
#include "amlcode.h"
#include "acdebug.h"
#include "acdispat.h"
#include "acnamesp.h"
#include "adcommon.h"

#include <stdio.h>


#define _COMPONENT          ACPI_TOOLS
        MODULE_NAME         ("adexec")


ACPI_PARSE_OBJECT       *AcpiGbl_ParsedNamespaceRoot;
ACPI_PARSE_OBJECT       *root;
UINT8                   *AmlStart;
UINT32                  AmlLength;
UINT8                   *DsdtPtr;
UINT32                  DsdtLength;


BOOLEAN
AcpiDsIsResultUsed (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_WALK_STATE         *WalkState)
{
    return TRUE;
}

ACPI_STATUS
AcpiDsRestartControlMethod (
    ACPI_WALK_STATE         *WalkState,
    ACPI_OPERAND_OBJECT     *ReturnDesc)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiDsTerminateControlMethod (
    ACPI_WALK_STATE         *WalkState)
{
    return (AE_OK);
}

void
AcpiDsScopeStackClear (
    ACPI_WALK_STATE         *WalkState)
{
}

ACPI_STATUS
AcpiDsCallControlMethod (
    ACPI_THREAD_STATE       *Thread,
    ACPI_WALK_STATE         *WalkState,
    ACPI_PARSE_OBJECT       *Op)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiDsMethodDataInitArgs (
    ACPI_OPERAND_OBJECT     **Params,
    UINT32                  MaxParamCount,
    ACPI_WALK_STATE         *WalkState)
{
    return (AE_OK);
}

void *
AcpiNsGetAttachedObject (
    ACPI_NAMESPACE_NODE     *Node)
{
    return NULL;
}

/******************************************************************************
 *
 * FUNCTION:    AdCreateTableHeaders
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

void
AdCreateTableHeaders (void)
{

    AcpiOsPrintf ("%s\n", "DefinitionBlock(\"SE0005B.aml\",\"DSDT\",1,\"Intel\",\"Seattl\",2)");
    AcpiOsPrintf ("%s\n", "{");
}


/******************************************************************************
 *
 * FUNCTION:    AdBlockType
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

#define BLOCK_PAREN 1
#define BLOCK_BRACE 2

INT32
c (
    ACPI_PARSE_OBJECT       *Op)
{

    switch (Op->Opcode)
    {
    case AML_METHOD_OP:
        return BLOCK_BRACE;
        break;

    default:
        break;
    }

    return BLOCK_PAREN;

}

/******************************************************************************
 *
 * FUNCTION:    AdDisplayTables
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Display (disassemble) loaded tables and dump raw tables
 *
 *****************************************************************************/

ACPI_STATUS
AdDisplayTables (void)
{


    if (!AcpiGbl_DSDT || !AcpiGbl_ParsedNamespaceRoot)
    {
        return AE_NOT_EXIST;
    }


    if (!AcpiGbl_DbOpt_verbose)
    {
        AdCreateTableHeaders ();
    }

    AcpiDbDisplayOp (NULL, AcpiPsGetChild (AcpiGbl_ParsedNamespaceRoot), ACPI_UINT32_MAX);

    AcpiOsPrintf ("\n\nDSDT Header:\n");
    AcpiUtDumpBuffer ((UINT8 *) AcpiGbl_DSDT, sizeof (ACPI_TABLE_HEADER), DB_BYTE_DISPLAY, ACPI_UINT32_MAX);

    AcpiOsPrintf ("DSDT Body (Length 0x%X)\n", AmlLength);
    AcpiUtDumpBuffer ((UINT8 *) AmlStart, AmlLength, DB_BYTE_DISPLAY, ACPI_UINT32_MAX);

    return AE_OK;
}


/******************************************************************************
 *
 * FUNCTION:    AdLoadDsdt
 *
 * PARAMETERS:
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Load the DSDT from the file pointer
 *
 *****************************************************************************/

ACPI_STATUS
AdLoadDsdt(
    FILE                    *fp,
    int                     seekable,
    UINT8                   **DsdtPtr,
    UINT32                  *DsdtLength)
{
    ACPI_TABLE_HEADER       dsdt_hdr;
    UINT8                   *AmlStart;
    UINT32                   AmlLength;


    if (fread(&dsdt_hdr, 1, sizeof (dsdt_hdr), fp) == sizeof (dsdt_hdr))
    {
        *DsdtLength = dsdt_hdr.Length;

        if (*DsdtLength)
        {
            *DsdtPtr = (UINT8*) malloc ((size_t) *DsdtLength);

            if (*DsdtPtr)
            {
                AmlStart = *DsdtPtr + sizeof (dsdt_hdr);
                AmlLength = *DsdtLength - sizeof (dsdt_hdr);

                memcpy (*DsdtPtr, &dsdt_hdr, sizeof (dsdt_hdr));
                if ((UINT32) fread (AmlStart, 1, (size_t) AmlLength, fp) == AmlLength)
                {
                    return AE_OK;
                }

                free(*DsdtPtr);
            }
        }
    }

    *DsdtPtr = NULL;
    *DsdtLength = 0;

    return AE_AML_ERROR;
}


/******************************************************************************
 *
 * FUNCTION:    AdSecondPassParse
 *
 * PARAMETERS:  Root            - Root of the parse tree
 *
 * RETURN:      None
 *
 * DESCRIPTION: Need to wait until second pass to parse the control methods
 *
 *****************************************************************************/

ACPI_STATUS
AdSecondPassParse (
    ACPI_PARSE_OBJECT       *Root)
{
    ACPI_PARSE_OBJECT       *Op = Root;
    ACPI_PARSE2_OBJECT      *Method;
    ACPI_PARSE_OBJECT       *SearchOp;
    ACPI_PARSE_OBJECT       *StartOp;
    ACPI_STATUS             Status = AE_OK;
    UINT32                  BaseAmlOffset;
    ACPI_WALK_STATE         *WalkState;


    PROC_NAME ("AdSecondPassParse");
    printf ("Parsing Control Methods \n");

    while (Op)
    {
        if (Op->Opcode == AML_METHOD_OP)
        {
            printf (".");

            Method = (ACPI_PARSE2_OBJECT *) Op;
            ACPI_DEBUG_PRINT ((ACPI_DB_INFO, "Parsing method [%4.4s]\n", &Method->Name));

            WalkState = AcpiDsCreateWalkState (TABLE_ID_DSDT,
                                            Op, NULL, NULL);
            if (!WalkState)
            {
                return_ACPI_STATUS (AE_NO_MEMORY);
            }

            Status = AcpiDsInitAmlWalk (WalkState, Op, NULL, Method->Data,
                            Method->Length, NULL, NULL, 1);
            if (ACPI_FAILURE (Status))
            {
                return_ACPI_STATUS (Status);
            }

            /* Parse the method */

            Status = AcpiPsParseAml (WalkState); //Op, Method->Data, Method->Length, 0,
                                        //NULL, NULL, NULL, AcpiPsFindObject, NULL);

            /*
             * We need to update all of the Aml offsets, since the parser thought
             * that the method began at offset zero.  In reality, it began somewhere
             * within the ACPI table, at the BaseAmlOffset.  Walk the entire tree that
             * was just created and update the AmlOffset in each Op
             */

            BaseAmlOffset = (Method->Value.Arg)->AmlOffset + 1;
            StartOp = (Method->Value.Arg)->Next;
            SearchOp = StartOp;

            /* Walk the parse tree */

            while (SearchOp)
            {
                SearchOp->AmlOffset += BaseAmlOffset;
                SearchOp = AcpiPsGetDepthNext (StartOp, SearchOp);
            }

        }

        if (Op->Opcode == AML_REGION_OP)
        {
            /* TBD: Code below isn't quite the right thing to do!
             * Is there any need to parse regions here?
             */

            // Method = (ACPI_DEFERRED_OP *) Op;
            // Status = AcpiPsParseAml (Op, Method->Body, Method->BodyLength);
        }

        if (ACPI_FAILURE (Status))
        {
            return Status;
        }

        Op = AcpiPsGetDepthNext (Root, Op);
    }

    printf ("\n");

    return Status;
}


/******************************************************************************
 *
 * FUNCTION:    AdGetTables
 *
 * PARAMETERS:  Filename        - Optional filename
 *
 * RETURN:      None
 *
 * DESCRIPTION: Get the ACPI tables from either memory or a file
 *
 *****************************************************************************/

ACPI_STATUS
AdGetTables (
    char                    *Filename)
{
    FILE                    *fp;
    ACPI_STATUS             Status;


    if (Filename)
    {
        printf ("Loading DSDT from file %s\n", Filename);
        fp = fopen (Filename, "rb");
        if (!fp)
        {
            printf ("Couldn't open %s\n", Filename);
            return AE_ERROR;
        }

        Status = AdLoadDsdt (fp, (fp != stdin), &DsdtPtr, &DsdtLength);
        if (fp != stdin)
        {
            fclose(fp);
        }
    }


    else
    {
#ifdef _IA16
#include "16bit.h"
        printf ("Scanning for DSDT\n");

        Status = AfFindDsdt (&DsdtPtr, &DsdtLength);

        if (ACPI_SUCCESS (Status))
        {
            printf ("About to dump DSDT\n");
            AfDumpTables ();
            printf ("Dumped DSDT\n");
        }
#else
        printf ("Must supply filename for ACPI tables, cannot scan memory\n");
        Status = AE_NO_ACPI_TABLES;
#endif
    }

    return Status;
}


ACPI_STATUS
AcpiDsInitCallbacks (
    ACPI_WALK_STATE         *WalkState,
    UINT32                  PassNumber)
{

    WalkState->ParseFlags         = 0;
    WalkState->DescendingCallback = AcpiPsFindObject;
    WalkState->AscendingCallback  = NULL;

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AdParseTable
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Parse all supported tables
 *
 *****************************************************************************/

ACPI_STATUS
AdParseTables (void)
{
    ACPI_STATUS             Status = AE_OK;
    ACPI_WALK_STATE         *WalkState;


    if (!AcpiGbl_DSDT)
    {
        return AE_NOT_EXIST;
    }

    /* Create the root object */

    AcpiGbl_ParsedNamespaceRoot = AcpiPsAllocOp (AML_SCOPE_OP);
    if (!AcpiGbl_ParsedNamespaceRoot)
    {
        return AE_NO_MEMORY;
    }

    /* Initialize the root object */

    ((ACPI_PARSE2_OBJECT *) AcpiGbl_ParsedNamespaceRoot)->Name = ACPI_ROOT_NAME;

    /* Pass 1:  Parse everything except control method bodies */

    printf ("Pass 1 parse\n");

    /* Create and initialize a new walk state */

    WalkState = AcpiDsCreateWalkState (TABLE_ID_DSDT,
                                    AcpiGbl_ParsedNamespaceRoot, NULL, NULL);
    if (!WalkState)
    {
        return (AE_NO_MEMORY);
    }

    DsdtLength = AcpiGbl_DSDT->Length;
    AmlLength  = DsdtLength  - sizeof (ACPI_TABLE_HEADER);
    AmlStart   = ((UINT8 *) AcpiGbl_DSDT + sizeof (ACPI_TABLE_HEADER));

    Status = AcpiDsInitAmlWalk (WalkState, AcpiGbl_ParsedNamespaceRoot, NULL, AmlStart,
                    AmlLength, NULL, NULL, 1);
    if (ACPI_FAILURE (Status))
    {
        return (Status);
    }


    Status = AcpiPsParseAml (WalkState); //, AmlStart, AmlLength, 0,
                                //NULL, NULL, NULL, AcpiPsFindObject, NULL);
    if (ACPI_FAILURE (Status))
    {
        return Status;
    }

    /* Pass 2: Parse control methods and link their parse trees into the main parse tree */

    printf ("Pass 2 parse\n");
    Status = AdSecondPassParse (AcpiGbl_ParsedNamespaceRoot);

    printf ("Parsing completed\n");
    return Status;
}


