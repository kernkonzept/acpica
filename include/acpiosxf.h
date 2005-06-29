
/******************************************************************************
 * 
 * Name: acpiosd.h - All interfaces to the OS-dependent layer.  These 
 *                   interfaces must be implemented by the OS-dependent 
 *                   front-end to the ACPI subsystem.
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
 * 2.1. Intel grants, free of charge, to any person ("Licensee") obtaining a 
 * copy of the source code appearing in this file ("Covered Code") a license 
 * under Intel's copyrights in the base code distributed originally by Intel 
 * ("Original Intel Code") to copy, make derivatives, distribute, use and 
 * display any portion of the Covered Code in any form; and
 *
 * 2.2. Intel grants Licensee a non-exclusive and non-transferable patent 
 * license (without the right to sublicense), under only those claims of Intel
 * patents that are infringed by the Original Intel Code, to make, use, sell, 
 * offer to sell, and import the Covered Code and derivative works thereof 
 * solely to the minimum extent necessary to exercise the above copyright 
 * license, and in no event shall the patent license extend to any additions to
 * or modifications of the Original Intel Code.  No other license or right is 
 * granted directly or by implication, estoppel or otherwise;
 *
 * the above copyright and patent license is granted only if the following 
 * conditions are met:
 *
 * 3. Conditions 
 *
 * 3.1. Redistribution of source code of any substantial portion of the Covered 
 * Code or modification must include the above Copyright Notice, the above 
 * License, this list of Conditions, and the following Disclaimer and Export 
 * Compliance provision.  In addition, Licensee must cause all Covered Code to 
 * which Licensee contributes to contain a file documenting the changes 
 * Licensee made to create that Covered Code and the date of any change.  
 * Licensee must include in that file the documentation of any changes made by
 * any predecessor Licensee.  Licensee must include a prominent statement that
 * the modification is derived, directly or indirectly, from Original Intel 
 * Code.
 *
 * 3.2. Redistribution in binary form of any substantial portion of the Covered 
 * Code or modification must reproduce the above Copyright Notice, and the 
 * following Disclaimer and Export Compliance provision in the documentation 
 * and/or other materials provided with the distribution.
 *
 * 3.3. Intel retains all right, title, and interest in and to the Original 
 * Intel Code.
 *
 * 3.4. Neither the name Intel nor any other trademark owned or controlled by 
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
 * event Licensee exports any such software from the United States or re-
 * exports any such software from a foreign destination, Licensee shall ensure
 * that the distribution and export/re-export of the software is in compliance 
 * with all laws, regulations, orders, or other restrictions of the U.S. Export 
 * Administration Regulations. Licensee agrees that neither it nor any of its 
 * subsidiaries will export/re-export any technical data, process, software, or 
 * service, directly or indirectly, to any country for which the United States 
 * government or any agency thereof requires an export license, other 
 * governmental approval, or letter of assurance, without first obtaining such
 * license, approval or letter.
 *
 *****************************************************************************/


#include <environment.h>
#include <datatypes.h>
#include <acpiasm.h>




/* 
 * Memory allocation and mapping 
 */

void *
OsdAllocate (
    UINT32          size);

void *
OsdCallocate (
    UINT32          size);

void 
OsdFree (
    void *          mem);

void *
OsdMapMemory (
    void            *PhysicalAddress,
    UINT32          Length);

void 
OsdUnMapMemory (
    void            *PhysicalAddress, 
    UINT32          Length);


/* 
 * Interrupt handlers 
 */

UINT32
OsdInstallInterruptHandler (
    UINT32          InterruptNumber,
    UINT32          (* Isr)(void),
    UINT32          *SciHandle);

void
OsdRemoveInterruptHandler (
    UINT32          Handle);


/* 
 * Scheduling 
 */

void
OsdSleep (
    UINT32          seconds,
    UINT32          milliseconds);

void
OsdSleepUsec (
    UINT32          microseconds);


/*
 * Platform/Hardware independent I/O interfaces
 */

UINT8
OsdIn8 (
    UINT16          InPort);

UINT16
OsdIn16 (
    UINT16          InPort);

UINT32
OsdIn32 (
    UINT16          InPort);

void
OsdOut8 (
    UINT16          OutPort, 
    UINT8           Value);

void
OsdOut16 (
    UINT16          OutPort, 
    UINT16          Value);

void
OsdOut32 (
    UINT16          OutPort, 
    UINT32          Value);


/*
 * Standard access to PCI configuration space
 */

UINT8 
OsdReadPciCfgByte (
    INT32           Bus, 
    INT32           DeviceFunction, 
    INT32           Register, 
    UINT8           *Value);

UINT8 
OsdReadPciCfgWord (
    INT32           Bus, 
    INT32           DeviceFunction, 
    INT32           Register, 
    UINT16          *Value);

UINT8 
OsdReadPciCfgDword (
    INT32           Bus, 
    INT32           DeviceFunction, 
    INT32           Register, 
    UINT32          *Value);

UINT8 
OsdWritePciCfgByte (
    INT32           Bus, 
    INT32           DeviceFunction, 
    INT32           Register, 
    UINT8           Value);

UINT8 
OsdWritePciCfgWord (
    INT32           Bus, 
    INT32           DeviceFunction, 
    INT32           Register, 
    UINT16          Value);

UINT8 
OsdWritePciCfgDword (
    INT32           Bus, 
    INT32           DeviceFunction, 
    INT32           Register, 
    UINT32          Value);


/*
 * Debug print routines 
 */

INT32 
OsdPrintf (
    OSD_FILE        *stream, 
    const char      *format, 
    ...);

INT32
OsdVprintf (
    OSD_FILE        *stream, 
    const char      *format, 
    va_list         args);


/* 
 * File I/O 
 * TBD: These may be removed completely since it is not always
 * possible to do file I/O from a kernel mode driver, especially
 * during system initialization!
 */

OSD_FILE *
OsdOpen (
    const char      *filename, 
    const char      *mode);

INT32 
OsdClose (
    OSD_FILE        *stream);

UINT32 
OsdRead (
    void            *buffer, 
    UINT32          size, 
    UINT32          count, 
    OSD_FILE        *stream);

UINT32 
OsdWrite (
    const void      *buffer, 
    UINT32          size, 
    UINT32          count, 
    OSD_FILE        *stream); 



