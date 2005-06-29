
/******************************************************************************
 * 
 * Name: amlcode.h - Definitions for AML, as included in "definition blocks"
 *                   Declarations and definitions contained herein are derived 
 *                   directly from the ACPI specification.
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

#ifndef __AMLCODE_H__
#define __AMLCODE_H__


/* primary opcodes */

#define AML_NullChar                (UINT16) 0x00

#define AML_ZeroOp                  (UINT16) 0x00
#define AML_OneOp                   (UINT16) 0x01
#define AML_UNASSIGNED              (UINT16) 0x02
#define AML_AliasOp                 (UINT16) 0x06
#define AML_NameOp                  (UINT16) 0x08
#define AML_ByteOp                  (UINT16) 0x0a
#define AML_WordOp                  (UINT16) 0x0b
#define AML_DWordOp                 (UINT16) 0x0c
#define AML_StringOp                (UINT16) 0x0d
#define AML_ScopeOp                 (UINT16) 0x10
#define AML_BufferOp                (UINT16) 0x11
#define AML_PackageOp               (UINT16) 0x12
#define AML_MethodOp                (UINT16) 0x14
#define AML_DualNamePrefix          (UINT16) 0x2e
#define AML_MultiNamePrefixOp       (UINT16) 0x2f
#define AML_NameCharSubseq          (UINT16) 0x30
#define AML_NameCharFirst           (UINT16) 0x41
#define AML_OpPrefix                (UINT16) 0x5b
#define AML_RootPrefix              (UINT16) 0x5c
#define AML_ParentPrefix            (UINT16) 0x5e
#define AML_LocalOp                 (UINT16) 0x60
#define AML_Local0                  (UINT16) 0x60
#define AML_Local1                  (UINT16) 0x61
#define AML_Local2                  (UINT16) 0x62
#define AML_Local3                  (UINT16) 0x63
#define AML_Local4                  (UINT16) 0x64
#define AML_Local5                  (UINT16) 0x65
#define AML_Local6                  (UINT16) 0x66
#define AML_Local7                  (UINT16) 0x67
#define AML_ArgOp                   (UINT16) 0x68
#define AML_Arg0                    (UINT16) 0x68
#define AML_Arg1                    (UINT16) 0x69
#define AML_Arg2                    (UINT16) 0x6a
#define AML_Arg3                    (UINT16) 0x6b
#define AML_Arg4                    (UINT16) 0x6c
#define AML_Arg5                    (UINT16) 0x6d
#define AML_Arg6                    (UINT16) 0x6e
#define AML_StoreOp                 (UINT16) 0x70
#define AML_RefOfOp                 (UINT16) 0x71
#define AML_AddOp                   (UINT16) 0x72
#define AML_ConcatOp                (UINT16) 0x73
#define AML_SubtractOp              (UINT16) 0x74
#define AML_IncrementOp             (UINT16) 0x75
#define AML_DecrementOp             (UINT16) 0x76
#define AML_MultiplyOp              (UINT16) 0x77
#define AML_DivideOp                (UINT16) 0x78
#define AML_ShiftLeftOp             (UINT16) 0x79
#define AML_ShiftRightOp            (UINT16) 0x7a
#define AML_BitAndOp                (UINT16) 0x7b
#define AML_BitNandOp               (UINT16) 0x7c
#define AML_BitOrOp                 (UINT16) 0x7d
#define AML_BitNorOp                (UINT16) 0x7e
#define AML_BitXorOp                (UINT16) 0x7f
#define AML_BitNotOp                (UINT16) 0x80
#define AML_FindSetLeftBitOp        (UINT16) 0x81
#define AML_FindSetRightBitOp       (UINT16) 0x82
#define AML_DerefOfOp               (UINT16) 0x83
#define AML_NotifyOp                (UINT16) 0x86
#define AML_SizeOfOp                (UINT16) 0x87
#define AML_IndexOp                 (UINT16) 0x88
#define AML_MatchOp                 (UINT16) 0x89
#define AML_DWordFieldOp            (UINT16) 0x8a
#define AML_WordFieldOp             (UINT16) 0x8b
#define AML_ByteFieldOp             (UINT16) 0x8c
#define AML_BitFieldOp              (UINT16) 0x8d
#define AML_TypeOp                  (UINT16) 0x8e
#define AML_LAndOp                  (UINT16) 0x90
#define AML_LOrOp                   (UINT16) 0x91
#define AML_LNotOp                  (UINT16) 0x92
#define AML_LEqualOp                (UINT16) 0x93
#define AML_LGreaterOp              (UINT16) 0x94
#define AML_LLessOp                 (UINT16) 0x95
#define AML_IfOp                    (UINT16) 0xa0
#define AML_ElseOp                  (UINT16) 0xa1
#define AML_WhileOp                 (UINT16) 0xa2
#define AML_NoopCode                (UINT16) 0xa3
#define AML_ReturnOp                (UINT16) 0xa4
#define AML_BreakOp                 (UINT16) 0xa5
#define AML_BreakPointOp            (UINT16) 0xcc
#define AML_OnesOp                  (UINT16) 0xff

/* prefixed opcodes */

#define AML_EXTOP                   (UINT16) 0x005b


#define AML_MutexOp                 (UINT16) 0x5b01
#define AML_EventOp                 (UINT16) 0x5b02
#define AML_ShiftRightBitOp         (UINT16) 0x5b10
#define AML_ShiftLeftBitOp          (UINT16) 0x5b11
#define AML_CondRefOfOp             (UINT16) 0x5b12
#define AML_CreateFieldOp           (UINT16) 0x5b13
#define AML_LoadOp                  (UINT16) 0x5b20
#define AML_StallOp                 (UINT16) 0x5b21
#define AML_SleepOp                 (UINT16) 0x5b22
#define AML_AcquireOp               (UINT16) 0x5b23
#define AML_SignalOp                (UINT16) 0x5b24
#define AML_WaitOp                  (UINT16) 0x5b25
#define AML_ResetOp                 (UINT16) 0x5b26
#define AML_ReleaseOp               (UINT16) 0x5b27
#define AML_FromBCDOp               (UINT16) 0x5b28
#define AML_ToBCDOp                 (UINT16) 0x5b29
#define AML_UnLoadOp                (UINT16) 0x5b2a
#define AML_RevisionOp              (UINT16) 0x5b30
#define AML_DebugOp                 (UINT16) 0x5b31
#define AML_FatalOp                 (UINT16) 0x5b32
#define AML_RegionOp                (UINT16) 0x5b80
#define AML_DefFieldOp              (UINT16) 0x5b81
#define AML_DeviceOp                (UINT16) 0x5b82
#define AML_ProcessorOp             (UINT16) 0x5b83
#define AML_PowerResOp              (UINT16) 0x5b84
#define AML_ThermalZoneOp           (UINT16) 0x5b85
#define AML_IndexFieldOp            (UINT16) 0x5b86
#define AML_BankFieldOp             (UINT16) 0x5b87


/* Bogus opcodes (they are actually two separate opcodes) */

#define AML_LGREATEREQUAL_OP        (UINT16) 0x9295
#define AML_LLESSEQUAL_OP           (UINT16) 0x9294
#define AML_LNOTEQUAL_OP            (UINT16) 0x9293


/* Internal opcodes */

#define AML_NAMEPATH_OP             (UINT16) 0x002d
#define AML_NAMEDFIELD_OP           (UINT16) 0x0030
#define AML_RESERVEDFIELD_OP        (UINT16) 0x0031
#define AML_ACCESSFIELD_OP          (UINT16) 0x0032
#define AML_BYTELIST_OP             (UINT16) 0x0033
#define AML_STATICSTRING_OP         (UINT16) 0x0034
#define AML_METHODCALL_OP           (UINT16) 0x0035




/*
 * argument types
 */

/*
#define AML_ASCIICHARLIST_ARG       'A'
#define AML_BYTEDATA_ARG            'b'
#define AML_BYTELIST_ARG            'B'
#define AML_DWORDDATA_ARG           'd'
#define AML_DATAOBJECT_ARG          'o'
#define AML_DATAOBJECTLIST_ARG      'O'
#define AML_FIELDLIST_ARG           'F'
#define AML_NAMESTRING_ARG          'n'
#define AML_OBJECTLIST_ARG          'P'
#define AML_PKGLENGTH_ARG           'p'
#define AML_SUPERNAME_ARG           's'
#define AML_TARGET_ARG              'l'
#define AML_TERMARG_ARG             't'
#define AML_TERMLIST_ARG            'T'
#define AML_WORDDATA_ARG            'w'
*/


#define ARG_NONE                    0x0

/*
 * Argument types for the AML Parser 
 */

#define ARGP_CHARLIST               0x1
#define ARGP_BYTEDATA               0x2
#define ARGP_BYTELIST               0x3
#define ARGP_WORDDATA               0x4
#define ARGP_DWORDDATA              0x5
#define ARGP_DATAOBJ                0x6
#define ARGP_DATAOBJLIST            0x7
#define ARGP_FIELDLIST              0x8
#define ARGP_NAMESTRING             0x9
#define ARGP_OBJLIST                0xA
#define ARGP_PKGLENGTH              0xB
#define ARGP_SUPERNAME              0xC
#define ARGP_TARGET                 0xD
#define ARGP_TERMARG                0xE
#define ARGP_TERMLIST               0xF

/*
 * Resolved argument types for the AML Interpreter
 */

#define ARGI_ANYTYPE                0x1
#define ARGI_TARGETREF              0x2
#define ARGI_LVALUE                 0x3
#define ARGI_IF                     0x4
#define ARGI_NUMBER                 0x5
#define ARGI_STRING                 0x6
#define ARGI_BUFFER                 0x7
#define ARGI_PACKAGE                0x8
#define ARGI_DATAOBJECT             0x9     /* Buffer, string, or package */
#define ARGI_COMPLEXOBJ             0xA     /* Buffer or package */
#define ARGI_MUTEX                  0xB
#define ARGI_EVENT                  0xC

#define ARGI_INVALID_OPCODE         0xFFFFFFFF



/*
 * hash offsets
 */
#define AML_EXTOP_HASH_OFFSET       22
#define AML_LNOT_HASH_OFFSET        19


/*
 * opcode groups and types
 */

#define OPGRP_NAMED                 0x01
#define OPGRP_FIELD                 0x02
#define OPGRP_BYTELIST              0x04

#define OPTYPE_UNDEFINED            0


#define OPTYPE_LITERAL              1
#define OPTYPE_CONSTANT             2
#define OPTYPE_METHOD_ARGUMENT      3
#define OPTYPE_LOCAL_VARIABLE       4
#define OPTYPE_DATA_TERM            5

/* Type 1 opcodes */

#define OPTYPE_MONADIC1             6
#define OPTYPE_DYADIC1              7


/* Type 2 opcodes */

#define OPTYPE_MONADIC2             8
#define OPTYPE_MONADIC2R            9
#define OPTYPE_DYADIC2              10
#define OPTYPE_DYADIC2R             11
#define OPTYPE_DYADIC2S             12
#define OPTYPE_INDEX                13
#define OPTYPE_MATCH                14

/* Generic for an op that returns a value */

#define OPTYPE_METHOD_CALL          15


/* Misc */

#define OPTYPE_CREATE_FIELD         16
#define OPTYPE_FATAL                17
#define OPTYPE_CONTROL              18
#define OPTYPE_RECONFIGURATION      19
#define OPTYPE_NAMED_OBJECT         20

#define OPTYPE_BOGUS                21




/* Comparison operation codes for MatchOp operator */

typedef enum 
{
    MATCH_MTR                   = 0,
    MATCH_MEQ                   = 1,
    MATCH_MLE                   = 2,
    MATCH_MLT                   = 3,
    MATCH_MGE                   = 4,
    MATCH_MGT                   = 5

} AML_MATCH_OPERATOR;

#define MAX_MATCH_OPERATOR      5


/* Field Access Types */

#define ACCESS_TYPE_MASK        0x0f
#define ACCESS_TYPE_SHIFT       0

typedef enum 
{
    ACCESS_AnyAcc               = 0,
    ACCESS_ByteAcc              = 1,
    ACCESS_WordAcc              = 2,
    ACCESS_DWordAcc             = 3,
    ACCESS_BlockAcc             = 4,
    ACCESS_SMBSendRecvAcc       = 5,
    ACCESS_SMBQuickAcc          = 6

} AML_ACCESS_TYPE;


/* Field Lock Rules */

#define LOCK_RULE_MASK          0x10
#define LOCK_RULE_SHIFT         4

typedef enum 
{
    GLOCK_NeverLock             = 0,
    GLOCK_AlwaysLock            = 1

} AML_LOCK_RULE;


/* Field Update Rules */

#define UPDATE_RULE_MASK        0x060
#define UPDATE_RULE_SHIFT       5

typedef enum 
{
    UPDATE_Preserve             = 0,
    UPDATE_WriteAsOnes          = 1,
    UPDATE_WriteAsZeros         = 2

} AML_UPDATE_RULE;


/* bit fields in MethodFlags byte */

#define METHOD_FLAGS_ARG_COUNT  0x07
#define METHOD_FLAGS_SERIALIZED 0x08


/* Array sizes.  Used for range checking also */

#define NUM_REGION_TYPES        5
#define NUM_ACCESS_TYPES        7
#define NUM_UPDATE_RULES        3
#define NUM_MATCH_OPS           7
#define NUM_OPCODES             256
#define NUM_FIELD_NAMES         2

/* External declarations of the AML tables */

extern UINT8                    Gbl_Aml             [NUM_OPCODES];
extern UINT16                   Gbl_Pfx             [NUM_OPCODES];
extern char                     *Gbl_ShortOps       [NUM_OPCODES];
extern char                     *Gbl_LongOps        [NUM_OPCODES];
extern char                     *Gbl_RegionTypes    [NUM_REGION_TYPES];
extern char                     *Gbl_MatchOps       [NUM_MATCH_OPS];
extern char                     *Gbl_AccessTypes    [NUM_ACCESS_TYPES];
extern char                     *Gbl_UpdateRules    [NUM_UPDATE_RULES];
extern char                     *Gbl_FENames        [NUM_FIELD_NAMES];


/*
 * AML tables
 */

#ifdef DEFINE_AML_GLOBALS

/* Data used in keeping track of fields */

char            *Gbl_FENames[NUM_FIELD_NAMES] = 
{ 
    "skip", 
    "?access?" 
};              /* FE = Field Element */



/* Region type decoding */

char *Gbl_RegionTypes[NUM_REGION_TYPES] = 
{   
    "SystemMemory",
    "SystemIO",
    "PCIConfig",
    "EmbeddedControl",
    "SMBus"
};


char *Gbl_MatchOps[NUM_MATCH_OPS] = 
{   
    "Error", 
    "MTR", 
    "MEQ", 
    "MLE", 
    "MLT", 
    "MGE", 
    "MGT" 
};


/* Access type decoding */

char *Gbl_AccessTypes[NUM_ACCESS_TYPES] = 
{   
    "AnyAcc",
    "ByteAcc",
    "WordAcc",
    "DWordAcc",
    "BlockAcc",
    "SMBSendRecvAcc",
    "SMBQuickAcc"
};


/* Update rule decoding */

char *Gbl_UpdateRules[NUM_UPDATE_RULES] = 
{   
    "Preserve",
    "WriteAsOnes",
    "WriteAsZeros"
};




#endif /* DEFINE_AML_GLOBALS */

#endif /* __AMLCODE_H__ */
