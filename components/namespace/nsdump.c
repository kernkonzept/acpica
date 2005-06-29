/******************************************************************************
 *
 * Module Name: nsdump - table dumping routines for debug
 *              $Revision: 1.121 $
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

#define __NSDUMP_C__

#include "acpi.h"
#include "acinterp.h"
#include "acnamesp.h"
#include "actables.h"
#include "acparser.h"


#define _COMPONENT          ACPI_NAMESPACE
        MODULE_NAME         ("nsdump")

#if defined(ACPI_DEBUG) || defined(ENABLE_DEBUGGER)


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsPrintPathname
 *
 * PARAMETERS:  NumSegment          - Number of ACPI name segments
 *              Pathname            - The compressed (internal) path
 *
 * DESCRIPTION: Print an object's full namespace pathname
 *
 ******************************************************************************/

void
AcpiNsPrintPathname (
    UINT32                  NumSegments,
    char                    *Pathname)
{
    PROC_NAME ("AcpiNsPrintPathname");


    if (!(AcpiDbgLevel & ACPI_LV_NAMES) || !(AcpiDbgLayer & ACPI_NAMESPACE))
    {
        return;
    }

        /* Print the entire name */

    ACPI_DEBUG_PRINT ((ACPI_DB_NAMES, "["));

    while (NumSegments)
    {
        AcpiOsPrintf ("%4.4s", Pathname);
        Pathname += ACPI_NAME_SIZE;

        NumSegments--;
        if (NumSegments)
        {
            AcpiOsPrintf (".");
        }
    }

    AcpiOsPrintf ("]\n");
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsDumpPathname
 *
 * PARAMETERS:  Handle              - Object
 *              Msg                 - Prefix message
 *              Level               - Desired debug level
 *              Component           - Caller's component ID
 *
 * DESCRIPTION: Print an object's full namespace pathname
 *              Manages allocation/freeing of a pathname buffer
 *
 ******************************************************************************/

ACPI_STATUS
AcpiNsDumpPathname (
    ACPI_HANDLE             Handle,
    NATIVE_CHAR             *Msg,
    UINT32                  Level,
    UINT32                  Component)
{
    NATIVE_CHAR             *Buffer;
    ACPI_SIZE               Length;


    FUNCTION_TRACE ("NsDumpPathname");


    /* Do this only if the requested debug level and component are enabled */

    if (!(AcpiDbgLevel & Level) || !(AcpiDbgLayer & Component))
    {
        return_ACPI_STATUS (AE_OK);
    }

    Buffer = ACPI_MEM_ALLOCATE (PATHNAME_MAX);
    if (!Buffer)
    {
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    /* Convert handle to a full pathname and print it (with supplied message) */

    Length = PATHNAME_MAX;
    if (ACPI_SUCCESS (AcpiNsHandleToPathname (Handle, &Length, Buffer)))
    {
        AcpiOsPrintf ("%s %s (Node %p)\n", Msg, Buffer, Handle);
    }

    ACPI_MEM_FREE (Buffer);

    return_ACPI_STATUS (AE_OK);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsDumpOneObject
 *
 * PARAMETERS:  Handle              - Node to be dumped
 *              Level               - Nesting level of the handle
 *              Context             - Passed into WalkNamespace
 *
 * DESCRIPTION: Dump a single Node
 *              This procedure is a UserFunction called by AcpiNsWalkNamespace.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiNsDumpOneObject (
    ACPI_HANDLE             ObjHandle,
    UINT32                  Level,
    void                    *Context,
    void                    **ReturnValue)
{
    ACPI_WALK_INFO          *Info = (ACPI_WALK_INFO *) Context;
    ACPI_NAMESPACE_NODE     *ThisNode;
    ACPI_OPERAND_OBJECT     *ObjDesc = NULL;
    ACPI_OBJECT_TYPE        ObjType;
    ACPI_OBJECT_TYPE        Type;
    UINT32                  BytesToDump;
    UINT32                  DownstreamSiblingMask = 0;
    UINT32                  LevelTmp;
    UINT32                  WhichBit;
    UINT32                  i;
    UINT32                  DbgLevel;


    PROC_NAME ("NsDumpOneObject");


    ThisNode = AcpiNsMapHandleToNode (ObjHandle);

    LevelTmp    = Level;
    Type        = ThisNode->Type;
    WhichBit    = 1;


    if (!(AcpiDbgLevel & Info->DebugLevel))
    {
        return (AE_OK);
    }

    if (!((ACPI_LV_TABLES & AcpiDbgLevel) && (_COMPONENT & AcpiDbgLayer)))
    {
        return (AE_OK);
    }

    if (!ObjHandle)
    {
        ACPI_DEBUG_PRINT ((ACPI_DB_INFO, "Null object handle\n"));
        return (AE_OK);
    }

    /* Check if the owner matches */

    if ((Info->OwnerId != ACPI_UINT32_MAX) &&
        (Info->OwnerId != ThisNode->OwnerId))
    {
        return (AE_OK);
    }

    /* Indent the object according to the level */

    while (LevelTmp--)
    {
        /* Print appropriate characters to form tree structure */

        if (LevelTmp)
        {
            if (DownstreamSiblingMask & WhichBit)
            {
                AcpiOsPrintf ("|");
            }
            else
            {
                AcpiOsPrintf (" ");
            }

            WhichBit <<= 1;
        }
        else
        {
            if (AcpiNsExistDownstreamSibling (ThisNode + 1))
            {
                DownstreamSiblingMask |= (1 << (Level - 1));
                AcpiOsPrintf ("+");
            }
            else
            {
                DownstreamSiblingMask &= ACPI_UINT32_MAX ^ (1 << (Level - 1));
                AcpiOsPrintf ("+");
            }

            if (ThisNode->Child == NULL)
            {
                AcpiOsPrintf ("-");
            }
            else if (AcpiNsExistDownstreamSibling (ThisNode->Child))
            {
                AcpiOsPrintf ("+");
            }
            else
            {
                AcpiOsPrintf ("-");
            }
        }
    }

    /* Check the integrity of our data */

    if (Type > INTERNAL_TYPE_MAX)
    {
        Type = INTERNAL_TYPE_DEF_ANY;  /* prints as *ERROR* */
    }

    if (!AcpiUtValidAcpiName (ThisNode->Name))
    {
        REPORT_WARNING (("Invalid ACPI Name %08X\n", ThisNode->Name));
    }

    /*
     * Now we can print out the pertinent information
     */
    AcpiOsPrintf (" %4.4s %-12s %p",
            (char *) &ThisNode->Name, AcpiUtGetTypeName (Type), ThisNode);

    DbgLevel = AcpiDbgLevel;
    AcpiDbgLevel = 0;
    ObjDesc = AcpiNsGetAttachedObject (ThisNode);
    AcpiDbgLevel = DbgLevel;

    switch (Info->DisplayType)
    {
    case ACPI_DISPLAY_SUMMARY:

        if (!ObjDesc)
        {
            /* No attached object, we are done */

            AcpiOsPrintf ("\n");
            return (AE_OK);
        }

        switch (Type)
        {
        case ACPI_TYPE_PROCESSOR:
            AcpiOsPrintf (" ID %d Addr %.4X Len %.4X\n",
                        ObjDesc->Processor.ProcId,
                        ObjDesc->Processor.Address,
                        ObjDesc->Processor.Length);
            break;

        case ACPI_TYPE_DEVICE:
            AcpiOsPrintf (" Notification object: %p", ObjDesc);
            break;

        case ACPI_TYPE_METHOD:
            AcpiOsPrintf (" Args %d Len %.4X Aml %p \n",
                        ObjDesc->Method.ParamCount,
                        ObjDesc->Method.AmlLength,
                        ObjDesc->Method.AmlStart);
            break;

        case ACPI_TYPE_INTEGER:
            AcpiOsPrintf (" = %8.8X%8.8X\n",
                        HIDWORD (ObjDesc->Integer.Value),
                        LODWORD (ObjDesc->Integer.Value));
            break;

        case ACPI_TYPE_PACKAGE:
            AcpiOsPrintf (" Elements %.2X\n",
                        ObjDesc->Package.Count);
            break;

        case ACPI_TYPE_BUFFER:
            AcpiOsPrintf (" Len %.2X",
                        ObjDesc->Buffer.Length);

            /* Dump some of the buffer */

            if (ObjDesc->Buffer.Length > 0)
            {
                AcpiOsPrintf (" =");
                for (i = 0; (i < ObjDesc->Buffer.Length && i < 12); i++)
                {
                    AcpiOsPrintf (" %.2X", ObjDesc->Buffer.Pointer[i]);
                }
            }
            AcpiOsPrintf ("\n");
            break;

        case ACPI_TYPE_STRING:
            AcpiOsPrintf (" Len %.2X", ObjDesc->String.Length);

            if (ObjDesc->String.Length > 0)
            {
                 AcpiOsPrintf (" = \"%.32s\"...", ObjDesc->String.Pointer);
            }
            AcpiOsPrintf ("\n");
            break;

        case ACPI_TYPE_REGION:
            AcpiOsPrintf (" [%s]", AcpiUtGetRegionName (ObjDesc->Region.SpaceId));
            if (ObjDesc->Region.Flags & AOPOBJ_DATA_VALID)
            {
                AcpiOsPrintf (" Addr %8.8X%8.8X Len %.4X\n",
                            HIDWORD(ObjDesc->Region.Address),
                            LODWORD(ObjDesc->Region.Address),
                            ObjDesc->Region.Length);
            }
            else
            {
                AcpiOsPrintf (" [Address/Length not evaluated]\n");
            }
            break;

        case INTERNAL_TYPE_REFERENCE:
            AcpiOsPrintf (" [%s]\n",
                    AcpiPsGetOpcodeName (ObjDesc->Reference.Opcode));
            break;

        case ACPI_TYPE_BUFFER_FIELD:
            if (ObjDesc->BufferField.BufferObj &&
                ObjDesc->BufferField.BufferObj->Buffer.Node)
            {
                AcpiOsPrintf (" Buf [%4.4s]",
                        (char *) &ObjDesc->BufferField.BufferObj->Buffer.Node->Name);
            }
            break;

        case INTERNAL_TYPE_REGION_FIELD:
            AcpiOsPrintf (" Rgn [%4.4s]",
                    (char *) &ObjDesc->CommonField.RegionObj->Region.Node->Name);
            break;

        case INTERNAL_TYPE_BANK_FIELD:
            AcpiOsPrintf (" Rgn [%4.4s] Bnk [%4.4s]",
                    (char *) &ObjDesc->CommonField.RegionObj->Region.Node->Name,
                    (char *) &ObjDesc->BankField.BankObj->CommonField.Node->Name);
            break;

        case INTERNAL_TYPE_INDEX_FIELD:
            AcpiOsPrintf (" Idx [%4.4s] Dat [%4.4s]",
                    (char *) &ObjDesc->IndexField.IndexObj->CommonField.Node->Name,
                    (char *) &ObjDesc->IndexField.DataObj->CommonField.Node->Name);
            break;

        default:
            AcpiOsPrintf (" Object %p\n", ObjDesc);
            break;
        }

        /* Common field handling */

        switch (Type)
        {
        case ACPI_TYPE_BUFFER_FIELD:
        case INTERNAL_TYPE_REGION_FIELD:
        case INTERNAL_TYPE_BANK_FIELD:
        case INTERNAL_TYPE_INDEX_FIELD:
            AcpiOsPrintf (" Off %.2X Len %.2X Acc %.2d\n",
                    (ObjDesc->CommonField.BaseByteOffset * 8)
                        + ObjDesc->CommonField.StartFieldBitOffset,
                    ObjDesc->CommonField.BitLength,
                    ObjDesc->CommonField.AccessByteWidth);
            break;
        }

        break;


    case ACPI_DISPLAY_OBJECTS:

        AcpiOsPrintf ("%p O:%p",
                ThisNode, ObjDesc);

        if (!ObjDesc)
        {
            /* No attached object, we are done */

            AcpiOsPrintf ("\n");
            return (AE_OK);
        }

        AcpiOsPrintf ("(R%d)",
                ObjDesc->Common.ReferenceCount);

        switch (Type)
        {
        case ACPI_TYPE_METHOD:

            /* Name is a Method and its AML offset/length are set */

            AcpiOsPrintf (" M:%p-%X\n", ObjDesc->Method.AmlStart,
                                        ObjDesc->Method.AmlLength);
            break;

        case ACPI_TYPE_INTEGER:

            AcpiOsPrintf (" N:%X%X\n", HIDWORD(ObjDesc->Integer.Value),
                                       LODWORD(ObjDesc->Integer.Value));
            break;

        case ACPI_TYPE_STRING:

            AcpiOsPrintf (" S:%p-%X\n", ObjDesc->String.Pointer,
                                        ObjDesc->String.Length);
            break;

        case ACPI_TYPE_BUFFER:

            AcpiOsPrintf (" B:%p-%X\n", ObjDesc->Buffer.Pointer,
                                        ObjDesc->Buffer.Length);
            break;

        default:

            AcpiOsPrintf ("\n");
            break;
        }
        break;


    default:
        AcpiOsPrintf ("\n");
        break;
    }

    /* If debug turned off, done */

    if (!(AcpiDbgLevel & ACPI_LV_VALUES))
    {
        return (AE_OK);
    }


    /* If there is an attached object, display it */

    DbgLevel = AcpiDbgLevel;
    AcpiDbgLevel = 0;
    ObjDesc = AcpiNsGetAttachedObject (ThisNode);
    AcpiDbgLevel = DbgLevel;

    /* Dump attached objects */

    while (ObjDesc)
    {
        ObjType = INTERNAL_TYPE_INVALID;
        AcpiOsPrintf ("        Attached Object %p: ", ObjDesc);

        /* Decode the type of attached object and dump the contents */

        switch (ACPI_GET_DESCRIPTOR_TYPE (ObjDesc))
        {
        case ACPI_DESC_TYPE_NAMED:

            AcpiOsPrintf ("(Ptr to Node)\n");
            BytesToDump = sizeof (ACPI_NAMESPACE_NODE);
            break;


        case ACPI_DESC_TYPE_INTERNAL:

            ObjType = ObjDesc->Common.Type;

            if (ObjType > INTERNAL_TYPE_MAX)
            {
                AcpiOsPrintf ("(Ptr to ACPI Object type %X [UNKNOWN])\n", ObjType);
                BytesToDump = 32;
            }
            else
            {
                AcpiOsPrintf ("(Ptr to ACPI Object type %2.2X [%s])\n",
                                    ObjType, AcpiUtGetTypeName (ObjType));
                BytesToDump = sizeof (ACPI_OPERAND_OBJECT);
            }
            break;


        default:

            AcpiOsPrintf ("(String or Buffer ptr - not an object descriptor)\n");
            BytesToDump = 16;
            break;
        }

        DUMP_BUFFER (ObjDesc, BytesToDump);

        /* If value is NOT an internal object, we are done */

        if (ACPI_GET_DESCRIPTOR_TYPE (ObjDesc) != ACPI_DESC_TYPE_INTERNAL)
        {
            goto Cleanup;
        }

        /*
         * Valid object, get the pointer to next level, if any
         */
        switch (ObjType)
        {
        case ACPI_TYPE_STRING:
            ObjDesc = (ACPI_OPERAND_OBJECT *) ObjDesc->String.Pointer;
            break;

        case ACPI_TYPE_BUFFER:
            ObjDesc = (ACPI_OPERAND_OBJECT *) ObjDesc->Buffer.Pointer;
            break;

        case ACPI_TYPE_BUFFER_FIELD:
            ObjDesc = (ACPI_OPERAND_OBJECT *) ObjDesc->BufferField.BufferObj;
            break;

        case ACPI_TYPE_PACKAGE:
            ObjDesc = (ACPI_OPERAND_OBJECT *) ObjDesc->Package.Elements;
            break;

        case ACPI_TYPE_METHOD:
            ObjDesc = (ACPI_OPERAND_OBJECT *) ObjDesc->Method.AmlStart;
            break;

        case INTERNAL_TYPE_REGION_FIELD:
            ObjDesc = (ACPI_OPERAND_OBJECT *) ObjDesc->Field.RegionObj;
            break;

        case INTERNAL_TYPE_BANK_FIELD:
            ObjDesc = (ACPI_OPERAND_OBJECT *) ObjDesc->BankField.RegionObj;
            break;

        case INTERNAL_TYPE_INDEX_FIELD:
            ObjDesc = (ACPI_OPERAND_OBJECT *) ObjDesc->IndexField.IndexObj;
            break;

       default:
            goto Cleanup;
        }

        ObjType = INTERNAL_TYPE_INVALID;   /* Terminate loop after next pass */
    }

Cleanup:
    AcpiOsPrintf ("\n");
    return (AE_OK);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsDumpObjects
 *
 * PARAMETERS:  Type                - Object type to be dumped
 *              MaxDepth            - Maximum depth of dump.  Use ACPI_UINT32_MAX
 *                                    for an effectively unlimited depth.
 *              OwnerId             - Dump only objects owned by this ID.  Use
 *                                    ACPI_UINT32_MAX to match all owners.
 *              StartHandle         - Where in namespace to start/end search
 *
 * DESCRIPTION: Dump typed objects within the loaded namespace.
 *              Uses AcpiNsWalkNamespace in conjunction with AcpiNsDumpOneObject.
 *
 ******************************************************************************/

void
AcpiNsDumpObjects (
    ACPI_OBJECT_TYPE        Type,
    UINT8                   DisplayType,
    UINT32                  MaxDepth,
    UINT32                  OwnerId,
    ACPI_HANDLE             StartHandle)
{
    ACPI_WALK_INFO          Info;


    FUNCTION_ENTRY ();


    Info.DebugLevel = ACPI_LV_TABLES;
    Info.OwnerId = OwnerId;
    Info.DisplayType = DisplayType;


    AcpiNsWalkNamespace (Type, StartHandle, MaxDepth, NS_WALK_NO_UNLOCK, AcpiNsDumpOneObject,
                        (void *) &Info, NULL);
}


#ifndef _ACPI_ASL_COMPILER
/*******************************************************************************
 *
 * FUNCTION:    AcpiNsDumpOneDevice
 *
 * PARAMETERS:  Handle              - Node to be dumped
 *              Level               - Nesting level of the handle
 *              Context             - Passed into WalkNamespace
 *
 * DESCRIPTION: Dump a single Node that represents a device
 *              This procedure is a UserFunction called by AcpiNsWalkNamespace.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiNsDumpOneDevice (
    ACPI_HANDLE             ObjHandle,
    UINT32                  Level,
    void                    *Context,
    void                    **ReturnValue)
{
    ACPI_DEVICE_INFO        Info;
    ACPI_STATUS             Status;
    UINT32                  i;


    PROC_NAME ("NsDumpOneDevice");


    Status = AcpiNsDumpOneObject (ObjHandle, Level, Context, ReturnValue);

    Status = AcpiGetObjectInfo (ObjHandle, &Info);
    if (ACPI_SUCCESS (Status))
    {
        for (i = 0; i < Level; i++)
        {
            ACPI_DEBUG_PRINT_RAW ((ACPI_DB_TABLES, " "));
        }

        ACPI_DEBUG_PRINT_RAW ((ACPI_DB_TABLES, "    HID: %s, ADR: %8.8X%8.8X, Status: %x\n",
                        Info.HardwareId, HIDWORD(Info.Address), LODWORD(Info.Address), Info.CurrentStatus));
    }

    return (Status);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsDumpRootDevices
 *
 * PARAMETERS:  None
 *
 * DESCRIPTION: Dump all objects of type "device"
 *
 ******************************************************************************/

void
AcpiNsDumpRootDevices (void)
{
    ACPI_HANDLE             SysBusHandle;


    PROC_NAME ("NsDumpRootDevices");


    /* Only dump the table if tracing is enabled */

    if (!(ACPI_LV_TABLES & AcpiDbgLevel))
    {
        return;
    }

    AcpiGetHandle (0, NS_SYSTEM_BUS, &SysBusHandle);

    ACPI_DEBUG_PRINT ((ACPI_DB_TABLES, "Display of all devices in the namespace:\n"));
    AcpiNsWalkNamespace (ACPI_TYPE_DEVICE, SysBusHandle, ACPI_UINT32_MAX, NS_WALK_NO_UNLOCK,
                        AcpiNsDumpOneDevice, NULL, NULL);
}

#endif

/*******************************************************************************
 *
 * FUNCTION:    AcpiNsDumpTables
 *
 * PARAMETERS:  SearchBase          - Root of subtree to be dumped, or
 *                                    NS_ALL to dump the entire namespace
 *              MaxDepth            - Maximum depth of dump.  Use INT_MAX
 *                                    for an effectively unlimited depth.
 *
 * DESCRIPTION: Dump the name space, or a portion of it.
 *
 ******************************************************************************/

void
AcpiNsDumpTables (
    ACPI_HANDLE             SearchBase,
    UINT32                  MaxDepth)
{
    ACPI_HANDLE             SearchHandle = SearchBase;


    FUNCTION_TRACE ("NsDumpTables");


    if (!AcpiGbl_RootNode)
    {
        /*
         * If the name space has not been initialized,
         * there is nothing to dump.
         */
        ACPI_DEBUG_PRINT ((ACPI_DB_TABLES, "name space not initialized!\n"));
        return_VOID;
    }

    if (NS_ALL == SearchBase)
    {
        /*  entire namespace    */

        SearchHandle = AcpiGbl_RootNode;
        ACPI_DEBUG_PRINT ((ACPI_DB_TABLES, "\\\n"));
    }


    AcpiNsDumpObjects (ACPI_TYPE_ANY, ACPI_DISPLAY_OBJECTS, MaxDepth,
            ACPI_UINT32_MAX, SearchHandle);
    return_VOID;
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiNsDumpEntry
 *
 * PARAMETERS:  Handle              - Node to be dumped
 *              DebugLevel          - Output level
 *
 * DESCRIPTION: Dump a single Node
 *
 ******************************************************************************/

void
AcpiNsDumpEntry (
    ACPI_HANDLE             Handle,
    UINT32                  DebugLevel)
{
    ACPI_WALK_INFO          Info;


    FUNCTION_ENTRY ();


    Info.DebugLevel = DebugLevel;
    Info.OwnerId = ACPI_UINT32_MAX;
    Info.DisplayType = ACPI_DISPLAY_SUMMARY;

    AcpiNsDumpOneObject (Handle, 1, &Info, NULL);
}

#endif

