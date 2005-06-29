/*__________________________________________________________________________
 |
 |
 |           Copyright (C) Intel Corporation 1994-1996
 |
 | All rights reserved.  No part of this program or publication may be
 | reproduced, transmitted, transcribed, stored in a retrieval system, or
 | translated into any language or computer language, in any form or by any
 | means, electronic, mechanical, magnetic, optical, chemical, manual, or
 | otherwise, without the prior written permission of Intel Corporation.
 |__________________________________________________________________________
 |
 | FILENAME: datatypes.h - Common data types for the ACPI subsystem
 |__________________________________________________________________________
 */

#ifndef _DATATYPES_H
#define _DATATYPES_H

#include <stddef.h>


/* 
 * Data types - Fixed across all compilation models
 * 
 * BOOLEAN	    Logical Boolean. 1 byte value containing a 0 for FALSE or a 1 for TRUE.  Other values are undefined.
 * INT	        Signed value. (4 bytes on IA-32, 8 bytes on IA-64)
 * UINT	        Unsigned value. (4 bytes on IA-32, 8 bytes on IA-64)
 * INT8	        1 byte signed value
 * UINT8	    1 byte unsigned value
 * INT16	    2 byte signed value
 * UINT16	    2 byte unsigned value
 * INT32	    4 byte signed value
 * UINT32	    4 byte unsigned value
 * INT64        8 byte signed value (not supported or used at this time)
 * UINT64	    8 byte unsigned value (not supported or used at this time)
 * UCHAR	    Character. 1 byte unsigned value. 
 * WCHAR	    Wide Character. 2 byte unsigned value (not supported or used at this time) 
 */

/* 
 * TBD:  Need a 32-bit vs. 64-bit section !!! 
 */

/*  IA-32 type definitions */

typedef unsigned char   BOOLEAN;
typedef int             INT;
typedef unsigned int    UINT;
typedef signed char     INT8;
typedef unsigned char   UINT8;
typedef short           INT16;
typedef unsigned short  UINT16;
typedef int             INT32;
typedef unsigned int    UINT32;
typedef unsigned char   UCHAR;

typedef unsigned char   UINT8_BIT;
typedef unsigned short  UINT16_BIT;
typedef unsigned long   UINT32_BIT;


/* 
 * Data manipulation macros 
 */

#ifndef LOWORD
#define LOWORD(l)       ((UINT16)(UINT32)(l))
#endif

#ifndef HIWORD
#define HIWORD(l)       ((UINT16)((((UINT32)(l)) >> 16) & 0xFFFF))
#endif  

#ifndef LOBYTE
#define LOBYTE(l)       ((UINT8)(UINT16)(l))
#endif

#ifndef HIBYTE
#define HIBYTE(l)       ((UINT8)((((UINT16)(l)) >> 8) & 0xFF))
#endif

#define BIT0(x) 	    ((((x) & 0x01) > 0) ? 1 : 0)
#define BIT1(x)    	    ((((x) & 0x02) > 0) ? 1 : 0)
#define BIT2(x)   	    ((((x) & 0x04) > 0) ? 1 : 0)
#define BIT3(x)   	    ((((x) & 0x08) > 0) ? 1 : 0)
#define BIT4(x)   	    ((((x) & 0x10) > 0) ? 1 : 0)
#define BIT5(x)    	    ((((x) & 0x20) > 0) ? 1 : 0)
#define BIT6(x)  	    ((((x) & 0x40) > 0) ? 1 : 0)
#define BIT7(x)    	    ((((x) & 0x80) > 0) ? 1 : 0)

#define LOW_BASE(w)     ((UINT16) ((w) & 0x0000FFFF))
#define MID_BASE(b)     ((UINT8) (((b) & 0x00FF0000) >> 16))
#define HI_BASE(b)      ((UINT8) (((b) & 0xFF000000) >> 24))
#define LOW_LIMIT(w)  	((UINT16) ((w) & 0x0000FFFF))
#define HI_LIMIT(b)     ((UINT8) (((b) & 0x00FF0000) >> 16))


/* 
 * Useful defines 
 */

#ifdef FALSE
#undef FALSE
#endif
#define FALSE           (1 == 0)

#ifdef TRUE
#undef TRUE
#endif
#define TRUE            (1 == 1)

#ifndef NULL
#define NULL            (void *)0
#endif


/* 
 * Local datatypes 
 */

/* Types specific to the OS-dependent interface */

typedef void            OSD_FILE;

/* File operations */

typedef INT32           LogHandle;
#define NO_LOG_HANDLE   ((LogHandle) -1)


/*
 * Debug/Error reporting table definition
 */

typedef struct
{
    char            *Key;
    char            Level;
    char            *Description;
    char            *Description2;

} ST_KEY_DESC_TABLE;



/* Operational mode of AML scanner */

typedef enum {
    Load    = 0,
    Exec    = 1,
    Load1   = 2
} OpMode;


/* Control method information struct */

typedef struct
{
   ptrdiff_t            Offset;      /* offset to MethodFlags in AML pcode block */
   UINT32               Length;      /* length of method code including MethodFlags */
} meth;

/* NsHandle is actually an nte * */

typedef void*           NsHandle;


/* 
 * Types associated with names
 * Values correspond to the definition of the ObjectType operator.
 * Must coordinate with NsProperties and NsTypeNames in acpinmsp.c
 * The NsType type is also used in OBJECT_DESCRIPTOR defined in amlpriv.h
 */

typedef enum {
   Any              =  0,
   Number           =  1, /* Byte/Word/Dword/Zero/One/Ones */
   String           =  2,
   Buffer           =  3,
   Package          =  4, /* ByteConst, multiple DataTerm/Constant/SuperName */
   FieldUnit        =  5,
   Device           =  6, /* Name, multiple NamedObject */
   Event            =  7,
   Method           =  8, /* Name, ByteConst, multiple Code */
   Mutex            =  9,
   Region           = 10,
   Power            = 11, /* Name,ByteConst,WordConst,multi NamedObject */
   Processor        = 12, /* Name,ByteConst,DWordConst,ByteConst,multi NmO */
   Thermal          = 13, /* Name, multiple NamedObject */
   Alias            = 14,

/* 
 * The remaining values do not relate to the ObjectType operator and are
 * used for various internal purposes.  A gap is provided in case more
 * official ObjectType's are added in the future.  Also, values exceeding
 * the largest ObjectType need to not overlap with defined AML opcodes.
 */
   DefField         = 25,
   BankField        = 26,
   IndexField       = 27,
   DefFieldDefn     = 28, /* Name, ByteConst, multiple FieldElement */
   BankFieldDefn    = 29, /* 2 Name,DWordConst,ByteConst,multi FieldElement */
   IndexFieldDefn   = 30, /* 2 Name, ByteConst, multiple FieldElement */
   If               = 31, /* OpCode, multiple Code */
   Else             = 32, /* multiple Code */
   While            = 33, /* OpCode, multiple Code */
   Scope            = 34, /* Name, multiple NamedObject */
   DefAny           = 35, /* type is Any, suppress search of enclosing scopes */
   Lvalue           = 36  /* Arg#, Local#, Name, Debug; used only in descriptors */
} NsType;




/* 
 * Typedef nte (name table entry) is private to acpinmsp.c to avoid global
 * impact in the event of changes to it.  The externally-known type NsHandle
 * is actually an (nte *).  If an external program needs to extract a field
 * from the nte, it should use an access function defined in acpinmsp.c
 *
 * If you need an access function not provided herein, add it to this module
 * rather than exporting the nte typedef.
 *
 * (nte *) are actually used in two different and not entirely compatible
 * ways: as pointer to an individual nte and as pointer to an entire name
 * table (which is an array of nte, sometimes referred to as a scope).  In
 * the latter case, the specific nte pointed to may be unused; however its
 * ParentScope member will be valid.
 */

typedef struct nte
{
    UINT32          NameSeg;        /* name segment, always 4 chars per ACPI spec.
                                     * NameSeg must be the first field in the nte
                                     * -- see the IsNsHandle macro in acpinmsp.h
                                     */
    struct nte      *ChildScope;    /* next level of names */
    struct nte      *ParentScope;   /* previous level of names */
    NsType          NtType;         /* type associated with name */
    void            *ptrVal;        /* pointer to value */
} nte;

#define NOTFOUND    (nte *)0


/* Stack of currently-open scopes, and pointer to top of that stack */

typedef struct
{
    nte         *Scope;
    /* 
     * Type of scope, typically the same as the type of its parent's entry 
     * (but not the same as the type of its parent's scope).
     */
    NsType      Type;   
} SCOPE_STACK;    




#endif /* DATATYPES_H */