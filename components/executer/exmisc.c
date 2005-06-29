
/******************************************************************************
 * 
 * Module Name: ieopexec - ACPI AML (p-code) execution - specific opcodes
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

#define __IEOPEXEC_C__

#include <acpi.h>
#include <parser.h>
#include <interpreter.h>
#include <amlcode.h>
#include <namespace.h>
#include <events.h>


#define _COMPONENT          INTERPRETER
        MODULE_NAME         ("ieopexec");


 
 /*****************************************************************************
 * 
 * FUNCTION:    AmlExecCreateField
 *
 * PARAMETERS:  Opcode              - The opcode to be executed
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Execute CreateField operators: CreateBitFieldOp,
 *              CreateByteFieldOp, CreateWordFieldOp, CreateDWordFieldOp,
 *              CreateFieldOp (which define fields in buffers)
 *
 * ALLOCATION:  Deletes CreateFieldOp's count operand descriptor
 *
 *
 *  ACPI SPECIFICATION REFERENCES:
 *  16.2.4.2    DefCreateBitField   :=  CreateBitFieldOp    SourceBuff  BitIndex    NameString
 *  16.2.4.2    DefCreateByteField  :=  CreateByteFieldOp   SourceBuff  ByteIndex   NameString
 *  16.2.4.2    DefCreateDWordField :=  CreateDWordFieldOp  SourceBuff  ByteIndex   NameString
 *  16.2.4.2    DefCreateField      :=  CreateFieldOp       SourceBuff  BitIndex    NumBits     NameString
 *  16.2.4.2    DefCreateWordField  :=  CreateWordFieldOp   SourceBuff  ByteIndex   NameString
 *  16.2.4.2    BitIndex            :=  TermArg=>Integer
 *  16.2.4.2    ByteIndex           :=  TermArg=>Integer
 *  16.2.4.2    NumBits             :=  TermArg=>Integer
 *  16.2.4.2    SourceBuff          :=  TermArg=>Buffer
 *
 ****************************************************************************/

ACPI_STATUS
AmlExecCreateField (
    UINT16                  Opcode)
{
    ACPI_OBJECT_INTERNAL    *ResDesc = NULL;
    ACPI_OBJECT_INTERNAL    *CntDesc = NULL;
    ACPI_OBJECT_INTERNAL    *OffDesc = NULL;
    ACPI_OBJECT_INTERNAL    *SrcDesc = NULL;
    ACPI_OBJECT_INTERNAL    *FieldDesc;
    ACPI_OBJECT_TYPE        ResType;
    ACPI_STATUS             Status;
    char                    *OpName = NULL;
    UINT32                  NumOperands;
    UINT16                  BitCount;
    UINT32                  BitOffset;
    UINT32                  StackIndex;
    UINT32                  Offset;
    UINT8                   TypeFound;



    FUNCTION_TRACE ("AmlExecCreateField");


    /* DefCreateField := CreateFieldOp SourceBuff BitIndex NumBits NameString  */

    if (AML_CreateFieldOp == Opcode)
    {
        Status = AmlPrepObjStack ("lnnb");
        NumOperands = 4;
    }

    
    /* 
     * Create[Bit|Byte|DWord|Word]Field
     * DefCreate*Field := Create*FieldOp SourceBuff [Bit|Byte]Index NameString
     */

    else
    {
        Status = AmlPrepObjStack ("lnb");
        NumOperands = 3;
    }

    if (Status != AE_OK)
    {
        /* Invalid parameters on object stack  */

        AmlAppendOperandDiag (_THIS_MODULE, __LINE__, Opcode, NumOperands);
        return_ACPI_STATUS (Status);
    }

    /* Get pointers to everything that is now on the object stack */

    OpName = PsGetOpcodeName (Opcode);
    AmlDumpObjStack (IMODE_Execute, OpName, NumOperands, "after AmlPrepObjStack");

    StackIndex = 0;
    ResDesc = AmlObjStackGetValue (StackIndex++);           /* result */
    
    if (AML_CreateFieldOp == Opcode)
    {
        CntDesc = AmlObjStackGetValue (StackIndex++);           /* count */
    }

    OffDesc = AmlObjStackGetValue (StackIndex++);           /* offset */
    Offset = OffDesc->Number.Value;

    SrcDesc = AmlObjStackGetValue (StackIndex++);           /* source */

    /* If ResDesc is a Name, it will be a direct name pointer after AmlPrepObjStack() */
    
    if (!VALID_DESCRIPTOR_TYPE (ResDesc, DESC_TYPE_NTE))
    {
        DEBUG_PRINT (ACPI_ERROR, ("AmlExecCreateField (%s): destination must be a Name(NTE)\n", OpName));
        return_ACPI_STATUS (AE_AML_ERROR);
    }


    /*
     * Setup the Bit offsets and counts, according to the opcode
     */

    switch (Opcode)
    {

    /* DefCreateBitField   :=  CreateBitFieldOp    SourceBuff  BitIndex    NameString  */

    case AML_BitFieldOp:

        BitOffset = Offset;                         /* offset is in bits */
        BitCount = 1;                               /* field is a bit */
        break;


    /* DefCreateByteField  :=  CreateByteFieldOp   SourceBuff  ByteIndex   NameString  */

    case AML_ByteFieldOp:

        BitOffset = 8 * Offset;                     /* offset is in bytes */
        BitCount = 8;                               /* field is a Byte */
        break;


    /* DefCreateWordField  :=  CreateWordFieldOp   SourceBuff  ByteIndex   NameString  */

    case AML_WordFieldOp:

        BitOffset = 8 * Offset;                     /* offset is in bytes */
        BitCount = 16;                              /* field is a Word */
        break;


    /* DefCreateDWordField :=  CreateDWordFieldOp  SourceBuff  ByteIndex   NameString  */

    case AML_DWordFieldOp:

        BitOffset = 8 * Offset;                     /* offset is in bytes */
        BitCount = 32;                              /* field is a DWord */
        break;


    /* DefCreateField  :=  CreateFieldOp   SourceBuff  BitIndex    NumBits NameString  */

    case AML_CreateFieldOp:

        BitOffset = Offset;                         /* offset is in bits */
        BitCount = (UINT16) CntDesc->Number.Value;  /* as is count */
        break;


    default:

        DEBUG_PRINT (ACPI_ERROR, (
                "AmlExecCreateField: Internal error - unknown field creation opcode %02x\n",
                Opcode));
        return_ACPI_STATUS (AE_AML_ERROR);

    } /* switch */



    /*
     * Setup field according to the object type
     */

    switch (SrcDesc->Common.Type)
    {
    
    /* SourceBuff  :=  TermArg=>Buffer */

    case ACPI_TYPE_Buffer:

        if (BitOffset + (UINT32) BitCount > 8 * (UINT32) SrcDesc->Buffer.Length)
        {
            DEBUG_PRINT (ACPI_ERROR, ("AmlExecCreateField: Field exceeds Buffer %d > %d\n",
                            BitOffset + (UINT32) BitCount,
                            8 * (UINT32) SrcDesc->Buffer.Length));
            return_ACPI_STATUS (AE_AML_ERROR);
        }


        /* Allocate a temporary object for the field */

        FieldDesc = CmCreateInternalObject (ACPI_TYPE_FieldUnit);
        if (!FieldDesc)
        {
            return_ACPI_STATUS (AE_NO_MEMORY);
        }

        /* Construct the field object */

        FieldDesc->FieldUnit.Access       = (UINT16) ACCESS_AnyAcc;
        FieldDesc->FieldUnit.LockRule     = (UINT16) GLOCK_NeverLock;
        FieldDesc->FieldUnit.UpdateRule   = (UINT16) UPDATE_Preserve;
        FieldDesc->FieldUnit.Length       = BitCount;
        FieldDesc->FieldUnit.BitOffset    = (UINT16) BitOffset % 8;
        FieldDesc->FieldUnit.Offset       = BitOffset / 8;
        FieldDesc->FieldUnit.Container    = SrcDesc;
        FieldDesc->FieldUnit.Sequence     = SrcDesc->Buffer.Sequence;

        /* An additional reference for SrcDesc */

        CmUpdateObjectReference (SrcDesc, REF_INCREMENT);

        break;


    /* Improper object type */

    default:

        TypeFound = SrcDesc->Common.Type;

        if ((TypeFound > (UINT8) INTERNAL_TYPE_Lvalue) ||
            (Gbl_BadType == Gbl_NsTypeNames[TypeFound]))
        {
            DEBUG_PRINT (ACPI_ERROR, (
                    "AmlExecCreateField: Tried to create field in improper object type - encoding %d\n",
                    TypeFound));
        }

        else
        {
            DEBUG_PRINT (ACPI_ERROR, (
                    "AmlExecCreateField: Tried to create field in improper object type - %s\n",
                    Gbl_NsTypeNames[TypeFound]));
        }

        return_ACPI_STATUS (AE_AML_ERROR);
    
    } /* switch */


    if (AML_CreateFieldOp == Opcode)
    {
        /* Delete object descriptor unique to CreateField  */

        CmDeleteInternalObject (CntDesc);
        CntDesc = NULL;
    }

    /* 
     * This operation is supposed to cause the destination Name to refer
     * to the defined FieldUnit -- it must not store the constructed
     * FieldUnit object (or its current value) in some location that the
     * Name may already be pointing to.  So, if the Name currently contains
     * a reference which would cause AmlExecStore() to perform an indirect
     * store rather than setting the value of the Name itself, clobber that
     * reference before calling AmlExecStore().
     */

    ResType = NsGetType (ResDesc);
    switch (ResType)                /* Type of Name's existing value */
    {

    case ACPI_TYPE_FieldUnit:

    case INTERNAL_TYPE_Alias:
    case INTERNAL_TYPE_BankField:
    case INTERNAL_TYPE_DefField:
    case INTERNAL_TYPE_IndexField:

        DUMP_PATHNAME (ResDesc, "AmlExecCreateField: clobber ", TRACE_BFIELD, _COMPONENT);

        DUMP_ENTRY (ResDesc, TRACE_BFIELD);
        DUMP_STACK_ENTRY (NsGetAttachedObject (ResDesc));
        
        NsAttachObject (ResDesc, NULL, ACPI_TYPE_Any);
        break;


    default:

        break;
    }


    /* Store constructed field descriptor in result location */
    
    Status = AmlExecStore (FieldDesc, ResDesc);


    /* All done with the temp field descriptor */

    CmDeleteInternalObject (FieldDesc);


    /* Delete the parameters */

    CmDeleteInternalObject (SrcDesc);
    CmDeleteInternalObject (OffDesc);

    /* 
     * Pop off everything from the stack except the result,
     * which we want to leave sitting at the stack top.
     */
    AmlObjStackPop (NumOperands - 1);


    return_ACPI_STATUS (Status);
}


/*****************************************************************************
 *
 * FUNCTION:    AmlExecCreateAlias
 *
 * PARAMETERS:  None
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Create a new named alias
 *
 ****************************************************************************/

ACPI_STATUS
AmlExecCreateAlias (void)
{
    ACPI_STATUS             Status = AE_NOT_IMPLEMENTED;
/*    ACPI_OBJECT_INTERNAL    *ObjDesc; */


    FUNCTION_TRACE ("AmlExecCreateAlias");


 BREAKPOINT3;

    return_ACPI_STATUS (Status);

}


/*****************************************************************************
 *
 * FUNCTION:    AmlExecCreateEvent 
 *
 * PARAMETERS:  None
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Create a new event object
 *
 ****************************************************************************/

ACPI_STATUS
AmlExecCreateEvent (void)
{
    ACPI_STATUS             Status;
    ACPI_OBJECT_INTERNAL    *ObjDesc;


    FUNCTION_TRACE ("AmlExecCreateEvent");


 BREAKPOINT3;

    ObjDesc = CmCreateInternalObject (ACPI_TYPE_Event);
    if (!ObjDesc)
    {
        /* Allocation failure */

        Status = AE_NO_MEMORY;
        goto Cleanup;
    }

    /* ObjDesc valid */
    
    if ((Status = AmlObjStackPushIfExec (IMODE_Execute)) == AE_OK)
    {
        /* Push goes into cleared space, so no CmDeleteInternalObject() */
        
        AmlObjStackSetValue (STACK_TOP, ObjDesc);

    
         /* Value on top, name below it */

        Status = NsAttachObject (AmlObjStackGetValue (1), AmlObjStackGetValue (STACK_TOP), 
                                (UINT8) ACPI_TYPE_Event);
    }

    if (ACPI_FAILURE (Status))
    {
        CmDeleteInternalObject (ObjDesc);
        goto Cleanup;
    }



Cleanup:

    /* Always delete the operand */

    AmlObjStackDeleteValue (STACK_TOP);
    AmlObjStackPop (1);

    return_ACPI_STATUS (Status);
}


/*****************************************************************************
 *
 * FUNCTION:    AmlExecCreateRegion
 *
 * PARAMETERS:  AmlPtr              - Pointer to the region declaration AML
 *              AmlLength           - Max length of the declaration AML
 *              InterpreterMode     - Load1/Load2/Execute
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Create a new operation region object
 *
 ****************************************************************************/

ACPI_STATUS
AmlExecCreateRegion (
    UINT8                   *AmlPtr,
    UINT32                  AmlLength,
    OPERATING_MODE          InterpreterMode)

{
    ACPI_STATUS             Status;
    ACPI_OBJECT_INTERNAL    *SpaceIdDesc;
    ACPI_OBJECT_INTERNAL    *AddressDesc;
    ACPI_OBJECT_INTERNAL    *LengthDesc;
    ACPI_OBJECT_INTERNAL    *ObjDescRegion;
    ACPI_HANDLE             *Entry;
    UINT32                  RegionSpace;


    FUNCTION_TRACE ("AmlExecCreateRegion");


    /* Get operands */

    LengthDesc  = AmlObjStackGetValue (0);
    AddressDesc = AmlObjStackGetValue (1);
    SpaceIdDesc = AmlObjStackGetValue (2);
    Entry       = AmlObjStackGetValue (3);


    /* Create the region descriptor */

    ObjDescRegion = CmCreateInternalObject (ACPI_TYPE_Region);
    if (!ObjDescRegion)
    {
        Status = AE_NO_MEMORY;
        goto Cleanup;
    }

    /* Init the region from the operands */

    RegionSpace = SpaceIdDesc->Number.Value;

    ObjDescRegion->Region.SpaceId   = RegionSpace;
    ObjDescRegion->Region.Address   = 0;
    ObjDescRegion->Region.Length    = 0;
    ObjDescRegion->Region.DataValid = 0;

    /* Decode the RegionSpace */

    if (RegionSpace >= NUM_REGION_TYPES)
    {
        DEBUG_PRINT (TRACE_LOAD, ("AmlDoNamedObject: Type out of range [*???*]\n"));
        REPORT_WARNING ("Unable to decode the RegionSpace");
    }

    else
    {
        DEBUG_PRINT (TRACE_LOAD, ("AmlDoNamedObject: Region Type [%s]\n",
                    Gbl_RegionTypes[RegionSpace]));
    }

    if ((IMODE_LoadPass1 == InterpreterMode) || 
        (IMODE_LoadPass2 == InterpreterMode))
    {
        /*
         * If not done already, we must allocate a method object
         * for this region.
         */
        if (!ObjDescRegion->Region.Method)
        {
            ObjDescRegion->Region.Method =
                CmCreateInternalObject (ACPI_TYPE_Method);

            if (!ObjDescRegion->Region.Method)
            {
                Status = AE_NO_MEMORY;
                /* Delete ObjDescRegion */
                goto Cleanup;
            }
        }

        /* 
         * Remember location in AML stream of address & length
         * operands since they need to be evaluated at run time.
         */
        ObjDescRegion->Region.Method->Method.Pcode       = AmlPtr;
        ObjDescRegion->Region.Method->Method.PcodeLength = AmlLength;
    }

        
    if (IMODE_Execute == InterpreterMode)
    {
        /* 
         * Record value computed for the "region base address" expression
         */
        
        if (VALID_DESCRIPTOR_TYPE (AddressDesc, DESC_TYPE_ACPI_OBJ))
        {
            if (AddressDesc->Common.Type != (UINT8) ACPI_TYPE_Number)
            {
                /* Object is not of type Number */

                DEBUG_PRINT (ACPI_ERROR, 
                    ("AmlDoNamedObject/RegionOp: Malformed Region/Address\n"));

                Status = AE_AML_ERROR;
                goto Cleanup;
            }

            ObjDescRegion->Region.Address = AddressDesc->Number.Value;
        }



        /* 
         * Record value computed for the "region length" expression 
         */
    
        LengthDesc = AmlObjStackGetValue (STACK_TOP);

        if (VALID_DESCRIPTOR_TYPE (LengthDesc, DESC_TYPE_ACPI_OBJ))
        {
            if (LengthDesc->Common.Type != (UINT8) ACPI_TYPE_Number)
            {
                /* Object is not of type Number */

                DEBUG_PRINT (ACPI_ERROR, 
                    ("AmlDoNamedObject/RegionOp: Malformed Region/Length\n"));

                Status = AE_AML_ERROR;
                goto Cleanup;
            }

            ObjDescRegion->Region.Length = LengthDesc->Number.Value;
            ObjDescRegion->Region.DataValid = 1;
        }
    } 

                    
    /* Install the new region object in the parent NTE */

    ObjDescRegion->Region.Nte = (NAME_TABLE_ENTRY *) Entry;

    Status = NsAttachObject (Entry, ObjDescRegion, (UINT8) ACPI_TYPE_Region);
    if (ACPI_FAILURE (Status))
    {
        goto Cleanup;
    }




Cleanup:

    if (Status != AE_OK)
    {
        /* Delete region object and method subobject */

        if (ObjDescRegion)
        {
            CmDeleteInternalObject (ObjDescRegion);     /* Deletes both objects! */
            ObjDescRegion = NULL;
        }
    }

    /* Delete operands and pop stack */

    AmlObjStackDeleteValue (STACK_TOP);
    AmlObjStackDeleteValue (1);
    AmlObjStackDeleteValue (2);
    AmlObjStackPop (3);

    /*
     * If we have a valid region and we are in load pass two, 
     * now is the time to initialize the region.
     * (The object stack is completely cleared.  Not executing, no nested methods)
     */
    if (ObjDescRegion)
    {      
        if (IMODE_LoadPass2 == InterpreterMode)
        {
            /*
             *  TBD: Is there anything we can or could do when this fails?
             *          We need to do something useful with a failure.
             */
            (void *) EvInitializeRegion (ObjDescRegion);
        }

    }

    return_ACPI_STATUS (Status);
}


/*****************************************************************************
 *
 * FUNCTION:    AmlExecCreateMutex
 *
 * PARAMETERS:  InterpreterMode     - Current running mode (load1/Load2/Exec)
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Create a new mutex object
 *
 ****************************************************************************/

ACPI_STATUS
AmlExecCreateMutex (
    OPERATING_MODE          InterpreterMode)

{
    ACPI_STATUS             Status = AE_OK;
    ACPI_OBJECT_INTERNAL    *ObjDesc;


    
    FUNCTION_TRACE ("AmlExecCreateMutex");


    if (IMODE_LoadPass2 == InterpreterMode)
    {   
        /* 
         * Successful execution
         * Convert the Number from AmlDoByteConst() into a Mutex 
         */

        ObjDesc = AmlObjStackGetValue (STACK_TOP);
        if (!ObjDesc)
        {
            /* Attempt to allocate a new object */

            ObjDesc = CmCreateInternalObject (ACPI_TYPE_Mutex);
            if (!ObjDesc)
            {
                Status = AE_NO_MEMORY;
                goto Cleanup;
            }

        }

        if (VALID_DESCRIPTOR_TYPE (ObjDesc, DESC_TYPE_ACPI_OBJ))
        {
            ObjDesc->Mutex.Type      = (UINT8) ACPI_TYPE_Mutex;
            ObjDesc->Mutex.SyncLevel = (UINT8) ObjDesc->Number.Value;
            ObjDesc->Mutex.Semaphore = 0;

            /* ObjDesc was on the stack top, and the name is below it */

            Status = NsAttachObject (AmlObjStackGetValue (1),  /* Name */
                                        ObjDesc, (UINT8) ACPI_TYPE_Mutex);
        } 
    }


Cleanup:

    /* Always delete the operand */

    AmlObjStackDeleteValue (STACK_TOP);
    AmlObjStackPop (1);


    return_ACPI_STATUS (Status);
}



/*****************************************************************************
 *
 * FUNCTION:    AmlExecCreateMutex
 *
 * PARAMETERS:  InterpreterMode     - Current running mode (load1/Load2/Exec)
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Create a new mutex object
 *
 ****************************************************************************/

ACPI_STATUS
AmlExecCreateMethod (
    UINT8                   *AmlPtr,
    UINT32                  AmlLength,
    UINT32                  MethodFlags,
    ACPI_HANDLE             Method)
{
    ACPI_OBJECT_INTERNAL    *ObjDesc;
    ACPI_STATUS             Status;


    ObjDesc = CmCreateInternalObject (ACPI_TYPE_Method);
    if (!ObjDesc)
    {
       return AE_NO_MEMORY;
    }

    /* Get the method's AML pointer/length from the Op */

    ObjDesc->Method.Pcode       = AmlPtr;
    ObjDesc->Method.PcodeLength = AmlLength;

    /* First argument is the Method Flags (contains parameter count for the method) */

    ObjDesc->Method.MethodFlags = MethodFlags;
    ObjDesc->Method.ParamCount =  MethodFlags & ACPI_METHOD_ARG_MASK;
    
    /* Method is not parsed yet */

    ObjDesc->Method.ParserOp = NULL;

    /* Another  +1 gets added when PsxExecute is called, no need for: ObjDesc->Method.Pcode++; */

    ObjDesc->Method.AcpiTable = NULL;   /* TBD: was (UINT8 *) PcodeAddr; */
    ObjDesc->Method.TableLength = 0;    /* TBD: needed? (UINT32) (WalkState->amlEnd - PcodeAddr); */

    Status = NsAttachObject (Method, ObjDesc, (UINT8) ACPI_TYPE_Method);
    if (ACPI_FAILURE (Status))
    {
        CmFree (ObjDesc);
    }

    return Status;
}
    


/*****************************************************************************
 * 
 * FUNCTION:    AmlExecFatal
 *
 * PARAMETERS:  none
 *
 * RETURN:      Status - AE_AML_ERROR on success
 *
 * DESCRIPTION: Execute Fatal operator
 *
 *  ACPI SPECIFICATION REFERENCES:
 *  16.2.4.3    DefFatal    :=  FatalOp FatalType   FatalCode   FatalArg
 *  16.2.4.3    FatalType   :=  ByteData
 *  16.2.4.3    FatalCode   :=  DWordData
 *  16.2.4.3    FatalArg    :=  TermArg=>Integer
 *
 ****************************************************************************/

ACPI_STATUS
AmlExecFatal (void)
{
    ACPI_OBJECT_INTERNAL    *TypeDesc;
    ACPI_OBJECT_INTERNAL    *CodeDesc;
    ACPI_OBJECT_INTERNAL    *ArgDesc;
    ACPI_STATUS             Status;


    FUNCTION_TRACE ("AmlExecFatal");


    Status = AmlPrepObjStack ("nnn");

    if (Status != AE_OK)
    {
        /* invalid parameters on object stack  */

        AmlAppendOperandDiag (_THIS_MODULE, __LINE__, (UINT16) AML_FatalOp, 3);
        return_ACPI_STATUS (Status);
    }

    AmlDumpObjStack (IMODE_Execute, PsGetOpcodeName (AML_FatalOp), 3, "after AmlPrepObjStack");


    /* DefFatal    :=  FatalOp FatalType   FatalCode   FatalArg    */

    ArgDesc  = AmlObjStackGetValue (0);
    CodeDesc = AmlObjStackGetValue (1);
    TypeDesc = AmlObjStackGetValue (2);

    DEBUG_PRINT (ACPI_INFO,
                ("FatalOp: Type %x Code %x Arg %x <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n",
                TypeDesc->Number.Value, CodeDesc->Number.Value,
                ArgDesc->Number.Value));

    DEBUG_PRINT (ACPI_ERROR, ("AmlExecFatal: FatalOp executed\n"));
    return_ACPI_STATUS (AE_AML_ERROR);
}


/*****************************************************************************
 * 
 * FUNCTION:    AmlExecIndex
 *
 * PARAMETERS:  none
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Execute Index operator
 *
 * ALLOCATION:  Deletes one operand descriptor -- other remains on stack
 *
 *  ACPI SPECIFICATION REFERENCES:
 *  16.2.4.4    DefIndex    :=  IndexOp BuffPkgObj IndexValue Result
 *  16.2.4.4    IndexValue  :=  TermArg=>Integer
 *  16.2.1      NameString  :=  <RootChar NamePath> | <PrefixPath NamePath>
 *  16.2.4.4    Result      :=  SuperName
 *  16.2.1      SuperName   :=  NameString | ArgObj | LocalObj | DebugObj | DefIndex
 *                              Local4Op | Local5Op | Local6Op | Local7Op
 *
 ****************************************************************************/

ACPI_STATUS
AmlExecIndex (void)
{
    ACPI_OBJECT_INTERNAL    *PkgDesc;
    ACPI_OBJECT_INTERNAL    *IdxDesc;
    ACPI_OBJECT_INTERNAL    *ResDesc;
    ACPI_STATUS             Status;


    FUNCTION_TRACE ("AmlExecIndex");


    Status = AmlPrepObjStack ("lnp");

    if (Status != AE_OK)
    {
        /* invalid parameters on object stack  */

        AmlAppendOperandDiag (_THIS_MODULE, __LINE__, (UINT16) AML_IndexOp, 3);
    }

    else
    {
        AmlDumpObjStack (IMODE_Execute, PsGetOpcodeName (AML_IndexOp), 3, "after AmlPrepObjStack");

        ResDesc = AmlObjStackGetValue (0);
        IdxDesc = AmlObjStackGetValue (1);
        PkgDesc = AmlObjStackGetValue (2);

        if (IdxDesc->Number.Value < 0 || 
            IdxDesc->Number.Value >= (UINT32) PkgDesc->Package.Count)
        {
            DEBUG_PRINT (ACPI_ERROR, ("AmlExecIndex: Index value out of range\n"));
            Status = AE_AML_ERROR;
        }

        else
        {
            /* 
             * TBD - possible dangling reference: if the package vector changes
             * TBD - before this pointer is used, the results may be surprising.
             */
            PkgDesc->Lvalue.Object  = (void *) &PkgDesc->Package.Elements[IdxDesc->Number.Value];
            PkgDesc->Common.Type    = (UINT8) INTERNAL_TYPE_Lvalue;
            PkgDesc->Lvalue.OpCode  = AML_IndexOp;

            Status = AmlExecStore (PkgDesc, ResDesc);
        }

        if (AE_OK == Status)
        {
            CmDeleteInternalObject (IdxDesc);
        }

        /* Pop and clear two stack entries */

        AmlObjStackPop (2);
    }

    return_ACPI_STATUS (Status);
}


/*****************************************************************************
 * 
 * FUNCTION:    AmlExecMatch
 *
 * PARAMETERS:  none
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Execute Match operator
 *
 * ALLOCATION:  Deletes 5 operands
 *
 *  ACPI SPECIFICATION REFERENCES:
 *  16.2.4.4    DefMatch    :=  MatchOp SearchPkg   Opcode1     Operand1
 *                              Opcode2 Operand2    StartIndex
 *  16.2.4.4    Opcode1     :=  ByteData: MTR, MEQ, MLE, MLT, MGE, or MGT
 *  16.2.4.4    Opcode2     :=  ByteData: MTR, MEQ, MLE, MLT, MGE, or MGT
 *  16.2.4.4    Operand1    :=  TermArg=>Integer
 *  16.2.4.4    Operand2    :=  TermArg=>Integer
 *  16.2.4.4    SearchPkg   :=  TermArg=>PackageObject
 *  16.2.4.4    StartIndex  :=  TermArg=>Integer
 *
 ****************************************************************************/

ACPI_STATUS
AmlExecMatch (void)
{
    ACPI_OBJECT_INTERNAL    *PkgDesc;
    ACPI_OBJECT_INTERNAL    *Op1Desc;
    ACPI_OBJECT_INTERNAL    *V1Desc;
    ACPI_OBJECT_INTERNAL    *Op2Desc;
    ACPI_OBJECT_INTERNAL    *V2Desc;
    ACPI_OBJECT_INTERNAL    *StartDesc;
    ACPI_STATUS             Status;
    UINT32                  Index;
    UINT32                  MatchValue = (UINT32) -1;


    FUNCTION_TRACE ("AmlExecMatch");


    Status = AmlPrepObjStack ("nnnnnp");

    if (Status != AE_OK)
    {
        /* invalid parameters on object stack  */

        AmlAppendOperandDiag (_THIS_MODULE, __LINE__, (UINT16) AML_MatchOp, 6);
        return_ACPI_STATUS (Status);
    }


    /* Get the parameters from the object stack */

    AmlDumpObjStack (IMODE_Execute, PsGetOpcodeName (AML_MatchOp), 6, "after AmlPrepObjStack");

    StartDesc = AmlObjStackGetValue (0);
    V2Desc    = AmlObjStackGetValue (1);
    Op2Desc   = AmlObjStackGetValue (2);
    V1Desc    = AmlObjStackGetValue (3);
    Op1Desc   = AmlObjStackGetValue (4);
    PkgDesc   = AmlObjStackGetValue (5);

    /* Validate match comparison sub-opcodes */
    
    if (Op1Desc->Number.Value < 0 || Op1Desc->Number.Value > 5 || 
        Op2Desc->Number.Value < 0 || Op2Desc->Number.Value > 5)
    {
        DEBUG_PRINT (ACPI_ERROR, ("AmlExecMatch: operation encoding out of range\n"));
        return_ACPI_STATUS (AE_AML_ERROR);
    }

    Index = StartDesc->Number.Value;
    if (Index < 0 || Index >= (UINT32) PkgDesc->Package.Count)
    {
        DEBUG_PRINT (ACPI_ERROR, ("AmlExecMatch: start position value out of range\n"));
        return_ACPI_STATUS (AE_AML_ERROR);
    }

    /* 
     * Examine each element until a match is found.  Within the loop,
     * "continue" signifies that the current element does not match
     * and the next should be examined.
     * Upon finding a match, the loop will terminate via "break" at
     * the bottom.  If it terminates "normally", MatchValue will be -1
     * (its initial value) indicating that no match was found.  When
     * returned as a Number, this will produce the Ones value as specified.
     */

    for ( ; Index < (UINT32) PkgDesc->Package.Count ; ++Index)
    {
        /* 
         * Treat any NULL or non-numeric elements as non-matching.
         * XXX - if an element is a Name, should we examine its value?
         */
        if (!PkgDesc->Package.Elements[Index] ||
            ACPI_TYPE_Number != PkgDesc->Package.Elements[Index]->Common.Type)
        {
            continue;
        }

        /* 
         * Within these switch statements:
         *      "break" (exit from the switch) signifies a match;
         *      "continue" (proceed to next iteration of enclosing
         *          "for" loop) signifies a non-match.
         */
        switch (Op1Desc->Number.Value)
        {

        case MATCH_MTR:   /* always true */

            break;


        case MATCH_MEQ:   /* true if equal   */

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 != V1Desc->Number.Value)
            {
                continue;
            }
            break;


        case MATCH_MLE:   /* true if less than or equal  */

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 > V1Desc->Number.Value)
            {
                continue;
            }
            break;


        case MATCH_MLT:   /* true if less than   */

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 >= V1Desc->Number.Value)
            {
                continue;
            }
            break;


        case MATCH_MGE:   /* true if greater than or equal   */

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 < V1Desc->Number.Value)
            {
                continue;
            }
            break;


        case MATCH_MGT:   /* true if greater than    */

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 <= V1Desc->Number.Value)
            {
                continue;
            }
            break;


        default:    /* undefined   */

            continue;
        }
 
        
        switch(Op2Desc->Number.Value)
        {

        case MATCH_MTR:

            break;


        case MATCH_MEQ:

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 != V2Desc->Number.Value)
            {
                continue;
            }
            break;


        case MATCH_MLE:

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 > V2Desc->Number.Value)
            {
                continue;
            }
            break;


        case MATCH_MLT:

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 >= V2Desc->Number.Value)
            {
                continue;
            }
            break;


        case MATCH_MGE:

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 < V2Desc->Number.Value)
            {
                continue;
            }
            break;


        case MATCH_MGT:

            if (PkgDesc->Package.Elements[Index]->Number.Value
                 <= V2Desc->Number.Value)
            {
                continue;
            }
            break;


        default:

            continue;
        }

        /* Match found: exit from loop */
        
        MatchValue = Index;
        break;
    }

    PkgDesc->Common.Type  = (UINT8) ACPI_TYPE_Number;
    PkgDesc->Number.Value = MatchValue;

    /* Free the operands */

    CmDeleteInternalObject (StartDesc);
    CmDeleteInternalObject (V2Desc);
    CmDeleteInternalObject (Op2Desc);
    CmDeleteInternalObject (V1Desc);
    CmDeleteInternalObject (Op1Desc);
    
    /* Remove operands from the object stack */

    AmlObjStackPop (5);          

    return_ACPI_STATUS (AE_OK);
}
