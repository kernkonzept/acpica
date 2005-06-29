
/******************************************************************************
 * 
 * Module Name: cmglobal - Global variables for the ACPI subsystem
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


#define __CMGLOBAL_C__
#define DEFINE_ACPI_GLOBALS

#include <acpi.h>
#include <events.h>
#include <namespace.h>
#include <interpreter.h>


#define _THIS_MODULE        "cmglobal.c"
#define _COMPONENT          MISCELLANEOUS


/* 
 * We want the debug switches statically initialized so they 
 * are already set when the debugger is entered.
 */

    /* Debug switch - level and trace mask */

#ifdef _DEBUG
UINT32                      DebugLevel = DEBUG_DEFAULT;
#else
UINT32                      DebugLevel = NORMAL_DEFAULT;
#endif

    /* Debug switch - layer (component) mask */

UINT32                      DebugLayer = ALL_COMPONENTS;



/* 
 * Human-readable decode of exception codes, mostly for debugging
 * These need to match the corresponding defines
 * Note that AE_PENDING is not an error, but indicates
 * that other alternatives should be checked.
 */
char            *ExceptionNames[] = 
{ 
    "AE_OK", 
    "AE_PENDING", 
    "AE_AML_ERROR",
    "AE_RETURN_VALUE",
    "AE_ERROR",
    "AE_NO_ACPI_TABLES",
    "AE_NO_NAMESPACE",
    "AE_NO_MEMORY",
    "AE_BAD_HEADER",
    "AE_BAD_CHECKSUM",
    "AE_BAD_PARAMETER",
    "AE_BAD_CHARACTER",
    "AE_NOT_FOUND",
    "AE_NULL_ENTRY",
    "AE_FILE_NOT_EXIST",
    "AE_BUFFER_OVERFLOW",
    "AE_HANDLER_EXISTS",
    "AE_TERMINATED",
    "AE_CONTEXT",
    "AE_NOT_IMPLEMENTED",
    "AE_UNKNOWN_STATUS"
};



/******************************************************************************
 *
 * Namespace globals
 *
 ******************************************************************************/



/* Scope stack */

SCOPE_STACK     ScopeStack[MAX_SCOPE_NESTING];
SCOPE_STACK     *CurrentScope;


/* 
 * Names built-in to the interpreter
 *
 * Initial values are currently supported only for types String and Number.
 * To avoid type punning, both are specified as strings in this table.
 */

PREDEFINED_NAMES PreDefinedNames[] = {
    {"_GPE",    TYPE_DefAny},
    {"_PR_",    TYPE_DefAny},
    {"_SB_",    TYPE_DefAny},
    {"_SI_",    TYPE_DefAny},
    {"_TZ_",    TYPE_DefAny},
    {"_REV",    TYPE_Number, "2"},
    {"_OS_",    TYPE_String, "Intel AML interpreter"},
    {"_GL_",    TYPE_Mutex},

    /* Table terminator */

    {(char *)0, TYPE_Any}
};


/* 
 * Elements of NsProperties are bit significant
 * and should be one-to-one with values of NsType in acpinmsp.h
 */

INT32 NsProperties[] = {     /* properties of types */
    0,                      /* Any              */
    0,                      /* Number           */
    0,                      /* String           */
    0,                      /* Buffer           */
    LOCAL,                  /* Package          */
    0,                      /* FieldUnit        */
    NEWSCOPE | LOCAL,       /* Device           */
    LOCAL,                  /* Event            */
    NEWSCOPE | LOCAL,       /* Method           */
    LOCAL,                  /* Mutex            */
    LOCAL,                  /* Region           */
    NEWSCOPE | LOCAL,       /* Power            */
    NEWSCOPE | LOCAL,       /* Processor        */
    NEWSCOPE | LOCAL,       /* Thermal          */
    0,                      /* Alias            */
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0,
    0,                      /* DefField         */
    0,                      /* BankField        */
    0,                      /* IndexField       */
    0,                      /* DefFieldDefn     */
    0,                      /* BankFieldDefn    */
    0,                      /* IndexFieldDefn   */
    0,                      /* If               */
    0,                      /* Else             */
    0,                      /* While            */
    NEWSCOPE,               /* Scope            */
    LOCAL,                  /* DefAny           */
    0                       /* Lvalue           */
};

char BadType[] = "ERROR: unused type encoding found in table";


/* 
 * Elements of NsTypeNames should be
 * one-to-one with values of NsType in acpinmsp.h
 */

/* 
 * The type Any is used as a "don't care" when searching; when stored in a
 * table it really means that we have thus far seen no evidence to indicate
 * what type is actually going to be stored for this entry.
 */

char *NsTypeNames[] = { /* printable names of types */
    "Unknown",
    "Number",
    "String",
    "Buffer",
    "Package",
    "FieldUnit",
    "Device",
    "Event",
    "Method",
    "Mutex",
    "Region",
    "Power",
    "Processor",
    "Thermal",
    "Alias",
    BadType, 
    BadType, 
    BadType, 
    BadType, 
    BadType,
    BadType, 
    BadType, 
    BadType, 
    BadType, 
    BadType,
    "DefField",
    "BankField",
    "IndexField",
    "DefFieldDefn",
    "BankFieldDefn",
    "IndexFieldDefn",
    "If",
    "Else",
    "While",
    "Scope",
    "ERROR: DefAny found in table", /* should never happen */
    "ERROR: Lvalue found in table"  /* should never happen */
};




/******************************************************************************
 *
 * Interpreter globals
 *
 ******************************************************************************/



/* 
 * Method Stack, containing locals and args
 * per level, 0-7 are Local# and 8-14 are Arg#
 */

ACPI_OBJECT             *MethodStack[AML_METHOD_MAX_NEST][ARGBASE+NUMARG];
INT32                   MethodStackTop = -1;



/* 
 * Package stack, used for keeping track of nested AML packages.
 * Grows upwards.
 */
INT32                   PkgStackLevel;
INT32                   PkgStack_Len[AML_PKG_MAX_NEST];
UINT8                   *PkgStack_Code[AML_PKG_MAX_NEST];


/* Object Stack */
/* values are NsHandle or ObjHandle */

void                    *ObjStack[AML_EXPR_MAX_NEST]; 
INT32                   ObjStackTop = 0;






/******************************************************************************
 *
 * Event globals
 *
 ******************************************************************************/

UINT32                  SciHandle;
FIXED_EVENT_HANDLER     FixedEventHandlers[NUM_FIXED_EVENTS];


#ifdef _DEBUG
UINT32                  EventCount[NUM_FIXED_EVENTS];   
#endif





/****************************************************************************
 *
 * FUNCTION:    InitAcpiLibGlobals
 *
 * PARAMETERS:  none
 *
 * DESCRIPTION: Init library globals.  All globals that require specific
 *              initialization should be initialized here!
 *
 ***************************************************************************/

void 
InitAcpiLibGlobals (void)
{
    FUNCTION_TRACE ("InitAcpiLibGlobals");

    
    /* Table pointers */
    
    RSDP                    = NULL;
    RSDT                    = NULL;
    FACS                    = NULL;
    FACP                    = NULL;
    MAPIC                   = NULL;
    DSDT                    = NULL;
    PSDT                    = NULL;
    SSDT                    = NULL;
    SBDT                    = NULL;
    
    /* Miscellaneous variables */
    
    Capabilities            = 0;
    GlobalLockSet           = FALSE;
    
    /* File handles and names */
    
    DsdtFile                = NULL;
    OutputFile              = NULL;
    InputFile               = NULL;
    
    RsdpOriginalLocation    = 0;
    
    /* Interpreter */

    BufSeq                  = 0;
    NamedObjectErr          = FALSE;

    /* Hardware oriented */

    Gpe0EnableRegisterSave  = NULL;
    Gpe1EnableRegisterSave  = NULL;
    OriginalMode            = -1;   /*  original ACPI/legacy mode   */
    SciHandle				= 0;

    /* Namespace */

    RootObject                  = &RootObjStruct;

    RootObject->Name            = NS_ROOT;
    RootObject->Scope           = NULL;
    RootObject->ParentScope     = NULL;
    RootObject->ParentEntry     = NULL;
    RootObject->NextEntry       = NULL;
    RootObject->PrevEntry       = NULL;
    RootObject->Type            = TYPE_Any;
    RootObject->Value           = NULL;


    /* Debug */

    NestingLevel            = 0;


    FUNCTION_EXIT;
}   


/******************************************************************************
 *
 * FUNCTION:    AcpiLocalCleanup
 *
 * PARAMETERS:  none
 *
 * RETURN:      none
 *
 * DESCRIPTION: free memory allocated for table storage.
 *
 ******************************************************************************/

void
AcpiLocalCleanup (void)
{
    FUNCTION_TRACE ("AcpiLocalCleanup");


    /* 
     * TBD: !!! MAKE OS INDEPENDENT!!
     *
     * iRMX does not allocate memory for the tables unless they are loaded from a
     * file. 
     */

    if (InputFile)
    {
        if (InputFile && RSDP != NULL)   
            OsdFree (RSDP);
        
        if (RSDT != NULL)  
            OsdFree (RSDT);
        
        if (FACS != NULL)  
            OsdFree (FACS);
        
        if (FACP != NULL)  
            OsdFree (FACP);
        
        if (MAPIC != NULL) 
            OsdFree (MAPIC);
        
        if (DSDT != NULL)  
            OsdFree (DSDT);
        
        if (PSDT != NULL)  
            OsdFree (PSDT);
        
        if (SSDT != NULL)  
            OsdFree (SSDT);
        
        if (SBDT != NULL)  
            OsdFree (SBDT);

    }

    FUNCTION_EXIT;
}

