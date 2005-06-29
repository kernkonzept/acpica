/******************************************************************************
 *
 * Module Name: amfield - ACPI AML (p-code) execution - field manipulation
 *              $Revision: 1.83 $
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


#define __AMFIELD_C__

#include "acpi.h"
#include "acdispat.h"
#include "acinterp.h"
#include "amlcode.h"
#include "acnamesp.h"
#include "achware.h"
#include "acevents.h"


#define _COMPONENT          ACPI_EXECUTER
        MODULE_NAME         ("amfield")





/*******************************************************************************
 *
 * FUNCTION:    AcpiAmlReadDataFromField
 *
 * PARAMETERS:  Mode                - ACPI_READ or ACPI_WRITE
 *              *FieldNode          - Parent node for field to be accessed
 *              *Buffer             - Value(s) to be read or written
 *              BufferLength        - Number of bytes to transfer
 *
 * RETURN:      Status3
 *
 * DESCRIPTION: Read or write a named field
 *
 ******************************************************************************/

ACPI_STATUS
AcpiAmlReadDataFromField (
    ACPI_OPERAND_OBJECT     *ObjDesc,
    ACPI_OPERAND_OBJECT     **RetBufferDesc)
{
    ACPI_STATUS             Status;
    ACPI_OPERAND_OBJECT     *BufferDesc;
    UINT32                  Length;
    void                    *Buffer;


    FUNCTION_TRACE_PTR ("AmlReadDataFromField", ObjDesc);


    /* Parameter validation */

    if (!ObjDesc)
    {
        return_ACPI_STATUS (AE_AML_NO_OPERAND)
    }

    /*
     * Allocate a buffer for the contents of the field.
     *
     * If the field is larger than the size of an ACPI_INTEGER, create
     * a BUFFER to hold it.  Otherwise, use an INTEGER.  This allows
     * the use of arithmetic operators on the returned value if the 
     * field size is equal or smaller than an Integer.
     *
     * Note: Field.length is in bits.
     */

    Length = ROUND_BITS_UP_TO_BYTES (ObjDesc->Field.BitLength);

    if (Length > sizeof (ACPI_INTEGER))
    {
        /* Field is too large for an Integer, create a Buffer instead */

        BufferDesc = AcpiCmCreateInternalObject (ACPI_TYPE_BUFFER);
        if (!BufferDesc)
        {
            return_ACPI_STATUS (AE_NO_MEMORY);
        }

        /* Create the actual read buffer */

        BufferDesc->Buffer.Pointer = AcpiCmCallocate (Length);
        if (!BufferDesc->Buffer.Pointer)
        {
            AcpiCmRemoveReference (BufferDesc);
            return_ACPI_STATUS (AE_NO_MEMORY);
        }

        BufferDesc->Buffer.Length = Length;
        Buffer = BufferDesc->Buffer.Pointer;
    }

    else
    {
        /* Field will fit within an Integer (normal case) */

        BufferDesc = AcpiCmCreateInternalObject (ACPI_TYPE_INTEGER);
        if (!BufferDesc)
        {
            return_ACPI_STATUS (AE_NO_MEMORY);
        }

        Length = sizeof (BufferDesc->Integer.Value);
        Buffer = &BufferDesc->Integer.Value;
    }


    /* Read from the appropriate field */

    switch (ObjDesc->Common.Type)
    {
    case ACPI_TYPE_BUFFER_FIELD:
        Status = AcpiAmlAccessBufferField (ACPI_READ, ObjDesc, Buffer, Length);
        break;

    case INTERNAL_TYPE_REGION_FIELD:
        Status = AcpiAmlAccessRegionField (ACPI_READ, ObjDesc, Buffer, Length);
        break;

    case INTERNAL_TYPE_BANK_FIELD:
        Status = AcpiAmlAccessBankField (ACPI_READ, ObjDesc, Buffer, Length);
        break;

    case INTERNAL_TYPE_INDEX_FIELD:
        Status = AcpiAmlAccessIndexField (ACPI_READ, ObjDesc, Buffer, Length);
        break;

    default:
        Status = AE_AML_INTERNAL;
    }


    if (ACPI_FAILURE (Status))
    {
        AcpiCmRemoveReference (BufferDesc);
    }

    else if (RetBufferDesc)
    {
        *RetBufferDesc = BufferDesc;
    }

    return_ACPI_STATUS (Status);
}



/*******************************************************************************
 *
 * FUNCTION:    AcpiAmlWriteDataToField
 *
 * PARAMETERS:  Mode                - ACPI_READ or ACPI_WRITE
 *              *FieldNode          - Parent node for field to be accessed
 *              *Buffer             - Value(s) to be read or written
 *              BufferLength        - Number of bytes to transfer
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read or write a named field
 *
 ******************************************************************************/



ACPI_STATUS
AcpiAmlWriteDataToField (
    ACPI_OPERAND_OBJECT     *SourceDesc,
    ACPI_OPERAND_OBJECT     *ObjDesc)
{
    ACPI_STATUS             Status;
    UINT32                  Length;
    void                    *Buffer;


    FUNCTION_TRACE_PTR ("AmlWriteDataToField", ObjDesc);


    /* Parameter validation */

    if (!SourceDesc || !ObjDesc)
    {
        return_ACPI_STATUS (AE_AML_NO_OPERAND)
    }


    /*
     * Get a pointer to the data to be written 
     */
    switch (SourceDesc->Common.Type)
    {
    case ACPI_TYPE_INTEGER:
        Buffer = &SourceDesc->Integer.Value;
        Length = sizeof (SourceDesc->Integer.Value);
        break;

    case ACPI_TYPE_BUFFER:
        Buffer = SourceDesc->Buffer.Pointer;
        Length = SourceDesc->Buffer.Length;
        break;

    case ACPI_TYPE_STRING:
        Buffer = SourceDesc->String.Pointer;
        Length = SourceDesc->String.Length;
        break;

    default:
        return_ACPI_STATUS (AE_AML_OPERAND_TYPE);
    }


    /*
     * Decode the type of field to be written
     */
    switch (ObjDesc->Common.Type)
    {
    case ACPI_TYPE_BUFFER_FIELD:
        Status = AcpiAmlAccessBufferField (ACPI_WRITE, ObjDesc, Buffer, Length);
        break;

    case INTERNAL_TYPE_REGION_FIELD:
        Status = AcpiAmlAccessRegionField (ACPI_WRITE, ObjDesc, Buffer, Length);
        break;

    case INTERNAL_TYPE_BANK_FIELD:
        Status = AcpiAmlAccessBankField (ACPI_WRITE, ObjDesc, Buffer, Length);
        break;

    case INTERNAL_TYPE_INDEX_FIELD:
        Status = AcpiAmlAccessIndexField (ACPI_WRITE, ObjDesc, Buffer, Length);
        break;

    default:
        return_ACPI_STATUS (AE_AML_INTERNAL);
    }


    return_ACPI_STATUS (Status);
}



/*******************************************************************************
 *
 * FUNCTION:    AcpiAmlAccessBufferField
 *
 * PARAMETERS:  Mode                - ACPI_READ or ACPI_WRITE
 *              *FieldNode          - Parent node for field to be accessed
 *              *Buffer             - Value(s) to be read or written
 *              BufferLength        - Number of bytes to transfer
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read or write a named field
 *
 ******************************************************************************/

ACPI_STATUS
AcpiAmlAccessBufferField (
    UINT32                  Mode,
    ACPI_OPERAND_OBJECT     *ObjDesc,
    void                    *Buffer,
    UINT32                  BufferLength)
{
    ACPI_STATUS             Status;


    FUNCTION_TRACE_PTR ("AcpiAmlAccessBufferField", ObjDesc);


    /*
     * If the BufferField arguments have not been previously evaluated,
     * evaluate them now and save the results.
     */
    if (!(ObjDesc->Common.Flags & AOPOBJ_DATA_VALID))
    {
        Status = AcpiDsGetBufferFieldArguments (ObjDesc);
        if (ACPI_FAILURE (Status))
        {
            return_ACPI_STATUS (Status);
        }
    }


    Status = AcpiAmlCommonAccessField (Mode, ObjDesc, Buffer, BufferLength);

    return_ACPI_STATUS (Status);
}



/*******************************************************************************
 *
 * FUNCTION:    AcpiAmlAccessRegionField
 *
 * PARAMETERS:  Mode                - ACPI_READ or ACPI_WRITE
 *              *FieldNode          - Parent node for field to be accessed
 *              *Buffer             - Value(s) to be read or written
 *              BufferLength        - Number of bytes to transfer
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read or write a named field
 *
 ******************************************************************************/

ACPI_STATUS
AcpiAmlAccessRegionField (
    UINT32                  Mode,
    ACPI_OPERAND_OBJECT     *ObjDesc,
    void                    *Buffer,
    UINT32                  BufferLength)
{
    ACPI_STATUS             Status;
    BOOLEAN                 Locked;


    FUNCTION_TRACE_PTR ("AmlAccessRegionField", ObjDesc);


    /*
     * Get the global lock if needed
     */
    Locked = AcpiAmlAcquireGlobalLock (ObjDesc->Field.LockRule);

    Status = AcpiAmlCommonAccessField (Mode, ObjDesc, Buffer, BufferLength);


    /*
     * Release global lock if we acquired it earlier
     */
    AcpiAmlReleaseGlobalLock (Locked);

    return_ACPI_STATUS (Status);
}



/*******************************************************************************
 *
 * FUNCTION:    AcpiAmlAccessBankField
 *
 * PARAMETERS:  Mode                - ACPI_READ or ACPI_WRITE
 *              *FieldNode          - Parent node for field to be accessed
 *              *Buffer             - Value(s) to be read or written
 *              BufferLength        - Number of bytes to transfer
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read or write a Bank Field
 *
 ******************************************************************************/

ACPI_STATUS
AcpiAmlAccessBankField (
    UINT32                  Mode,
    ACPI_OPERAND_OBJECT     *ObjDesc,
    void                    *Buffer,
    UINT32                  BufferLength)
{
    ACPI_STATUS             Status;
    BOOLEAN                 Locked;


    FUNCTION_TRACE_PTR ("AmlAccessBankField", ObjDesc);



    /*
     * Get the global lock if needed
     */
    Locked = AcpiAmlAcquireGlobalLock (ObjDesc->BankField.LockRule);


    /*
     * Write the BankValue to the BankRegister to select the bank.  
     * The BankValue for this BankField is specified in the
     * BankField ASL declaration.  The BankRegister is always a Field in
     * an operation region.
     */

    Status = AcpiAmlCommonAccessField (ACPI_WRITE, 
                            ObjDesc->BankField.BankRegisterObj,
                            &ObjDesc->BankField.Value,
                            sizeof (ObjDesc->BankField.Value));
    if (ACPI_FAILURE (Status))
    {
        goto Cleanup;
    }

    /*
     * The bank was successfully selected, now read or write the actual
     * data.
     */
    Status = AcpiAmlCommonAccessField (Mode, ObjDesc, Buffer, BufferLength);


Cleanup:
    /*
     * Release global lock if we acquired it earlier
     */
    AcpiAmlReleaseGlobalLock (Locked);

    return_ACPI_STATUS (Status);
}



/*******************************************************************************
 *
 * FUNCTION:    AcpiAmlAccessIndexField
 *
 * PARAMETERS:  Mode                - ACPI_READ or ACPI_WRITE
 *              *FieldNode          - Parent node for field to be accessed
 *              *Buffer             - Value(s) to be read or written
 *              BufferLength        - Number of bytes to transfer
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read or write a Index Field
 *
 ******************************************************************************/

ACPI_STATUS
AcpiAmlAccessIndexField (
    UINT32                  Mode,
    ACPI_OPERAND_OBJECT     *ObjDesc,
    void                    *Buffer,
    UINT32                  BufferLength)
{
    ACPI_STATUS             Status;
    BOOLEAN                 Locked;


    FUNCTION_TRACE_PTR ("AmlAccessIndexField", ObjDesc);


    /*
     * Get the global lock if needed
     */
    Locked = AcpiAmlAcquireGlobalLock (ObjDesc->IndexField.LockRule);


    /*
     * Set Index value to select proper Data register
     */
    Status = AcpiAmlCommonAccessField (ACPI_WRITE,
                            (ObjDesc->IndexField.Index)->Object,
                            &ObjDesc->IndexField.Value,
                            sizeof (ObjDesc->IndexField.Value));
    if (ACPI_FAILURE (Status))
    {
        goto Cleanup;
    }

    /* Now read/write the data register */

    Status = AcpiAmlCommonAccessField (Mode, (ObjDesc->IndexField.Data)->Object,
                    Buffer, BufferLength);

Cleanup:
    /*
     * Release global lock if we acquired it earlier
     */
    AcpiAmlReleaseGlobalLock (Locked);

    return_ACPI_STATUS (Status);
}




/*******************************************************************************
 *
 * FUNCTION:    AcpiAmlCommonAccessField
 *
 * PARAMETERS:  Mode                - ACPI_READ or ACPI_WRITE
 *              *FieldNode          - Parent node for field to be accessed
 *              *Buffer             - Value(s) to be read or written
 *              BufferLength        - Size of buffer, in bytes.  Must be large
 *                                    enough for all bits of the field.
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read or write a named field
 *
 ******************************************************************************/

ACPI_STATUS
AcpiAmlCommonAccessField (
    UINT32                  Mode,
    ACPI_OPERAND_OBJECT     *ObjDesc,
    void                    *Buffer,
    UINT32                  BufferLength)
{
    ACPI_STATUS             Status;
    UINT32                  BitGranularity;
    UINT32                  ByteGranularity;
    UINT32                  DatumLength;
    UINT32                  ActualByteLength;
    UINT32                  ByteFieldLength;


    FUNCTION_TRACE_PTR ("AmlCommonAccessField", ObjDesc);


    DEBUG_PRINT (ACPI_INFO,
        ("CommonAccessField: Obj=%p Type=%X Buf=%p Len=%X\n",
        ObjDesc, ObjDesc->Common.Type, Buffer, BufferLength));
    DEBUG_PRINT (ACPI_INFO,
        ("CommonAccessField: Mode=%d BitLen=%X BitOff=%X ByteOff=%X\n",
        Mode, ObjDesc->CommonField.BitLength, ObjDesc->CommonField.BitOffset,
        ObjDesc->CommonField.Offset));



    /* 
     * Granularity was decoded from the field access type
     * (AnyAcc will be the same as ByteAcc)
     */

    BitGranularity = ObjDesc->CommonField.Granularity;
    ByteGranularity = DIV_8 (BitGranularity);

    /*
     * Check if request is too large for the field, and silently truncate
     * the request to fit the field (if necessary).  This allows full 
     * Integer writes to small bit fields (lower bits of integer are used)
     */

    /* TBD: what about reads?  Should this code be in the write code only? */

    ByteFieldLength = ROUND_BITS_UP_TO_BYTES (ObjDesc->CommonField.BitLength);
    ActualByteLength = BufferLength;

    if (BufferLength > ByteFieldLength)
    {
        DEBUG_PRINT (ACPI_INFO,
            ("AmlCommonAccessField: Byte length %X truncated to %X\n",
            ActualByteLength, ByteFieldLength));

        ActualByteLength = ByteFieldLength;
    }


    /* Convert byte count to datum count, round up if necessary */

    DatumLength = (ActualByteLength + (ByteGranularity-1)) / ByteGranularity;

    DEBUG_PRINT (ACPI_INFO,
        ("ByteLen=%x, DatumLen=%x, BitGran=%x, ByteGran=%x\n",
        ActualByteLength, DatumLength, BitGranularity, ByteGranularity));



    /* Perform the actual read or write of the buffer */

    switch (Mode)
    {
    case ACPI_READ:

        Status = AcpiAmlExtractFromField (ObjDesc, Buffer, BufferLength,
                                    ActualByteLength, DatumLength,
                                    BitGranularity, ByteGranularity);
        break;


    case ACPI_WRITE:

        Status = AcpiAmlInsertIntoField (ObjDesc, Buffer, BufferLength,
                                    ActualByteLength, DatumLength,
                                    BitGranularity, ByteGranularity);
        break;


    default:

        DEBUG_PRINT (ACPI_ERROR,
            ("AccessNamedField: Unknown I/O Mode: %X\n", Mode));
        Status = AE_BAD_PARAMETER;
        break;
    }


    return_ACPI_STATUS (Status);
}

