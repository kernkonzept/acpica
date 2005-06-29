
/******************************************************************************
 * 
 * Name: common.h -- prototypes for the common (subsystem-wide) procedures
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


#ifndef _COMMON_H
#define _COMMON_H

#include "acpiobj.h"
#include "acpiosd.h"


/* Global initialization interfaces */

ACPI_STATUS
InitAcpiGetRsdt (
    UINT32              *NumberOfTables, 
    OSD_FILE            *FilePtr);

ACPI_STATUS
InitAcpiGetAllTables (
    UINT32              NumberOfTables, 
    OSD_FILE            *FilePtr);

ACPI_STATUS
InitAcpiRegisters (
    void);

ACPI_STATUS
InitOpenFile (
    char                *Filename, 
    OSD_FILE            **FilePtr);
void
InitCloseFile (
    OSD_FILE            *FilePtr);



/* Debug interfaces */

INT32
GetDebugLevel (
    void);

void
SetDebugLevel (
    INT32               level);

void
FunctionTrace (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId, 
    char                *FunctionName);

void
FunctionExit (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId, 
    char                *FunctionName);

void
DebugPrintPrefix (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId);

void
DebugPrint (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId, 
    INT32               PrintLevel, 
    char                *Format, ...);

void
DebugPrintRaw (
    char                *Format, ...);

void
_ReportInfo (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId, 
    char                *Message);

void
_ReportError (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId, 
    char                *Message);

void
_ReportWarning (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId, 
    char                *Message);

void
_ReportSuccess (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId, 
    char                *Message);

void 
DumpBuffer (
    char               *Buffer, 
    UINT32              Count, 
    INT32               Flags, 
    INT32               componentId);


/* TBD: simplify or remove entirely */

void 
_Kinc_error (char *, INT32, INT32, char *, INT32, INT32); 
void 
_Kinc_info (char *, INT32, INT32, char *, INT32, INT32); 
void 
_Kinc_warning (char *, INT32, INT32, char *, INT32, INT32);


// assert macros

#define ACPI_ASSERT(exp)	                                        \
    if(!(exp))												\
        OsdDbgAssert(#exp, __FILE__, __LINE__, "Failed Assertion")


#define DEBUG_ASSERT(msg, exp)								\
    if(!(exp))												\
        OsdDbgAssert(#exp, __FILE__, __LINE__, msg)




/*
 * Memory allocation functions and related macros.
 * Macros that expand to include filename and line number
 */

void *
_AllocateObjectDesc (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId); 

void *
_LocalAllocate (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId, 
    INT32               AllocSize);

void *
_LocalCallocate (
    char                *ModuleName, 
    INT32               LineNumber, 
    INT32               ComponentId, 
    INT32               AllocSize);


void
LocalDeleteObject (
    ACPI_OBJECT         **ObjDesc);



/*
 * The point of these macros is to add the caller's module information - to be used
 * in case of an error during allocation
 */
#define LocalAllocate(a)                _LocalAllocate(_THIS_MODULE,__LINE__,_COMPONENT,a)
#define LocalCallocate(a)               _LocalCallocate(_THIS_MODULE,__LINE__,_COMPONENT,a)
#define AllocateObjectDesc()            _AllocateObjectDesc(_THIS_MODULE,__LINE__,_COMPONENT)




#endif /* _COMMON_H */
