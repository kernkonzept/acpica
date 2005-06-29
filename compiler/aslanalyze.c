
/******************************************************************************
 *
 * Module Name: aslanalyze.c - check for semantic errors
 *              $Revision: 1.9 $
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


#include "AslCompiler.h"
#include "AslCompiler.y.h"

#include <ctype.h>


#define ASL_RSVD_RETURN_VALUE   0x01
UINT32 AslGbl_ReservedMethods = 0;

typedef struct
{
    char                        *Name;
    UINT8                       NumArguments;
    UINT8                       Flags;

} ASL_RESERVED_INFO;


ASL_RESERVED_INFO               ReservedMethods[] = {
    "_AC0",     0,      ASL_RSVD_RETURN_VALUE,
    "_AC1",     0,      ASL_RSVD_RETURN_VALUE,
    "_AC2",     0,      ASL_RSVD_RETURN_VALUE,
    "_AC3",     0,      ASL_RSVD_RETURN_VALUE,
    "_ADR",     0,      ASL_RSVD_RETURN_VALUE,
    "_AL0",     0,      ASL_RSVD_RETURN_VALUE,
    "_AL1",     0,      ASL_RSVD_RETURN_VALUE,
    "_AL2",     0,      ASL_RSVD_RETURN_VALUE,
    "_AL3",     0,      ASL_RSVD_RETURN_VALUE,
    "_BBN",     0,      ASL_RSVD_RETURN_VALUE,
    "_BCL",     0,      ASL_RSVD_RETURN_VALUE,
    "_BCM",     1,      0,
    "_BDN",     0,      ASL_RSVD_RETURN_VALUE,
    "_BFS",     1,      0,
    "_BIF",     0,      ASL_RSVD_RETURN_VALUE,
    "_BST",     0,      ASL_RSVD_RETURN_VALUE,
    "_BTP",     1,      0,
    "_CID",     0,      ASL_RSVD_RETURN_VALUE,
    "_CRS",     0,      ASL_RSVD_RETURN_VALUE,
    "_CRT",     0,      ASL_RSVD_RETURN_VALUE,
    "_CST",     0,      ASL_RSVD_RETURN_VALUE,
    "_DCK",     1,      ASL_RSVD_RETURN_VALUE,
    "_DCS",     0,      ASL_RSVD_RETURN_VALUE,
    "_DDC",     1,      ASL_RSVD_RETURN_VALUE,
    "_DDN",     1,      0,                          /* Spec is ambiguous about this */
    "_DGS",     0,      ASL_RSVD_RETURN_VALUE,
    "_DIS",     0,      0,
    "_DMA",     0,      ASL_RSVD_RETURN_VALUE,
    "_DOD",     0,      ASL_RSVD_RETURN_VALUE,
    "_DOS",     1,      0,
    "_DSS",     1,      0,
    "_EC_",     0,      ASL_RSVD_RETURN_VALUE,
    "_EDL",     0,      ASL_RSVD_RETURN_VALUE,
    "_EJ0",     1,      0,
    "_EJ1",     1,      0,
    "_EJ2",     1,      0,
    "_EJ3",     1,      0,
    "_EJ4",     1,      0,
    "_EJD",     0,      ASL_RSVD_RETURN_VALUE,
    "_FDE",     0,      ASL_RSVD_RETURN_VALUE,
    "_FDI",     0,      ASL_RSVD_RETURN_VALUE,
    "_FDM",     1,      0,
    "_FIX",     0,      ASL_RSVD_RETURN_VALUE,
    "_GLK",     0,      ASL_RSVD_RETURN_VALUE,
    "_GPD",     0,      ASL_RSVD_RETURN_VALUE,
    "_GPE",     0,      ASL_RSVD_RETURN_VALUE,
    "_GTF",     0,      ASL_RSVD_RETURN_VALUE,
    "_GTM",     0,      ASL_RSVD_RETURN_VALUE,
    "_GTS",     1,      0,
    "_HID",     0,      ASL_RSVD_RETURN_VALUE,
    "_HOT",     0,      ASL_RSVD_RETURN_VALUE,
    "_HPP",     0,      ASL_RSVD_RETURN_VALUE,
    "_INI",     0,      0,
    "_IRC",     0,      0,
    "_LCK",     1,      0,
    "_LID",     0,      ASL_RSVD_RETURN_VALUE,
    "_MAT",     0,      ASL_RSVD_RETURN_VALUE,
    "_MSG",     1,      0,
    "_OFF",     0,      0,
    "_ON_",     0,      0,
    "_PCL",     0,      ASL_RSVD_RETURN_VALUE,
    "_PCT",     0,      ASL_RSVD_RETURN_VALUE,
    "_PIC",     1,      0,
    "_PPC",     0,      ASL_RSVD_RETURN_VALUE,
    "_PR0",     0,      ASL_RSVD_RETURN_VALUE,
    "_PR1",     0,      ASL_RSVD_RETURN_VALUE,
    "_PR2",     0,      ASL_RSVD_RETURN_VALUE,
    "_PRS",     0,      ASL_RSVD_RETURN_VALUE,
    "_PRT",     0,      ASL_RSVD_RETURN_VALUE,
    "_PRW",     0,      ASL_RSVD_RETURN_VALUE,
    "_PS0",     0,      0,
    "_PS1",     0,      0,
    "_PS2",     0,      0,
    "_PS3",     0,      0,
    "_PSC",     0,      ASL_RSVD_RETURN_VALUE,
    "_PSL",     0,      ASL_RSVD_RETURN_VALUE,
    "_PSR",     0,      ASL_RSVD_RETURN_VALUE,
    "_PSS",     0,      ASL_RSVD_RETURN_VALUE,
    "_PSV",     0,      ASL_RSVD_RETURN_VALUE,
    "_PSW",     1,      0,
    "_PTC",     0,      ASL_RSVD_RETURN_VALUE,
    "_PTS",     1,      0,
    "_PXM",     0,      ASL_RSVD_RETURN_VALUE,
    "_REG",     2,      0,
    "_RMV",     0,      ASL_RSVD_RETURN_VALUE,
    "_ROM",     2,      ASL_RSVD_RETURN_VALUE,
    "_S0_",     0,      ASL_RSVD_RETURN_VALUE,
    "_S1_",     0,      ASL_RSVD_RETURN_VALUE,
    "_S2_",     0,      ASL_RSVD_RETURN_VALUE,
    "_S3_",     0,      ASL_RSVD_RETURN_VALUE,
    "_S4_",     0,      ASL_RSVD_RETURN_VALUE,
    "_S5_",     0,      ASL_RSVD_RETURN_VALUE,
    "_S1D",     0,      ASL_RSVD_RETURN_VALUE,
    "_S2D",     0,      ASL_RSVD_RETURN_VALUE,
    "_S3D",     0,      ASL_RSVD_RETURN_VALUE,
    "_S4D",     0,      ASL_RSVD_RETURN_VALUE,
    "_SBS",     0,      ASL_RSVD_RETURN_VALUE,
    "_SCP",     1,      0,
    "_SEG",     0,      ASL_RSVD_RETURN_VALUE,
    "_SPD",     1,      ASL_RSVD_RETURN_VALUE,
    "_SRS",     1,      0,
    "_SST",     1,      0,
    "_STA",     0,      ASL_RSVD_RETURN_VALUE,
    "_STM",     3,      0,
    "_STR",     0,      ASL_RSVD_RETURN_VALUE,
    "_SUN",     0,      ASL_RSVD_RETURN_VALUE,
    "_TC1",     0,      ASL_RSVD_RETURN_VALUE,
    "_TC2",     0,      ASL_RSVD_RETURN_VALUE,
    "_TMP",     0,      ASL_RSVD_RETURN_VALUE,
    "_TSP",     0,      ASL_RSVD_RETURN_VALUE,
    "_TZD",     0,      ASL_RSVD_RETURN_VALUE,
    "_TZP",     0,      ASL_RSVD_RETURN_VALUE,
    "_UID",     0,      ASL_RSVD_RETURN_VALUE,
    "_VPO",     0,      ASL_RSVD_RETURN_VALUE,
    "_WAK",     1,      ASL_RSVD_RETURN_VALUE,
    NULL,       0,      0,
};

char    MsgBuffer[256];



/*******************************************************************************
 *
 * FUNCTION:    
 *
 * PARAMETERS:  
 *
 * RETURN:      
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

void
AnCheckForReservedMethod (
    ASL_PARSE_NODE              *Node,
    ASL_METHOD_INFO             *MethodInfo)
{
    UINT32                      i;



    /* All reserved names are prefixed with a single underscore */

    if (Node->ExternalName[0] != '_')
    {
        return;
    }


    /* Check for a standard reserved method name */

    for (i = 0; ReservedMethods[i].Name; i++)
    {
        if (!STRCMP (Node->ExternalName, ReservedMethods[i].Name))
        {
            AslGbl_ReservedMethods++;

            /* Matched a reserved method name */

            if (MethodInfo->NumArguments != ReservedMethods[i].NumArguments)
            {
                sprintf (MsgBuffer, "\n%32s requires %d arg(s) not %d", 
                            ReservedMethods[i].Name,
                            ReservedMethods[i].NumArguments, 
                            MethodInfo->NumArguments);

                AslError (ASL_WARNING, ASL_MSG_RESERVED_ARG_COUNT, Node, MsgBuffer);
            }

            if (MethodInfo->NumReturnNoValue &&
                ReservedMethods[i].Flags & ASL_RSVD_RETURN_VALUE)
            {
                sprintf (MsgBuffer, "%s", 
                            ReservedMethods[i].Name);

                AslError (ASL_WARNING, ASL_MSG_RESERVED_RETURN_VALUE, Node, MsgBuffer);
            }

            return;
        }
    }

    /*
     * Now check for the "special" reserved names --
     * GPE:  _Lxx
     * GPE:  _Exx
     * EC:   _Qxx
     */

    if ((Node->ExternalName[1] == 'L') ||
        (Node->ExternalName[1] == 'E') ||
        (Node->ExternalName[1] == 'Q'))
    {

        AslGbl_ReservedMethods++;

        /* The next two characters must be hex digits */

        if ((isxdigit (Node->ExternalName[2])) &&
            (isxdigit (Node->ExternalName[3])) &&
            (MethodInfo->NumArguments != 0))
        {
            sprintf (MsgBuffer, "\n%32s requires %d arg(s) not %d", 
                        Node->ExternalName,
                        0, 
                        MethodInfo->NumArguments);

            AslError (ASL_WARNING, ASL_MSG_RESERVED_ARG_COUNT, Node, MsgBuffer);
        }

        return;
    }


    /* Check for the names reserved for the compiler itself: _T_x */

    if ((Node->ExternalName[1] == 'T') &&
        (Node->ExternalName[2] == '_'))
    {
        AslError (ASL_ERROR, ASL_MSG_RESERVED_WORD, Node, Node->ExternalName);
    }
}



/*******************************************************************************
 *
 * FUNCTION:    
 *
 * PARAMETERS:  
 *
 * RETURN:      
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

void
AnSemanticAnalysisWalkBegin (
    ASL_PARSE_NODE              *Node,
    UINT32                      Level,
    void                        *Context)
{
    ASL_ANALYSIS_WALK_INFO      *WalkInfo = (ASL_ANALYSIS_WALK_INFO *) Context;
    ASL_METHOD_INFO             *MethodInfo = WalkInfo->MethodStack;
    ASL_PARSE_NODE              *Next;
    UINT32                      RegisterNumber;
    char                        LocalName[] = "Local0";
    char                        ArgName[] = "Arg0";



    switch (Node->ParseOpcode)
    {
    case METHOD:

        MethodInfo = UtLocalCalloc (sizeof (ASL_METHOD_INFO));
        MethodInfo->Next = WalkInfo->MethodStack;
        WalkInfo->MethodStack = MethodInfo;

        /* Get the NumArguments node */

        Next = Node->Child;
        Next = Next->Peer;
        MethodInfo->NumArguments = (Next->Value.Integer8 & 0x07);


        break;


    case LOCAL0:
    case LOCAL1:
    case LOCAL2:
    case LOCAL3:
    case LOCAL4:
    case LOCAL5:
    case LOCAL6:
    case LOCAL7:
        RegisterNumber = (Node->AmlOpcode & 0x000F);

        if (Node->Flags & NODE_IS_TARGET)
        {
            MethodInfo->LocalInitialized[RegisterNumber] = TRUE;

        }
        else if (!MethodInfo->LocalInitialized[RegisterNumber])
        {
            LocalName[strlen (LocalName) -1] = RegisterNumber + 0x30;
            AslError (ASL_ERROR, ASL_MSG_LOCAL_INIT, Node, LocalName);
        }
        break;


    case ARG0:
    case ARG1:
    case ARG2:
    case ARG3:
    case ARG4:
    case ARG5:
    case ARG6:
        RegisterNumber = (Node->AmlOpcode & 0x000F) - 8;

        if (RegisterNumber >= MethodInfo->NumArguments)
        {
            ArgName[strlen (ArgName) -1] = RegisterNumber + 0x30;
            AslError (ASL_ERROR, ASL_MSG_ARG_INVALID, Node, ArgName);
        }
        break;
        

    case RETURN:

        if ((Node->Child) &&
            (Node->Child->ParseOpcode != DEFAULT_ARG))
        {
            MethodInfo->NumReturnWithValue++;
        }
        else
        {
            MethodInfo->NumReturnNoValue++;
        }
        break;


    case NAMESEG:
    case NAMESTRING:

       break;

    } /* end switch */


}


/*******************************************************************************
 *
 * FUNCTION:    
 *
 * PARAMETERS:  
 *
 * RETURN:      
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

BOOLEAN
AnLastStatementIsReturn (
    ASL_PARSE_NODE              *Node)
{    
    ASL_PARSE_NODE              *Next;


    /*
     * Check if last statement is a return
     */
    Next = ASL_GET_CHILD_NODE (Node);
    while (Next)
    {
        if ((!Next->Peer) &&
            (Next->ParseOpcode == RETURN))
        {
            return TRUE;
        }

        Next = ASL_GET_PEER_NODE (Next);
    }

    return FALSE;
}


/*******************************************************************************
 *
 * FUNCTION:    
 *
 * PARAMETERS:  
 *
 * RETURN:      
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

void
AnSemanticAnalysisWalkEnd (
    ASL_PARSE_NODE              *Node,
    UINT32                      Level,
    void                        *Context)
{
    ASL_ANALYSIS_WALK_INFO      *WalkInfo = (ASL_ANALYSIS_WALK_INFO *) Context;
    ASL_METHOD_INFO             *MethodInfo;
    ASL_PARSE_NODE              *SecondToLastNode = NULL;
    ASL_PARSE_NODE              *LastNode = NULL;




    switch (Node->ParseOpcode)
    {
    case METHOD:

        MethodInfo = WalkInfo->MethodStack;
        WalkInfo->MethodStack = MethodInfo->Next;


        
        /*
         * Check if there is no return statement at the end of the
         * method AND we can actually get there -- i.e., the execution
         * of the method can possibly terminate without a return statement.
         */
        if ((!AnLastStatementIsReturn (Node)) &&
            (!(Node->Flags & NODE_HAS_NO_EXIT)))
        {
            /* 
             * No return statement, and execution can possibly exit
             * via this path.  This is equivalent to Return () 
             */

            MethodInfo->NumReturnNoValue++;
        }


        /*
         * Check for case where some return statements have a return value
         * and some do not.  Exit without a return statement is a return with
         * no value
         */

        if (MethodInfo->NumReturnNoValue &&
            MethodInfo->NumReturnWithValue)
        {
            AslError (ASL_WARNING, ASL_MSG_RETURN_TYPES, Node, Node->ExternalName);
        }


        /*
         * Check predefined method names for correct return behavior
         * and correct number of arguments
         */
        AnCheckForReservedMethod (Node, MethodInfo);
        free (MethodInfo);

        break;


    case RETURN:

        Node->Parent->Flags |= NODE_HAS_NO_EXIT;
        break;


    case IF:

        if ((Node->Flags & NODE_HAS_NO_EXIT) &&
            (Node->Peer) &&
            (Node->Peer->ParseOpcode == ELSE))
        {
            Node->Peer->Flags |= NODE_IF_HAS_NO_EXIT;
        }
        break;


    case ELSE:

        if ((Node->Flags & NODE_HAS_NO_EXIT) &&
            (Node->Flags & NODE_IF_HAS_NO_EXIT))
        {
            Node->Parent->Flags |= NODE_HAS_NO_EXIT;
        }

        break;


    default:

        if (Node->Flags & NODE_HAS_NO_EXIT)
        {
            Node->Parent->Flags |= NODE_HAS_NO_EXIT;
        }
        break;

    }
}







