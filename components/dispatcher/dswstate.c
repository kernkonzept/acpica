/******************************************************************************
 * 
 * Module Name: pswstate - parser WALK_STATE management routines
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



#include <acpi.h>
#include <amlcode.h>
#include <parser.h>
#include <namesp.h>
#include <interp.h>

#define _COMPONENT          PARSER
        MODULE_NAME         ("pswstate");




/*******************************************************************************
 *
 * FUNCTION:    
 *
 * PARAMETERS:  WalkState       - State to push
 *
 * RETURN:      Status
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

ACPI_STATUS
PsxResultStackClear (
    ACPI_WALK_STATE         *WalkState)
{

    WalkState->NumResults = 0;
    WalkState->CurrentResult = 0;

    return AE_OK;
}


/*******************************************************************************
 *
 * FUNCTION:    
 *
 * PARAMETERS:  WalkState       - State to push
 *
 * RETURN:      Status
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

ACPI_STATUS
PsxResultStackPush (
    void                    *Object,
    ACPI_WALK_STATE         *WalkState)
{


    if (WalkState->NumResults >= OBJ_NUM_OPERANDS)
    {
        DEBUG_PRINT (ACPI_ERROR, ("PsxResultStackPush: overflow! Obj=%p State=%p Num=%X\n", 
                        Object, WalkState, WalkState->NumResults));
        return AE_STACK_OVERFLOW;
    }

    WalkState->Results [WalkState->NumResults] = Object;
    WalkState->NumResults++;

    DEBUG_PRINT (TRACE_EXEC, ("PsxResultStackPush: Obj=%p State=%p Num=%X Cur=%X\n", 
                    Object, WalkState, WalkState->NumResults, WalkState->CurrentResult));

    return AE_OK;
}


/*******************************************************************************
 *
 * FUNCTION:    
 *
 * PARAMETERS:  WalkState       - State to push
 *
 * RETURN:      Status
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

ACPI_STATUS
PsxResultStackPop (
    ACPI_OBJECT_INTERNAL    **Object,
    ACPI_WALK_STATE         *WalkState)
{


    if ((WalkState->NumResults == 0) ||
        (WalkState->CurrentResult >= WalkState->NumResults))
    {
        DEBUG_PRINT (ACPI_ERROR, ("PsxResultStackPop: Underflow! State=%p Cur=%X Num=%X\n", 
                        WalkState, WalkState->CurrentResult, WalkState->NumResults));
        return AE_STACK_UNDERFLOW;
    }

    *Object = WalkState->Results [WalkState->CurrentResult];
    WalkState->Results [WalkState->CurrentResult] = NULL;
    WalkState->CurrentResult++;

    if (WalkState->CurrentResult == WalkState->NumResults)
    {
        WalkState->CurrentResult = 0;
        WalkState->NumResults = 0;

        DEBUG_PRINT (TRACE_EXEC, ("PsxResultStackPop: Stack is now empty.  State=%X Next=%X\n", 
                        WalkState, WalkState->CurrentResult));
    }

    DEBUG_PRINT (TRACE_EXEC, ("PsxResultStackPop: Obj=%p State=%p Num=%X Cur=%X\n", 
                    *Object, WalkState, WalkState->NumResults, WalkState->CurrentResult));

    return AE_OK;
}

   


/*******************************************************************************
 *
 * FUNCTION:    PsxObjStackDeleteAll
 *
 * PARAMETERS:  WalkState       - State to push
 *
 * RETURN:      Status
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

ACPI_STATUS
PsxObjStackDeleteAll (
    ACPI_WALK_STATE         *WalkState)
{
    UINT32                  i;


    FUNCTION_TRACE_PTR ("PsxObjStackDeleteAll", WalkState);


    for (i = 0; i < OBJ_NUM_OPERANDS; i++)
    {
        if (WalkState->Operands[i])
        {
            CmDeleteOperand (&WalkState->Operands[i]);
        }
    }

    return_ACPI_STATUS (AE_OK);
}



/*******************************************************************************
 *
 * FUNCTION:    PsxObjStackPush
 *
 * PARAMETERS:  WalkState       - State to push
 *
 * RETURN:      Status
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

ACPI_STATUS
PsxObjStackPush (
    void                    *Object,
    ACPI_WALK_STATE         *WalkState)
{


    if (WalkState->NumOperands >= OBJ_NUM_OPERANDS)
    {
        DEBUG_PRINT (ACPI_ERROR, ("PsxObjStackPush: overflow! Obj=%p State=%p #Ops=%X\n", 
                        Object, WalkState, WalkState->NumOperands));
        return AE_STACK_OVERFLOW;
    }

    WalkState->Operands [WalkState->NumOperands] = Object;
    WalkState->NumOperands++;

    DEBUG_PRINT (TRACE_EXEC, ("PsxObjStackPush: Obj=%p State=%p #Ops=%X\n", 
                    Object, WalkState, WalkState->NumOperands));

    return AE_OK;
}


/*******************************************************************************
 *
 * FUNCTION:    PsxObjStackPop 
 *
 * PARAMETERS:  WalkState       - State to push
 *
 * RETURN:      Status
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

ACPI_STATUS
PsxObjStackPop (
    UINT32                  PopCount,
    ACPI_WALK_STATE         *WalkState)
{
    UINT32                  i;


    for (i = 0; i < PopCount; i++)
    {
        if (WalkState->NumOperands == 0)
        {
            DEBUG_PRINT (ACPI_ERROR, ("PsxObjStackPop: Underflow! Count=%X State=%p #Ops=%X\n", 
                            PopCount, WalkState, WalkState->NumOperands));
            return AE_STACK_UNDERFLOW;
        }

        WalkState->NumOperands--;
        WalkState->Operands [WalkState->NumOperands] = NULL;
    }

    DEBUG_PRINT (TRACE_EXEC, ("PsxObjStackPop: Count=%X State=%p #Ops=%X\n", 
                    PopCount, WalkState, WalkState->NumOperands));

    return AE_OK;
}

    
/*******************************************************************************
 *
 * FUNCTION:    PsxObjStackGetValue 
 *
 * PARAMETERS:  WalkState       - State to push
 *
 * RETURN:      Status
 *
 * DESCRIPTION: 
 *
 ******************************************************************************/

void *
PsxObjStackGetValue (
    UINT32                  Index,
    ACPI_WALK_STATE         *WalkState)
{

    FUNCTION_TRACE_PTR ("PsxObjStackGetValue", WalkState);


    if (WalkState->NumOperands == 0)
    {
        return_VALUE (NULL);
    }

    if (Index > (WalkState->NumOperands - (UINT32) 1))
    {
        return_VALUE (NULL);
    }

    return_VALUE (WalkState->Operands [(WalkState->NumOperands - 1) - Index]);
}




/*******************************************************************************
 *
 * FUNCTION:    PsGetCurrentWalkState
 *
 * PARAMETERS:  WalkList        - Get current active state for this walk list
 *
 * RETURN:      Pointer to the current walk state
 *
 * DESCRIPTION: Get the walk state that is at the head of the list (the "current"
 *              walk state.
 *
 ******************************************************************************/

ACPI_WALK_STATE *
PsGetCurrentWalkState (
    ACPI_WALK_LIST          *WalkList)

{

    DEBUG_PRINT (TRACE_PARSE, ("PsGetCurrentWalkState, =%p\n", WalkList->WalkState));

    return WalkList->WalkState;
}


/*******************************************************************************
 *
 * FUNCTION:    PsDeleteWalkState
 *
 * PARAMETERS:  WalkState       - State to delete
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Delete a walk state including all internal data structures
 *
 ******************************************************************************/

void
PsDeleteWalkState (
    ACPI_WALK_STATE         *WalkState)
{
    ACPI_CTRL_STATE         *ControlState;


    FUNCTION_TRACE_PTR ("PsDeleteWalkState", WalkState);


    while (WalkState->ControlState)
    {
        ControlState = WalkState->ControlState;
        WalkState->ControlState = ControlState->Next;

        CmFree (ControlState);
    }

    CmFree (WalkState);

    return_VOID;
}


/*******************************************************************************
 *
 * FUNCTION:    PsPushWalkState
 *
 * PARAMETERS:  WalkState       - State to push
 *              WalkList        - The list that owns the walk stack
 *
 * RETURN:      None
 *
 * DESCRIPTION: Place the WalkState at the head of the state list.
 *
 ******************************************************************************/

void
PsPushWalkState (
    ACPI_WALK_STATE         *WalkState,
    ACPI_WALK_LIST          *WalkList)
{


    FUNCTION_TRACE ("PsPushWalkState");


    WalkState->Next     = WalkList->WalkState;
    WalkList->WalkState = WalkState;

    return_VOID;
}


/*******************************************************************************
 *
 * FUNCTION:    PsPopWalkState 
 *
 * PARAMETERS:  WalkList        - The list that owns the walk stack
 *
 * RETURN:      A WalkState object popped from the stack
 *
 * DESCRIPTION: Remove and return the walkstate object that is at the head of
 *              the walk stack for the given walk list.  NULL indicates that 
 *              the list is empty.
 *
 ******************************************************************************/

ACPI_WALK_STATE *
PsPopWalkState (
    ACPI_WALK_LIST          *WalkList)
{
    ACPI_WALK_STATE         *WalkState;


    FUNCTION_TRACE ("PsPopWalkState");


    WalkState = WalkList->WalkState;

    if (WalkState)
    {
        /* Next walk state becomes the current walk state */

        WalkList->WalkState = WalkState->Next;
        WalkState->Next = NULL;
    }

    return_VALUE (WalkState);
}


/*******************************************************************************
 *
 * FUNCTION:    PsCreateWalkState
 *
 * PARAMETERS:  Origin          - Starting point for this walk
 *              WalkList        - Owning walk list
 *
 * RETURN:      Pointer to the new walk state.
 *
 * DESCRIPTION: Allocate and initialize a new walk state.  The current walk state
 *              is set to this new state.
 *
 ******************************************************************************/

ACPI_WALK_STATE *
PsCreateWalkState (
    ACPI_GENERIC_OP         *Origin,
    ACPI_WALK_LIST          *WalkList)
{
    ACPI_WALK_STATE         *WalkState;


    FUNCTION_TRACE ("PsCreateWalkState");


    WalkState = CmCallocate (sizeof (ACPI_WALK_STATE));
    if (!WalkState)
    {
        return_VALUE (NULL);
    }

    WalkState->Origin           = Origin;

    /* Init the method args/local */

    PsxMthStackInit (WalkState);

    /* Put the new state at the head of the walk list */

    PsPushWalkState (WalkState, WalkList);

    return_VALUE (WalkState);
}



