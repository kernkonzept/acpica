/******************************************************************************
 *
 * Module Name: pstree - Parser op tree manipulation/traversal/search
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


#define __PSTREE_C__

#include "acpi.h"
#include "acparser.h"
#include "amlcode.h"

#define _COMPONENT          PARSER
        MODULE_NAME         ("pstree");


/*******************************************************************************
 *
 * FUNCTION:    AcpiPsGetArg
 *
 * PARAMETERS:  Op              - Get an argument for this op
 *              Argn            - Nth argument to get
 *
 * RETURN:      The argument (as an Op object).  NULL if argument does not exist
 *
 * DESCRIPTION: Get the specified op's argument.
 *
 ******************************************************************************/

ACPI_GENERIC_OP *
AcpiPsGetArg (
    ACPI_GENERIC_OP         *Op,
    UINT32                  Argn)
{
    ACPI_GENERIC_OP         *Arg = NULL;
    ACPI_OP_INFO            *OpInfo;


    /* Get the info structure for this opcode */

    OpInfo = AcpiPsGetOpcodeInfo (Op->Opcode);
    if (!OpInfo)
    {
        /* Invalid opcode */

        return NULL;
    }

    /* Check if this opcode requires argument sub-objects */

    if (!(OpInfo->Flags & OP_INFO_HAS_ARGS))
    {
        /* Has no linked argument objects */

        return NULL;
    }

    /* Get the requested argument object */

    Arg = Op->Value.Arg;
    while (Arg && Argn)
    {
        Argn--;
        Arg = Arg->Next;
    }

    return Arg;
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiPsAppendArg
 *
 * PARAMETERS:  Op              - Append an argument to this Op.
 *              Arg             - Argument Op to append
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Append an argument to an op's argument list (a NULL arg is OK)
 *
 ******************************************************************************/

void
AcpiPsAppendArg (
    ACPI_GENERIC_OP         *Op,
    ACPI_GENERIC_OP         *Arg)
{
    ACPI_GENERIC_OP         *PrevArg;
    ACPI_OP_INFO            *OpInfo;


    if (!Op)
    {
        return;
    }

    /* Get the info structure for this opcode */

    OpInfo = AcpiPsGetOpcodeInfo (Op->Opcode);
    if (!OpInfo)
    {
        /* Invalid opcode */

        return;
    }

    /* Check if this opcode requires argument sub-objects */

    if (!(OpInfo->Flags & OP_INFO_HAS_ARGS))
    {
        /* Has no linked argument objects */

        return;
    }


    /* Append the argument to the linked argument list */

    if (Op->Value.Arg)
    {
        /* Append to existing argument list */

        PrevArg = Op->Value.Arg;
        while (PrevArg->Next)
        {
            PrevArg = PrevArg->Next;
        }
        PrevArg->Next = Arg;
    }

    else
    {
        /* No argument list, this will be the first argument */

        Op->Value.Arg = Arg;
    }


    /* Set the parent in this arg and any args linked after it */

    while (Arg)
    {
        Arg->Parent = Op;
        Arg = Arg->Next;
    }
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiPsGetChild
 *
 * PARAMETERS:  Op              - Get the child of this Op
 *
 * RETURN:      Child Op, Null if none is found.
 *
 * DESCRIPTION: Get op's children or NULL if none
 *
 ******************************************************************************/

ACPI_GENERIC_OP *
AcpiPsGetChild (
    ACPI_GENERIC_OP         *Op)
{
    ACPI_GENERIC_OP         *Child = NULL;


    switch (Op->Opcode)
    {
    case AML_SCOPE_OP:
    case AML_ELSE_OP:
    case AML_DEVICE_OP:
    case AML_THERMAL_ZONE_OP:
    case AML_METHODCALL_OP:

        Child = AcpiPsGetArg (Op, 0);
        break;


    case AML_BUFFER_OP:
    case AML_PACKAGE_OP:
    case AML_METHOD_OP:
    case AML_IF_OP:
    case AML_WHILE_OP:
    case AML_DEF_FIELD_OP:

        Child = AcpiPsGetArg (Op, 1);
        break;


    case AML_POWER_RES_OP:
    case AML_INDEX_FIELD_OP:

        Child = AcpiPsGetArg (Op, 2);
        break;


    case AML_PROCESSOR_OP:
    case AML_BANK_FIELD_OP:

        Child = AcpiPsGetArg (Op, 3);
        break;

    }

    return Child;
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiPsGetDepthNext
 *
 * PARAMETERS:  Origin          - Root of subtree to search
 *              Op              - Last (previous) Op that was found
 *
 * RETURN:      Next Op found in the search.
 *
 * DESCRIPTION: Get next op in tree (walking the tree in depth-first order)
 *              Return NULL when reaching "origin" or when walking up from root
 *
 ******************************************************************************/

ACPI_GENERIC_OP *
AcpiPsGetDepthNext (
    ACPI_GENERIC_OP         *Origin,
    ACPI_GENERIC_OP         *Op)
{
    ACPI_GENERIC_OP         *Next = NULL;
    ACPI_GENERIC_OP         *Parent;
    ACPI_GENERIC_OP         *Arg;


    if (!Op)
    {
        return NULL;
    }

    /* look for an argument or child */

    Next = AcpiPsGetArg (Op, 0);
    if (Next)
    {
        return Next;
    }

    /* look for a sibling */

    Next = Op->Next;
    if (Next)
    {
        return Next;
    }

    /* look for a sibling of parent */

    Parent = Op->Parent;

    while (Parent)
    {
        Arg = AcpiPsGetArg (Parent, 0);
        while (Arg && (Arg != Origin) && (Arg != Op))
        {
            Arg = Arg->Next;
        }

        if (Arg == Origin)
        {
            /* reached parent of origin, end search */

            return NULL;
        }

        if (Parent->Next)
        {
            /* found sibling of parent */
            return Parent->Next;
        }

        Op = Parent;
        Parent = Parent->Parent;
    }

    return Next;
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiPsFetchPrefix
 *
 * PARAMETERS:  Scope           - Op to fetch prefix for
 *              Path            - A namestring containing the prefix
 *              io              - Direction flag
 *
 * RETURN:      Op referenced by the prefix
 *
 * DESCRIPTION: Fetch and handle path prefix ('\\' or '^')
 *
 ******************************************************************************/

ACPI_GENERIC_OP *
AcpiPsFetchPrefix (
    ACPI_GENERIC_OP         *Scope,
    char                    **Path,
    UINT32                  io)
{
    UINT32                  prefix = io ? GET8 (*Path):**Path;


    switch (prefix)
    {
    case '\\':
    case '/':

        /* go to the root */

        *Path += 1;
        while (Scope->Parent)
        {
            Scope = Scope->Parent;
        }
        break;


    case '^':

        /* go up one level */

        *Path += 1;
        Scope = Scope->Parent;
        break;
    }

    if (Scope && !Scope->Parent)
    {
        /* searching from the root, start with its children */

        Scope = AcpiPsGetChild (Scope);
    }

    return Scope;
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiPsFetchName
 *
 * PARAMETERS:  Path            - A string containing the name segment
 *              io              - Direction flag
 *
 * RETURN:      The 4-char ASCII ACPI Name as a UINT32
 *
 * DESCRIPTION: Fetch ACPI name segment (dot-delimited)
 *
 ******************************************************************************/

UINT32
AcpiPsFetchName (
    char                    **Path,
    UINT32                  io)
{
    UINT32                  Name = 0;
    char                    *nm;
    UINT32                  i;
    char                    ch;


    if (io)
    {
        /* Get the name from the path pointer */

        MOVE_UNALIGNED32_TO_32 (&Name, *Path);
        *Path += 4;
    }

    else
    {
        if (**Path == '.')
        {
            *Path += 1;
        }

        nm = (char*) &Name;
        for (i = 0; i < 4; i++)
        {
            ch = **Path;
            if (ch && ch != '.')
            {
                *nm = ch;
                *Path += 1;
            }

            else
            {
                *nm = '_';
            }
            nm++;
        }
    }

    return Name;
}


