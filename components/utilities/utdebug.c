/*
  __________________________________________________________________________
 |
 |
 |           Copyright (C) Intel Corporation 1994-1998
 |
 | All rights reserved. No part of this program or publication may be
 | reproduced, transmitted, transcribed, stored in a retrieval system, or
 | translated into any language or computer language, in any form or by any
 | means, electronic, mechanical, magnetic, optical, chemical, manual, or
 | otherwise, without the prior written permission of Intel Corporation.
 |__________________________________________________________________________
 |
 | excommon.c - Debug and error reporting routines
 | 
 |__________________________________________________________________________

*/

#define __EXCOMMON_C__
#define _THIS_MODULE        "excommon.c"

#include <acpi.h>
#include <acpiobj.h>
#include <amlexec.h>

#include <stdarg.h>


#define WIN_DS_REGISTER     0x0030

/* Globals */

char        * Why;
char        WhyBuf [WHYBUF_SIZE];
INT32       AcpiHook = 0;
INT32       __AcpiLibInitStatus = 0;


/* Debug switch */

#ifdef _DEBUG
INT32 DebugLevel = 0xFFFFFFFF;
#else
INT32 DebugLevel = 0;
#endif


/*****************************************************************************
 * 
 * FUNCTION:    Get/Set debug level
 *
 * DESCRIPTION: Get or set value of the debug flag
 *
 ****************************************************************************/


INT32
GetDebugLevel (void)
{

    return DebugLevel;
}

void
SetDebugLevel (INT32 dl)
{

    DebugLevel = dl;
}

void 
DisplayTable (void *Header, INT32 DisplayBitFlags)
{
    OsdPrintf (NULL, "DisplayTable called, not supported **********\n");
    return;
}

LogHandle
GetMasterLogHandle (void)
{

/*  return NO_LOG_HANDLE; */

    return 1; /* stdout; TEMPORARY!!! */
}



/*****************************************************************************
 * 
 * FUNCTION:    FunctionTrace
 *
 * DESCRIPTION: Function entry trace
 *
 ****************************************************************************/

void
FunctionTrace (INT32 LineNumber, char *ModuleName, char * FunctionName)
{

    DebugPrint (LineNumber, ModuleName, TRACE_FUNCTIONS,
                    "Entered Function: %s\n", FunctionName);
}


/*****************************************************************************
 * 
 * FUNCTION:    DebugPrint
 *
 * DESCRIPTION: Print error message 
 *
 ****************************************************************************/

void
DebugPrint (INT32 LineNumber, char *ModuleName, INT32 DebugLevel, char *Format, ...)
{
    va_list         args;


    va_start (args, Format);

    /* Need a case statement here, switch on the Debug level */

    OsdPrintf (NULL, "%10s(%04d): ", ModuleName, LineNumber);
    OsdVprintf ((OSD_FILE *) 1, Format, args);

    va_end (args);
}


/*****************************************************************************
 * 
 * FUNCTION:    DebugPrintRaw
 *
 * DESCRIPTION: Print error message -- without module/line indentifiers 
 *
 ****************************************************************************/

void
DebugPrintRaw (INT32 DebugLevel, char *Format, ...)
{
    va_list         args;


    va_start (args, Format);

    /* Need a case statement here, switch on the Debug level */

    OsdVprintf ((OSD_FILE *) 1, Format, args);

    va_end (args);
}


/*****************************************************************************
 * 
 * FUNCTION:    _ReportError
 *
 * DESCRIPTION: Print error message from KD table
 *
 ****************************************************************************/

void
_ReportError (ST_KEY_DESC_TABLE *KdtEntry, INT32 LineNumber, char *ModuleName)
{

    DebugPrint (LineNumber, ModuleName, GLOBAL_FATAL, 
                "*** %s\n", KdtEntry->Description);

    _Kinc_error (KdtEntry->Key, 
                    PACRLF, LineNumber, ModuleName, 0, 0);
    Why = KdtEntry->Description;
}


/*****************************************************************************
 * 
 * FUNCTION:    _ReportWarning
 *
 * DESCRIPTION: Print warning message from KD table
 *
 ****************************************************************************/

void
_ReportWarning (ST_KEY_DESC_TABLE *KdtEntry, INT32 LineNumber, char *ModuleName)
{

    DebugPrint (LineNumber, ModuleName, GLOBAL_WARN, 
                "*** %s\n", KdtEntry->Description);

    _Kinc_warning (KdtEntry->Key, 
                    PACRLF, LineNumber, ModuleName, 0, 0);
    Why = KdtEntry->Description;
}


/*****************************************************************************
 * 
 * FUNCTION:    _ReportInfo
 *
 * DESCRIPTION: Print information message from KD table
 *
 ****************************************************************************/

void
_ReportInfo (ST_KEY_DESC_TABLE *KdtEntry, INT32 LineNumber, char *ModuleName)
{

    DebugPrint (LineNumber, ModuleName, GLOBAL_INFO, 
                "*** %s\n", KdtEntry->Description);

    _Kinc_info (KdtEntry->Key, 
                    PACRLF, LineNumber, ModuleName, 0, 0);
    Why = KdtEntry->Description;
}


/*****************************************************************************
 * 
 * FUNCTION:    _AllocateObjectDesc
 *
 * RETURN:      Pointer to newly allocated object descriptor
 *
 * DESCRIPTION: Allocate a new object descriptor.  Gracefully handle
 *              error conditions.
 *
 ****************************************************************************/

void *
_AllocateObjectDesc (ST_KEY_DESC_TABLE *KdtEntry, INT32 LineNumber, char *ModuleName)
{
    OBJECT_DESCRIPTOR       *NewDesc;


    /* Attempt to allocate new descriptor */

    NewDesc = NEW (OBJECT_DESCRIPTOR);
    if (!NewDesc)
    {
        /* Allocation failed */
        
        _REPORT_ERROR (KdtEntry, LineNumber, ModuleName);
        OutOfMemory = TRUE;
    }

    return NewDesc;
}


/*****************************************************************************
 * 
 * FUNCTION:    _LocalAllocate 
 *
 * RETURN:      Pointer to newly allocated memory
 *
 * DESCRIPTION: Allocate memory.  Gracefully handle
 *              error conditions.
 *
 ****************************************************************************/

void *
_LocalAllocate (INT32 AllocSize, INT32 LineNumber, char *ModuleName)
{
    ST_KEY_DESC_TABLE   AKDT = {"0000", '1', "LocalAllocate: Memory allocation failure", 
                                              "LocalAllocate: Memory allocation failure"};
    void                *Block;


    Block = OsdAllocate ((size_t) AllocSize);
    if (!Block)
    {
        /* Report allocation error */

        _REPORT_ERROR (&AKDT, LineNumber, ModuleName);
        OutOfMemory = TRUE;
    }

    return Block;
}


/*****************************************************************************
 * 
 * FUNCTION:    Kinc functions
 *
 * RETURN:      none
 *
 * DESCRIPTION: Print error messages, perhaps increment global counters ?? TBD
 *
 ****************************************************************************/


void
_Kinc_error (char *a, INT32 b, INT32 c, char * d, INT32 e, INT32 f)
{

    OsdPrintf (NULL, "%10s(%04d): *** Error %s\n", d, c, a); 
    return;
}


void
Kinc_error (char *a, INT32 b)
{

    OsdPrintf (NULL, "*** Error %s\n", a); 
    return;
}


void
_Kinc_info (char *a, INT32 b, INT32 c, char * d, INT32 e, INT32 f)
{

    OsdPrintf (NULL, "%10s(%04d): *** Info %s\n", d, c, a); 
    return;
}

void
Kinc_info (char *a, INT32 b, INT32 c, char * d, INT32 e, INT32 f)
{

    OsdPrintf (NULL, "%10s(%04d): *** Info %s\n", d, c, a); 
    return;
}

void
_Kinc_warning (char *a, INT32 b, INT32 c, char * d, INT32 e, INT32 f)
{

    OsdPrintf (NULL, "%10s(%04d): *** Warning %s\n", d, c, a); 
    return;
}

void
Kinc_warning (char *a, INT32 b)
{

    OsdPrintf (NULL, "*** Warning %s\n", a); 
    return;
}

void
KFatalError (char * a, char * b, ...)
{
    /* Optional parameters (...) not implemented */


    OsdPrintf (NULL, "*** Fatal Error %s: %s\n", a, b);
    return;
}



/*****************************************************************************
 * 
 * FUNCTION:    OFT functions
 *
 * RETURN:      none
 *
 * DESCRIPTION: Purpose unknown ?? TBD
 *
 ****************************************************************************/

void
CloseOFT (void)
{

    OsdPrintf (NULL, "CloseOFT called, not supported **********\n");
    return;
}

void
RestoreOFT (void)
{

    OsdPrintf (NULL, "RestoreOFT called, not supported **********\n");
    return;
}

void
SetNotSupported (void)
{
    OsdPrintf (NULL, "SetNotSupported called, not supported **********\n");
    return;
}


/*****************************************************************************
 * 
 * FUNCTION:    Interrupt functions
 *
 * RETURN:      none
 *
 * DESCRIPTION: Install/deinstall
 *
 ****************************************************************************/

/* Interrupt handlers */

UINT32
InstallInterruptHandler (
    UINT8               InterruptNumber,
    INT32               (* Isr)(void),
    UINT8               InterruptTaskFlag,
    UINT32 *            ExceptPtr)
{

    UINT32 RetVal;


    OsdPrintf (NULL, "InstallInterruptHandler called, not supported **********\n");

    RetVal = (UINT32) OsdInstallInterruptHandler ((UINT32) InterruptNumber, 
                                                    Isr, ExceptPtr);

    
    return(RetVal);

}

INT32
RemoveInterruptHandler (UINT32 Key)
{

    OsdPrintf (NULL, "RemoveInterruptHandler called, not supported **********\n");
    return 0;
}


/*****************************************************************************
 * 
 * FUNCTION:    DumpBuf
 *
 * RETURN:      none
 *
 * DESCRIPTION: Generic dump buffer in hex and ascii.
 *
 ****************************************************************************/

void
DumpBuf (UINT8 *Buffer, UINT32 Count, INT32 Flags, LogHandle LogFile,
            INT32 iLogFlags)
{
    UINT32      i = 0;
    UINT32      j;
    UINT8       BufChar;


    /*
     * Nasty little dump buffer routine!
     */
    while (i <= Count)
    {
        /* Print current offset */

        OsdPrintf (NULL, "%05X    ", i);


        /* Print 16 hex chars */

        for (j = 0; j < 16; j++)
        {
            if (i + j >= Count)
                goto cleanup;

            OsdPrintf (NULL, "%02X ", Buffer[i + j]);
        }

        /* 
         * Print the ASCII equivalent characters
         * But watch out for the bad unprintable ones...
         */

        for (j = 0; j < 16; j++)
        {
            if (i + j >= Count)
                goto cleanup;

            BufChar = Buffer[i + j];
            if ((BufChar > 0x1F && BufChar < 0x2E) ||
                (BufChar > 0x2F && BufChar < 0x61) ||
                (BufChar > 0x60 && BufChar < 0x7F))

                OsdPrintf (NULL, "%c", BufChar);
            else
                OsdPrintf (NULL, ".");
                
        }

        /* Done with that line. */

        OsdPrintf (NULL, "\n");
        i += 16;
    }

    return;

cleanup:
    OsdPrintf (NULL, "\n");
    return;

}


