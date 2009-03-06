
/*******************************************************************************
 *
 * Module Name: hwregs - Read/write access functions for the various ACPI
 *                       control and status registers.
 *
 ******************************************************************************/

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

#define __HWREGS_C__

#include "acpi.h"
#include "accommon.h"
#include "acevents.h"

#define _COMPONENT          ACPI_HARDWARE
        ACPI_MODULE_NAME    ("hwregs")


/* Local Prototypes */

static ACPI_STATUS
AcpiHwReadMultiple (
    UINT32                  *Value,
    ACPI_GENERIC_ADDRESS    *RegisterA,
    ACPI_GENERIC_ADDRESS    *RegisterB);

static ACPI_STATUS
AcpiHwWriteMultiple (
    UINT32                  Value,
    ACPI_GENERIC_ADDRESS    *RegisterA,
    ACPI_GENERIC_ADDRESS    *RegisterB);


/*******************************************************************************
 *
 * FUNCTION:    AcpiHwClearAcpiStatus
 *
 * PARAMETERS:  None
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Clears all fixed and general purpose status bits
 *
 ******************************************************************************/

ACPI_STATUS
AcpiHwClearAcpiStatus (
    void)
{
    ACPI_STATUS             Status;
    ACPI_CPU_FLAGS          LockFlags = 0;


    ACPI_FUNCTION_TRACE (HwClearAcpiStatus);


    ACPI_DEBUG_PRINT ((ACPI_DB_IO, "About to write %04X to %08X\n",
        ACPI_BITMASK_ALL_FIXED_STATUS,
        AcpiGbl_XPm1aStatus.Address));

    LockFlags = AcpiOsAcquireLock (AcpiGbl_HardwareLock);

    /* Clear the fixed events in PM1 A/B */

    Status = AcpiHwRegisterWrite (ACPI_REGISTER_PM1_STATUS,
                ACPI_BITMASK_ALL_FIXED_STATUS);
    if (ACPI_FAILURE (Status))
    {
        goto UnlockAndExit;
    }

    /* Clear the GPE Bits in all GPE registers in all GPE blocks */

    Status = AcpiEvWalkGpeList (AcpiHwClearGpeBlock, NULL);

UnlockAndExit:
    AcpiOsReleaseLock (AcpiGbl_HardwareLock, LockFlags);
    return_ACPI_STATUS (Status);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiHwGetRegisterBitMask
 *
 * PARAMETERS:  RegisterId          - Index of ACPI Register to access
 *
 * RETURN:      The bitmask to be used when accessing the register
 *
 * DESCRIPTION: Map RegisterId into a register bitmask.
 *
 ******************************************************************************/

ACPI_BIT_REGISTER_INFO *
AcpiHwGetBitRegisterInfo (
    UINT32                  RegisterId)
{
    ACPI_FUNCTION_ENTRY ();


    if (RegisterId > ACPI_BITREG_MAX)
    {
        ACPI_ERROR ((AE_INFO, "Invalid BitRegister ID: %X", RegisterId));
        return (NULL);
    }

    return (&AcpiGbl_BitRegisterInfo[RegisterId]);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiHwWritePm1Control
 *
 * PARAMETERS:  Pm1aControl         - Value to be written to PM1A control
 *              Pm1bControl         - Value to be written to PM1B control
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Write the PM1 A/B control registers. These registers are
 *              different than than the PM1 A/B status and enable registers
 *              in that different values can be written to the A/B registers.
 *              Most notably, the SLP_TYP bits can be different, as per the
 *              values returned from the _Sx predefined methods.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiHwWritePm1Control (
    UINT32                  Pm1aControl,
    UINT32                  Pm1bControl)
{
    ACPI_STATUS             Status;


    ACPI_FUNCTION_TRACE (HwWritePm1Control);


    Status = AcpiWrite (Pm1aControl, &AcpiGbl_FADT.XPm1aControlBlock);
    if (ACPI_FAILURE (Status))
    {
        return_ACPI_STATUS (Status);
    }

    if (AcpiGbl_FADT.XPm1bControlBlock.Address)
    {
        Status = AcpiWrite (Pm1bControl, &AcpiGbl_FADT.XPm1bControlBlock);
    }
    return_ACPI_STATUS (Status);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiHwRegisterRead
 *
 * PARAMETERS:  RegisterId          - ACPI Register ID
 *              ReturnValue         - Where the register value is returned
 *
 * RETURN:      Status and the value read.
 *
 * DESCRIPTION: Read from the specified ACPI register
 *
 ******************************************************************************/

ACPI_STATUS
AcpiHwRegisterRead (
    UINT32                  RegisterId,
    UINT32                  *ReturnValue)
{
    UINT32                  Value = 0;
    ACPI_STATUS             Status;


    ACPI_FUNCTION_TRACE (HwRegisterRead);


    switch (RegisterId)
    {
    case ACPI_REGISTER_PM1_STATUS:           /* PM1 A/B: 16-bit access each */

        Status = AcpiHwReadMultiple (&Value,
                    &AcpiGbl_XPm1aStatus,
                    &AcpiGbl_XPm1bStatus);
        break;


    case ACPI_REGISTER_PM1_ENABLE:           /* PM1 A/B: 16-bit access each */

        Status = AcpiHwReadMultiple (&Value,
                    &AcpiGbl_XPm1aEnable,
                    &AcpiGbl_XPm1bEnable);
        break;


    case ACPI_REGISTER_PM1_CONTROL:          /* PM1 A/B: 16-bit access each */

        Status = AcpiHwReadMultiple (&Value,
                    &AcpiGbl_FADT.XPm1aControlBlock,
                    &AcpiGbl_FADT.XPm1bControlBlock);
        break;


    case ACPI_REGISTER_PM2_CONTROL:          /* 8-bit access */

        Status = AcpiRead (&Value, &AcpiGbl_FADT.XPm2ControlBlock);
        break;


    case ACPI_REGISTER_PM_TIMER:             /* 32-bit access */

        Status = AcpiRead (&Value, &AcpiGbl_FADT.XPmTimerBlock);
        break;


    case ACPI_REGISTER_SMI_COMMAND_BLOCK:    /* 8-bit access */

        Status = AcpiOsReadPort (AcpiGbl_FADT.SmiCommand, &Value, 8);
        break;


    default:
        ACPI_ERROR ((AE_INFO, "Unknown Register ID: %X",
            RegisterId));
        Status = AE_BAD_PARAMETER;
        break;
    }

    if (ACPI_SUCCESS (Status))
    {
        *ReturnValue = Value;
    }

    return_ACPI_STATUS (Status);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiHwRegisterWrite
 *
 * PARAMETERS:  RegisterId          - ACPI Register ID
 *              Value               - The value to write
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Write to the specified ACPI register
 *
 * NOTE: In accordance with the ACPI specification, this function automatically
 * preserves the value of the following bits, meaning that these bits cannot be
 * changed via this interface:
 *
 * PM1_CONTROL[0] = SCI_EN
 * PM1_CONTROL[9]
 * PM1_STATUS[11]
 *
 * ACPI References:
 * 1) Hardware Ignored Bits: When software writes to a register with ignored
 *      bit fields, it preserves the ignored bit fields
 * 2) SCI_EN: OSPM always preserves this bit position
 *
 ******************************************************************************/

ACPI_STATUS
AcpiHwRegisterWrite (
    UINT32                  RegisterId,
    UINT32                  Value)
{
    ACPI_STATUS             Status;
    UINT32                  ReadValue;


    ACPI_FUNCTION_TRACE (HwRegisterWrite);


    switch (RegisterId)
    {
    case ACPI_REGISTER_PM1_STATUS:           /* PM1 A/B: 16-bit access each */
        /*
         * Handle the "ignored" bit in PM1 Status. According to the ACPI
         * specification, ignored bits are to be preserved when writing.
         * Normally, this would mean a read/modify/write sequence. However,
         * preserving a bit in the status register is different. Writing a
         * one clears the status, and writing a zero preserves the status.
         * Therefore, we must always write zero to the ignored bit.
         *
         * This behavior is clarified in the ACPI 4.0 specification.
         */
        Value &= ~ACPI_PM1_STATUS_PRESERVED_BITS;

        Status = AcpiHwWriteMultiple (Value,
                    &AcpiGbl_XPm1aStatus,
                    &AcpiGbl_XPm1bStatus);
        break;


    case ACPI_REGISTER_PM1_ENABLE:           /* PM1 A/B: 16-bit access each */

        Status = AcpiHwWriteMultiple (Value,
                    &AcpiGbl_XPm1aEnable,
                    &AcpiGbl_XPm1bEnable);
        break;


    case ACPI_REGISTER_PM1_CONTROL:          /* PM1 A/B: 16-bit access each */

        /*
         * Perform a read first to preserve certain bits (per ACPI spec)
         * Note: This includes SCI_EN, we never want to change this bit
         */
        Status = AcpiHwReadMultiple (&ReadValue,
                    &AcpiGbl_FADT.XPm1aControlBlock,
                    &AcpiGbl_FADT.XPm1bControlBlock);
        if (ACPI_FAILURE (Status))
        {
            goto Exit;
        }

        /* Insert the bits to be preserved */

        ACPI_INSERT_BITS (Value, ACPI_PM1_CONTROL_PRESERVED_BITS, ReadValue);

        /* Now we can write the data */

        Status = AcpiHwWriteMultiple (Value,
                    &AcpiGbl_FADT.XPm1aControlBlock,
                    &AcpiGbl_FADT.XPm1bControlBlock);
        break;


    case ACPI_REGISTER_PM2_CONTROL:          /* 8-bit access */

        Status = AcpiWrite (Value, &AcpiGbl_FADT.XPm2ControlBlock);
        break;


    case ACPI_REGISTER_PM_TIMER:             /* 32-bit access */

        Status = AcpiWrite (Value, &AcpiGbl_FADT.XPmTimerBlock);
        break;


    case ACPI_REGISTER_SMI_COMMAND_BLOCK:    /* 8-bit access */

        /* SMI_CMD is currently always in IO space */

        Status = AcpiOsWritePort (AcpiGbl_FADT.SmiCommand, Value, 8);
        break;


    default:
        ACPI_ERROR ((AE_INFO, "Unknown Register ID: %X",
            RegisterId));
        Status = AE_BAD_PARAMETER;
        break;
    }

Exit:
    return_ACPI_STATUS (Status);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiHwReadMultiple
 *
 * PARAMETERS:  Value               - Where the register value is returned
 *              RegisterA           - First ACPI register (required)
 *              RegisterB           - Second ACPI register (optional)
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read from the specified two-part ACPI register (such as PM1 A/B)
 *
 ******************************************************************************/

static ACPI_STATUS
AcpiHwReadMultiple (
    UINT32                  *Value,
    ACPI_GENERIC_ADDRESS    *RegisterA,
    ACPI_GENERIC_ADDRESS    *RegisterB)
{
    UINT32                  ValueA = 0;
    UINT32                  ValueB = 0;
    ACPI_STATUS             Status;


    /* The first register is always required */

    Status = AcpiRead (&ValueA, RegisterA);
    if (ACPI_FAILURE (Status))
    {
        return (Status);
    }

    /* Second register is optional */

    if (RegisterB->Address)
    {
        Status = AcpiRead (&ValueB, RegisterB);
        if (ACPI_FAILURE (Status))
        {
            return (Status);
        }
    }

    /*
     * OR the two return values together. No shifting or masking is necessary,
     * because of how the PM1 registers are defined in the ACPI specification:
     *
     * "Although the bits can be split between the two register blocks (each
     * register block has a unique pointer within the FADT), the bit positions
     * are maintained. The register block with unimplemented bits (that is,
     * those implemented in the other register block) always returns zeros,
     * and writes have no side effects"
     */
    *Value = (ValueA | ValueB);
    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiHwWriteMultiple
 *
 * PARAMETERS:  Value               - The value to write
 *              RegisterA           - First ACPI register (required)
 *              RegisterB           - Second ACPI register (optional)
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Write to the specified two-part ACPI register (such as PM1 A/B)
 *
 ******************************************************************************/

static ACPI_STATUS
AcpiHwWriteMultiple (
    UINT32                  Value,
    ACPI_GENERIC_ADDRESS    *RegisterA,
    ACPI_GENERIC_ADDRESS    *RegisterB)
{
    ACPI_STATUS             Status;


    /* The first register is always required */

    Status = AcpiWrite (Value, RegisterA);
    if (ACPI_FAILURE (Status))
    {
        return (Status);
    }

    /*
     * Second register is optional
     *
     * No bit shifting or clearing is necessary, because of how the PM1
     * registers are defined in the ACPI specification:
     *
     * "Although the bits can be split between the two register blocks (each
     * register block has a unique pointer within the FADT), the bit positions
     * are maintained. The register block with unimplemented bits (that is,
     * those implemented in the other register block) always returns zeros,
     * and writes have no side effects"
     */
    if (RegisterB->Address)
    {
        Status = AcpiWrite (Value, RegisterB);
    }

    return (Status);
}

