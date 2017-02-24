/******************************************************************************
 *
 * Module Name: cvcompiler - ASL-/ASL+ converter functions
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2017, Intel Corp.
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

#include "aslcompiler.h"
#include "acparser.h"
#include "amlcode.h"
#include "acdebug.h"
#include "acconvert.h"


static void
CvPrintInclude(
    ACPI_FILE_NODE          *FNode,
    UINT32                  Level);

static BOOLEAN
CvListIsSingleton (
    ACPI_COMMENT_NODE       *CommentList);


/*******************************************************************************
 *
 * FUNCTION:    CvPrintOneCommentList
 *
 * PARAMETERS:  CommentList
 *              Level
 *
 * RETURN:      None
 *
 * DESCRIPTION: Prints all comments within the given list.
 *              This is referred as ASL_CV_PRINT_ONE_COMMENT_LIST.
 *
 ******************************************************************************/

void
CvPrintOneCommentList (
    ACPI_COMMENT_NODE       *CommentList,
    UINT32                  Level)
{
    ACPI_COMMENT_NODE       *Current = CommentList;
    ACPI_COMMENT_NODE       *Previous;


    while (Current)
    {
        Previous = Current;
        if (Current->Comment)
        {
            AcpiDmIndent(Level);
            AcpiOsPrintf("%s\n", Current->Comment);
            Current->Comment = NULL;
        }
        Current = Current->Next;
        AcpiOsReleaseObject(AcpiGbl_RegCommentCache, Previous);
    }
}


/*******************************************************************************
 *
 * FUNCTION:    CvListIsSingleton
 *
 * PARAMETERS:  CommentList -- check to see if this is a single item list.
 *
 * RETURN:      BOOLEAN
 *
 * DESCRIPTION: Returns TRUE if CommentList only contains 1 node.
 *
 ******************************************************************************/

static BOOLEAN
CvListIsSingleton (
    ACPI_COMMENT_NODE       *CommentList)

{
    if (!CommentList)
    {
        return FALSE;
    }
    else if (CommentList->Next)
    {
        return FALSE;
    }

    return TRUE;
}


/*******************************************************************************
 *
 * FUNCTION:    CvPrintOneCommentType
 *
 * PARAMETERS:  Op
 *              CommentType
 *              EndStr - String to print after printing the comment
 *              Level  - indentation level for comment lists.
 *
 * RETURN:      None
 *
 * DESCRIPTION: Prints all comments of CommentType within the given Op and
 *              clears the printed comment from the Op.
 *              This is referred as ASL_CV_PRINT_ONE_COMMENT.
 *
 ******************************************************************************/

void
CvPrintOneCommentType (
    ACPI_PARSE_OBJECT       *Op,
    UINT8                   CommentType,
    char*                   EndStr,
    UINT32                  Level)
{
    BOOLEAN                 CommentExists = FALSE;
    char                    **CommentToPrint = NULL;


    switch (CommentType)
    {
    case AML_COMMENT_STANDARD:

        if (CvListIsSingleton (Op->Common.CommentList))
        {
            CvPrintOneCommentList (Op->Common.CommentList, Level);
            AcpiOsPrintf ("\n");
        }
        else
        {
            CvPrintOneCommentList (Op->Common.CommentList, Level);
        }
        Op->Common.CommentList = NULL;
        return;

    case AML_COMMENT_ENDBLK:

        if (Op->Common.EndBlkComment)
        {
            CvPrintOneCommentList (Op->Common.EndBlkComment, Level);
            Op->Common.EndBlkComment = NULL;
            AcpiDmIndent(Level);
        }
        return;

    case AMLCOMMENT_INLINE:

        CommentToPrint = &Op->Common.InlineComment;
        break;

    case AML_COMMENT_END_NODE:

        CommentToPrint = &Op->Common.EndNodeComment;
        break;

    case AML_NAMECOMMENT:

        CommentToPrint = &Op->Common.NameComment;
        break;

    case AML_COMMENT_CLOSE_BRACE:

        CommentToPrint = &Op->Common.CloseBraceComment;
        break;

    default:
        return;
    }

    if (*CommentToPrint)
    {
        AcpiOsPrintf ("%s", *CommentToPrint);
        *CommentToPrint = NULL;
    }

    if (CommentExists && EndStr)
    {
        AcpiOsPrintf ("%s", EndStr);
    }
}


/*******************************************************************************
 *
 * FUNCTION:    CvCloseBraceWriteComment
 *
 * PARAMETERS:  Op
 *              Level
 *
 * RETURN:      none
 *
 * DESCRIPTION: Print a close brace } and any open brace comments associated
 *              with this parse object.
 *              This is referred as ASL_CV_CLOSE_BRACE.
 *
 ******************************************************************************/

void
CvCloseBraceWriteComment(
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level)
{
    if (!Gbl_CaptureComments)
    {
        AcpiOsPrintf ("}");
        return;
    }

    CvPrintOneCommentType (Op, AML_COMMENT_ENDBLK, NULL, Level);
    AcpiOsPrintf ("}");
    CvPrintOneCommentType (Op, AML_COMMENT_CLOSE_BRACE, NULL, Level);
}


/*******************************************************************************
 *
 * FUNCTION:    CvCloseParenWriteComment
 *
 * PARAMETERS:  Op
 *              Level
 *
 * RETURN:      none
 *
 * DESCRIPTION: Print a closing paren ) and any end node comments associated
 *              with this parse object.
 *              This is referred as ASL_CV_CLOSE_PAREN.
 *
 ******************************************************************************/

void
CvCloseParenWriteComment(
    ACPI_PARSE_OBJECT       *Op,
    UINT32                  Level)
{
    if (!Gbl_CaptureComments)
    {
        AcpiOsPrintf (")");
        return;
    }

    /*
     * If this op has a BLOCK_BRACE, then output the comment when the
     * disassembler calls CvCloseBraceWriteComment
     */
    if (AcpiDmBlockType (Op) == BLOCK_PAREN)
    {
        CvPrintOneCommentType (Op, AML_COMMENT_ENDBLK, NULL, Level);
    }

    AcpiOsPrintf (")");

    if (Op->Common.EndNodeComment)
    {
        CvPrintOneCommentType (Op, AML_COMMENT_END_NODE, NULL, Level);
    }
    else if ((Op->Common.Parent->Common.AmlOpcode == AML_IF_OP) &&
         Op->Common.Parent->Common.EndNodeComment)
    {
        CvPrintOneCommentType (Op->Common.Parent,
            AML_COMMENT_END_NODE, NULL, Level);
    }
}


/*******************************************************************************
 *
 * FUNCTION:    CvFileHasSwitched
 *
 * PARAMETERS:  Op
 *
 * RETURN:      BOOLEAN
 *
 * DESCRIPTION: Determine whether if a file has switched.
 *              TRUE - file has switched.
 *              FALSE - file has not switched.
 *              This is referred as ASL_CV_FILE_HAS_SWITCHED.
 *
 ******************************************************************************/

BOOLEAN
CvFileHasSwitched(
    ACPI_PARSE_OBJECT       *Op)
{
    if (Op->Common.CvFilename   &&
        AcpiGbl_CurrentFilename &&
        AcpiUtStricmp(Op->Common.CvFilename, AcpiGbl_CurrentFilename))
    {
        return TRUE;
    }
    return FALSE;
}


/*******************************************************************************
 *
 * FUNCTION:    CvPrintInclude
 *
 * PARAMETERS:  FNode - Write an Include statement for the file that is pointed
 *                      by FNode->File.
 *              Level - indentation level
 *
 * RETURN:      None
 *
 * DESCRIPTION: Write the ASL Include statement for FNode->File in the file
 *              indicated by FNode->Parent->File. Note this function emits
 *              actual ASL code rather than comments. This switches the output
 *              file to FNode->Parent->File.
 *
 ******************************************************************************/

static void
CvPrintInclude(
    ACPI_FILE_NODE          *FNode,
    UINT32                  Level)
{
    if (!FNode || FNode->IncludeWritten)
    {
        return;
    }

    CvDbgPrint ("Writing include for %s within %s\n", FNode->Filename, FNode->Parent->Filename);
    AcpiOsRedirectOutput (FNode->Parent->File);
    CvPrintOneCommentList (FNode->IncludeComment, Level);
    AcpiDmIndent (Level);
    AcpiOsPrintf ("Include (\"%s\")\n", FNode->Filename);
    CvDbgPrint ("emitted the following: Include (\"%s\")\n", FNode->Filename);
    FNode->IncludeWritten = TRUE;
}


/*******************************************************************************
 *
 * FUNCTION:    CvSwitchFiles
 *
 * PARAMETERS:  Level - indentation level
 *              Op
 *
 * RETURN:      None
 *
 * DESCRIPTION: Switch the outputfile and write ASL Include statement. Note,
 *              this function emits actual ASL code rather than comments.
 *              This is referred as ASL_CV_SWITCH_FILES.
 *
 ******************************************************************************/

void
CvSwitchFiles(
    UINT32                  Level,
    ACPI_PARSE_OBJECT       *Op)
{
    char                    *Filename = Op->Common.CvFilename;
    ACPI_FILE_NODE          *FNode;

    CvDbgPrint ("Switching from %s to %s\n", AcpiGbl_CurrentFilename, Filename);
    FNode = CvFilenameExists (Filename, AcpiGbl_FileTreeRoot);
    if (!FNode)
    {
        /*
         * At this point, each Filename should exist in AcpiGbl_FileTreeRoot
         * if it does not exist, then abort.
         */
        FlDeleteFile (ASL_FILE_AML_OUTPUT);
        sprintf (MsgBuffer, "\"Cannot find %s\" - %s", Filename, strerror (errno));
        AslCommonError (ASL_ERROR, ASL_MSG_OPEN, 0, 0, 0, 0, NULL, MsgBuffer);
        AslAbort ();
    }

    /*
     * If the previous file is a descendent of the current file,
     * make sure that Include statements from the current file
     * to the previous have been emitted.
     */
    while (FNode &&
           FNode->Parent &&
           AcpiUtStricmp (FNode->Filename, AcpiGbl_CurrentFilename))
    {
        CvPrintInclude (FNode, Level);
        FNode = FNode->Parent;
    }

    /* Redirect output to the Op->Common.CvFilename */

    FNode = CvFilenameExists (Filename, AcpiGbl_FileTreeRoot);
    AcpiOsRedirectOutput (FNode->File);
    AcpiGbl_CurrentFilename = FNode->Filename;
}
