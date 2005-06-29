/******************************************************************************
 *
 * Module Name: adisasm - Application-level disassembler routines
 *              $Revision: 1.47 $
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2002, Intel Corp.
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
#include "acdisasm.h"
#include "acdispat.h"
#include "acnamesp.h"
#include "adisasm.h"

#include <stdio.h>
#include <time.h>


#define _COMPONENT          ACPI_TOOLS
        ACPI_MODULE_NAME    ("adisasm")


ACPI_PARSE_OBJECT       *AcpiGbl_ParsedNamespaceRoot;
ACPI_PARSE_OBJECT       *root;
UINT8                   *AmlStart;
UINT32                  AmlLength;
UINT8                   *DsdtPtr;
UINT32                  DsdtLength;

#ifndef _ACPI_ASL_COMPILER
BOOLEAN
AcpiDsIsResultUsed (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_WALK_STATE         *WalkState)
{
    return TRUE;
}
#endif


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


#define FILE_SUFFIX_DISASSEMBLY     "dsl"
#define ACPI_TABLE_FILE_SUFFIX      ".dat"
char                        FilenameBuf[20];

/******************************************************************************
 *
 * FUNCTION:    AfGenerateFilename
 *
 * PARAMETERS:
 *
 * RETURN:
 *
 * DESCRIPTION: Build an output filename from an ACPI table ID string
 *
 ******************************************************************************/

char *
AdGenerateFilename (char *TableId)
{
    NATIVE_UINT              i;


    for (i = 0; i < 8 && TableId[i] != ' ' && TableId[i] != 0; i++)
    {
        FilenameBuf [i] = TableId[i];
    }

    FilenameBuf [i] = 0;
    strcat (FilenameBuf, ACPI_TABLE_FILE_SUFFIX);
    return FilenameBuf;
}


/******************************************************************************
 *
 * FUNCTION:    AfWriteBuffer
 *
 * PARAMETERS:
 *
 * RETURN:
 *
 * DESCRIPTION: Open a file and write out a single buffer
 *
 ******************************************************************************/

NATIVE_INT
AdWriteBuffer (
    char                *Filename,
    char                *Buffer,
    UINT32              Length)
{
    FILE                *fp;
    NATIVE_INT          Actual;


    fp = fopen (Filename, "wb");
    if (!fp)
    {
        printf ("Couldn't open %s\n", Filename);
        return -1;
    }

    Actual = fwrite (Buffer, (size_t) Length, 1, fp);
    fclose (fp);
    return Actual;
}


/******************************************************************************
 *
 * FUNCTION:    AfDumpTables
 *
 * PARAMETERS:
 *
 * RETURN:
 *
 * DESCRIPTION: Dump the loaded tables to a file (or files)
 *
 ******************************************************************************/

void
AdWriteDsdt (void)
{
    char                    *Filename;


    if (!AcpiGbl_DSDT)
    {
        AcpiOsPrintf ("No DSDT!\n");
        return;
    }


    Filename = AdGenerateFilename (AcpiGbl_DSDT->OemTableId);
    AdWriteBuffer (Filename,
            (char *) AcpiGbl_DSDT, AcpiGbl_DSDT->Length);

    AcpiOsPrintf ("DSDT AML written to \"%s\"\n", Filename);
}


/*******************************************************************************
 *
 * FUNCTION:    AdInitialize
 *
 * PARAMETERS:  None.
 *
 * RETURN:      Status
 *
 * DESCRIPTION: CA initialization
 *
 ******************************************************************************/

ACPI_STATUS
AdInitialize (
    void)
{
    ACPI_STATUS             Status;


    /* ACPI CA subsystem initialization */

    AcpiUtInitGlobals ();
    Status = AcpiUtMutexInitialize ();
    if (ACPI_FAILURE (Status))
    {
        return Status;
    }

    Status = AcpiNsRootInitialize ();
    return Status;
}


/*******************************************************************************
 *
 * FUNCTION:    FlGenerateFilename
 *
 * PARAMETERS:  InputFilename       - Original ASL source filename
 *              Suffix              - New extension.
 *
 * RETURN:      New filename containing the original base + the new suffix
 *
 * DESCRIPTION: Generate a new filename from the ASL source filename and a new
 *              extension.  Used to create the *.LST, *.TXT, etc. files.
 *
 ******************************************************************************/

char *
FlGenerateFilename (
    char                    *InputFilename,
    char                    *Suffix)
{
    char                    *Position;
    char                    *NewFilename;


    /* Copy the original filename to a new buffer */

    NewFilename = ACPI_MEM_CALLOCATE (strlen (InputFilename) + strlen (Suffix));
    strcpy (NewFilename, InputFilename);

    /* Try to find the last dot in the filename */

    Position = strrchr (NewFilename, '.');
    if (Position)
    {
        /* Tack on the new suffix */

        Position++;
        *Position = 0;
        strcat (Position, Suffix);
    }
    else
    {
        /* No dot, add one and then the suffix */

        strcat (NewFilename, ".");
        strcat (NewFilename, Suffix);
    }

    return NewFilename;
}


/******************************************************************************
 *
 * FUNCTION:    AdAmlDisassemble
 *
 * PARAMETERS:  OutToFile       - TRUE if output should go to a file
 *              Filename        - AML input filename
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Disassemble an entire ACPI table
 *
 *****************************************************************************/

ACPI_STATUS
AdAmlDisassemble (
    BOOLEAN                 OutToFile,
    char                    *Filename,
    char                    **OutFilename)
{
    ACPI_STATUS             Status;
    char                    *DisasmFilename = NULL;
    FILE                    *File = NULL;


    /* Get the ACPI Tables (always) */

    if (Filename)
    {
        Status = AcpiDbLoadAcpiTable (Filename);
        if (ACPI_FAILURE (Status))
        {
            return Status;
        }
    }
    else
    {
        Status = AdGetTables (Filename);
        if (ACPI_FAILURE (Status))
        {
            AcpiOsPrintf ("Could not get ACPI tables, %s\n",
                AcpiFormatException (Status));
            return Status;
        }

        if (!AcpiGbl_DbOpt_disasm)
        {
            return AE_OK;
        }

        AcpiOsPrintf ("\nDisassembly of DSDT\n");
        Filename = AdGenerateFilename (AcpiGbl_DSDT->OemTableId);
        *OutFilename = Filename;
    }

    if (OutToFile)
    {
        /* Create/Open a disassembly output file */

        DisasmFilename = FlGenerateFilename (Filename, FILE_SUFFIX_DISASSEMBLY);
        if (!OutFilename)
        {
            fprintf (stderr, "Could not generate output filename\n");
        }
        File = fopen (DisasmFilename, "w+");
        if (!File)
        {
            fprintf (stderr, "Could not open output filen\n");
        }

        AcpiOsRedirectOutput (File);
    }

    *OutFilename = DisasmFilename;

    /* Always parse the tables, only option is what to display */

    Status = AdParseTables ();
    if (ACPI_FAILURE (Status))
    {
        AcpiOsPrintf ("Could not parse ACPI tables, %s\n",
            AcpiFormatException (Status));
        goto Cleanup;
    }

    /* Optional displays */

    if (AcpiGbl_DbOpt_disasm)
    {
        AdDisplayTables (Filename);
        fprintf (stderr, "Disassembly completed, written to \"%s\"\n", DisasmFilename);
    }

Cleanup:
    if (OutToFile)
    {
        fclose (File);
        AcpiOsRedirectOutput (stdout);
    }

    AcpiPsDeleteParseTree (AcpiGbl_ParsedNamespaceRoot);
    return AE_OK;
}


/******************************************************************************
 *
 * FUNCTION:    AdCreateTableHeader
 *
 * PARAMETERS:  Filename            - Input file for the table
 *              Table               - Pointer to the raw table
 *
 * RETURN:      None
 *
 * DESCRIPTION: Create the ASL table header, including ACPI CA signon with
 *              current time and date.
 *
 *****************************************************************************/

void
AdCreateTableHeader (
    char                    *Filename,
    ACPI_TABLE_HEADER       *Table)
{
    time_t                  Timer;

    time (&Timer);

    AcpiOsPrintf ("/*\n * Intel ACPI Component Architecture\n");
    AcpiOsPrintf (" * AML Disassembler version %8.8X\n", ACPI_CA_VERSION);
    AcpiOsPrintf (" *\n * Disassembly of %s, %s */\n", Filename, ctime (&Timer));

    AcpiOsPrintf (
        "DefinitionBlock (\"DSDT.aml\", \"%4.4s\", %hd, \"%.6s\", \"%.8s\", %d)\n",
        Table->Signature, Table->Revision,
        Table->OemId, Table->OemTableId, Table->OemRevision);
}


/******************************************************************************
 *
 * FUNCTION:    AdDisplayTables
 *
 * PARAMETERS:  Filename            - Input file for the table
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Display (disassemble) loaded tables and dump raw tables
 *
 *****************************************************************************/

ACPI_STATUS
AdDisplayTables (
    char                    *Filename)
{


    if (!AcpiGbl_DSDT || !AcpiGbl_ParsedNamespaceRoot)
    {
        return AE_NOT_EXIST;
    }


    if (!AcpiGbl_DbOpt_verbose)
    {
        AdCreateTableHeader (Filename, AcpiGbl_DSDT);
    }

    AcpiDmDisassemble (NULL, AcpiGbl_ParsedNamespaceRoot, ACPI_UINT32_MAX);

    if (AcpiGbl_DbOpt_verbose)
    {
        AcpiOsPrintf ("\n\nDSDT Header:\n");
        AcpiUtDumpBuffer ((UINT8 *) AcpiGbl_DSDT, sizeof (ACPI_TABLE_HEADER),
            DB_BYTE_DISPLAY, ACPI_UINT32_MAX);

        AcpiOsPrintf ("DSDT Body (Length 0x%X)\n", AmlLength);
        AcpiUtDumpBuffer ((UINT8 *) AmlStart, AmlLength,
            DB_BYTE_DISPLAY, ACPI_UINT32_MAX);
    }

    return AE_OK;
}


/******************************************************************************
 *
 * FUNCTION:    AdLoadDsdt
 *
 * PARAMETERS:  fp                  - Input file
 *              seekable            - can seek on the input file?
 *              DsdtPtr             - Where pointer to the dsdt is returned
 *              DsdtLength          - Where dsdt length is returned
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Load the DSDT from the file pointer
 *
 *****************************************************************************/

ACPI_STATUS
AdLoadDsdt (
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

    return AE_NO_ACPI_TABLES;
}


/******************************************************************************
 *
 * FUNCTION:    AdDeferredParse
 *
 * PARAMETERS:  Op              - Root Op of the deferred opcode
 *              Aml             - Pointer to the raw AML
 *              AmlLength       - Length of the AML
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Parse one deferred opcode
 *              (Methods, operation regions, etc.)
 *
 *****************************************************************************/

ACPI_STATUS
AdDeferredParse (
    ACPI_PARSE_OBJECT       *Op,
    UINT8                   *Aml,
    UINT32                  AmlLength)
{
    ACPI_WALK_STATE         *WalkState;
    ACPI_STATUS             Status;
    ACPI_PARSE_OBJECT       *SearchOp;
    ACPI_PARSE_OBJECT       *StartOp;
    UINT32                  BaseAmlOffset;
    ACPI_PARSE_OBJECT       *ExtraOp;


    ACPI_FUNCTION_TRACE ("AdDeferredParse");


    fprintf (stderr, ".");

    if (!Aml || !AmlLength)
    {
        return_ACPI_STATUS (AE_OK);
    }

    ACPI_DEBUG_PRINT ((ACPI_DB_INFO, "Parsing %s [%4.4s]\n",
        Op->Common.AmlOpName, (char *) &Op->Named.Name));

    WalkState = AcpiDsCreateWalkState (TABLE_ID_DSDT, Op, NULL, NULL);
    if (!WalkState)
    {
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    Status = AcpiDsInitAmlWalk (WalkState, Op, NULL, Aml,
                    AmlLength, NULL, NULL, 1);
    if (ACPI_FAILURE (Status))
    {
        return_ACPI_STATUS (Status);
    }

    /* Parse the method */

    WalkState->ParseFlags &= ~ACPI_PARSE_DELETE_TREE;
    Status = AcpiPsParseAml (WalkState);

    /*
     * We need to update all of the Aml offsets, since the parser thought
     * that the method began at offset zero.  In reality, it began somewhere
     * within the ACPI table, at the BaseAmlOffset.  Walk the entire tree that
     * was just created and update the AmlOffset in each Op
     */
    BaseAmlOffset = (Op->Common.Value.Arg)->Common.AmlOffset + 1;
    StartOp = (Op->Common.Value.Arg)->Common.Next;
    SearchOp = StartOp;

    /* Walk the parse tree */

    while (SearchOp)
    {
        SearchOp->Common.AmlOffset += BaseAmlOffset;
        SearchOp = AcpiPsGetDepthNext (StartOp, SearchOp);
    }

    /*
     * Link the newly parsed subtree into the main parse tree
     */
    switch (Op->Common.AmlOpcode)
    {
    case AML_BUFFER_OP:
    case AML_PACKAGE_OP:
    case AML_VAR_PACKAGE_OP:

        switch (Op->Common.AmlOpcode)
        {
        case AML_PACKAGE_OP:
        case AML_VAR_PACKAGE_OP:
            ExtraOp = Op->Common.Value.Arg;
            ExtraOp = ExtraOp->Common.Next;
            Op->Common.Value.Arg = ExtraOp->Common.Value.Arg;
            break;

        case AML_BUFFER_OP:
        default:
            ExtraOp = Op->Common.Value.Arg;
            Op->Common.Value.Arg = ExtraOp->Common.Value.Arg;
            break;
        }

        /* Must point all parents to the main tree */

        StartOp = Op;
        SearchOp = StartOp;
        while (SearchOp)
        {
            if (SearchOp->Common.Parent == ExtraOp)
            {
                SearchOp->Common.Parent = Op;
            }
            SearchOp = AcpiPsGetDepthNext (StartOp, SearchOp);
        }
        break;

    default:
        break;
    }

    return_ACPI_STATUS (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AdParseDeferredOps
 *
 * PARAMETERS:  Root            - Root of the parse tree
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Parse the deferred opcodes (Methods, regions, etc.)
 *
 *****************************************************************************/

ACPI_STATUS
AdParseDeferredOps (
    ACPI_PARSE_OBJECT       *Root)
{
    ACPI_PARSE_OBJECT       *Op = Root;
    ACPI_STATUS             Status = AE_OK;
    const ACPI_OPCODE_INFO  *OpInfo;


    ACPI_FUNCTION_NAME ("AdParseDeferredOps");
    fprintf (stderr, "Parsing Deferred Opcodes (Methods/Buffers/Packages/Regions)\n");

    while (Op)
    {
        OpInfo = AcpiPsGetOpcodeInfo (Op->Common.AmlOpcode);
        if (!(OpInfo->Flags & AML_DEFER))
        {
            Op = AcpiPsGetDepthNext (Root, Op);
            continue;
        }

        switch (Op->Common.AmlOpcode)
        {
        case AML_METHOD_OP:
        case AML_BUFFER_OP:
        case AML_PACKAGE_OP:
        case AML_VAR_PACKAGE_OP:

            Status = AdDeferredParse (Op, Op->Named.Data, Op->Named.Length);
            if (ACPI_FAILURE (Status))
            {
                return_ACPI_STATUS (Status);
            }
            break;

        case AML_REGION_OP:
        case AML_CREATE_QWORD_FIELD_OP:
        case AML_CREATE_DWORD_FIELD_OP:
        case AML_CREATE_WORD_FIELD_OP:
        case AML_CREATE_BYTE_FIELD_OP:
        case AML_CREATE_BIT_FIELD_OP:
        case AML_CREATE_FIELD_OP:

            /* Nothing to do in these cases */

            break;

        default:
            ACPI_DEBUG_PRINT ((ACPI_DB_ERROR, "Unhandled deferred opcode [%s]\n",
                Op->Common.AmlOpName));
            break;
        }

        Op = AcpiPsGetDepthNext (Root, Op);
    }

    fprintf (stderr, "\n");
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
    ACPI_TABLE_HEADER       TableHeader;
    ACPI_TABLE_HEADER       *NewTable;


    if (Filename)
    {
        fprintf (stderr, "Loading DSDT from file %s\n", Filename);
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
        ACPI_STRNCPY (TableHeader.Signature, DSDT_SIG, 4);
        AcpiOsTableOverride (&TableHeader, &NewTable);

        if (NewTable)
        {
            Status = AE_OK;
            AcpiGbl_DSDT = NewTable;
            DsdtPtr = (UINT8 *) AcpiGbl_DSDT;
            DsdtLength = AcpiGbl_DSDT->Length;
            AdWriteDsdt ();
        }
        else
        {
            fprintf (stderr, "Could not obtain DSDT\n");
            Status = AE_NO_ACPI_TABLES;
        }
    }

    return Status;
}

/******************************************************************************
 *
 * FUNCTION:    AdParseTables
 *
 * PARAMETERS:  None
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Parse the DSDT.
 *
 *****************************************************************************/

ACPI_STATUS
AdParseTables (
    void)
{
    ACPI_STATUS             Status = AE_OK;
    ACPI_WALK_STATE         *WalkState;
    ACPI_TABLE_DESC         TableDesc;


    if (!AcpiGbl_DSDT)
    {
        return AE_NOT_EXIST;
    }

    /* Pass 1:  Parse everything except control method bodies */

    fprintf (stderr, "Pass 1 parse\n");

    DsdtLength = AcpiGbl_DSDT->Length;
    AmlLength  = DsdtLength  - sizeof (ACPI_TABLE_HEADER);
    AmlStart   = ((UINT8 *) AcpiGbl_DSDT + sizeof (ACPI_TABLE_HEADER));

    /* Create the root object */

    AcpiGbl_ParsedNamespaceRoot = AcpiPsCreateScopeOp ();
    if (!AcpiGbl_ParsedNamespaceRoot)
    {
        return AE_NO_MEMORY;
    }

    /* Create and initialize a new walk state */

    WalkState = AcpiDsCreateWalkState (TABLE_ID_DSDT,
                                    AcpiGbl_ParsedNamespaceRoot, NULL, NULL);
    if (!WalkState)
    {
        return (AE_NO_MEMORY);
    }

    Status = AcpiDsInitAmlWalk (WalkState, AcpiGbl_ParsedNamespaceRoot, NULL, AmlStart,
                    AmlLength, NULL, NULL, 1);
    if (ACPI_FAILURE (Status))
    {
        return (Status);
    }

    WalkState->ParseFlags &= ~ACPI_PARSE_DELETE_TREE;

    Status = AcpiPsParseAml (WalkState);
    if (ACPI_FAILURE (Status))
    {
        return Status;
    }

    /* Pass 2 */

    TableDesc.AmlStart = AmlStart;
    TableDesc.AmlLength = AmlLength;
    fprintf (stderr, "Pass 2 parse\n");

    Status = AcpiNsOneCompleteParse (2, &TableDesc);
    if (ACPI_FAILURE (Status))
    {
        return (Status);
    }

    /* Pass 3: Parse control methods and link their parse trees into the main parse tree */

    Status = AdParseDeferredOps (AcpiGbl_ParsedNamespaceRoot);

    fprintf (stderr, "Parsing completed\n");
    return AE_OK;
}


