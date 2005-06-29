
/******************************************************************************
 * 
 * Module Name: nsobject - Utilities for objects attached to namespace table entries
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


#define __NSOBJECT_C__

#include <acpi.h>
#include <amlcode.h>
#include <namespace.h>
#include <interpreter.h>


#define _THIS_MODULE        "nsobject.c"
#define _COMPONENT          NAMESPACE



/****************************************************************************
 *
 * FUNCTION:    NsAttachObject
 *
 * PARAMETERS:  Handle              - Handle of nte
 *              Object              - Object to be attached
 *              Type                - Type of object, or ACPI_TYPE_Any if not known
 *
 * DESCRIPTION: Record the given object as the value associated with the
 *              name whose ACPI_HANDLE is passed.  If Object is NULL 
 *              and Type is ACPI_TYPE_Any, set the name as having no value.
 *
 ***************************************************************************/

ACPI_STATUS
NsAttachObject (
    ACPI_HANDLE             Handle, 
    ACPI_HANDLE             Object, 
    UINT8                   Type)
{
    NAME_TABLE_ENTRY        *ThisEntry = (NAME_TABLE_ENTRY *) Handle;
    ACPI_OBJECT_INTERNAL    *ObjDesc;
    ACPI_OBJECT_INTERNAL    *PreviousObjDesc;
    ACPI_OBJECT_TYPE        ObjType = ACPI_TYPE_Any;
    UINT8                   Flags;


    FUNCTION_TRACE ("NsAttachObject");


    /*
     * Parameter validation
     */

    if (!Gbl_RootObject->Scope)
    {
        /* Name space not initialized  */

        REPORT_ERROR ("NsAttachObject: Name space not initialized");
        return_ACPI_STATUS (AE_NO_NAMESPACE);
    }
    
    if (!Handle)
    {
        /* Invalid handle */

        REPORT_ERROR ("NsAttachObject: Null name handle");
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }
    
    if (!Object && (ACPI_TYPE_Any != Type))
    {
        /* Null object */

        REPORT_ERROR ("NsAttachObject: Null object, but type not ACPI_TYPE_Any");
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }
    
    if (!IS_NS_HANDLE (Handle))
    {
        /* Not a name handle */

        REPORT_ERROR ("NsAttachObject: Invalid handle");
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    /* Check if this object is already attached */

    if (ThisEntry->Object == Object)
    {
        DEBUG_PRINT (TRACE_EXEC,("NsAttachObject: Obj %p already installed in NTE %p\n",
                        Object, Handle));
    
        return_ACPI_STATUS (AE_OK);
    }


    /* Get the current flags field of the NTE */

    Flags = ThisEntry->Flags;
    Flags &= ~NTE_AML_ATTACHMENT;

    /*
     * If the object is an NTE with an attached object, we will use that object
     */

    if (IS_NS_HANDLE (Object) && 
        ((NAME_TABLE_ENTRY *) Object)->Object)
    {
        /* 
         * Value passed is a name handle and that name has a non-null value.
         * Use that name's value and type.
         */

        ObjDesc = ((NAME_TABLE_ENTRY *) Object)->Object;
        ObjType = ((NAME_TABLE_ENTRY *) Object)->Type;

        /*
         * Copy appropriate flags
         */

        if (((NAME_TABLE_ENTRY *) Object)->Flags & NTE_AML_ATTACHMENT)
        {
            Flags |= NTE_AML_ATTACHMENT;
        }
    }


    /*
     * Otherwise, we will use the parameter object, but we must type it first
     */

    else
    {
        ObjDesc = (ACPI_OBJECT_INTERNAL *) Object;


        /* Set the type if given, or if it can be discerned */

        if (ACPI_TYPE_Any != Type)
        {
            ObjType = (ACPI_OBJECT_TYPE) Type;
        }

        else if (!Object)
        {
            ObjType = ACPI_TYPE_Any;
        }

        /*
         * Check if value points into the AML code
         */
        else if (NsIsInSystemTable (Object))
        {
            /* Object points into the AML stream.  Set a flag bit in the NTE to indicate this */
    
            Flags |= NTE_AML_ATTACHMENT;


            /* Check for a recognized OpCode */

            switch (*(UINT8 *) Object)
            {

            case AML_OpPrefix:

                if (*(UINT16 *) Object != AML_RevisionOp)
                {
                    /* OpPrefix is unrecognized unless part of RevisionOp */
            
                    break;
                }

                /* Else fall through to set type as Number */
    

            case AML_ZeroOp: case AML_OnesOp: case AML_OneOp:
            case AML_ByteOp: case AML_WordOp: case AML_DWordOp:

                ObjType = ACPI_TYPE_Number;
                break;


            case AML_StringOp:

                ObjType = ACPI_TYPE_String;
                break;


            case AML_BufferOp:

                ObjType = ACPI_TYPE_Buffer;
                break;


            case AML_MutexOp:

                ObjType = ACPI_TYPE_Mutex;
                break;


            case AML_PackageOp:

                ObjType = ACPI_TYPE_Package;
                break;


            default:

                DEBUG_PRINT (ACPI_ERROR, ("AML Opcode/Type [%x] not supported in attach\n",
                               *(UINT16 *) Object));

                return_ACPI_STATUS (AE_AML_ERROR);

                break;
            }
        }

        else
        {
            /* 
             * Cannot figure out the type -- set to DefAny which will print as an
             * error in the name table dump
             */

            if (GetDebugLevel () > 0)
            {
                NsDumpPathname (Handle, "NsAttachObject confused: setting bogus type for  ", 
                                ACPI_INFO, _COMPONENT);

                if (NsIsInSystemTable (Object))
                {
                    DEBUG_PRINT (ACPI_INFO,
                                ("AML-stream code %02x\n", *(UINT8 *) Object));
                }
        
                else if (IS_NS_HANDLE (Object))
                {
                    NsDumpPathname (Object, "name ", ACPI_INFO, _COMPONENT);
                }
        
                else
                {
                    NsDumpPathname (Object, "object ", ACPI_INFO, _COMPONENT);
                    DUMP_STACK_ENTRY (Object);
                }
            }

            ObjType = INTERNAL_TYPE_DefAny;
        }
    }



    DEBUG_PRINT (TRACE_EXEC,("NsAttachObject: Installing obj %p into NTE %p\n",
                    ObjDesc, Handle));
    

    /* Must increment the new value's reference count (if it is an internal object) */

    CmUpdateObjectReference (ObjDesc, REF_INCREMENT);

    CmAcquireMutex (MTX_NAMESPACE);
    {
        /* Save the existing object (if any) for deletion later */

        PreviousObjDesc = ThisEntry->Object;

        /* Install the object and set the type, flags */

        ThisEntry->Object = ObjDesc;
        ThisEntry->Type = ObjType;
        ThisEntry->Flags = Flags;
    }
    CmReleaseMutex (MTX_NAMESPACE);


    /* 
     * Delete an existing attached object. 
     */

    if (PreviousObjDesc)
    {
        CmUpdateObjectReference (PreviousObjDesc, REF_DECREMENT);
        CmDeleteInternalObject (PreviousObjDesc);
    }

    return_ACPI_STATUS (AE_OK);
}


/****************************************************************************
 *
 * FUNCTION:    NsAttachMethod
 *
 * PARAMETERS:  Handle              - Handle of nte to be set
 *              Offset              - Value to be set
 *              Length              - Length associated with value
 *
 * DESCRIPTION: Record the given offset and p-code length of the method
 *              whose handle is passed
 *
 ***************************************************************************/

ACPI_STATUS
NsAttachMethod (
    ACPI_HANDLE             Handle, 
    UINT8                   *PcodeAddr, 
    UINT32                  PcodeLength)
{
    ACPI_OBJECT_INTERNAL    *ObjDesc;
    ACPI_OBJECT_INTERNAL    *PreviousObjDesc;
    NAME_TABLE_ENTRY        *ThisEntry = (NAME_TABLE_ENTRY *) Handle;


    FUNCTION_TRACE ("NsAttachMethod");


    /* Parameter validation */

    if (!Gbl_RootObject->Scope)
    {
        /* Name space uninitialized */

        REPORT_ERROR ("NsAttachMethod: name space uninitialized");
        return_ACPI_STATUS (AE_NO_NAMESPACE);
    }
    
    if (!Handle)
    {
        /* Null name handle */

        REPORT_ERROR ("NsAttachMethod: null name handle");
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    
    /* Allocate a method descriptor */

    ObjDesc = CmCreateInternalObject (ACPI_TYPE_Method);
    if (!ObjDesc)
    {
        /* Method allocation failure  */

        REPORT_ERROR ("NsAttachMethod: allocation failure");
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    /* Init the method info */

    ObjDesc->Method.Pcode       = PcodeAddr;
    ObjDesc->Method.PcodeLength = PcodeLength;

    /* Update reference count and install */

    CmUpdateObjectReference (ObjDesc, REF_INCREMENT);

    CmAcquireMutex (MTX_NAMESPACE);
    {
        PreviousObjDesc = ThisEntry->Object;
        ThisEntry->Object = ObjDesc;
    }
    CmReleaseMutex (MTX_NAMESPACE);


    /* 
     * Delete an existing object.  Don't try to re-use in case it is shared
     */
    if (PreviousObjDesc)
    {
        DEBUG_PRINT (ACPI_INFO, ("NsAttachMethod: ***Old: %p Obj %p Pcode %p Len 0x%X\n",
                                    Handle, PreviousObjDesc, PreviousObjDesc->Method.Pcode, PreviousObjDesc->Method.PcodeLength));

        CmUpdateObjectReference (PreviousObjDesc, REF_DECREMENT);
        CmDeleteInternalObject (PreviousObjDesc);
    }

    DEBUG_PRINT (ACPI_INFO, ("NsAttachMethod: %p Obj %p Pcode %p Len 0x%X\n",
                            Handle, ObjDesc, ObjDesc->Method.Pcode, ObjDesc->Method.PcodeLength));

    return_ACPI_STATUS (AE_OK);
}


/****************************************************************************
 *
 * FUNCTION:    NsDetachObject
 *
 * PARAMETERS:  Object           - An object whose Value will be deleted
 *
 * RETURN:      None.
 *
 * DESCRIPTION: Delete the Value associated with a namespace object.  If the
 *              Value is an allocated object, it is freed.  Otherwise, the
 *              field is simply cleared.
 *
 ***************************************************************************/

void
NsDetachObject (
    ACPI_HANDLE             Object)
{
    NAME_TABLE_ENTRY        *Entry = Object;
    ACPI_OBJECT_INTERNAL    *ObjDesc;

    
    FUNCTION_TRACE ("NsDetachObject");

    ObjDesc = Entry->Object;
    if (!ObjDesc)
    {
        return_VOID;
    }

    /* Clear the entry in all cases */

    Entry->Object = NULL;
    
    /* Found a valid value */

    DEBUG_PRINT (ACPI_INFO, ("NsDetachObject: Object=%p Value=%p Name %4.4s\n", 
                            Entry, ObjDesc, &Entry->Name));

    /* Not every value is an object allocated via CmCallocate, must check */

    if (!NsIsInSystemTable (ObjDesc)) /*&&
        !IS_NS_HANDLE      (ObjDesc))*/
    {

        /* Delete the object (and all subobjects) */

        CmUpdateObjectReference (ObjDesc, REF_DECREMENT);   /* Removed from Namespace */
        CmDeleteInternalObject (ObjDesc);
    }

    return_VOID;
}


/****************************************************************************
 *
 * FUNCTION:    NsGetAttachedObject
 *
 * PARAMETERS:  Handle              - Handle of nte to be examined
 *
 * RETURN:      Current value of the object field from nte whose handle is passed
 *
 ***************************************************************************/

void *
NsGetAttachedObject (
    ACPI_HANDLE             Handle)
{
    FUNCTION_TRACE_PTR ("NsGetAttachedObject", Handle);


    if (!Handle)
    {
        /* handle invalid */

        REPORT_WARNING ("NsGetAttachedObject: Null handle");
        return_VALUE (NULL);
    }

    return_VALUE (((NAME_TABLE_ENTRY *) Handle)->Object);
}


/*****************************************************************************
 *
 * FUNCTION:    IsNsObject
 *
 * PARAMETERS:  *ObjDesc            - An object descriptor
 *
 * RETURN:      TRUE if the passed descriptor is the value of a Name in
 *              the name space, else FALSE
 *
 ****************************************************************************/

INT32
IsNsObject (
    ACPI_OBJECT_INTERNAL    *ObjDesc)
{
    ACPI_HANDLE             RetHandle;


    FUNCTION_TRACE ("IsNsObject");

    RetHandle = NsFindAttachedObject (ObjDesc, NS_ALL, ACPI_INT_MAX);

    return_VALUE ((RetHandle != (ACPI_HANDLE) 0));
}


/****************************************************************************
 * 
 * FUNCTION:    NsCompareObject
 *
 * PARAMETERS:  ObjHandle           - A namespace entry
 *              Level               - Current nesting level
 *              ObjDesc             - The value to be compared
 *
 * DESCRIPTION: A UserFunction called by NsWalkNamespace().  It performs
 *              a comparison for NsFindAttachedObject().  The comparison is against
 *              the value in the value field of the ObjHandle (an NTE).
 *              If a match is found, the handle is returned, which aborts
 *              NsWalkNamespace.
 *
 ***************************************************************************/

ACPI_STATUS
NsCompareObject (
    ACPI_HANDLE             ObjHandle, 
    UINT32                  Level, 
    void                    *ObjDesc,
    void                    **ReturnValue)
{

    if (((NAME_TABLE_ENTRY *) ObjHandle)->Object == ObjDesc)
    {
        DEBUG_PRINT (ACPI_INFO, ("NsCompareObject: match found, Obj %x Val %x\n",
                        ObjHandle, ((NAME_TABLE_ENTRY *) ObjHandle)->Object));

        if (ReturnValue)
        {
            *ReturnValue = ObjHandle;
        }

        return AE_TERMINATE;        /* Stop the walk */
    }

    return AE_OK;   /* Not found, continue the walk */
}


/****************************************************************************
 *
 * FUNCTION:    NsFindAttachedObject
 *
 * PARAMETERS:  *ObjDesc            - Value to be found in ptrVal field.
 *              StartHandle         - Root of subtree to be searched, or
 *                                    NS_ALL to search the entire namespace
 *              MaxDepth            - Maximum depth of search.  Use INT_MAX
 *                                    for an effectively unlimited depth.
 *
 * DESCRIPTION: Traverse the name space until finding a name whose Value field
 *              matches the ObjDesc parameter, and return a handle to that
 *              name, or (ACPI_HANDLE)0 if none exists.
 *              if StartHandle is NS_ALL (null) search from the root,
 *              else it is a handle whose children are to be searched.
 *
 ***************************************************************************/

ACPI_HANDLE
NsFindAttachedObject (
    ACPI_OBJECT_INTERNAL    *ObjDesc, 
    ACPI_HANDLE             StartHandle, 
    INT32                   MaxDepth)
{
    ACPI_HANDLE             RetObject;
    ACPI_STATUS             Status;


    FUNCTION_TRACE ("NsFindAttachedObject");


    /* Parameter validation */

    if (!ObjDesc)
    {
        return_VALUE (NULL);
    }

    if (0 == MaxDepth)
    {
        return_VALUE (NULL);
    }

    if (!Gbl_RootObject->Scope)
    {
        /* 
         * If the name space has not been initialized,
         * there surely are no matching values.
         */
        return_VALUE (NULL);
    }

    if (NS_ALL == StartHandle)
    {
        StartHandle = Gbl_RootObject;
    }
    
    else
    {
        /* 
         * If base is not the root and has no children,
         * there is nothing to search.
         */
        return_VALUE (NULL);
    }


    /* 
     * Walk namespace until a match is found.
     * Either the matching object is returned, or NULL in case
     * of no match.
     */
    Status = AcpiWalkNamespace (ACPI_TYPE_Any, StartHandle, MaxDepth, NsCompareObject, 
                                ObjDesc, &RetObject);
    if (ACPI_FAILURE (Status))
    {
        DEBUG_PRINT (ACPI_ERROR, ("NsFindAttachedObject: WalkNamespace failed! %x\n", Status));
        RetObject = NULL;
    }

    return_VALUE (RetObject);
}




