/******************************************************************************
 *
 * Module Name: dbcmds - debug commands and output routines
 *              $Revision: 1.36 $
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
#include "acparser.h"
#include "acdispat.h"
#include "amlcode.h"
#include "acnamesp.h"
#include "acparser.h"
#include "acevents.h"
#include "acinterp.h"
#include "acdebug.h"
#include "actables.h"

#ifdef ENABLE_DEBUGGER

#define _COMPONENT          DEBUGGER
        MODULE_NAME         ("dbcmds")


/* These object types map directly to the ACPI_TYPES */


ARGUMENT_INFO               AcpiDbObjectTypes [] =
{
    {"ANY"},
    {"NUMBERS"},
    {"STRINGS"},
    {"BUFFERS"},
    {"PACKAGES"},
    {"FIELDS"},
    {"DEVICES"},
    {"EVENTS"},
    {"METHODS"},
    {"MUTEXES"},
    {"REGIONS"},
    {"POWERRESOURCES"},
    {"PROCESSORS"},
    {"THERMALZONES"},
    {"BUFFERFIELDS"},
    {"DDBHANDLES"},
    {NULL}           /* Must be null terminated */
};

/******************************************************************************
 *
 * FUNCTION:    AcpiDbWalkForReferences
 *
 * PARAMETERS:  Callback from WalkNamespace
 *
 * RETURN:      None
 *
 * DESCRIPTION: Called for a single method found in the namespace
 *
 *****************************************************************************/

ACPI_STATUS
AcpiDbWalkForReferences (
    ACPI_HANDLE             ObjHandle,
    UINT32                  NestingLevel,
    void                    *Context,
    void                    **ReturnValue)
{
    ACPI_OBJECT_INTERNAL    *ObjDesc = (ACPI_OBJECT_INTERNAL *) Context;
    ACPI_NAMED_OBJECT       *NameDesc = (ACPI_NAMED_OBJECT *) ObjHandle;


    if (NameDesc == (void *) ObjDesc)
    {
        AcpiOsPrintf ("Object is a Named Object [%4.4s]\n", &NameDesc->Name);
    }

    if (NameDesc->Object == ObjDesc)
    {
        AcpiOsPrintf ("Reference at NameDesc->Object %p [%4.4s]\n", NameDesc, &NameDesc->Name);
    }

    if (NameDesc->Child == (void *) ObjDesc)
    {
        AcpiOsPrintf ("Reference at NameDesc->Child %p [%4.4s]\n", NameDesc, &NameDesc->Name);
    }

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbFindReferences
 *
 * PARAMETERS:
 *
 * RETURN:
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

void
AcpiDbFindReferences (
    NATIVE_CHAR             *ObjectArg)
{
    ACPI_OBJECT_INTERNAL    *ObjDesc;


    ObjDesc = (ACPI_OBJECT_INTERNAL *) STRTOUL (ObjectArg, NULL, 16);

    AcpiWalkNamespace (ACPI_TYPE_ANY, ACPI_ROOT_OBJECT, ACPI_UINT32_MAX,
                    AcpiDbWalkForReferences, (void *) ObjDesc, NULL);


}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbDisplayTableInfo
 *
 * PARAMETERS:
 *
 * RETURN:
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

void
AcpiDbDisplayTableInfo (
    NATIVE_CHAR             *TableArg)
{
    UINT32                  i;


    for (i = 0; i < NUM_ACPI_TABLES; i++)
    {
        if (AcpiGbl_AcpiTables[i].Pointer)
        {
            AcpiOsPrintf ("%s at 0x%p length 0x%X\n", AcpiGbl_AcpiTableData[i].Name,
                        AcpiGbl_AcpiTables[i].Pointer, AcpiGbl_AcpiTables[i].Length);
        }
    }
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbUnloadAcpiTable
 *
 * PARAMETERS:
 *
 * RETURN:
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

void
AcpiDbUnloadAcpiTable (
    NATIVE_CHAR             *TableArg,
    NATIVE_CHAR             *InstanceArg)
{
    UINT32                  i;
    ACPI_STATUS             Status;


    for (i = 0; i < NUM_ACPI_TABLES; i++)
    {
        if (!STRNCMP (TableArg, AcpiGbl_AcpiTableData[i].Signature, AcpiGbl_AcpiTableData[i].SigLength))
        {
            Status = AcpiUnloadTable (i);
            if (ACPI_SUCCESS (Status))
            {
                AcpiOsPrintf ("[%s] unloaded and uninstalled\n", TableArg);
            }
            else
            {
                AcpiOsPrintf ("%s, while unloading [%s]\n", AcpiCmFormatException (Status), TableArg);
            }

            return;
        }
    }

    AcpiOsPrintf ("Unknown table type [%s]\n", TableArg);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbSetMethodBreakpoint
 *
 * PARAMETERS:  Location            - AML offset of breakpoint
 *              Op                  - Current Op (from parse walk)
 *
 * RETURN:      None
 *
 * DESCRIPTION: Set a breakpoint in a control method at the specified
 *              AML offset
 *
 *****************************************************************************/

void
AcpiDbSetMethodBreakpoint (
    NATIVE_CHAR             *Location,
    ACPI_WALK_STATE         *WalkState,
    ACPI_GENERIC_OP         *Op)
{
    UINT32                  Address;


    if (!Op)
    {
        AcpiOsPrintf ("There is no method currently executing\n");
        return;
    }

    /* Get and verify the breakpoint address */

    Address = STRTOUL (Location, NULL, 16);
    if (Address <= Op->AmlOffset)
    {
        AcpiOsPrintf ("Breakpoint 0x%X is beyond current address 0x%X\n", Address, Op->AmlOffset);
    }

    /* Just save the breakpoint in a global */

    WalkState->MethodBreakpoint = Address;
    AcpiOsPrintf ("Breakpoint set at AML offset 0x%X\n", Address);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbSetMethodCallBreakpoint
 *
 * PARAMETERS:  Location            - AML offset of breakpoint
 *              Op                  - Current Op (from parse walk)
 *
 * RETURN:      None
 *
 * DESCRIPTION: Set a breakpoint in a control method at the specified
 *              AML offset
 *
 *****************************************************************************/

void
AcpiDbSetMethodCallBreakpoint (
    ACPI_GENERIC_OP         *Op)
{


    if (!Op)
    {
        AcpiOsPrintf ("There is no method currently executing\n");
        return;
    }


    AcpiGbl_StepToNextCall = TRUE;
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbDisassembleAml
 *
 * PARAMETERS:  Statements          - Number of statements to disassemble
 *              Op                  - Current Op (from parse walk)
 *
 * RETURN:      None
 *
 * DESCRIPTION: Lookup a name in the ACPI namespace
 *
 *****************************************************************************/

void
AcpiDbDisassembleAml (
    NATIVE_CHAR             *Statements,
    ACPI_GENERIC_OP         *Op)
{
    UINT32                  NumStatements = 8;


    if (!Op)
    {
        AcpiOsPrintf ("There is no method currently executing\n");
        return;
    }

    if (Statements)
    {
        NumStatements = STRTOUL (Statements, NULL, 0);
    }


    AcpiDbDisplayOp (Op, NumStatements);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbDumpNamespace
 *
 * PARAMETERS:
 *
 * RETURN:      None
 *
 * DESCRIPTION: Dump entire namespace or a subtree
 *
 *****************************************************************************/

void
AcpiDbDumpNamespace (
    NATIVE_CHAR             *StartArg,
    NATIVE_CHAR             *DepthArg)
{
    ACPI_HANDLE             SubtreeEntry = AcpiGbl_RootObject;
    UINT32                  MaxDepth = ACPI_UINT32_MAX;


    /* No argument given, just start at the root and dump entire namespace */

    if (StartArg)
    {

        /* Check if numeric argument, must be a Named Object */

        if ((StartArg[0] >= 0x30) && (StartArg[0] <= 0x39))
        {
            SubtreeEntry = (ACPI_HANDLE) STRTOUL (StartArg, NULL, 16);
            if (!AcpiOsReadable (SubtreeEntry, sizeof (ACPI_NAMED_OBJECT)))
            {
                AcpiOsPrintf ("Address %p is invalid in this address space\n", SubtreeEntry);
                return;
            }
            if (!VALID_DESCRIPTOR_TYPE ((SubtreeEntry), ACPI_DESC_TYPE_NAMED))
            {
                AcpiOsPrintf ("Address %p is not a valid Named object\n", SubtreeEntry);
                return;
            }
        }

        /* Alpha argument */

        else
        {

            /* The parameter is a name string that must be resolved to a Named obj*/

            SubtreeEntry = AcpiDbLocalNsLookup (StartArg);
            if (!SubtreeEntry)
            {
                SubtreeEntry = AcpiGbl_RootObject;
            }
        }


        /* Now we can check for the depth argument */

        if (DepthArg)
        {
            MaxDepth = STRTOUL (DepthArg, NULL, 0);
        }
    }


    AcpiDbSetOutputDestination (DB_DUPLICATE_OUTPUT);
    AcpiOsPrintf ("ACPI Namespace (from %p subtree):\n", SubtreeEntry);

    /* Display the subtree */

    AcpiDbSetOutputDestination (DB_REDIRECTABLE_OUTPUT);
    AcpiNsDumpObjects (ACPI_TYPE_ANY, MaxDepth, ACPI_UINT32_MAX, SubtreeEntry);
    AcpiDbSetOutputDestination (DB_CONSOLE_OUTPUT);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbDumpNamespaceByOwner
 *
 * PARAMETERS:
 *
 * RETURN:      None
 *
 * DESCRIPTION: Dump entire namespace or a subtree
 *
 *****************************************************************************/

void
AcpiDbDumpNamespaceByOwner (
    NATIVE_CHAR             *OwnerArg,
    NATIVE_CHAR             *DepthArg)
{
    ACPI_HANDLE             SubtreeEntry = AcpiGbl_RootObject;
    UINT32                  MaxDepth = ACPI_UINT32_MAX;
    UINT16                  OwnerId;


    OwnerId = (UINT16) STRTOUL (OwnerArg, NULL, 0);


    /* Now we can check for the depth argument */

    if (DepthArg)
    {
        MaxDepth = STRTOUL (DepthArg, NULL, 0);
    }


    AcpiDbSetOutputDestination (DB_DUPLICATE_OUTPUT);
    AcpiOsPrintf ("ACPI Namespace by owner 0x%X:\n", OwnerId);

    /* Display the subtree */

    AcpiDbSetOutputDestination (DB_REDIRECTABLE_OUTPUT);
    AcpiNsDumpObjects (ACPI_TYPE_ANY, MaxDepth, OwnerId, SubtreeEntry);
    AcpiDbSetOutputDestination (DB_CONSOLE_OUTPUT);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbSendNotify
 *
 * PARAMETERS:
 *
 * RETURN:      None
 *
 * DESCRIPTION: Send an ACPI notificationn
 *
 *****************************************************************************/

void
AcpiDbSendNotify (
    NATIVE_CHAR             *Name,
    UINT32                  Value)
{
    ACPI_NAMED_OBJECT       *NameDesc;


    /* Translate name to an Named object */

    NameDesc = AcpiDbLocalNsLookup (Name);
    if (!NameDesc)
    {
        return;
    }

    /* Decode Named object type */

    switch (NameDesc->Type)
    {
    case ACPI_TYPE_DEVICE:
    case ACPI_TYPE_THERMAL:

         /* Send the notify */

        AcpiEvNotifyDispatch (NameDesc, Value);
        break;

    default:
        AcpiOsPrintf ("Named object is not a device or a thermal object\n");
        break;
    }

}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbSetMethodData
 *
 * PARAMETERS:
 *
 * RETURN:      None
 *
 * DESCRIPTION: Set control method local or argument
 *
 *****************************************************************************/

void
AcpiDbSetMethodData (
    NATIVE_CHAR             *TypeArg,
    NATIVE_CHAR             *IndexArg,
    NATIVE_CHAR             *ValueArg)
{
    NATIVE_CHAR             Type;
    UINT32                  Index;
    UINT32                  Value;
    ACPI_WALK_STATE         *WalkState;
    ACPI_OBJECT_INTERNAL    *ObjDesc;


    STRUPR (TypeArg);
    Type = TypeArg[0];
    if ((Type != 'L') &&
        (Type != 'A'))
    {
        AcpiOsPrintf ("Invalid SET operand: %s\n", TypeArg);
        return;
    }

    Index = STRTOUL (IndexArg, NULL, 16);
    Value = STRTOUL (ValueArg, NULL, 16);

    WalkState = AcpiDsGetCurrentWalkState (AcpiGbl_CurrentWalkList);
    if (!WalkState)
    {
        AcpiOsPrintf ("There is no method currently executing\n");
        return;
    }


    ObjDesc = AcpiCmCreateInternalObject (ACPI_TYPE_NUMBER);
    if (!ObjDesc)
    {
        AcpiOsPrintf ("Could not create an internal object\n");
        return;
    }

    ObjDesc->Number.Value = Value;

    switch (Type)
    {
    case 'A':

        /* Set a method argument */

        if (Index > MTH_NUM_ARGS)
        {
            AcpiOsPrintf ("Arg%d - Invalid argument name\n", Index);
            return;
        }

        AcpiDsMethodDataSetValue (MTH_TYPE_ARG, Index, ObjDesc, WalkState);
        ObjDesc = WalkState->Arguments[Index].Object;
        AcpiOsPrintf ("Arg%d: ", Index);
        AcpiDbDisplayInternalObject (ObjDesc, WalkState);
        break;

    case 'L':

        /* Set a method local */

        if (Index > MTH_NUM_LOCALS)
        {
            AcpiOsPrintf ("Local%d - Invalid local variable name\n", Index);
            return;
        }

        AcpiDsMethodDataSetValue (MTH_TYPE_LOCAL, Index, ObjDesc, WalkState);
        ObjDesc = WalkState->LocalVariables[Index].Object;
        AcpiOsPrintf ("Local%d: ", Index);
        AcpiDbDisplayInternalObject (ObjDesc, WalkState);
        break;

    default:
        break;
    }
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbWalkMethods
 *
 * PARAMETERS:  Callback from WalkNamespace
 *
 * RETURN:      None
 *
 * DESCRIPTION: Called for a single method found in the namespace
 *
 *****************************************************************************/

ACPI_STATUS
AcpiDbWalkForSpecificObjects (
    ACPI_HANDLE             ObjHandle,
    UINT32                  NestingLevel,
    void                    *Context,
    void                    **ReturnValue)
{
    ACPI_OBJECT_INTERNAL    *ObjDesc;
    ACPI_STATUS             Status;
    UINT32                  BufSize;
    NATIVE_CHAR             buffer[64];


    ObjDesc = ((ACPI_NAMED_OBJECT*)ObjHandle)->Object;

    /* Get the full pathname to this method */

    Status = AcpiNsHandleToPathname (ObjHandle, &BufSize, buffer);

    if (ACPI_FAILURE (Status))
    {
        AcpiOsPrintf ("Could Not get pathname for object %p\n", ObjHandle);
        return (AE_OK);
    }

    AcpiOsPrintf ("%32s", buffer);

    if (ObjDesc)
    {
        switch (ObjDesc->Common.Type)
        {
        case ACPI_TYPE_METHOD:
            AcpiOsPrintf ("  #Args %d  Concurrency %d", ObjDesc->Method.ParamCount, ObjDesc->Method.Concurrency);
            break;

        case ACPI_TYPE_NUMBER:
            AcpiOsPrintf ("  Value 0x%X", ObjDesc->Number.Value);
            break;

        case ACPI_TYPE_STRING:
            AcpiOsPrintf ("  \"%s\"", ObjDesc->String.Pointer);
            break;

        case ACPI_TYPE_REGION:
            AcpiOsPrintf ("  SpaceId %d Address %X Length %X", ObjDesc->Region.SpaceId, ObjDesc->Region.Address, ObjDesc->Region.Length);
            break;

        case ACPI_TYPE_PACKAGE:
            AcpiOsPrintf ("  #Elements %d", ObjDesc->Package.Count);
            break;

        case ACPI_TYPE_BUFFER:
            AcpiOsPrintf ("  Length %d", ObjDesc->Buffer.Length);
            break;
        }
    }

    AcpiOsPrintf ("\n");
    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbDisplayObjects
 *
 * PARAMETERS:  ObjTypeArg          - Type of object to display
 *
 * RETURN:      None
 *
 * DESCRIPTION: Display objects in the namespace of the requested type
 *
 *****************************************************************************/

ACPI_STATUS
AcpiDbDisplayObjects (
    NATIVE_CHAR             *ObjTypeArg,
    NATIVE_CHAR             *DisplayCountArg)
{
    UINT32                  DisplayCount;
    OBJECT_TYPE_INTERNAL    Type;


    STRUPR (ObjTypeArg);
    Type = AcpiDbMatchArgument (ObjTypeArg, AcpiDbObjectTypes);
    if (Type == ACPI_TYPE_NOT_FOUND)
    {
        AcpiOsPrintf ("Invalid or unsupported argument\n");
        return (AE_OK);
    }


    if (DisplayCountArg)
    {
        DisplayCount = STRTOUL (DisplayCountArg, NULL, 0);
    }

    else
    {
        DisplayCount = ACPI_UINT32_MAX;
    }

    AcpiDbSetOutputDestination (DB_DUPLICATE_OUTPUT);
    AcpiOsPrintf ("Objects of type [%s] defined in the current ACPI Namespace: \n", AcpiCmGetTypeName (Type));

    AcpiDbSetOutputDestination (DB_REDIRECTABLE_OUTPUT);

    AcpiWalkNamespace (Type, ACPI_ROOT_OBJECT, ACPI_UINT32_MAX,
                        AcpiDbWalkForSpecificObjects, (void *) &Type, NULL);

    AcpiDbSetOutputDestination (DB_CONSOLE_OUTPUT);
    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbWalkAndMatchName
 *
 * PARAMETERS:
 *
 * RETURN:      None
 *
 * DESCRIPTION:
 *
 *****************************************************************************/

ACPI_STATUS
AcpiDbWalkAndMatchName (
    ACPI_HANDLE             ObjHandle,
    UINT32                  NestingLevel,
    void                    *Context,
    void                    **ReturnValue)
{
    ACPI_OBJECT_INTERNAL    *ObjDesc;
    ACPI_STATUS             Status;
    NATIVE_CHAR             *RequestedName = (NATIVE_CHAR *) Context;
    UINT32                  i;
    UINT32                  BufSize;
    NATIVE_CHAR             Buffer[96];


    ObjDesc = ((ACPI_NAMED_OBJECT*)ObjHandle)->Object;


    /* Check for a name match */

    for (i = 0; i < 4; i++)
    {
        /* Wildcard support */

        if ((RequestedName[i] != '?') &&
            (RequestedName[i] != ((NATIVE_CHAR *)(&((ACPI_NAMED_OBJECT*)ObjHandle)->Name))[i]))
        {
            /* No match, just exit */

            return (AE_OK);
        }
    }


    /* Get the full pathname to this object */

    BufSize = sizeof (Buffer);

    Status = AcpiNsHandleToPathname (ObjHandle, &BufSize, Buffer);
    if (ACPI_FAILURE (Status))
    {
        AcpiOsPrintf ("Could Not get pathname for object %p\n", ObjHandle);
    }

    else
    {
        AcpiOsPrintf ("%32s (0x%p) - %s\n", Buffer, ObjHandle, AcpiCmGetTypeName (((ACPI_NAMED_OBJECT*)ObjHandle)->Type));
    }

    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbFindNameInNamespace
 *
 * PARAMETERS:
 *
 * RETURN:      None
 *
 * DESCRIPTION: Search the namespace for a given name (with wildcards)
 *
 *****************************************************************************/

ACPI_STATUS
AcpiDbFindNameInNamespace (
    NATIVE_CHAR             *NameArg)
{

    if (STRLEN (NameArg) > 4)
    {
        AcpiOsPrintf ("Name must be no longer than 4 characters\n");
        return (AE_OK);
    }

    AcpiWalkNamespace (ACPI_TYPE_ANY, ACPI_ROOT_OBJECT, ACPI_UINT32_MAX,
                        AcpiDbWalkAndMatchName, NameArg, NULL);

    AcpiDbSetOutputDestination (DB_CONSOLE_OUTPUT);
    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiDbSetScope
 *
 * PARAMETERS:  Name                - New scope path
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Set the "current scope" as maintained by this utility.
 *              The scope is used as a prefix to ACPI paths.
 *
 *****************************************************************************/

void
AcpiDbSetScope (
    NATIVE_CHAR             *Name)
{

    if (!Name || Name[0] == 0)
    {
        AcpiOsPrintf ("Current scope: %s\n", ScopeBuf);
        return;
    }

    AcpiDbPrepNamestring (Name);

    /* TBD: [Future] Validate scope here */

    if (Name[0] == '\\')
    {
        STRCPY (ScopeBuf, Name);
        STRCAT (ScopeBuf, "\\");
    }

    else
    {
        STRCAT (ScopeBuf, Name);
        STRCAT (ScopeBuf, "\\");
    }

    AcpiOsPrintf ("New scope: %s\n", ScopeBuf);
}

#endif /* ENABLE_DEBUGGER */
