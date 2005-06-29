
/******************************************************************************
 * 
 * Module Name: nswalk - Functions for walking the APCI namespace
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


#define __NSWALK_C__

#include <acpi.h>
#include <interp.h>
#include <namesp.h>


#define _COMPONENT          NAMESPACE
        MODULE_NAME         ("nswalk");


/****************************************************************************
 *
 * FUNCTION:    AcpiGetNextObject
 *
 * PARAMETERS:  Type            - Type of object to be searched for
 *              Parent          - Parent object whose children we are getting
 *              LastChild       - Previous child that was found.  
 *                                The NEXT child will be returned
 *              RetHandle       - Where handle to the next object is placed
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Return the next peer object within the namespace.  If Handle is
 *              valid, Scope is ignored.  Otherwise, the first object within 
 *              Scope is returned.
 *
 ******************************************************************************/

NAME_TABLE_ENTRY *
NsGetNextObject (
    OBJECT_TYPE_INTERNAL    Type, 
    NAME_TABLE_ENTRY        *Parent, 
    NAME_TABLE_ENTRY        *Child)
{
    NAME_TABLE_ENTRY        *ThisEntry;



    if (!Child)
    {

        /* It's really the parent's _scope_ that we want */

        ThisEntry = Parent->Scope;
    }

    else
    {
        /* Start search at the NEXT object */

        ThisEntry = Child->NextEntry;
    }


    /* If any type is OK, we are done */

    if (Type == ACPI_TYPE_Any)
    {
        /* Make sure this is valid entry first */

        if ((!ThisEntry) ||
            (!ThisEntry->Name))
        {
            return NULL;
        }

        return (ThisEntry);
    }


    /* Must search for the object -- but within this scope only */

    while (ThisEntry)
    {
        /* If type matches, we are done */

        if (ThisEntry->Type == Type)
        {
            return (ThisEntry);
        }

        /* Otherwise, move on to the next object */

        ThisEntry = ThisEntry->NextEntry;
    }


    /* Not found */

    return NULL;
}



/******************************************************************************
 *
 * FUNCTION:    NsWalkNamespace
 *
 * PARAMETERS:  Type                - ACPI_OBJECT_TYPE to search for
 *              StartObject         - Handle in namespace where search begins
 *              MaxDepth            - Depth to which search is to reach
 *              UnlockBeforeCallback- Whether to unlock the NS before invoking the 
 *                                    callback routine
 *              UserFunction        - Called when an object of "Type" is found
 *              Context             - Passed to user function
 *
 * RETURNS      Return value from the UserFunction if terminated early.
 *              Otherwise, returns NULL.
 *
 * DESCRIPTION: Performs a modified depth-first walk of the namespace tree,
 *              starting (and ending) at the object specified by StartHandle.
 *              The UserFunction is called whenever an object that matches
 *              the type parameter is found.  If the user function returns
 *              a non-zero value, the search is terminated immediately and this
 *              value is returned to the caller.
 *
 *              The point of this procedure is to provide a generic namespace
 *              walk routine that can be called from multiple places to 
 *              provide multiple services;  the User Function can be tailored
 *              to each task, whether it is a print function, a compare 
 *              function, etc.
 *
 ******************************************************************************/

ACPI_STATUS
NsWalkNamespace (
    OBJECT_TYPE_INTERNAL    Type, 
    ACPI_HANDLE             StartObject, 
    UINT32                  MaxDepth,
    BOOLEAN                 UnlockBeforeCallback,
    WALK_CALLBACK           UserFunction, 
    void                    *Context, 
    void                    **ReturnValue)
{
    ACPI_STATUS             Status;
    NAME_TABLE_ENTRY        *ChildHandle;
    NAME_TABLE_ENTRY        *ParentHandle;
    OBJECT_TYPE_INTERNAL    ChildType;
    UINT32                  Level;


    FUNCTION_TRACE ("NsWalkNamespace");

    /* Special case for the namespace root object */

    if (StartObject == ACPI_ROOT_OBJECT)
    {
        StartObject = Gbl_RootObject;
    }


    /* Null child means "get first object" */

    ParentHandle    = StartObject;
    ChildHandle     = 0;
    ChildType       = ACPI_TYPE_Any;
    Level           = 1;

    /* 
     * Traverse the tree of objects until we bubble back up to where we
     * started. When Level is zero, the loop is done because we have 
     * bubbled up to (and passed) the original parent handle (StartHandle)
     */

    while (Level > 0)
    {
        /* Get the next typed object in this scope.  Null returned if not found */

        Status = AE_OK;
        ChildHandle = NsGetNextObject (ACPI_TYPE_Any, ParentHandle, ChildHandle);
        if (ChildHandle)
        {
            /* Found an object, Get the type if we are not searching for ANY */

            if (Type != ACPI_TYPE_Any)
            {
                ChildType = ChildHandle->Type;
            }

            if (ChildType == Type)
            {
                /* Found a matching object, invoke the user callback function */


                if (UnlockBeforeCallback)
                {
                    CmReleaseMutex (MTX_NAMESPACE);
                }

                Status = UserFunction (ChildHandle, Level, Context, ReturnValue);

                if (UnlockBeforeCallback)
                {
                    CmAcquireMutex (MTX_NAMESPACE);
                }

                switch (Status)
                {
                case AE_OK:
                case AE_CTRL_DEPTH:
                    break;                          /* Just keep going */

                case AE_CTRL_TERMINATE:
                    return_ACPI_STATUS (AE_OK);     /* Exit now, with OK status */
                    break;

                default:
                    return_ACPI_STATUS (Status);    /* All others are valid exceptions */
                    break;
                }
            }

            /* 
             * Depth first search:
             * Attempt to go down another level in the namespace if we are allowed to.
             * Don't go any further if we have reached the caller specified maximum depth
             * or if the user function has specified that the maximum depth has been reached.
             */

            if ((Level < MaxDepth) && (Status != AE_CTRL_DEPTH))
            {
                if (NsGetNextObject (ACPI_TYPE_Any, ChildHandle, 0))
                {
                    /* There is at least one child of this object, visit the object */

                    Level++;
                    ParentHandle    = ChildHandle;
                    ChildHandle     = 0;
                }
            }
        }

        else
        {
            /* 
             * No more children in this object (NsGetNextObject failed), 
             * go back upwards in the namespace tree to the object's parent.
             */
            Level--;
            ChildHandle = ParentHandle;
            ParentHandle = ParentHandle->ParentEntry;
        }
    }

    return_ACPI_STATUS (AE_OK);                   /* Complete walk, not terminated by user function */
}


