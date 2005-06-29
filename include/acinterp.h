
/******************************************************************************
 *
 * Name: acinterp.h - Interpreter subcomponent prototypes and defines
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

#ifndef __ACINTERP_H__
#define __ACINTERP_H__


#define WALK_OPERANDS       &(WalkState->Operands [WalkState->NumOperands -1])


/* Interpreter constants */

#define AML_END_OF_BLOCK            -1
#define PUSH_PKG_LENGTH             1
#define DO_NOT_PUSH_PKG_LENGTH      0


#define STACK_TOP                   0
#define STACK_BOTTOM                (UINT32) -1

/* Constants for global "WhenToParseMethods" */

#define METHOD_PARSE_AT_INIT        0x0
#define METHOD_PARSE_JUST_IN_TIME   0x1
#define METHOD_DELETE_AT_COMPLETION 0x2


ACPI_STATUS
AcpiAmlResolveOperands (
    UINT16                  Opcode,
    ACPI_OBJECT_INTERNAL    **StackPtr);


/*
 * amxface - External interpreter interfaces
 */

ACPI_STATUS
AcpiAmlLoadTable (
    ACPI_TABLE_TYPE         TableId);

ACPI_STATUS
AcpiAmlExecuteMethod (
    ACPI_NAMED_OBJECT       *MethodEntry,
    ACPI_OBJECT_INTERNAL    **Params,
    ACPI_OBJECT_INTERNAL    **ReturnObjDesc);


/*
 * amcopy - Interpreter object copy support
 */

ACPI_STATUS
AcpiAmlBuildCopyInternalPackageObject (
    ACPI_OBJECT_INTERNAL    *SourceObj,
    ACPI_OBJECT_INTERNAL    *DestObj);


/*
 * amfield - ACPI AML (p-code) execution - field manipulation
 */


ACPI_STATUS
AcpiAmlReadField (
    ACPI_OBJECT_INTERNAL    *ObjDesc,
    void                    *Buffer,
    UINT32                  BufferLength,
    UINT32                  ByteLength,
    UINT32                  DatumLength,
    UINT32                  BitGranularity,
    UINT32                  ByteGranularity);

ACPI_STATUS
AcpiAmlWriteField (
    ACPI_OBJECT_INTERNAL    *ObjDesc,
    void                    *Buffer,
    UINT32                  BufferLength,
    UINT32                  ByteLength,
    UINT32                  DatumLength,
    UINT32                  BitGranularity,
    UINT32                  ByteGranularity);

ACPI_STATUS
AcpiAmlSetupField (
    ACPI_OBJECT_INTERNAL    *ObjDesc,
    ACPI_OBJECT_INTERNAL    *RgnDesc,
    INT32                   FieldBitWidth);

ACPI_STATUS
AcpiAmlReadFieldData (
    ACPI_OBJECT_INTERNAL    *ObjDesc,
    UINT32                  FieldByteOffset,
    UINT32                  FieldBitWidth,
    UINT32                  *Value);

ACPI_STATUS
AcpiAmlAccessNamedField (
    INT32                   Mode,
    ACPI_HANDLE             NamedField,
    void                    *Buffer,
    UINT32                  Length);

ACPI_STATUS
AcpiAmlSetNamedFieldValue (
    ACPI_HANDLE             NamedField,
    void                    *Buffer,
    UINT32                  Length);

ACPI_STATUS
AcpiAmlGetNamedFieldValue (
    ACPI_HANDLE             NamedField,
    void                    *Buffer,
    UINT32                  Length);


/*
 * ammisc - ACPI AML (p-code) execution - specific opcodes
 */

ACPI_STATUS
AcpiAmlExecCreateField (
    UINT16                  Opcode,
    ACPI_WALK_STATE         *WalkState);

ACPI_STATUS
AcpiAmlExecReconfiguration (
    UINT16                  Opcode,
    ACPI_WALK_STATE         *WalkState);

ACPI_STATUS
AcpiAmlExecFatal (
    ACPI_WALK_STATE         *WalkState);

ACPI_STATUS
AcpiAmlExecIndex (
    ACPI_WALK_STATE         *WalkState,
    ACPI_OBJECT_INTERNAL    **ReturnDesc);

ACPI_STATUS
AcpiAmlExecMatch (
    ACPI_WALK_STATE         *WalkState,
    ACPI_OBJECT_INTERNAL    **ReturnDesc);

ACPI_STATUS
AcpiAmlExecCreateMutex (
    ACPI_WALK_STATE         *WalkState);

ACPI_STATUS
AcpiAmlExecCreateProcessor (
    ACPI_GENERIC_OP         *Op,
    ACPI_HANDLE             ProcessorNTE);

ACPI_STATUS
AcpiAmlExecCreatePowerResource (
    ACPI_GENERIC_OP         *Op,
    ACPI_HANDLE             ProcessorNTE);

ACPI_STATUS
AcpiAmlExecCreateRegion (
    UINT8                   *AmlPtr,
    UINT32                  AcpiAmlLength,
    UINT32                  RegionSpace,
    ACPI_WALK_STATE         *WalkState);

ACPI_STATUS
AcpiAmlExecCreateEvent (
    ACPI_WALK_STATE         *WalkState);

ACPI_STATUS
AcpiAmlExecCreateAlias (
    ACPI_WALK_STATE         *WalkState);

ACPI_STATUS
AcpiAmlExecCreateMethod (
    UINT8                   *AmlPtr,
    UINT32                  AcpiAmlLength,
    UINT32                  MethodFlags,
    ACPI_HANDLE             Method);


/*
 * amprep - ACPI AML (p-code) execution - prep utilities
 */

ACPI_STATUS
AcpiAmlPrepDefFieldValue (
    ACPI_NAMED_OBJECT       *ThisEntry,
    ACPI_HANDLE             Region,
    UINT8                   FieldFlags,
    UINT8                   FieldAttribute,
    UINT32                  FieldPosition,
    UINT32                  FieldLength);

ACPI_STATUS
AcpiAmlPrepBankFieldValue (
    ACPI_NAMED_OBJECT       *ThisEntry,
    ACPI_HANDLE             Region,
    ACPI_HANDLE             BankReg,
    UINT32                  BankVal,
    UINT8                   FieldFlags,
    UINT8                   FieldAttribute,
    UINT32                  FieldPosition,
    UINT32                  FieldLength);

ACPI_STATUS
AcpiAmlPrepIndexFieldValue (
    ACPI_NAMED_OBJECT       *ThisEntry,
    ACPI_HANDLE             IndexReg,
    ACPI_HANDLE             DataReg,
    UINT8                   FieldFlags,
    UINT8                   FieldAttribute,
    UINT32                  FieldPosition,
    UINT32                  FieldLength);

ACPI_STATUS
AcpiAmlPrepOperands (
    INT8                    *Types,
    ACPI_OBJECT_INTERNAL    **StackPtr);


/*
 * amsystem - Interface to OS services
 */

UINT16
AcpiAmlSystemThreadId (
    void);

ACPI_STATUS
AcpiAmlSystemDoNotifyOp (
    ACPI_OBJECT_INTERNAL    *Value,
    ACPI_OBJECT_INTERNAL    *ObjDesc);

void
AcpiAmlSystemDoSuspend(
    UINT32                  Time);

void
AcpiAmlSystemDoStall (
    UINT32                  Time);

ACPI_STATUS
AcpiAmlSystemAcquireMutex(
    ACPI_OBJECT_INTERNAL    *Time,
    ACPI_OBJECT_INTERNAL    *ObjDesc);

ACPI_STATUS
AcpiAmlSystemReleaseMutex(
    ACPI_OBJECT_INTERNAL    *ObjDesc);

ACPI_STATUS
AcpiAmlSystemSignalEvent(
    ACPI_OBJECT_INTERNAL    *ObjDesc);

ACPI_STATUS
AcpiAmlSystemWaitEvent(
    ACPI_OBJECT_INTERNAL    *Time,
    ACPI_OBJECT_INTERNAL    *ObjDesc);

ACPI_STATUS
AcpiAmlSystemResetEvent(
    ACPI_OBJECT_INTERNAL    *ObjDesc);

ACPI_STATUS
AcpiAmlSystemWaitSemaphore (
    ACPI_HANDLE             Semaphore,
    UINT32                  Timeout);


/*
 * ammonadic - ACPI AML (p-code) execution, monadic operators
 */

ACPI_STATUS
AcpiAmlExecMonadic1 (
    UINT16                  Opcode,
    ACPI_WALK_STATE         *WalkState);

ACPI_STATUS
AcpiAmlExecMonadic2 (
    UINT16                  Opcode,
    ACPI_WALK_STATE         *WalkState,
    ACPI_OBJECT_INTERNAL    **ReturnDesc);

ACPI_STATUS
AcpiAmlExecMonadic2R (
    UINT16                  Opcode,
    ACPI_WALK_STATE         *WalkState,
    ACPI_OBJECT_INTERNAL    **ReturnDesc);


/*
 * amdyadic - ACPI AML (p-code) execution, dyadic operators
 */

ACPI_STATUS
AcpiAmlExecDyadic1 (
    UINT16                  Opcode,
    ACPI_WALK_STATE         *WalkState);

ACPI_STATUS
AcpiAmlExecDyadic2 (
    UINT16                  Opcode,
    ACPI_WALK_STATE         *WalkState,
    ACPI_OBJECT_INTERNAL    **ReturnDesc);

ACPI_STATUS
AcpiAmlExecDyadic2R (
    UINT16                  Opcode,
    ACPI_WALK_STATE         *WalkState,
    ACPI_OBJECT_INTERNAL    **ReturnDesc);

ACPI_STATUS
AcpiAmlExecDyadic2S (
    UINT16                  Opcode,
    ACPI_WALK_STATE         *WalkState,
    ACPI_OBJECT_INTERNAL    **ReturnDesc);


/*
 * amresolv  - Object resolution and get value functions
 */

ACPI_STATUS
AcpiAmlResolveToValue (
    ACPI_OBJECT_INTERNAL    **StackPtr);

ACPI_STATUS
AcpiAmlResolveEntryToValue (
    ACPI_NAMED_OBJECT       **StackPtr);

ACPI_STATUS
AcpiAmlResolveObjectToValue (
    ACPI_OBJECT_INTERNAL    **StackPtr);

ACPI_STATUS
AcpiAmlGetFieldUnitValue (
    ACPI_OBJECT_INTERNAL    *FieldDesc,
    ACPI_OBJECT_INTERNAL    *ResultDesc);


/*
 * amdump - Scanner debug output routines
 */

void
AcpiAmlShowHexValue (
    INT32                   ByteCount,
    UINT8                   *AmlPtr,
    INT32                   LeadSpace);


ACPI_STATUS
AcpiAmlDumpOperand (
    ACPI_OBJECT_INTERNAL    *EntryDesc);

void
AcpiAmlDumpOperands (
    ACPI_OBJECT_INTERNAL    **Operands,
    OPERATING_MODE          InterpreterMode,
    INT8                    *Ident,
    INT32                   NumLevels,
    INT8                    *Note,
    INT8                    *ModuleName,
    INT32                   LineNumber);

void
AcpiAmlDumpObjectDescriptor (
    ACPI_OBJECT_INTERNAL    *Object,
    UINT32                  Flags);


void
AcpiAmlDumpAcpiNamedObject (
    ACPI_NAMED_OBJECT       *Entry,
    UINT32                  Flags);


/*
 * amnames - interpreter/scanner name load/execute
 */

INT8 *
AcpiAmlAllocateNameString (
    UINT32                  PrefixCount,
    UINT32                  NumNameSegs);

INT32
AcpiAmlGoodChar (
    INT32                   Character);

ACPI_STATUS
AcpiAmlExecNameSegment (
    UINT8                   **InAmlAddress,
    INT8                    *NameString);

ACPI_STATUS
AcpiAmlGetNameString (
    OBJECT_TYPE_INTERNAL    DataType,
    UINT8                   *InAmlAddress,
    INT8                    **OutNameString,
    UINT32                  *OutNameLength);

UINT32
AcpiAmlDecodePackageLength (
    UINT32                  LastPkgLen);


ACPI_STATUS
AcpiAmlDoName (
    ACPI_OBJECT_TYPE        DataType,
    OPERATING_MODE          LoadExecMode);


/*
 * amstore - Object store support
 */

ACPI_STATUS
AcpiAmlExecStore (
    ACPI_OBJECT_INTERNAL    *op1,
    ACPI_OBJECT_INTERNAL    *res);

ACPI_STATUS
AcpiAmlStoreObjectToObject (
    ACPI_OBJECT_INTERNAL    *ValDesc,
    ACPI_OBJECT_INTERNAL    *DestDesc);

ACPI_STATUS
AcpiAmlStoreObjectToNte (
    ACPI_OBJECT_INTERNAL    *ValDesc,
    ACPI_NAMED_OBJECT       *Entry);


/*
 * amutils - interpreter/scanner utilities
 */

void
AcpiAmlEnterInterpreter (
    void);

void
AcpiAmlExitInterpreter (
    void);

BOOLEAN
AcpiAmlValidateObjectType (
    ACPI_OBJECT_TYPE        Type);

BOOLEAN
AcpiAmlAcquireGlobalLock (
    UINT32                  Rule);

ACPI_STATUS
AcpiAmlReleaseGlobalLock (
    BOOLEAN                 Locked);

UINT32
AcpiAmlBufSeq (
    void);

INT32
AcpiAmlDigitsNeeded (
    INT32                   Value,
    INT32                   Base);

ACPI_STATUS
AcpiAmlEisaIdToString (
    UINT32                  NumericId,
    INT8                    *OutString);


/*
 * amregion - default OpRegion handlers
 */

ACPI_STATUS
AcpiAmlSystemMemorySpaceHandler (
    UINT32                  Function,
    UINT32                  Address,
    UINT32                  BitWidth,
    UINT32                  *Value,
    void                    *HandlerContext,
    void                    *RegionContext);

ACPI_STATUS
AcpiAmlSystemIoSpaceHandler (
    UINT32                  Function,
    UINT32                  Address,
    UINT32                  BitWidth,
    UINT32                  *Value,
    void                    *HandlerContext,
    void                    *RegionContext);

ACPI_STATUS
AcpiAmlPciConfigSpaceHandler (
    UINT32                  Function,
    UINT32                  Address,
    UINT32                  BitWidth,
    UINT32                  *Value,
    void                    *HandlerContext,
    void                    *RegionContext);

ACPI_STATUS
AcpiAmlEmbeddedControllerSpaceHandler (
    UINT32                  Function,
    UINT32                  Address,
    UINT32                  BitWidth,
    UINT32                  *Value,
    void                    *HandlerContext,
    void                    *RegionContext);

ACPI_STATUS
AcpiAmlSmBusSpaceHandler (
    UINT32                  Function,
    UINT32                  Address,
    UINT32                  BitWidth,
    UINT32                  *Value,
    void                    *HandlerContext,
    void                    *RegionContext);


#endif /* __INTERP_H__ */
