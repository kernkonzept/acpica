
/******************************************************************************
 *
 * Module Name: cmdelete - object deletion utilities
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

#define __CMDELETE_C__

#include "acpi.h"
#include "interp.h"
#include "namesp.h"
#include "tables.h"
#include "parser.h"

#define _COMPONENT          MISCELLANEOUS
        MODULE_NAME         ("cmdelete");



/******************************************************************************
 *
 * FUNCTION:    AcpiCmDeleteInternalObj
 *
 * PARAMETERS:  *Object        - Pointer to the list to be deleted
 *
 * RETURN:      None
 *
 * DESCRIPTION: Low level object deletion, after reference counts have been
 *              updated (All reference counts, including sub-objects!)
 *
 ******************************************************************************/

void
AcpiCmDeleteInternalObj (
    ACPI_OBJECT_INTERNAL    *Object)
{
    void                    *ObjPointer = NULL;


    FUNCTION_TRACE_PTR ("CmDeleteInternalObj", Object);


    if (!Object)
    {
        return_VOID;
    }

    /*
     * Must delete or free any pointers within the object that are not
     * actual ACPI objects (for example, a raw buffer pointer).
     */

    switch (Object->Common.Type)
    {

    case ACPI_TYPE_STRING:

        DEBUG_PRINT (ACPI_INFO, ("CmDeleteInternalObj: **** String %p, ptr %p\n",
                                Object, Object->String.Pointer));

        /* Free the actual string buffer */

        ObjPointer = Object->String.Pointer;
        break;


    case ACPI_TYPE_BUFFER:

        DEBUG_PRINT (ACPI_INFO, ("CmDeleteInternalObj: **** Buffer %p, ptr %p\n",
                                Object, Object->Buffer.Pointer));

        /* Free the actual buffer */

        ObjPointer = Object->Buffer.Pointer;
        break;


    case ACPI_TYPE_PACKAGE:

        DEBUG_PRINT (ACPI_INFO, ("CmDeleteInternalObj: **** Package of count %d\n",
                                Object->Package.Count));

        /* Elements of the package are not handled here, they are deleted separately */

        /* Free the (variable length) element pointer array */

        ObjPointer = Object->Package.Elements;
        break;


    case ACPI_TYPE_MUTEX:

        DEBUG_PRINT (ACPI_INFO, ("CmDeleteInternalObj: ***** Mutex %p, Semaphore %p\n",
                                Object, Object->Mutex.Semaphore));

        AcpiOsdDeleteSemaphore (Object->Mutex.Semaphore);
        break;


    case ACPI_TYPE_EVENT:

        DEBUG_PRINT (ACPI_INFO, ("CmDeleteInternalObj: ***** AcpiEvent %p, Semaphore %p\n",
                                Object, Object->Event.Semaphore));

        AcpiOsdDeleteSemaphore (Object->Event.Semaphore);
        Object->Event.Semaphore = NULL;
        break;


    case ACPI_TYPE_METHOD:

        DEBUG_PRINT (ACPI_INFO, ("CmDeleteInternalObj: ***** Method %p, ParserOp %p\n",
                                Object, Object->Method.ParserOp));

        /* Delete parse tree if it exists */

        if (Object->Method.ParserOp)
        {
            AcpiPsDeleteParseTree (Object->Method.ParserOp);
            Object->Method.ParserOp = NULL;
        }

        /* Delete semaphore if it exists */

        if (Object->Method.Semaphore)
        {
            AcpiOsdDeleteSemaphore (Object->Method.Semaphore);
            Object->Method.Semaphore = NULL;
        }

        break;


    default:
        break;
    }


    /*
     * Delete any allocated memory found above
     */

    if (ObjPointer)
    {
        if (!AcpiTbSystemTablePointer (ObjPointer))
        {
            DEBUG_PRINT (ACPI_INFO, ("CmDeleteInternalObj: Deleting Obj Ptr %p \n",
                                    ObjPointer));

            AcpiCmFree (ObjPointer);
        }
    }


    /* Only delete the object if it was dynamically allocated */

    if (Object->Common.Flags & AO_STATIC_ALLOCATION)
    {
        DEBUG_PRINT (ACPI_INFO, ("CmDeleteInternalObj: Object %p [%s] static allocation, no delete\n",
                                Object, AcpiCmGetTypeName (Object->Common.Type)));
    }

    else
    {
        DEBUG_PRINT (ACPI_INFO, ("CmDeleteInternalObj: Deleting object %p [%s]\n",
                                Object, AcpiCmGetTypeName (Object->Common.Type)));

        AcpiCmDeleteObjectDesc (Object);

    }

    return_VOID;
}





/******************************************************************************
 *
 * FUNCTION:    AcpiCmDeleteInternalObjectList
 *
 * PARAMETERS:  *ObjList        - Pointer to the list to be deleted
 *
 * RETURN:      Status          - the status of the call
 *
 * DESCRIPTION: This function deletes an internal object list, including both
 *              simple objects and package objects
 *
 ******************************************************************************/

ACPI_STATUS
AcpiCmDeleteInternalObjectList (
    ACPI_OBJECT_INTERNAL    **ObjList)
{
    ACPI_OBJECT_INTERNAL    **InternalObj;

 
    FUNCTION_TRACE ("CmDeleteInternalObjectList");


    /* Walk the null-terminated internal list */

    for (InternalObj = ObjList; *InternalObj; InternalObj++)
    {
        /*
         * Check for a package
         * Simple objects are simply stored in the array and do not
         * need to be deleted separately.
         */

        if (IS_THIS_OBJECT_TYPE ((*InternalObj), ACPI_TYPE_PACKAGE))
        {
            /* Delete the package */

            /*
             * TBD: [Investigate] This might not be the right thing to do, depending
             * on how the internal package object was allocated!!!
             */

            AcpiCmDeleteInternalObj (*InternalObj);
        }

    }


    /* Free the combined parameter pointer list and object array */

    AcpiCmFree (ObjList);

    return_ACPI_STATUS (AE_OK);
}


