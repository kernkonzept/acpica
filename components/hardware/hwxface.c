
/******************************************************************************
 *
 * Module Name: hwxface - Public ACPICA hardware interfaces
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2009, Intel Corp.
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

#include "acpi.h"
#include "accommon.h"
#include "acnamesp.h"

#define _COMPONENT          ACPI_HARDWARE
        ACPI_MODULE_NAME    ("hwxface")


/******************************************************************************
 *
 * FUNCTION:    AcpiReset
 *
 * PARAMETERS:  None
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Set reset register in memory or IO space. Note: Does not
 *              support reset register in PCI config space, this must be
 *              handled separately.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiReset (
    void)
{
    ACPI_GENERIC_ADDRESS    *ResetReg;
    ACPI_STATUS             Status;


    ACPI_FUNCTION_TRACE (AcpiReset);


    ResetReg = &AcpiGbl_FADT.ResetRegister;

    /* Check if the reset register is supported */

    if (!(AcpiGbl_FADT.Flags & ACPI_FADT_RESET_REGISTER) ||
        !ResetReg->Address)
    {
        return_ACPI_STATUS (AE_NOT_EXIST);
    }

    /* Write the reset value to the reset register */

    Status = AcpiWrite (AcpiGbl_FADT.ResetValue, ResetReg);
    return_ACPI_STATUS (Status);
}

ACPI_EXPORT_SYMBOL (AcpiReset)


/******************************************************************************
 *
 * FUNCTION:    AcpiRead
 *
 * PARAMETERS:  Value               - Where the value is returned
 *              Reg                 - GAS register structure
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read from either memory or IO space.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiRead (
    UINT32                  *Value,
    ACPI_GENERIC_ADDRESS    *Reg)
{
    UINT32                  Width;
    UINT64                  Address;
    ACPI_STATUS             Status;


    ACPI_FUNCTION_NAME (AcpiRead);


    /*
     * Must have a valid pointer to a GAS structure, and
     * a non-zero address within. However, don't return an error
     * because the PM1A/B code must not fail if B isn't present.
     */
    if (!Reg)
    {
        return (AE_OK);
    }

    /* Get a local copy of the address. Handles possible alignment issues */

    ACPI_MOVE_64_TO_64 (&Address, &Reg->Address);
    if (!Address)
    {
        return (AE_OK);
    }

    /* Supported widths are 8/16/32 */

    Width = Reg->BitWidth;
    if ((Width != 8) && (Width != 16) && (Width != 32))
    {
        return (AE_SUPPORT);
    }

    /* Initialize entire 32-bit return value to zero */

    *Value = 0;

    /*
     * Two address spaces supported: Memory or IO.
     * PCI_Config is not supported here because the GAS struct is insufficient
     */
    switch (Reg->SpaceId)
    {
    case ACPI_ADR_SPACE_SYSTEM_MEMORY:

        Status = AcpiOsReadMemory (
                    (ACPI_PHYSICAL_ADDRESS) Address, Value, Width);
        break;


    case ACPI_ADR_SPACE_SYSTEM_IO:

        Status = AcpiOsReadPort ((ACPI_IO_ADDRESS) Address, Value, Width);
        break;


    default:
        ACPI_ERROR ((AE_INFO,
            "Unsupported address space: %X", Reg->SpaceId));
        return (AE_BAD_PARAMETER);
    }

    ACPI_DEBUG_PRINT ((ACPI_DB_IO,
        "Read:  %8.8X width %2d from %8.8X%8.8X (%s)\n",
        *Value, Width, ACPI_FORMAT_UINT64 (Address),
        AcpiUtGetRegionName (Reg->SpaceId)));

    return (Status);
}

ACPI_EXPORT_SYMBOL (AcpiRead)


/******************************************************************************
 *
 * FUNCTION:    AcpiWrite
 *
 * PARAMETERS:  Value               - To be written
 *              Reg                 - GAS register structure
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Write to either memory or IO space.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiWrite (
    UINT32                  Value,
    ACPI_GENERIC_ADDRESS    *Reg)
{
    UINT32                  Width;
    UINT64                  Address;
    ACPI_STATUS             Status;


    ACPI_FUNCTION_NAME (AcpiWrite);


    /*
     * Must have a valid pointer to a GAS structure, and
     * a non-zero address within. However, don't return an error
     * because the PM1A/B code must not fail if B isn't present.
     */
    if (!Reg)
    {
        return (AE_OK);
    }

    /* Get a local copy of the address. Handles possible alignment issues */

    ACPI_MOVE_64_TO_64 (&Address, &Reg->Address);
    if (!Address)
    {
        return (AE_OK);
    }

    /* Supported widths are 8/16/32 */

    Width = Reg->BitWidth;
    if ((Width != 8) && (Width != 16) && (Width != 32))
    {
        return (AE_SUPPORT);
    }

    /*
     * Two address spaces supported: Memory or IO.
     * PCI_Config is not supported here because the GAS struct is insufficient
     */
    switch (Reg->SpaceId)
    {
    case ACPI_ADR_SPACE_SYSTEM_MEMORY:

        Status = AcpiOsWriteMemory (
                    (ACPI_PHYSICAL_ADDRESS) Address, Value, Width);
        break;


    case ACPI_ADR_SPACE_SYSTEM_IO:

        Status = AcpiOsWritePort (
                    (ACPI_IO_ADDRESS) Address, Value, Width);
        break;


    default:
        ACPI_ERROR ((AE_INFO,
            "Unsupported address space: %X", Reg->SpaceId));
        return (AE_BAD_PARAMETER);
    }

    ACPI_DEBUG_PRINT ((ACPI_DB_IO,
        "Wrote: %8.8X width %2d   to %8.8X%8.8X (%s)\n",
        Value, Width, ACPI_FORMAT_UINT64 (Address),
        AcpiUtGetRegionName (Reg->SpaceId)));

    return (Status);
}

ACPI_EXPORT_SYMBOL (AcpiWrite)


/*******************************************************************************
 *
 * FUNCTION:    AcpiGetRegisterUnlocked
 *
 * PARAMETERS:  RegisterId      - ID of ACPI BitRegister to access
 *              ReturnValue     - Value that was read from the register
 *
 * RETURN:      Status and the value read from specified Register. Value
 *              returned is normalized to bit0 (is shifted all the way right)
 *
 * DESCRIPTION: ACPI BitRegister read function. Does not acquire the HW lock.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiGetRegisterUnlocked (
    UINT32                  RegisterId,
    UINT32                  *ReturnValue)
{
    UINT32                  RegisterValue = 0;
    ACPI_BIT_REGISTER_INFO  *BitRegInfo;
    ACPI_STATUS             Status;


    ACPI_FUNCTION_TRACE (AcpiGetRegisterUnlocked);


    /* Get the info structure corresponding to the requested ACPI Register */

    BitRegInfo = AcpiHwGetBitRegisterInfo (RegisterId);
    if (!BitRegInfo)
    {
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    /* Read from the register */

    Status = AcpiHwRegisterRead (BitRegInfo->ParentRegister,
                &RegisterValue);

    if (ACPI_SUCCESS (Status))
    {
        /* Normalize the value that was read */

        RegisterValue = ((RegisterValue & BitRegInfo->AccessBitMask)
                            >> BitRegInfo->BitPosition);

        *ReturnValue = RegisterValue;

        ACPI_DEBUG_PRINT ((ACPI_DB_IO, "Read value %8.8X register %X\n",
            RegisterValue, BitRegInfo->ParentRegister));
    }

    return_ACPI_STATUS (Status);
}

ACPI_EXPORT_SYMBOL (AcpiGetRegisterUnlocked)


/*******************************************************************************
 *
 * FUNCTION:    AcpiGetRegister
 *
 * PARAMETERS:  RegisterId      - ID of ACPI BitRegister to access
 *              ReturnValue     - Value that was read from the register
 *
 * RETURN:      Status and the value read from specified Register. Value
 *              returned is normalized to bit0 (is shifted all the way right)
 *
 * DESCRIPTION: ACPI BitRegister read function.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiGetRegister (
    UINT32                  RegisterId,
    UINT32                  *ReturnValue)
{
    ACPI_STATUS             Status;
    ACPI_CPU_FLAGS          Flags;


    Flags = AcpiOsAcquireLock (AcpiGbl_HardwareLock);
    Status = AcpiGetRegisterUnlocked (RegisterId, ReturnValue);
    AcpiOsReleaseLock (AcpiGbl_HardwareLock, Flags);

    return (Status);
}

ACPI_EXPORT_SYMBOL (AcpiGetRegister)


/*******************************************************************************
 *
 * FUNCTION:    AcpiSetRegister
 *
 * PARAMETERS:  RegisterId      - ID of ACPI BitRegister to access
 *              Value           - (only used on write) value to write to the
 *                                Register, NOT pre-normalized to the bit pos
 *
 * RETURN:      Status
 *
 * DESCRIPTION: ACPI Bit Register write function.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiSetRegister (
    UINT32                  RegisterId,
    UINT32                  Value)
{
    UINT32                  RegisterValue = 0;
    ACPI_BIT_REGISTER_INFO  *BitRegInfo;
    ACPI_STATUS             Status;
    ACPI_CPU_FLAGS          LockFlags;


    ACPI_FUNCTION_TRACE_U32 (AcpiSetRegister, RegisterId);


    /* Get the info structure corresponding to the requested ACPI Register */

    BitRegInfo = AcpiHwGetBitRegisterInfo (RegisterId);
    if (!BitRegInfo)
    {
        ACPI_ERROR ((AE_INFO, "Bad ACPI HW RegisterId: %X", RegisterId));
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    LockFlags = AcpiOsAcquireLock (AcpiGbl_HardwareLock);

    /* Always do a register read first so we can insert the new bits  */

    Status = AcpiHwRegisterRead (BitRegInfo->ParentRegister,
                &RegisterValue);
    if (ACPI_FAILURE (Status))
    {
        goto UnlockAndExit;
    }

    /*
     * Decode the Register ID
     * Register ID = [Register block ID] | [bit ID]
     *
     * Check bit ID to fine locate Register offset.
     * Check Mask to determine Register offset, and then read-write.
     */
    switch (BitRegInfo->ParentRegister)
    {
    case ACPI_REGISTER_PM1_STATUS:

        /*
         * Status Registers are different from the rest. Clear by
         * writing 1, and writing 0 has no effect. So, the only relevant
         * information is the single bit we're interested in, all others should
         * be written as 0 so they will be left unchanged.
         */
        Value = ACPI_REGISTER_PREPARE_BITS (Value,
                    BitRegInfo->BitPosition, BitRegInfo->AccessBitMask);
        if (Value)
        {
            Status = AcpiHwRegisterWrite (ACPI_REGISTER_PM1_STATUS,
                        (UINT16) Value);
            RegisterValue = 0;
        }
        break;


    case ACPI_REGISTER_PM1_ENABLE:

        ACPI_REGISTER_INSERT_VALUE (RegisterValue, BitRegInfo->BitPosition,
            BitRegInfo->AccessBitMask, Value);

        Status = AcpiHwRegisterWrite (ACPI_REGISTER_PM1_ENABLE,
                    (UINT16) RegisterValue);
        break;


    case ACPI_REGISTER_PM1_CONTROL:

        /*
         * Write the PM1 Control register.
         * Note that at this level, the fact that there are actually TWO
         * registers (A and B - and B may not exist) is abstracted.
         */
        ACPI_DEBUG_PRINT ((ACPI_DB_IO, "PM1 control: Read %X\n",
            RegisterValue));

        ACPI_REGISTER_INSERT_VALUE (RegisterValue, BitRegInfo->BitPosition,
            BitRegInfo->AccessBitMask, Value);

        Status = AcpiHwRegisterWrite (ACPI_REGISTER_PM1_CONTROL,
                    (UINT16) RegisterValue);
        break;


    case ACPI_REGISTER_PM2_CONTROL:

        Status = AcpiHwRegisterRead (ACPI_REGISTER_PM2_CONTROL,
                    &RegisterValue);
        if (ACPI_FAILURE (Status))
        {
            goto UnlockAndExit;
        }

        ACPI_DEBUG_PRINT ((ACPI_DB_IO, "PM2 control: Read %X from %8.8X%8.8X\n",
            RegisterValue,
            ACPI_FORMAT_UINT64 (AcpiGbl_FADT.XPm2ControlBlock.Address)));

        ACPI_REGISTER_INSERT_VALUE (RegisterValue, BitRegInfo->BitPosition,
                BitRegInfo->AccessBitMask, Value);

        ACPI_DEBUG_PRINT ((ACPI_DB_IO, "About to write %4.4X to %8.8X%8.8X\n",
            RegisterValue,
            ACPI_FORMAT_UINT64 (AcpiGbl_FADT.XPm2ControlBlock.Address)));

        Status = AcpiHwRegisterWrite (ACPI_REGISTER_PM2_CONTROL,
                    (UINT8) (RegisterValue));
        break;


    default:
        break;
    }


UnlockAndExit:

    AcpiOsReleaseLock (AcpiGbl_HardwareLock, LockFlags);

    /* Normalize the value that was read */

    ACPI_DEBUG_EXEC (RegisterValue =
        ((RegisterValue & BitRegInfo->AccessBitMask) >>
            BitRegInfo->BitPosition));

    ACPI_DEBUG_PRINT ((ACPI_DB_IO, "Set bits: %8.8X actual %8.8X register %X\n",
        Value, RegisterValue, BitRegInfo->ParentRegister));
    return_ACPI_STATUS (Status);
}

ACPI_EXPORT_SYMBOL (AcpiSetRegister)


/*******************************************************************************
 *
 * FUNCTION:    AcpiGetSleepTypeData
 *
 * PARAMETERS:  SleepState          - Numeric sleep state
 *              *SleepTypeA         - Where SLP_TYPa is returned
 *              *SleepTypeB         - Where SLP_TYPb is returned
 *
 * RETURN:      Status - ACPI status
 *
 * DESCRIPTION: Obtain the SLP_TYPa and SLP_TYPb values for the requested sleep
 *              state.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiGetSleepTypeData (
    UINT8                   SleepState,
    UINT8                   *SleepTypeA,
    UINT8                   *SleepTypeB)
{
    ACPI_STATUS             Status = AE_OK;
    ACPI_EVALUATE_INFO      *Info;


    ACPI_FUNCTION_TRACE (AcpiGetSleepTypeData);


    /* Validate parameters */

    if ((SleepState > ACPI_S_STATES_MAX) ||
        !SleepTypeA || !SleepTypeB)
    {
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    /* Allocate the evaluation information block */

    Info = ACPI_ALLOCATE_ZEROED (sizeof (ACPI_EVALUATE_INFO));
    if (!Info)
    {
        return_ACPI_STATUS (AE_NO_MEMORY);
    }

    Info->Pathname = ACPI_CAST_PTR (char, AcpiGbl_SleepStateNames[SleepState]);

    /* Evaluate the namespace object containing the values for this state */

    Status = AcpiNsEvaluate (Info);
    if (ACPI_FAILURE (Status))
    {
        ACPI_DEBUG_PRINT ((ACPI_DB_EXEC,
            "%s while evaluating SleepState [%s]\n",
            AcpiFormatException (Status), Info->Pathname));

        goto Cleanup;
    }

    /* Must have a return object */

    if (!Info->ReturnObject)
    {
        ACPI_ERROR ((AE_INFO, "No Sleep State object returned from [%s]",
            Info->Pathname));
        Status = AE_NOT_EXIST;
    }

    /* It must be of type Package */

    else if (ACPI_GET_OBJECT_TYPE (Info->ReturnObject) != ACPI_TYPE_PACKAGE)
    {
        ACPI_ERROR ((AE_INFO, "Sleep State return object is not a Package"));
        Status = AE_AML_OPERAND_TYPE;
    }

    /*
     * The package must have at least two elements. NOTE (March 2005): This
     * goes against the current ACPI spec which defines this object as a
     * package with one encoded DWORD element. However, existing practice
     * by BIOS vendors seems to be to have 2 or more elements, at least
     * one per sleep type (A/B).
     */
    else if (Info->ReturnObject->Package.Count < 2)
    {
        ACPI_ERROR ((AE_INFO,
            "Sleep State return package does not have at least two elements"));
        Status = AE_AML_NO_OPERAND;
    }

    /* The first two elements must both be of type Integer */

    else if ((ACPI_GET_OBJECT_TYPE (Info->ReturnObject->Package.Elements[0])
                != ACPI_TYPE_INTEGER) ||
             (ACPI_GET_OBJECT_TYPE (Info->ReturnObject->Package.Elements[1])
                != ACPI_TYPE_INTEGER))
    {
        ACPI_ERROR ((AE_INFO,
            "Sleep State return package elements are not both Integers (%s, %s)",
            AcpiUtGetObjectTypeName (Info->ReturnObject->Package.Elements[0]),
            AcpiUtGetObjectTypeName (Info->ReturnObject->Package.Elements[1])));
        Status = AE_AML_OPERAND_TYPE;
    }
    else
    {
        /* Valid _Sx_ package size, type, and value */

        *SleepTypeA = (UINT8)
            (Info->ReturnObject->Package.Elements[0])->Integer.Value;
        *SleepTypeB = (UINT8)
            (Info->ReturnObject->Package.Elements[1])->Integer.Value;
    }

    if (ACPI_FAILURE (Status))
    {
        ACPI_EXCEPTION ((AE_INFO, Status,
            "While evaluating SleepState [%s], bad Sleep object %p type %s",
            Info->Pathname, Info->ReturnObject,
            AcpiUtGetObjectTypeName (Info->ReturnObject)));
    }

    AcpiUtRemoveReference (Info->ReturnObject);

Cleanup:
    ACPI_FREE (Info);
    return_ACPI_STATUS (Status);
}

ACPI_EXPORT_SYMBOL (AcpiGetSleepTypeData)
