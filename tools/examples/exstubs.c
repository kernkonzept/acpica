/******************************************************************************
 *
 * Module Name: exstubs - Stub routines for the Example program
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2014, Intel Corp.
 * All rights reserved.
 *
 * 2. License
 *
 * 2.1. This is your license from Intel Corp. under its intellectual property
 * rights. You may have additional license terms from the party that provided
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
 * to or modifications of the Original Intel Code. No other license or right
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
 * and the following Disclaimer and Export Compliance provision. In addition,
 * Licensee must cause all Covered Code to which Licensee contributes to
 * contain a file documenting the changes Licensee made to create that Covered
 * Code and the date of any change. Licensee must include in that file the
 * documentation of any changes made by any predecessor Licensee. Licensee
 * must include a prominent statement that the modification is derived,
 * directly or indirectly, from Original Intel Code.
 *
 * 3.2. Redistribution of Source with no Rights to Further Distribute Source.
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification without rights to further distribute source must
 * include the following Disclaimer and Export Compliance provision in the
 * documentation and/or other materials provided with distribution. In
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
 * HERE. ANY SOFTWARE ORIGINATING FROM INTEL OR DERIVED FROM INTEL SOFTWARE
 * IS PROVIDED "AS IS," AND INTEL WILL NOT PROVIDE ANY SUPPORT, ASSISTANCE,
 * INSTALLATION, TRAINING OR OTHER SERVICES. INTEL WILL NOT PROVIDE ANY
 * UPDATES, ENHANCEMENTS OR EXTENSIONS. INTEL SPECIFICALLY DISCLAIMS ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * 4.2. IN NO EVENT SHALL INTEL HAVE ANY LIABILITY TO LICENSEE, ITS LICENSEES
 * OR ANY OTHER THIRD PARTY, FOR ANY LOST PROFITS, LOST DATA, LOSS OF USE OR
 * COSTS OF PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, OR FOR ANY INDIRECT,
 * SPECIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THIS AGREEMENT, UNDER ANY
 * CAUSE OF ACTION OR THEORY OF LIABILITY, AND IRRESPECTIVE OF WHETHER INTEL
 * HAS ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES. THESE LIMITATIONS
 * SHALL APPLY NOTWITHSTANDING THE FAILURE OF THE ESSENTIAL PURPOSE OF ANY
 * LIMITED REMEDY.
 *
 * 4.3. Licensee shall not export, either directly or indirectly, any of this
 * software or system incorporating such software without first obtaining any
 * required license or other approval from the U. S. Department of Commerce or
 * any other agency or department of the United States Government. In the
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

#include "examples.h"

#include <acutils.h>
#include <acevents.h>
#include <acdispat.h>

#define _COMPONENT          ACPI_EXAMPLE
        ACPI_MODULE_NAME    ("exstubs")


/******************************************************************************
 *
 * DESCRIPTION: Stubs used to facilitate linkage of the example program
 *
 *****************************************************************************/


/* Utilities */

void
AcpiUtSubsystemShutdown (
    void)
{
}

ACPI_STATUS
AcpiUtExecute_STA (
    ACPI_NAMESPACE_NODE     *DeviceNode,
    UINT32                  *StatusFlags)
{
    return (AE_NOT_IMPLEMENTED);
}

ACPI_STATUS
AcpiUtExecute_HID (
    ACPI_NAMESPACE_NODE     *DeviceNode,
    ACPI_PNP_DEVICE_ID      **ReturnId)
{
    return (AE_NOT_IMPLEMENTED);
}

ACPI_STATUS
AcpiUtExecute_CID (
    ACPI_NAMESPACE_NODE     *DeviceNode,
    ACPI_PNP_DEVICE_ID_LIST **ReturnCidList)
{
    return (AE_NOT_IMPLEMENTED);
}

ACPI_STATUS
AcpiUtExecute_UID (
    ACPI_NAMESPACE_NODE     *DeviceNode,
    ACPI_PNP_DEVICE_ID      **ReturnId)
{
    return (AE_NOT_IMPLEMENTED);
}

ACPI_STATUS
AcpiUtExecute_SUB (
    ACPI_NAMESPACE_NODE     *DeviceNode,
    ACPI_PNP_DEVICE_ID      **ReturnId)
{
    return (AE_NOT_IMPLEMENTED);
}

ACPI_STATUS
AcpiUtExecutePowerMethods (
    ACPI_NAMESPACE_NODE     *DeviceNode,
    const char              **MethodNames,
    UINT8                   MethodCount,
    UINT8                   *OutValues)
{
    return (AE_NOT_IMPLEMENTED);
}

ACPI_STATUS
AcpiUtEvaluateNumericObject (
    char                    *ObjectName,
    ACPI_NAMESPACE_NODE     *DeviceNode,
    UINT64                  *Value)
{
    return (AE_NOT_IMPLEMENTED);
}

ACPI_STATUS
AcpiUtGetResourceEndTag (
    ACPI_OPERAND_OBJECT     *ObjDesc,
    UINT8                   **EndTag)
{
    return (AE_OK);
}


/* Hardware manager */

UINT32
AcpiHwGetMode (
    void)
{
    return (0);
}

ACPI_STATUS
AcpiHwReadPort (
    ACPI_IO_ADDRESS         Address,
    UINT32                  *Value,
    UINT32                  Width)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiHwWritePort (
    ACPI_IO_ADDRESS         Address,
    UINT32                  Value,
    UINT32                  Width)
{
    return (AE_OK);
}


/* Event manager */

ACPI_STATUS
AcpiInstallNotifyHandler (
    ACPI_HANDLE             Device,
    UINT32                  HandlerType,
    ACPI_NOTIFY_HANDLER     Handler,
    void                    *Context)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiEvInstallXruptHandlers (
    void)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiEvInitializeEvents (
    void)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiEvInstallRegionHandlers (
    void)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiEvInitializeOpRegions (
    void)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiEvInitializeRegion (
    ACPI_OPERAND_OBJECT     *RegionObj,
    BOOLEAN                 AcpiNsLocked)
{
    return (AE_OK);
}

#if (!ACPI_REDUCED_HARDWARE)
ACPI_STATUS
AcpiEvDeleteGpeBlock (
    ACPI_GPE_BLOCK_INFO     *GpeBlock)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiEnable (
    void)
{
    return (AE_OK);
}
#endif /* !ACPI_REDUCED_HARDWARE */

void
AcpiEvUpdateGpes (
    ACPI_OWNER_ID           TableOwnerId)
{
}

ACPI_STATUS
AcpiEvAddressSpaceDispatch (
    ACPI_OPERAND_OBJECT     *RegionObj,
    ACPI_OPERAND_OBJECT     *FieldObj,
    UINT32                  Function,
    UINT32                  RegionOffset,
    UINT32                  BitWidth,
    UINT64                  *Value)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiEvAcquireGlobalLock (
    UINT16                  Timeout)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiEvReleaseGlobalLock (
    void)
{
    return (AE_OK);
}

ACPI_STATUS
AcpiEvQueueNotifyRequest (
    ACPI_NAMESPACE_NODE     *Node,
    UINT32                  NotifyValue)
{
    return (AE_OK);
}

BOOLEAN
AcpiEvIsNotifyObject (
    ACPI_NAMESPACE_NODE     *Node)
{
    return (TRUE);
}


/* Namespace manager */

ACPI_STATUS
AcpiNsCheckReturnValue (
    ACPI_NAMESPACE_NODE         *Node,
    ACPI_EVALUATE_INFO          *Info,
    UINT32                      UserParamCount,
    ACPI_STATUS                 ReturnStatus,
    ACPI_OPERAND_OBJECT         **ReturnObjectPtr)
{
    return (AE_OK);
}

void
AcpiNsCheckArgumentTypes (
    ACPI_EVALUATE_INFO          *Info)
{
    return;
}

void
AcpiNsCheckArgumentCount (
    char                        *Pathname,
    ACPI_NAMESPACE_NODE         *Node,
    UINT32                      UserParamCount,
    const ACPI_PREDEFINED_INFO  *Predefined)
{
    return;
}

void
AcpiNsCheckAcpiCompliance (
    char                        *Pathname,
    ACPI_NAMESPACE_NODE         *Node,
    const ACPI_PREDEFINED_INFO  *Predefined)
{
    return;
}

const ACPI_PREDEFINED_INFO *
AcpiUtMatchPredefinedMethod (
    char                        *Name)
{
    return (NULL);
}

/* OSL interfaces */

ACPI_THREAD_ID
AcpiOsGetThreadId (
    void)
{
    return (1);
}

ACPI_STATUS
AcpiOsExecute (
    ACPI_EXECUTE_TYPE       Type,
    ACPI_OSD_EXEC_CALLBACK  Function,
    void                    *Context)
{
    return (AE_SUPPORT);
}
