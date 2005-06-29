 
/******************************************************************************
 * 
 * Module Name: cmobject - Internal to external object translation utilities
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

#define __CMOBJECT_C__

#include <acpi.h>
#include <acobject.h>
#include <interp.h>
#include <namesp.h>
#include <pnp.h>


#define _COMPONENT          MISCELLANEOUS
        MODULE_NAME         ("cmobject");


typedef struct Search_st 
{
    ACPI_OBJECT_INTERNAL        *InternalObj;
    UINT32                      Index;
    ACPI_OBJECT                 *ExternalObj;

} PKG_SEARCH_INFO;


/* Used to traverse nested packages */

PKG_SEARCH_INFO                 Level[MAX_PACKAGE_DEPTH];

typedef struct Internal_Search_st 
{
    ACPI_OBJECT_INTERNAL        *DestObj;
    UINT32                      Index;
    ACPI_OBJECT_INTERNAL        *SourceObj;

} INTERNAL_PKG_SEARCH_INFO;


/* Used to traverse nested packages when copying*/

INTERNAL_PKG_SEARCH_INFO        CopyLevel[MAX_PACKAGE_DEPTH];


/******************************************************************************
 *
 * FUNCTION:    CmBuildExternalSimpleObject
 *
 * PARAMETERS:  *InternalObj    - Pointer to the object we are examining
 *              *Buffer         - Where the object is returned
 *              *SpaceUsed      - Where the data length is returned
 * 
 * RETURN:      Status          - the status of the call
 * 
 * DESCRIPTION: This function is called to place a simple object in a user buffer.
 *
 *              The buffer is assumed to have sufficient space for the object.
 * 
 ******************************************************************************/

ACPI_STATUS
CmBuildExternalSimpleObject (
    ACPI_OBJECT_INTERNAL    *InternalObj,
    ACPI_OBJECT             *ExternalObj,
    UINT8                   *DataSpace,
    UINT32                  *BufferSpaceUsed)
{
    UINT32                  Length = 0;
    char                    *SourcePtr = NULL;


    FUNCTION_TRACE ("CmBuildExternalSimpleObject");

    MEMSET(ExternalObj , 0 , sizeof(ACPI_OBJECT));

    /* In general, the external object will be the same type as the internal object */

    ExternalObj->Type = InternalObj->Common.Type;

    /* However, only a limited number of external types are supported */

    switch (ExternalObj->Type)
    {

    case ACPI_TYPE_String:

        Length = InternalObj->String.Length;
        ExternalObj->String.Length = InternalObj->String.Length;
        ExternalObj->String.Pointer = (char *) DataSpace;
        SourcePtr = InternalObj->String.Pointer;
        break;


    case ACPI_TYPE_Buffer:

        Length = InternalObj->Buffer.Length;
        ExternalObj->Buffer.Length = InternalObj->Buffer.Length;
        ExternalObj->Buffer.Pointer = DataSpace;
        SourcePtr = (char *) InternalObj->Buffer.Pointer;
        break;


    case ACPI_TYPE_Number:

        ExternalObj->Number.Value= InternalObj->Number.Value;
        break;


    case INTERNAL_TYPE_Reference:

        /* 
         * This is an object reference.  We use the object type of "Any" to
         * indicate a reference object containing a handle to an ACPI
         * named object.
         */

        ExternalObj->Type = ACPI_TYPE_Any;
        ExternalObj->Reference.Handle = InternalObj->Reference.Nte;
        break;


    case ACPI_TYPE_Processor:

        ExternalObj->Processor.ProcId = InternalObj->Processor.ProcId;
        ExternalObj->Processor.PBLKAddress = InternalObj->Processor.PBLKAddress;
        ExternalObj->Processor.PBLKLength = InternalObj->Processor.PBLKLength;
        break;

    case ACPI_TYPE_Power:

        ExternalObj->PowerResource.SystemLevel = InternalObj->PowerResource.SystemLevel;
        ExternalObj->PowerResource.ResourceOrder = InternalObj->PowerResource.ResourceOrder;
        break;

    default:
        return_ACPI_STATUS (AE_RETURN_VALUE);
        break;
    }


    /* Copy data if necessary (strings or buffers) */

    if (Length)
    {
        /*
         * Copy the return data to the caller's buffer
         */
        MEMCPY ((void *) DataSpace, (void *) SourcePtr, Length);
    }


    *BufferSpaceUsed = Length;
    
    return_ACPI_STATUS (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    CmBuildExternalPackageObject
 *
 * PARAMETERS:  *InternalObj    - Pointer to the object we are returning
 *              *Buffer         - Where the object is returned
 *              *SpaceUsed      - Where the object length is returned
 * 
 * RETURN:      Status          - the status of the call
 * 
 * DESCRIPTION: This function is called to place a package object in a user
 *              buffer.  A package object by definition contains other objects.
 *
 *              The buffer is assumed to have sufficient space for the object.
 *              The caller must have verified the buffer length needed using the
 *              CmGetObjectSize function before calling this function.
 * 
 ******************************************************************************/

ACPI_STATUS
CmBuildExternalPackageObject (
    ACPI_OBJECT_INTERNAL    *InternalObj, 
    UINT8                   *Buffer, 
    UINT32                  *SpaceUsed)
{
    UINT8                   *FreeSpace;
    ACPI_OBJECT             *ExternalObj;
    UINT32                  CurrentDepth = 0;
    ACPI_STATUS             Status;
    UINT32                  Length = 0;
    UINT32                  ThisIndex;
    UINT32                  ObjectSpace;
    ACPI_OBJECT_INTERNAL    *ThisInternalObj;
    ACPI_OBJECT             *ThisExternalObj;
    PKG_SEARCH_INFO         *LevelPtr;
    

    FUNCTION_TRACE ("CmBuildExternalPackageObject");


    /*
     * First package at head of the buffer
     */
    ExternalObj = (ACPI_OBJECT *) Buffer;

    /*
     * Free space begins right after the first package
     */
    FreeSpace = Buffer + sizeof (ACPI_OBJECT);


    /*
     * Initialize the working variables
     */

    MEMSET ((void *) Level, 0, sizeof (Level));

    Level[0].InternalObj    = InternalObj;
    Level[0].ExternalObj    = ExternalObj;
    Level[0].Index          = 0;
    LevelPtr                = &Level[0];
    CurrentDepth            = 0;

    ExternalObj->Type               = InternalObj->Common.Type;
    ExternalObj->Package.Count      = InternalObj->Package.Count;
    ExternalObj->Package.Elements   = (ACPI_OBJECT *) FreeSpace;


    /*
     * Build an array of ACPI_OBJECTS in the buffer
     * and move the free space past it
     */

    FreeSpace += ExternalObj->Package.Count * sizeof (ACPI_OBJECT);


    while (1)
    {
        ThisIndex       = LevelPtr->Index;
        ThisInternalObj = (ACPI_OBJECT_INTERNAL *) LevelPtr->InternalObj->Package.Elements[ThisIndex];
        ThisExternalObj = (ACPI_OBJECT *) &LevelPtr->ExternalObj->Package.Elements[ThisIndex];

        if ((VALID_DESCRIPTOR_TYPE (ThisInternalObj, DESC_TYPE_ACPI_OBJ)) &&
            (IS_THIS_OBJECT_TYPE (ThisInternalObj, ACPI_TYPE_Package)))
        {
            /*
             * If this object is a package then we go one deeper
             */
            if (CurrentDepth >= MAX_PACKAGE_DEPTH-1)
            {
                /*
                 * Too many nested levels of packages for us to handle
                 */
                DEBUG_PRINT (ACPI_ERROR, ("CmBuildPackageObject: Pkg nested too deep (max %d)\n",
                                            MAX_PACKAGE_DEPTH));
                return_ACPI_STATUS (AE_LIMIT);
            }

            /*
             * Build the package object
             */
            ThisExternalObj->Type              = ACPI_TYPE_Package;
            ThisExternalObj->Package.Count     = ThisInternalObj->Package.Count;
            ThisExternalObj->Package.Elements  = (ACPI_OBJECT *) FreeSpace;

            /*
             * Save space for the array of objects (Package elements)
             * update the buffer length counter
             */
            ObjectSpace             = ThisExternalObj->Package.Count * sizeof(ACPI_OBJECT);
            FreeSpace               += ObjectSpace;
            Length                  += ObjectSpace;

            CurrentDepth++;
            LevelPtr                = &Level[CurrentDepth];
            LevelPtr->InternalObj   = ThisInternalObj;
            LevelPtr->ExternalObj   = ThisExternalObj;
            LevelPtr->Index         = 0;

        }   /* if object is a package */
        
        else
        {
            Status = CmBuildExternalSimpleObject (ThisInternalObj, ThisExternalObj, FreeSpace, &ObjectSpace);
            if (Status != AE_OK) 
            {
                /*
                 * Failure get out
                 */
                return_ACPI_STATUS (Status);
            }

            FreeSpace   += ObjectSpace;
            Length      += ObjectSpace;

            LevelPtr->Index++;
            while (LevelPtr->Index >= LevelPtr->InternalObj->Package.Count)
            {
                /*
                 * We've handled all of the objects at this level,  This means that we
                 * have just completed a package.  That package may have contained one
                 * or more packages itself
                 */
                if (CurrentDepth == 0)
                {
                    /*
                     * We have handled all of the objects in the top level package
                     * just add the length of the package objects and get out
                     */
                    *SpaceUsed = Length;
                    return_ACPI_STATUS (AE_OK);
                }

                /*
                 * go back up a level and move the index past the just completed
                 * package object.
                 */
                CurrentDepth--;
                LevelPtr = &Level[CurrentDepth];
                LevelPtr->Index++;
            }
        }   /* else object is NOT a package */
    }   /* while (1)  */


    /*
     * We'll never get here, but the compiler whines about return value
     */
    return_ACPI_STATUS (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    CmBuildExternalObject
 *
 * PARAMETERS:  *InternalObj    - The internal object to be converted
 *              *BufferPtr      - Where the object is returned
 * 
 * RETURN:      Status          - the status of the call
 * 
 * DESCRIPTION: This function is called to build an API object to be returned to
 *              the caller.
 *
 ******************************************************************************/

ACPI_STATUS
CmBuildExternalObject (
    ACPI_OBJECT_INTERNAL    *InternalObj, 
    ACPI_BUFFER             *RetBuffer)
{
    ACPI_STATUS             Status;


    FUNCTION_TRACE ("CmBuildExternalObject");


    if (IS_THIS_OBJECT_TYPE (InternalObj, ACPI_TYPE_Package))
    {
        /*
         * Package objects contain other objects (which can be objects)
         * buildpackage does it all
         */
        Status = CmBuildExternalPackageObject(InternalObj, RetBuffer->Pointer,
                        &RetBuffer->Length);
    }

    else
    {
        /*
         * Build a simple object (no nested objects)
         */
        Status = CmBuildExternalSimpleObject(InternalObj,
                        (ACPI_OBJECT *) RetBuffer->Pointer,
                        ((UINT8 *) RetBuffer->Pointer + sizeof (ACPI_OBJECT)),
                        &RetBuffer->Length);
        /*
         * build simple does not include the object size in the length
         * so we add it in here
         */
        RetBuffer->Length += sizeof (ACPI_OBJECT);
    }

    return_ACPI_STATUS (Status);
}


/******************************************************************************
 *
 * FUNCTION:    CmBuildInternalSimpleObject
 *
 * PARAMETERS:  *ExternalObj    - The external object to be converted
 *              *InternalObj    - Where the internal object is returned
 * 
 * RETURN:      Status          - the status of the call
 * 
 * DESCRIPTION: This function copies an external object to an internal one.
 *              NOTE: Pointers can be copied, we don't need to copy data.
 *              (The pointers have to be valid in our address space no matter
 *              what we do with them!)
 *
 ******************************************************************************/

ACPI_STATUS
CmBuildInternalSimpleObject (
    ACPI_OBJECT             *ExternalObj,
    ACPI_OBJECT_INTERNAL    *InternalObj)
{

    FUNCTION_TRACE ("CmBuildInternalSimpleObject");


    InternalObj->Common.Type = (UINT8) ExternalObj->Type;

    switch (ExternalObj->Type)
    {

    case ACPI_TYPE_String:

        InternalObj->String.Length      = ExternalObj->String.Length;
        InternalObj->String.Pointer     = ExternalObj->String.Pointer;
        break;


    case ACPI_TYPE_Buffer:

        InternalObj->Buffer.Length      = ExternalObj->Buffer.Length;
        InternalObj->Buffer.Pointer     = ExternalObj->Buffer.Pointer;
        break;


    case ACPI_TYPE_Number:
        /*
         * Number is included in the object itself
         */
        InternalObj->Number.Value           = ExternalObj->Number.Value;
        break;


    default:
        return_ACPI_STATUS (AE_RETURN_VALUE);
        break;
    }


    return_ACPI_STATUS (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    CmBuildInternalPackageObject
 *
 * PARAMETERS:  *InternalObj    - Pointer to the object we are returning
 *              *Buffer         - Where the object is returned
 *              *SpaceUsed      - Where the length of the object is returned
 * 
 * RETURN:      Status          - the status of the call
 * 
 * DESCRIPTION: This function is called to place a package object in a user
 *              buffer.  A package object by definition contains other objects.
 *
 *              The buffer is assumed to have sufficient space for the object.
 *              The caller must have verified the buffer length needed using the
 *              CmGetObjectSize function before calling this function.
 * 
 ******************************************************************************/

ACPI_STATUS
CmBuildInternalPackageObject (
    ACPI_OBJECT_INTERNAL    *InternalObj, 
    UINT8                   *Buffer, 
    UINT32                  *SpaceUsed)
{
    UINT8                   *FreeSpace;
    ACPI_OBJECT             *ExternalObj;
    UINT32                  CurrentDepth = 0;
    ACPI_STATUS             Status = AE_OK;
    UINT32                  Length = 0;
    UINT32                  ThisIndex;
    UINT32                  ObjectSpace = 0;
    ACPI_OBJECT_INTERNAL    *ThisInternalObj;
    ACPI_OBJECT             *ThisExternalObj;
    PKG_SEARCH_INFO         *LevelPtr;
    

    
    FUNCTION_TRACE ("CmBuildInternalPackageObject");


    /*
     * First package at head of the buffer
     */
    ExternalObj = (ACPI_OBJECT *)Buffer;

    /*
     * Free space begins right after the first package
     */
    FreeSpace = Buffer + sizeof(ACPI_OBJECT);


    /*
     * Initialize the working variables
     */
    
    MEMSET ((void *) Level, 0, sizeof(Level));

    Level[0].InternalObj    = InternalObj;
    Level[0].ExternalObj    = ExternalObj;
    LevelPtr                = &Level[0];
    CurrentDepth            = 0;

    ExternalObj->Type               = InternalObj->Common.Type;
    ExternalObj->Package.Count      = InternalObj->Package.Count;
    ExternalObj->Package.Elements   = (ACPI_OBJECT *)FreeSpace;


    /*
     * Build an array of ACPI_OBJECTS in the buffer
     * and move the free space past it
     */

    FreeSpace += ExternalObj->Package.Count * sizeof(ACPI_OBJECT);


    while (1)
    {
        ThisIndex       = LevelPtr->Index;
        ThisInternalObj = (ACPI_OBJECT_INTERNAL *) &LevelPtr->InternalObj->Package.Elements[ThisIndex];
        ThisExternalObj = (ACPI_OBJECT *) &LevelPtr->ExternalObj->Package.Elements[ThisIndex];

        if (IS_THIS_OBJECT_TYPE (ThisInternalObj, ACPI_TYPE_Package))
        {
            /*
             * If this object is a package then we go one deeper
             */
            if (CurrentDepth >= MAX_PACKAGE_DEPTH-1)
            {
                /*
                 * Too many nested levels of packages for us to handle
                 */
                DEBUG_PRINT (ACPI_ERROR, ("CmBuildPackageObject: Pkg nested too deep (max %d)\n",
                                            MAX_PACKAGE_DEPTH));
                return_ACPI_STATUS (AE_LIMIT);
            }

            /*
             * Build the package object
             */
            ThisExternalObj->Type               = ACPI_TYPE_Package;
            ThisExternalObj->Package.Count      = ThisInternalObj->Package.Count;
            ThisExternalObj->Package.Elements   = (ACPI_OBJECT *) FreeSpace;

            /*
             * Save space for the array of objects (Package elements)
             * update the buffer length counter
             */
            ObjectSpace             = ThisExternalObj->Package.Count * sizeof (ACPI_OBJECT);
            FreeSpace               += ObjectSpace;
            Length                  += ObjectSpace;

            CurrentDepth++;
            LevelPtr                = &Level[CurrentDepth];
            LevelPtr->InternalObj   = ThisInternalObj;
            LevelPtr->ExternalObj   = ThisExternalObj;
            LevelPtr->Index         = 0;

        }   /* if object is a package */
        
        else
        {
/*            Status = CmBuildSimpleObject(ThisInternalObj, ThisExternalObj, FreeSpace, &ObjectSpace);
*/
            if (Status != AE_OK) 
            {
                /*
                 * Failure get out
                 */
                return_ACPI_STATUS (Status);
            }

            FreeSpace   += ObjectSpace;
            Length      +=ObjectSpace;

            LevelPtr->Index++;
            while (LevelPtr->Index >= LevelPtr->InternalObj->Package.Count)
            {
                /*
                 * We've handled all of the objects at this level,  This means that we
                 * have just completed a package.  That package may have contained one
                 * or more packages itself
                 */
                if (CurrentDepth == 0)
                {
                    /*
                     * We have handled all of the objects in the top level package
                     * just add the length of the package objects and get out
                     */
                    *SpaceUsed = Length;
                    return_ACPI_STATUS (AE_OK);
                }

                /*
                 * go back up a level and move the index past the just completed
                 * package object.
                 */
                CurrentDepth--;
                LevelPtr = &Level[CurrentDepth];
                LevelPtr->Index++;
            }
        }   /* else object is NOT a package */
    }   /* while (1)  */


    /*
     * We'll never get here, but the compiler whines about return value
     */
    return_ACPI_STATUS (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    CmBuildInternalObject
 *
 * PARAMETERS:  *InternalObj    - The external object to be converted
 *              *BufferPtr      - Where the internal object is returned
 * 
 * RETURN:      Status          - the status of the call
 * 
 * DESCRIPTION: Converts an external object to an internal object.
 *
 ******************************************************************************/

ACPI_STATUS
CmBuildInternalObject (
    ACPI_OBJECT             *ExternalObj, 
    ACPI_OBJECT_INTERNAL    *InternalObj)
{
    ACPI_STATUS             Status;


    FUNCTION_TRACE ("CmBuildInternalObject");


    if (ExternalObj->Type == ACPI_TYPE_Package)
    {
        /*
         * Package objects contain other objects (which can be objects)
         * buildpackage does it all
         */
/*
        Status = CmBuildInternalPackageObject(InternalObj, RetBuffer->Pointer,
                        &RetBuffer->Length);
*/
        DEBUG_PRINT (ACPI_ERROR, ("CmBuildInternalObject: Packages as parameters not implemented!\n"));
        
        return_ACPI_STATUS (AE_NOT_IMPLEMENTED);
    }

    else
    {
        /*
         * Build a simple object (no nested objects)
         */
        Status = CmBuildInternalSimpleObject (ExternalObj, InternalObj);
        /*
         * build simple does not include the object size in the length
         * so we add it in here
         */
    }

    return_ACPI_STATUS (Status);
}

/******************************************************************************
 *
 * FUNCTION:    CmCopyInternalSimpleObject
 *
 * PARAMETERS:  *SourceObj      - The Internal object to be copied
 *              *DestObj        - Where the internal object is returned
 * 
 * RETURN:      Status          - the status of the call
 * 
 * DESCRIPTION: This function copies one internal object to another internal 
 *              one.
 *              NOTE: Pointers can be copied, we don't need to copy data.
 *              (The pointers have to be valid in our address space no matter
 *              what we do with them!)
 *
 ******************************************************************************/

ACPI_STATUS
CmCopyInternalSimpleObject (
    ACPI_OBJECT_INTERNAL    *SourceObj,
    ACPI_OBJECT_INTERNAL    *DestObj)
{

    FUNCTION_TRACE ("CmCopyInternalSimpleObject");

    /*
     * TBD: Type conversion should be done here!
     */
    if (DestObj->Common.Type != (UINT8) SourceObj->Common.Type)
    {
        DEBUG_PRINT (ACPI_ERROR, ("CmCopyInternalSimpleObject: Source type: 0x%x != Dest type: 0x%x!\n",
                SourceObj->Common.Type, DestObj->Common.Type));
        
        return_ACPI_STATUS (AE_NOT_IMPLEMENTED);
    }
    
    switch (SourceObj->Common.Type)
    {

    case ACPI_TYPE_String:

        DestObj->String.Length      = SourceObj->String.Length;
        DestObj->String.Pointer     = SourceObj->String.Pointer;
        break;


    case ACPI_TYPE_Buffer:

        DestObj->Buffer.Length      = SourceObj->Buffer.Length;
        DestObj->Buffer.Pointer     = SourceObj->Buffer.Pointer;
        break;


    case ACPI_TYPE_Number:
        /*
         * Number is included in the object itself
         */
        DestObj->Number.Value           = SourceObj->Number.Value;
        break;


    default:
        return_ACPI_STATUS (AE_RETURN_VALUE);
        break;
    }


    return_ACPI_STATUS (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    CmBuildCopyInternalPackageObject
 *
 * PARAMETERS:  *SourceObj      - Pointer to the source package object
 *              *DestObj        - Where the internal object is returned
 * 
 * RETURN:      Status          - the status of the call
 * 
 * DESCRIPTION: This function is called to copy an internal package object 
 *              into another internal package object.
 *
 ******************************************************************************/

ACPI_STATUS
CmBuildCopyInternalPackageObject (
    ACPI_OBJECT_INTERNAL    *SourceObj, 
    ACPI_OBJECT_INTERNAL    *DestObj)
{
    UINT32                      CurrentDepth = 0;
    ACPI_STATUS                 Status = AE_OK;
    UINT32                      Length = 0;
    UINT32                      ThisIndex;
    UINT32                      ObjectSpace = 0;
    ACPI_OBJECT_INTERNAL        *ThisDestObj;
    ACPI_OBJECT_INTERNAL        *ThisSourceObj;
    INTERNAL_PKG_SEARCH_INFO    *LevelPtr;
    

    
    FUNCTION_TRACE ("CmBuildCopyInternalPackageObject");

    /*
     * Initialize the working variables
     */
    
    MEMSET ((void *) CopyLevel, 0, sizeof(CopyLevel));

    CopyLevel[0].DestObj    = DestObj;
    CopyLevel[0].SourceObj  = SourceObj;
    LevelPtr                = &CopyLevel[0];
    CurrentDepth            = 0;

    DestObj->Common.Type        = SourceObj->Common.Type;
    DestObj->Package.Count      = SourceObj->Package.Count;


    /*
     * Build an array of ACPI_OBJECTS in the buffer
     * and move the free space past it
     */

    DestObj->Package.Elements   = CmCallocate ((ACPI_SIZE) (DestObj->Package.Count + 1) *
                                                     sizeof (void *));
    if (!DestObj->Package.Elements)
    {
        /* Package vector allocation failure   */

        REPORT_ERROR ("CmBuildCopyInternalPackageObj: Package vector allocation failure");
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    DestObj->Package.NextElement = DestObj->Package.Elements;


    while (1)
    {
        ThisIndex       = LevelPtr->Index;
        ThisDestObj     = (ACPI_OBJECT_INTERNAL *) LevelPtr->DestObj->Package.Elements[ThisIndex];
        ThisSourceObj   = (ACPI_OBJECT_INTERNAL *) LevelPtr->SourceObj->Package.Elements[ThisIndex];

        if (IS_THIS_OBJECT_TYPE (ThisSourceObj, ACPI_TYPE_Package))
        {
            /*
             * If this object is a package then we go one deeper
             */
            if (CurrentDepth >= MAX_PACKAGE_DEPTH-1)
            {
                /*
                 * Too many nested levels of packages for us to handle
                 */
                DEBUG_PRINT (ACPI_ERROR, ("CmBuildCopyPackageObject: Pkg nested too deep (max %d)\n",
                                            MAX_PACKAGE_DEPTH));
                return_ACPI_STATUS (AE_LIMIT);
            }

            /*
             * Build the package object
             */
            ThisDestObj = CmCreateInternalObject (ACPI_TYPE_Package);
            LevelPtr->DestObj->Package.Elements[ThisIndex] = ThisDestObj;


            ThisDestObj->Common.Type        = ACPI_TYPE_Package;
            ThisDestObj->Package.Count      = ThisDestObj->Package.Count;

            /*
             * Save space for the array of objects (Package elements)
             * update the buffer length counter
             */
            ObjectSpace             = ThisDestObj->Package.Count * sizeof (ACPI_OBJECT_INTERNAL);
            Length                  += ObjectSpace;

            CurrentDepth++;
            LevelPtr                = &CopyLevel[CurrentDepth];
            LevelPtr->DestObj       = ThisDestObj;
            LevelPtr->SourceObj     = ThisSourceObj;
            LevelPtr->Index         = 0;

        }   /* if object is a package */
        
        else
        {
            
            ThisDestObj = CmCreateInternalObject (ThisSourceObj->Common.Type);
            LevelPtr->DestObj->Package.Elements[ThisIndex] = ThisDestObj;

            Status = CmCopyInternalSimpleObject(ThisSourceObj, ThisDestObj);

            if (Status != AE_OK) 
            {
                /*
                 * Failure get out
                 */
                return_ACPI_STATUS (Status);
            }

            Length      +=ObjectSpace;

            LevelPtr->Index++;
            while (LevelPtr->Index >= LevelPtr->DestObj->Package.Count)
            {
                /*
                 * We've handled all of the objects at this level,  This means that we
                 * have just completed a package.  That package may have contained one
                 * or more packages itself
                 */
                if (CurrentDepth == 0)
                {
                    /*
                     * We have handled all of the objects in the top level package
                     * just add the length of the package objects and get out
                     */
                    // *SpaceUsed = Length;
                    return_ACPI_STATUS (AE_OK);
                }

                /*
                 * go back up a level and move the index past the just completed
                 * package object.
                 */
                CurrentDepth--;
                LevelPtr = &CopyLevel[CurrentDepth];
                LevelPtr->Index++;
            }
        }   /* else object is NOT a package */
    }   /* while (1)  */


    /*
     * We'll never get here, but the compiler whines about return value
     */
    return_ACPI_STATUS (AE_OK);
}


