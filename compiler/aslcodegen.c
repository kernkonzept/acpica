
/******************************************************************************
 *
 * Module Name: aslcodegen - AML code generation
 *              $Revision: 1.38 $
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


#include "aslcompiler.h"
#include "aslcompiler.y.h"
#include "aslresource.h"
#include "amlcode.h"
#include "acparser.h"

#define _COMPONENT          ACPI_COMPILER
        MODULE_NAME         ("aslcodegen")




/*******************************************************************************
 *
 * FUNCTION:    CgGenerateAmlOutput
 *
 * PARAMETERS:  None.
 *
 * RETURN:      None
 *
 * DESCRIPTION: Generate AML code.  Currently generates the listing file
 *              simultaneously.
 *
 ******************************************************************************/

void
CgGenerateAmlOutput (void)
{

    DbgPrint (ASL_DEBUG_OUTPUT, "\nWriting AML\n\n");

    /* Generate the AML output file */

    FlSeekFile (ASL_FILE_SOURCE_OUTPUT, 0);
    Gbl_SourceLine = 0;
    Gbl_NextError = Gbl_ErrorLog;

    TrWalkParseTree (RootNode, ASL_WALK_VISIT_DOWNWARD, CgAmlWriteWalk, NULL, NULL);
    CgCloseTable ();
}



/*******************************************************************************
 *
 * FUNCTION:    CgAmlWriteWalk
 *
 * PARAMETERS:  ASL_WALK_CALLBACK
 *
 * RETURN:      None
 *
 * DESCRIPTION: Parse tree walk to generate the AML code.
 *
 ******************************************************************************/

ACPI_STATUS
CgAmlWriteWalk (
    ASL_PARSE_NODE          *Node,
    UINT32                  Level,
    void                    *Context)
{


    DbgPrint (ASL_TREE_OUTPUT,
        "%5.5d [%d]", Node->LogicalLineNumber, Level);
    UtPrintFormattedName (Node->ParseOpcode, Level);

    if (Node->ParseOpcode == NAMESEG    ||
        Node->ParseOpcode == NAMESTRING ||
        Node->ParseOpcode == METHODCALL)
    {
        DbgPrint (ASL_TREE_OUTPUT,
            "%10.32s      ", Node->ExternalName);
    }
    else
    {
        DbgPrint (ASL_TREE_OUTPUT, "                ");
    }

    DbgPrint (ASL_TREE_OUTPUT,
        "Val-%08X POp-%04X AOp-%04X OpLen-%01X PByts-%01X Len-%04X SubLen-%04X PSubLen-%04X Node-%08X Chld-%08X Paren-%08X Flags-%04X AcTyp-%08X C-%2d L-%d\n",
                Node->Value.Integer32,
                Node->ParseOpcode,
                Node->AmlOpcode,
                Node->AmlOpcodeLength,
                Node->AmlPkgLenBytes,
                Node->AmlLength,
                Node->AmlSubtreeLength,
                Node->Parent ? Node->Parent->AmlSubtreeLength : 0,
                Node,
                Node->Child,
                Node->Parent,
                Node->Flags,
                Node->AcpiBtype,
                Node->Column,
                Node->LineNumber);


    CgWriteNode (Node);
    return (AE_OK);
}


/*******************************************************************************
 *
 * FUNCTION:    CgLocalWriteAmlData
 *
 * PARAMETERS:  Buffer          - Buffer to write
 *              Length          - Size of data in buffer
 *
 * RETURN:      None
 *
 * DESCRIPTION: Write a buffer of AML data to the AML output file.
 *
 ******************************************************************************/

void
CgLocalWriteAmlData (
    ASL_PARSE_NODE          *Node,
    void                    *Buffer,
    UINT32                  Length)
{


    /* Write the raw data to the AML file */

    FlWriteFile (ASL_FILE_AML_OUTPUT, Buffer, Length);

    /* Update the final AML length for this node (used for listings) */

    if (Node)
    {
        Node->FinalAmlLength += Length;
    }
}


/*******************************************************************************
 *
 * FUNCTION:    CgWriteAmlOpcode
 *
 * PARAMETERS:  Node            - Parse node with an AML opcode
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Write the AML opcode corresponding to a parse node.
 *
 ******************************************************************************/

void
CgWriteAmlOpcode (
    ASL_PARSE_NODE              *Node)
{
    union {
        UINT16                      Opcode;
        UINT8                       OpcodeBytes[2];
    } Aml;
    union {
        UINT32                      Len;
        UINT8                       LenBytes[4];
    } PkgLen;

    UINT8                       PkgLenFirstByte;
    UINT32                      i;


    /* We expect some DEFAULT_ARGs, just ignore them */

    if (Node->ParseOpcode == DEFAULT_ARG)
    {
        return;
    }

    switch (Node->AmlOpcode)
    {
    case AML_UNASSIGNED_OPCODE:

        /* These opcodes should not get here */

        printf ("Found a node with an unassigned AML opcode\n");
        fprintf (stderr, "Found a node with an unassigned AML opcode\n");
        return;

    case AML_INT_RESERVEDFIELD_OP:

        /* Special opcodes for within a field definition */

        Aml.Opcode = 0x00;
        break;

    case AML_INT_ACCESSFIELD_OP:

        Aml.Opcode = 0x01;
        break;

    default:
        Aml.Opcode = Node->AmlOpcode;
        break;
    }


    switch (Aml.Opcode)
    {
    case AML_PACKAGE_LENGTH:

        /* Value is the length to be encoded (Used in field definitions) */

        PkgLen.Len = Node->Value.Integer32;
        break;

    default:

        /* Check for two-byte opcode */

        if (Aml.Opcode > 0x00FF)
        {
            /* Write the high byte first */

            CgLocalWriteAmlData (Node, &Aml.OpcodeBytes[1], 1);
        }

        CgLocalWriteAmlData (Node, &Aml.OpcodeBytes[0], 1);

        /* Subtreelength doesn't include length of package length bytes */

        PkgLen.Len = Node->AmlSubtreeLength + Node->AmlPkgLenBytes;
        break;
    }


    /* Does this opcode have an associated "PackageLength" field? */

    if (Node->Flags & NODE_AML_PACKAGE)
    {
        if (Node->AmlPkgLenBytes == 1)
        {
            /* Simplest case -- no bytes to follow, just write the count */

            CgLocalWriteAmlData (Node, &PkgLen.LenBytes[0], 1);
        }

        else
        {
            /*
             * Encode the "bytes to follow" in the first byte, top two bits.
             * The low-order nybble of the length is in the bottom 4 bits
             */
            PkgLenFirstByte = (UINT8) (((Node->AmlPkgLenBytes - 1) << 6) |
                                        (PkgLen.LenBytes[0] & 0x0F));

            CgLocalWriteAmlData (Node, &PkgLenFirstByte, 1);

            /* Shift the length over by the 4 bits we just stuffed in the first byte */

            PkgLen.Len >>= 4;

            /* Now we can write the remaining bytes - either 1, 2, or 3 bytes */

            for (i = 0; i < (UINT32) (Node->AmlPkgLenBytes - 1); i++)
            {
                CgLocalWriteAmlData (Node, &PkgLen.LenBytes[i], 1);
            }
        }
    }

    switch (Aml.Opcode)
    {
    case AML_BYTE_OP:
        CgLocalWriteAmlData (Node, &Node->Value.Integer8, 1);
        break;

    case AML_WORD_OP:
        CgLocalWriteAmlData (Node, &Node->Value.Integer16, 2);
       break;

    case AML_DWORD_OP:
        CgLocalWriteAmlData (Node, &Node->Value.Integer32, 4);
        break;

    case AML_QWORD_OP:
        CgLocalWriteAmlData (Node, &Node->Value.Integer64, 8);
        break;

    case AML_STRING_OP:
        CgLocalWriteAmlData (Node, Node->Value.String, Node->AmlLength);
    }
}


/*******************************************************************************
 *
 * FUNCTION:    CgWriteTableHeader
 *
 * PARAMETERS:  Node        - The DEFINITIONBLOCK node
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Write a table header corresponding to the DEFINITIONBLOCK
 *
 ******************************************************************************/

void
CgWriteTableHeader (
    ASL_PARSE_NODE              *Node)
{
    ASL_PARSE_NODE              *Child;


    /* AML filename */

    Child = Node->Child;


    /* Signature */

    Child = Child->Peer;
    strncpy (TableHeader.Signature, Child->Value.String, 4);

    /* Revision */

    Child = Child->Peer;
    TableHeader.Revision = Child->Value.Integer8;

    /* OEMID */

    Child = Child->Peer;
    strncpy (TableHeader.OemId, Child->Value.String, 6);

    /* OEM TableID */

    Child = Child->Peer;
    strncpy (TableHeader.OemTableId, Child->Value.String, 8);

    /* OEM Revision */

    Child = Child->Peer;
    TableHeader.OemRevision = Child->Value.Integer32;

    /* Compiler ID */

    strncpy (TableHeader.AslCompilerId, CompilerCreatorId, 4);

    /* Compiler version */

    TableHeader.AslCompilerRevision = CompilerCreatorRevision;

    /* Table length.  Checksum zero for now, will rewrite later */

    TableHeader.Length   = Gbl_TableLength;
    TableHeader.Checksum = 0;

    CgLocalWriteAmlData (Node, &TableHeader, sizeof (ACPI_TABLE_HEADER));
}


/*******************************************************************************
 *
 * FUNCTION:    CgCloseTable
 *
 * PARAMETERS:  None.
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Complete the ACPI table by calculating the checksum and
 *              re-writing the header.
 *
 ******************************************************************************/

void
CgCloseTable (void)
{
    signed char         Sum;
    UINT8               FileByte;


    /* Calculate the checksum over the entire file */

    FlSeekFile (ASL_FILE_AML_OUTPUT, 0);
    Sum = 0;

    while (FlReadFile (ASL_FILE_AML_OUTPUT, &FileByte, 1) == AE_OK)
    {
        Sum = (signed char) (Sum + FileByte);
    }

    /* Re-write the table header with the checksum */

    TableHeader.Checksum = (UINT8) (0 - Sum);

    FlSeekFile (ASL_FILE_AML_OUTPUT, 0);
    CgLocalWriteAmlData (NULL, &TableHeader, sizeof (ACPI_TABLE_HEADER));
}


/*******************************************************************************
 *
 * FUNCTION:    CgWriteNode
 *
 * PARAMETERS:  Node            - Parse node to write.
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Write the AML that corresponds to a parse node.
 *
 ******************************************************************************/

void
CgWriteNode (
    ASL_PARSE_NODE          *Node)
{
    ASL_RESOURCE_NODE       *Rnode;


    Node->FinalAmlLength = 0;

    /* Always check for DEFAULT_ARG and other "Noop" nodes */
    /* TBD: this may not be the best place for this check */

    if ((Node->ParseOpcode == DEFAULT_ARG)  ||
        (Node->ParseOpcode == EXTERNAL)     ||
        (Node->ParseOpcode == INCLUDE)      ||
        (Node->ParseOpcode == INCLUDE_END))
    {
        return;
    }

    switch (Node->AmlOpcode)
    {
    case AML_RAW_DATA_BYTE:
    case AML_RAW_DATA_WORD:
    case AML_RAW_DATA_DWORD:
    case AML_RAW_DATA_QWORD:

        CgLocalWriteAmlData (Node, &Node->Value.Integer, Node->AmlLength);
        return;


    case AML_RAW_DATA_BUFFER:

        CgLocalWriteAmlData (Node, Node->Value.Pointer, Node->AmlLength);
        return;


    case AML_RAW_DATA_CHAIN:

        Rnode = Node->Value.Pointer;
        while (Rnode)
        {
            CgLocalWriteAmlData (Node, Rnode->Buffer, Rnode->BufferLength);
            Rnode = Rnode->Next;
        }
        return;
    }

    switch (Node->ParseOpcode)
    {
    case DEFAULT_ARG:
        break;

    case DEFINITIONBLOCK:
        CgWriteTableHeader (Node);
        break;

    case NAMESEG:
    case NAMESTRING:
    case METHODCALL:
        CgLocalWriteAmlData (Node, Node->Value.String, Node->AmlLength);
        break;

    default:
        CgWriteAmlOpcode (Node);
        break;
    }
}


