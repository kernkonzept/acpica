
/******************************************************************************
 *
 * Module Name: dsmethod - Parser/Interpreter interface - control method parsing
 *              $Revision: 1.47 $
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

#define __DSMETHOD_C__

#include "acpi.h"
#include "acparser.h"
#include "amlcode.h"
#include "acdispat.h"
#include "acinterp.h"
#include "acnamesp.h"
#include "actables.h"
#include "acdebug.h"


#define _COMPONENT          DISPATCHER
        MODULE_NAME         ("dsmethod");



/*******************************************************************************
 *
 * FUNCTION:    AcpiDsParseMethod
 *
 * PARAMETERS:  ObjHandle       - Named Object of the method
 *              Level           - Current nesting level
 *              Context         - Points to a method counter
 *              ReturnValue     - Not used
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Call the parser and parse the AML that is
 *              associated with the method.
 *
 * MUTEX:       Assumes parser is locked
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsParseMethod (
    ACPI_HANDLE             ObjHandle)
{
    ACPI_STATUS             Status;
    ACPI_OBJECT_INTERNAL    *ObjDesc;
    ACPI_GENERIC_OP         *Op;
    ACPI_NAMED_OBJECT       *NameDesc;
    ACPI_OWNER_ID           OwnerId;


    FUNCTION_TRACE_PTR ("DsParseMethod", ObjHandle);


    /* Parameter Validation */

    if (!ObjHandle)
    {
        return_ACPI_STATUS (AE_NULL_ENTRY);
    }

    DEBUG_PRINT (ACPI_INFO,
        ("DsParseMethod: **** Parsing [%4.4s] **** NamedObj=%p\n",
        &((ACPI_NAMED_OBJECT*)ObjHandle)->Name, ObjHandle));


    /* Extract the method object from the method Named Object */

    NameDesc = (ACPI_NAMED_OBJECT*) ObjHandle;
    ObjDesc = NameDesc->Object;
    if (!ObjDesc)
    {
        return_ACPI_STATUS (AE_NULL_OBJECT);
    }

     /* Create a mutex for the method if there is a concurrency limit */

    if ((ObjDesc->Method.Concurrency != INFINITE_CONCURRENCY) &&
        (!ObjDesc->Method.Semaphore))
    {
        Status = AcpiOsCreateSemaphore (1,ObjDesc->Method.Concurrency,
                                        &ObjDesc->Method.Semaphore);
        if (ACPI_FAILURE (Status))
        {
            return_ACPI_STATUS (Status);
        }
    }

    /*
     * Allocate a new parser op to be the root of the parsed
     * method tree
     */

    Op = AcpiPsAllocOp (AML_METHOD_OP);
    if (!Op)
    {
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    /* Init new op with the method name and pointer back to the Named Object */

    AcpiPsSetName (Op, NameDesc->Name);
    Op->AcpiNamedObject = NameDesc;


    /*
     * Parse the method, first pass
     *
     * The first pass load is
     * where newly declared named objects are
     * added into the namespace.  Actual evaluation of
     * the named objects (what would be called a "second
     * pass") happens during the actual execution of the
     * method so that operands to the named objects can
     * take on dynamic run-time values.
     */

    Status = AcpiPsParseAml (Op, ObjDesc->Method.Pcode,
                        ObjDesc->Method.PcodeLength, 
                        ACPI_PARSE_LOAD_PASS1 | ACPI_PARSE_DELETE_TREE,
                        NameDesc, NULL, NULL, 
                        AcpiDsLoad1BeginOp, AcpiDsLoad1EndOp);

    if (ACPI_FAILURE (Status))
    {
        return_ACPI_STATUS (Status);
    }

    /* Get a new OwnerId for objects created by this method */

    OwnerId = AcpiCmAllocateOwnerId (OWNER_TYPE_METHOD);
    ObjDesc->Method.OwningId = OwnerId;

    DEBUG_EXEC (AcpiDbGenerateStatistics (Op, 1));

    DEBUG_PRINT (ACPI_INFO,
        ("DsParseMethod: **** [%4.4s] Parsed **** NamedObj=%p Op=%p\n",
        &((ACPI_NAMED_OBJECT*)ObjHandle)->Name, ObjHandle, Op));

    /* Install the parsed tree in the method object */
    /* TBD: [Restructure] Obsolete field? */


    AcpiPsDeleteParseTree (Op);


    return_ACPI_STATUS (Status);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsBeginMethodExecution
 *
 * PARAMETERS:  MethodNameDesc         - Named Object of the method
 *              ObjDesc             - The method object
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Prepare a method for execution.  Parses the method if necessary,
 *              increments the thread count, and waits at the method semaphore
 *              for clearance to execute.
 *
 * MUTEX:       Locks/unlocks parser.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsBeginMethodExecution (
    ACPI_NAMED_OBJECT       *MethodNameDesc,
    ACPI_OBJECT_INTERNAL    *ObjDesc)
{
    ACPI_STATUS             Status = AE_OK;


    FUNCTION_TRACE_PTR ("DsBeginMethodExecution", MethodNameDesc);


    if (!MethodNameDesc)
    {
        return_ACPI_STATUS (AE_NULL_ENTRY);
    }

    ObjDesc = AcpiNsGetAttachedObject (MethodNameDesc);
    if (!ObjDesc)
    {
        return_ACPI_STATUS (AE_NULL_OBJECT);
    }


    /*
     * If there is a concurrency limit on this method, we need to
     * obtain a unit from the method semaphore.  This releases the
     * interpreter if we block
     */

    if (ObjDesc->Method.Semaphore)
    {
        Status = AcpiAmlSystemWaitSemaphore (ObjDesc->Method.Semaphore,
                                            WAIT_FOREVER);
    }


    /*
     * Increment the method parse tree thread count since there
     * is one additional thread executing in it.  If configured
     * for deletion-on-exit, the parse tree will be deleted when
     * the last thread completes execution of the method
     */

    ObjDesc->Method.ThreadCount++;


    return_ACPI_STATUS (Status);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsCallControlMethod
 *
 * PARAMETERS:  WalkState           - Current state of the walk
 *              Op                  - Current Op to be walked
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Transfer execution to a called control method
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsCallControlMethod (
    ACPI_WALK_LIST          *WalkList,
    ACPI_WALK_STATE         *ThisWalkState,
    ACPI_GENERIC_OP         *Op)
{
    ACPI_STATUS             Status;
    ACPI_NAMED_OBJECT       *MethodNameDesc;
    ACPI_OBJECT_INTERNAL    *ObjDesc;
    ACPI_WALK_STATE         *NextWalkState;
    ACPI_PARSE_STATE        *ParserState;
    UINT32                  i;


    FUNCTION_TRACE_PTR ("DsCallControlMethod", ThisWalkState);

    DEBUG_PRINT (TRACE_DISPATCH,
        ("DsCall, execute method %p, currentstate=%p\n",
        ThisWalkState->PrevOp, ThisWalkState));

    /*
     * Get the namespace entry for the control method we are about to call
     */

    MethodNameDesc = ThisWalkState->MethodCallNameDesc;
    if (!MethodNameDesc)
    {
        return_ACPI_STATUS (AE_NULL_ENTRY);
    }

    ObjDesc = AcpiNsGetAttachedObject (MethodNameDesc);
    if (!ObjDesc)
    {
        return_ACPI_STATUS (AE_NULL_OBJECT);
    }


    /* Init for new method, wait on concurrency semaphore */

    Status = AcpiDsBeginMethodExecution (MethodNameDesc, ObjDesc);
    if (ACPI_FAILURE (Status))
    {
        return_ACPI_STATUS (Status);
    }


    /* Create and initialize a new parser state */

    ParserState = AcpiPsCreateState (ObjDesc->Method.Pcode, 
                        ObjDesc->Method.PcodeLength);
    if (!ParserState)
    {
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    AcpiPsInitScope (ParserState, NULL);
    ParserState->StartEntry = MethodNameDesc;


    /* Create a new state for the preempting walk */

    NextWalkState = AcpiDsCreateWalkState (ObjDesc->Method.OwningId,
                                            NULL, ObjDesc, WalkList);
    if (!NextWalkState)
    {
        /* TBD: delete parser state */

        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    NextWalkState->WalkType             = WALK_METHOD;
    NextWalkState->MethodNameDesc       = MethodNameDesc;
    NextWalkState->ParserState          = ParserState;
    NextWalkState->ParseFlags           = ThisWalkState->ParseFlags;
    NextWalkState->DescendingCallback   = ThisWalkState->DescendingCallback;
    NextWalkState->AscendingCallback    = ThisWalkState->AscendingCallback;

    /* The NextOp of the NextWalk will be the beginning of the method */
    /* TBD: [Restructure] -- obsolete? */

    NextWalkState->NextOp = NULL;

    /* Open a new scope */

    Status = AcpiDsScopeStackPush (MethodNameDesc,
                                    ACPI_TYPE_METHOD, NextWalkState);
    if (ACPI_FAILURE (Status))
    {
        goto Cleanup;
    }


    /*
     * Initialize the arguments for the method.  The resolved
     * arguments were put on the previous walk state's operand
     * stack.  Operands on the previous walk state stack always
     * start at index 0.
     */

    Status = AcpiDsMethodDataInitArgs (&ThisWalkState->Operands[0],
                                        ThisWalkState->NumOperands, 
                                        NextWalkState);
    if (ACPI_FAILURE (Status))
    {
        goto Cleanup;
    }



    /* Create and init a root object */

    Op = AcpiPsAllocOp (AML_SCOPE_OP);
    if (!Op)
    {
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    Status = AcpiPsParseAml (Op, ObjDesc->Method.Pcode,
                                ObjDesc->Method.PcodeLength, 
                                ACPI_PARSE_LOAD_PASS1 | ACPI_PARSE_DELETE_TREE,
                                MethodNameDesc, NULL, NULL,
                                AcpiDsLoad1BeginOp, AcpiDsLoad1EndOp);
    AcpiPsDeleteParseTree (Op);



    /*
     * Delete the operands on the previous walkstate operand stack
     * (they were copied to new objects)
     */

    for (i = 0; i < ObjDesc->Method.ParamCount; i++)
    {
        AcpiCmRemoveReference (ThisWalkState->Operands [i]);
        ThisWalkState->Operands [i] = NULL;
    }

    /* Clear the operand stack */

    ThisWalkState->NumOperands = 0;


    DEBUG_PRINT (TRACE_DISPATCH,
        ("DsCall, starting nested execution, newstate=%p\n", NextWalkState));

    return_ACPI_STATUS (AE_OK);


    /* On error, we must delete the new walk state */

Cleanup:
    AcpiDsTerminateControlMethod (NextWalkState);
    AcpiDsDeleteWalkState (NextWalkState);
    return_ACPI_STATUS (Status);

}


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsRestartControlMethod
 *
 * PARAMETERS:  WalkState           - State of the method when it was preempted
 *              Op                  - Pointer to new current op
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Restart a method that was preempted
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsRestartControlMethod (
    ACPI_WALK_STATE         *WalkState,
    ACPI_OBJECT_INTERNAL    *ReturnDesc)
{
    ACPI_STATUS             Status;


    FUNCTION_TRACE_PTR ("DsRestartControlMethod", WalkState);


    if (ReturnDesc)
    {
        if (WalkState->ReturnUsed)
        {
            /*
             * Get the return value (if any) from the previous method.
             * NULL if no return value
             */

            Status = AcpiDsResultStackPush (ReturnDesc, WalkState);
            if (ACPI_FAILURE (Status))
            {
                AcpiCmRemoveReference (ReturnDesc);
                return_ACPI_STATUS (Status);
            }
        }
    
        else
        {
            /*
             * Delete the return value if it will not be used by the
             * calling method
             */
            AcpiCmRemoveReference (ReturnDesc);
        }

    }

    DEBUG_PRINT (TRACE_DISPATCH,
        ("DsRestart: Method=%p Return=%p State=%p\n",
        WalkState->MethodCallOp, ReturnDesc, WalkState));


    return_ACPI_STATUS (AE_OK);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsTerminateControlMethod
 *
 * PARAMETERS:  WalkState           - State of the method
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Terminate a control method.  Delete everything that the method
 *              created, delete all locals and arguments, and delete the parse
 *              tree if requested.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsTerminateControlMethod (
    ACPI_WALK_STATE         *WalkState)
{
    ACPI_STATUS             Status;
    ACPI_OBJECT_INTERNAL    *ObjDesc;
    ACPI_NAMED_OBJECT       *MethodNameDesc;


    FUNCTION_TRACE_PTR ("DsTerminateControlMethod", WalkState);


    /* The method object should be stored in the walk state */

    ObjDesc = WalkState->MethodDesc;
    if (!ObjDesc)
    {
        return_ACPI_STATUS (AE_OK);
    }

    /* Delete all arguments and locals */

    AcpiDsMethodDataDeleteAll (WalkState);

    /*
     * Lock the parser while we terminate this method.
     * If this is the last thread executing the method,
     * we have additional cleanup to perform
     */

    AcpiCmAcquireMutex (ACPI_MTX_PARSER);


    /* Signal completion of the execution of this method if necessary */

    if (WalkState->MethodDesc->Method.Semaphore)
    {
        Status = AcpiOsSignalSemaphore (
                    WalkState->MethodDesc->Method.Semaphore, 1);
    }

    /* Decrement the thread count on the method parse tree */

    WalkState->MethodDesc->Method.ThreadCount--;
    if (!WalkState->MethodDesc->Method.ThreadCount)
    {
        /*
         * There are no more threads executing this method.  Perform
         * additional cleanup.
         *
         * The method Named Object is stored in the walk state
         */
        MethodNameDesc = WalkState->MethodNameDesc;
        /*
         * Delete any namespace entries created immediately underneath
         * the method
         */
        AcpiCmAcquireMutex (ACPI_MTX_NAMESPACE);
        if (MethodNameDesc->Child)
        {
            AcpiNsDeleteNamespaceSubtree (MethodNameDesc);
        }

        /*
         * Delete any namespace entries created anywhere else within
         * the namespace
         */
        AcpiNsDeleteNamespaceByOwner (WalkState->MethodDesc->Method.OwningId);
        AcpiCmReleaseMutex (ACPI_MTX_NAMESPACE);
    }

    AcpiCmReleaseMutex (ACPI_MTX_PARSER);
    return_ACPI_STATUS (AE_OK);
}


