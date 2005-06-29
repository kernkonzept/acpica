/*__________________________________________________________________________
 |
 |
 |           Copyright (C) Intel Corporation 1994-1998
 |
 | All rights reserved.  No part of this program or publication may be
 | reproduced, transmitted, transcribed, stored in a retrieval system, or
 | translated into any language or computer language, in any form or by any
 | means, electronic, mechanical, magnetic, optical, chemical, manual, or
 | otherwise, without the prior written permission of Intel Corporation.
 |__________________________________________________________________________
 |
 | ModuleName: iesystem -- Interface to OS services
 |__________________________________________________________________________
*/

#define __IESYSTEM_C__

#include <acpi.h>
#include <interpreter.h>
#include <namespace.h>


#define _THIS_MODULE        "iesystem.c"
#define _COMPONENT          INTERPRETER

/* Global Variables */

ACPI_EXTERN FIRMWARE_ACPI_CONTROL_STRUCTURE * FACS;


static ST_KEY_DESC_TABLE KDT[] = {
    {"0000", 'W', "NOT IMPLEMENTED", "NOT IMPLEMENTED"},
    {NULL, 'I', NULL, NULL}
};


/******************************************************************************
 * 
 * FUNCTION:    ThreadId
 *
 * PARAMETERS:  void
 *
 * RETURN:      Current Thread ID (for this implementation a 1 is returned)
 *
 * DESCRIPTION: An invocation is identified by its Thread ID.  In a single
 *              threaded OS the Thread ID is undefined so a 1 will be
 *              returned.
 *
 ******************************************************************************/

UINT16 
ThreadId (void)
{
    return (1);
}



/******************************************************************************
 * 
 * FUNCTION:    DoNotifyOp 
 *
 * PARAMETERS:  OBJECT_DESCRIPTOR *ValDesc -    The value of the opcode to be
 *                                          executed.
 *
 * RETURN:      S_SUCCESS or S_ERROR (S_SUCCESS for now only)
 *
 * DESCRIPTION: prints a message that these operations are not implemented
 *
 ******************************************************************************/

INT32
DoNotifyOp (OBJECT_DESCRIPTOR *ValDesc, OBJECT_DESCRIPTOR *ObjDesc)
{

    if (ValDesc)
    {
        if (ObjDesc)
        {
            DEBUG_PRINT (ACPI_INFO, 
                        ("NotifyOp: %s %s \n", NsTypeNames[ObjDesc->ValType],
                        NsFullyQualifiedName (ObjDesc->Device.Device)));
        }
    
        switch (ValDesc->Number.Number)
        {
        case 0:
            DEBUG_PRINT (ACPI_INFO, ("Re-enumerate Devices \n"));
            break;

        case 1:
            DEBUG_PRINT (ACPI_INFO, ("Ejection Request \n"));
            break;

        case 2:
            DEBUG_PRINT (ACPI_INFO, ("Device Wake \n"));
            break;

        case 0x80:
            DEBUG_PRINT (ACPI_INFO, ("Status Change \n"));
            break;

        default:
            DEBUG_PRINT (ACPI_INFO, ("%lx \n",
                    ValDesc->Number.Number));
            break;
        }

        REPORT_WARNING (&KDT[0]);
    }
    
    return S_SUCCESS;
}


/******************************************************************************
 * 
 * FUNCTION:    DoSuspend
 *
 * PARAMETERS:  UINT32 HowLong - The amount of time to suspend
 *
 * RETURN:      void
 *
 * DESCRIPTION: Suspend processing for specified amount of time.  This
 *              function should be suspending the current thread but is using
 *              busy wait instead since this implementation is single threaded
 *
 ******************************************************************************/

void
DoSuspend (UINT32 HowLong)
{
    OsdSleep ((UINT16) (HowLong / (UINT32) 1000), (UINT16) (HowLong % (UINT32) 1000));
}


/******************************************************************************
 * 
 * FUNCTION:    AcquireOpRqst
 *
 * PARAMETERS:  OBJECT_DESCRIPTOR *TimeDesc  - The 'time to delay' object descriptor
 *              OBJECT_DESCRIPTOR *ObjDesc   - The object descriptor for this op
 *
 * RETURN:      S_SUCCESS/S_ERROR
 *
 * DESCRIPTION: Provides an access point to perform synchronization operations
 *              within the AML.  Current implementation is for single thread
 *              OS only.  This function will cause a lock to be generated
 *              for the Mutex pointed to by ObjDesc.
 *              Timeout is being ignored for now since this is single threaded.
 *
 ******************************************************************************/

INT32
AcquireOpRqst (OBJECT_DESCRIPTOR *TimeDesc, OBJECT_DESCRIPTOR *ObjDesc)
{
    UINT16      CurrentId;
    INT32       Excep = S_SUCCESS;


    if (ObjDesc)
    {
        if (ObjDesc->Mutex.LockCount == 0)
        {
            ObjDesc->Mutex.ThreadId = ThreadId ();
        }
    
        else if (ObjDesc->Mutex.ThreadId != (CurrentId = ThreadId ()))
        {
            DEBUG_PRINT (ACPI_ERROR, ("Thread %02Xh attemted to Aquire a resource owned "
                    "by thread %02Xh\n", CurrentId, ObjDesc->Mutex.ThreadId));
            Excep = S_ERROR;
        }

        if (S_SUCCESS == Excep)
        {
            ObjDesc->Mutex.LockCount++;
        }
    }

    return (Excep);
}


/******************************************************************************
 * 
 * FUNCTION:    ReleaseOpRqst
 *
 * PARAMETERS:  OBJECT_DESCRIPTOR *ObjDesc  - The object descriptor for this op
 *
 * RETURN:      S_SUCCESS/S_ERROR
 *
 * DESCRIPTION: Provides an access point to perform synchronization operations
 *              within the AML.  Current implementation is for single thread
 *              OS only.  This operation is a request to release a previously
 *              acquired Mutex.  If the Mutex variable is set then it will be
 *              decremented.  Otherwise S_ERROR will be returned.
 *
 ******************************************************************************/

INT32
ReleaseOpRqst (OBJECT_DESCRIPTOR *ObjDesc)
{
    UINT16      CurrentId;
    INT32       Excep = S_SUCCESS;


    if (ObjDesc)
    {
        if (ObjDesc->Mutex.LockCount == 0)
        {
            DEBUG_PRINT (ACPI_ERROR, ("Attempting to Release a Mutex that is not locked\n"));
            Excep == S_ERROR;
        }
    
        else if (ObjDesc->Mutex.ThreadId != (CurrentId = ThreadId ()))
        {
            DEBUG_PRINT (ACPI_ERROR, ("Thread %02Xh attemted to Release a Mutex owned "
                        "by thread %02Xh\n", CurrentId, ObjDesc->Mutex.ThreadId));
            Excep = S_ERROR;
        }
    
        if (S_SUCCESS == Excep)
        {
            ObjDesc->Mutex.LockCount--;
        }
    }

    return (Excep);
}


/******************************************************************************
 * 
 * FUNCTION:    SignalOpRqst
 *
 * PARAMETERS:  OBJECT_DESCRIPTOR *ObjDesc  - The object descriptor for this op
 *
 * RETURN:      S_SUCCESS
 *
 * DESCRIPTION: Provides an access point to perform synchronization operations
 *              within the AML.  Current implementation is for single thread
 *              OS only.  This will signal when an event has been posted.
 *              if this occures prior to a wait then the wait will succeed.
 *              if a wait occures without a signal then the wait will fail
 *              because the signal will never occur.
 *
 ******************************************************************************/

INT32
SignalOpRqst (OBJECT_DESCRIPTOR *ObjDesc)
{

    if (ObjDesc)
    {
        ObjDesc->Event.SignalCount++;
    }

    return (S_SUCCESS);
}


/******************************************************************************
 * 
 * FUNCTION:    WaitOpRqst
 *
 * PARAMETERS:  OBJECT_DESCRIPTOR *TimeDesc - The 'time to delay' object descriptor
 *              OBJECT_DESCRIPTOR *ObjDesc   - The object descriptor for this op
 *
 * RETURN:      S_SUCCESS/S_ERROR
 *
 * DESCRIPTION: Provides an access point to perform synchronization operations
 *              within the AML.  Current implementation is for single thread
 *              OS only.  This operation is a request to wait for an event.
 *              In a single thread OS,  if the signal for this event has
 *              already occured then dec the signal count and return success.
 *              If the signal has not already occured then it never will
 *              because the wait would have to give the processor over to a
 *              new process and that can't happen in a single threaded OS.
 *              Timeout is being ignored for the current implementation.
 *
 ******************************************************************************/

INT32
WaitOpRqst (OBJECT_DESCRIPTOR *TimeDesc, OBJECT_DESCRIPTOR *ObjDesc)
{
    INT32       Excep = S_SUCCESS;


    if (ObjDesc)
    {
        if (0 == ObjDesc->Event.SignalCount)
        {
            /* Error for single threaded OS */
        
            DEBUG_PRINT (ACPI_ERROR, ("Waiting for a signal that has not occured.  In a single threaded"
                    "\nOperating System the signal would never be received.\n"));
            Excep = S_ERROR;
        }
    
        else
        {
            ObjDesc->Event.SignalCount--;
        }
    }
  
    return (Excep);
}


/******************************************************************************
 * 
 * FUNCTION:    ResetOpRqst
 *
 * PARAMETERS:  OBJECT_DESCRIPTOR *ObjDesc  - The object descriptor for this op
 *
 * RETURN:      S_SUCCESS/S_ERROR
 *
 * DESCRIPTION: Provides an access point to perform synchronization operations
 *              within the AML.  Current implementation is for single thread
 *              OS only.  This will reset the total signal count for this
 *              event to 0.
 *
 ******************************************************************************/

INT32
ResetOpRqst (OBJECT_DESCRIPTOR *ObjDesc)
{
    INT32       Excep = S_SUCCESS;


    if (ObjDesc)
    {
        ObjDesc->Event.SignalCount = 0;
    }

    return (Excep);
}


/***************************************************************************
 * 
 * FUNCTION:    GetGlobalLock
 *
 * RETURN:      S_SUCCESS/S_ERROR
 *
 * DESCRIPTION: attemts to gain ownership of the Global Lock.  Since this
 *              is a single threaded implementation if the Global Lock
 *              ownership is not successful then the test will fail.
 *
 **************************************************************************/

INT32
GetGlobalLock(void)
{
    UINT32          GlobalLockReg;
    INT32           Excep = S_ERROR;


    if (FACS)
    {
        /* Only if the FACS is valid */

        GlobalLockReg = FACS->GlobalLock;

        if (GlobalLockReg & GL_OWNED)
        {
            DEBUG_PRINT (ACPI_ERROR, ("The Global Lock is owned by another process\n"\
                    "This is a single threaded implementation. There is no way some\n"\
                    "other process can own the Global Lock!\n"));
            Excep = S_ERROR;
        }
    
        else
        {
            /* Its not owned so take ownership and return S_SUCCESS */
        
            FACS->GlobalLock |= GL_OWNED;
            Excep = S_SUCCESS;
        }
    }

    return (Excep);
}


/***************************************************************************
 * 
 * FUNCTION:    ReleaseGlobalLock
 *
 * DESCRIPTION: Releases the ownership of the Global Lock.
 *
 **************************************************************************/

void
ReleaseGlobalLock (void)
{
    
    if (FACS)
    {
        FACS->GlobalLock &= 0xFFFFFFFF ^ GL_OWNED;
    }
}
