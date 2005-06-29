/******************************************************************************
 *
 * Module Name: aemain - Main routine for the AcpiExec utility
 *              $Revision: 1.64 $
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2002, Intel Corp.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "acpi.h"
#include "amlcode.h"
#include "acparser.h"
#include "acnamesp.h"
#include "acinterp.h"
#include "acdebug.h"

#include "aecommon.h"

#ifdef _DEBUG  
#if ACPI_MACHINE_WIDTH != 16
#include <crtdbg.h>
#endif     
#endif

#define _COMPONENT          PARSER
        ACPI_MODULE_NAME    ("aemain")

/*
 * TBD: Debug only, remove!
 */
#if ACPI_MACHINE_WIDTH == 32
void
AcpiCompare (
    UINT64_OVERLAY          Dividend,
    UINT64_OVERLAY          Divisor,
    UINT64_OVERLAY          LibDiv,
    UINT64_OVERLAY          Div,
    UINT64_OVERLAY          LibMod,
    UINT64_OVERLAY          Mod)
{

    if (LibDiv.Full != Div.Full)
    {
        AcpiOsPrintf ("Mismatch-DIV: n=%8.8X%8.8X d=%8.8X%8.8X, lr=%8.8X%8.8X ar=%8.8X%8.8X\n",
                Dividend.Part.Hi,       Dividend.Part.Lo,
                Divisor.Part.Hi,        Divisor.Part.Lo,
                LibDiv.Part.Hi,         LibDiv.Part.Lo,
                Div.Part.Hi,            Div.Part.Lo);
    }

    if (LibMod.Full != Mod.Full)
    {
        AcpiOsPrintf ("Mismatch-MOD: n=%8.8X%8.8X d=%8.8X%8.8X, lr=%8.8X%8.8X ar=%8.8X%8.8X\n",
                Dividend.Part.Hi,       Dividend.Part.Lo,
                Divisor.Part.Hi,        Divisor.Part.Lo,
                LibMod.Part.Hi,         LibMod.Part.Lo,
                Mod.Part.Hi,            Mod.Part.Lo);
    }
}

    /* Check answer against the library (DEBUG ONLY) */
/*
    CompareDiv.Full = Dividend.Full / Divisor.Full;
    CompareMod.Full = Dividend.Full % Divisor.Full;
    AcpiCompare (Dividend, Divisor, CompareDiv, Quotient, CompareMod, Remainder);
*/
void
AeDoDivideCheck (void)
{
    UINT32                  i;
    UINT64_OVERLAY          CompareDiv;
    UINT64_OVERLAY          CompareMod;
    UINT64_OVERLAY          Dividend;
    UINT64_OVERLAY          Divisor;
    UINT64_OVERLAY          Quotient;
    UINT64_OVERLAY          Remainder;


    for (i = 1; i < 0xFFFFFF; i++)
    {
        Dividend.Part.Hi = rand ();
        Dividend.Part.Lo = rand ();
        Divisor.Part.Hi  = rand ();
        Divisor.Part.Lo  = rand ();

        CompareDiv.Full = Dividend.Full / Divisor.Full;
        CompareMod.Full = Dividend.Full % Divisor.Full;

        AcpiUtDivide (&Dividend.Full, &Divisor.Full, &Quotient.Full, &Remainder.Full);

        AcpiCompare (Dividend, Divisor, CompareDiv, Quotient, CompareMod, Remainder);
    }

}
#else
void
AeDoDivideCheck (void)
{
}
#endif


#if ACPI_MACHINE_WIDTH == 16

ACPI_STATUS
AcpiGetIrqRoutingTable  (
    ACPI_HANDLE             DeviceHandle,
    ACPI_BUFFER             *RetBuffer)
{
    return AE_NOT_IMPLEMENTED;
}
#endif

/******************************************************************************
 *
 * FUNCTION:    usage
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Print a usage message
 *
 *****************************************************************************/

void
usage (void)
{
    printf ("Usage: acpiexec [-?dgis] [-l DebugLevel] [-o OutputFile] [AcpiTableFile]\n");
    printf ("Where:\n");
    printf ("    Input Options\n");
    printf ("        AcpiTableFile       Get ACPI tables from this file\n");
    printf ("    Output Options\n");
    printf ("    Miscellaneous Options\n");
    printf ("        -?                  Display this message\n");
    printf ("        -i                  Do not run INI methods\n");
    printf ("        -l DebugLevel       Specify debug output level\n");
    printf ("        -v                  Verbose init output\n");
}


/******************************************************************************
 *
 * FUNCTION:    main
 *
 * PARAMETERS:  argc, argv
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Main routine for AcpiDump utility
 *
 *****************************************************************************/

int ACPI_SYSTEM_XFACE
main (
    int                     argc,
    char                    **argv)
{
    int                     j;
    ACPI_STATUS             Status;
    UINT32                  InitFlags;
    ACPI_BUFFER             ReturnBuf;
    char                    Buffer[32];


#ifdef _DEBUG 
#if ACPI_MACHINE_WIDTH != 16
    _CrtSetDbgFlag (_CRTDBG_CHECK_ALWAYS_DF | _CrtSetDbgFlag(0));
#endif    
#endif

    /* Init globals */

    AcpiDbgLevel = NORMAL_DEFAULT;
    AcpiDbgLayer = 0xFFFFFFFF;


    AeDoDivideCheck ();

    printf ("ACPI AML Execution/Debug Utility ");

#if ACPI_MACHINE_WIDTH == 16
    printf ("(16-bit) ");
#else
    printf ("(32-bit) ");
#endif

    printf ("CA version %8.8lX [%s]\n", ACPI_CA_VERSION, __DATE__);

    /* Get the command line options */

    while ((j = getopt (argc, argv, "?dgil:o:sv")) != EOF) switch(j)
    {
    case 'd':
        AcpiGbl_DbOpt_disasm = TRUE;
        AcpiGbl_DbOpt_stats = TRUE;
        break;

    case 'g':
        AcpiGbl_DbOpt_tables = TRUE;
        AcpiGbl_DbFilename = NULL;
        break;

    case 'i':
        AcpiGbl_DbOpt_ini_methods = FALSE;
        break;

    case 'l':
        AcpiDbgLevel = strtoul (optarg, NULL, 0);
        AcpiGbl_DbConsoleDebugLevel = AcpiDbgLevel;
        printf ("Debug Level: %lX\n", AcpiDbgLevel);
        break;

    case 'o':
        printf ("O option is not implemented\n");
        break;

    case 's':
        AcpiGbl_DbOpt_stats = TRUE;
        break;

    case 'v':
        AcpiDbgLevel |= ACPI_LV_INIT;
        break;

    case '?':
    default:
        usage();
        return -1;
    }

    /* Init ACPI and start debugger thread */

    AcpiInitializeSubsystem ();

    InitFlags = (ACPI_NO_HANDLER_INIT | ACPI_NO_ACPI_ENABLE);
    if (!AcpiGbl_DbOpt_ini_methods)
    {
        InitFlags |= (ACPI_NO_DEVICE_INIT | ACPI_NO_OBJECT_INIT);
    }

    /* Standalone filename is the only argument */

    if (argv[optind])
    {
        AcpiGbl_DbOpt_tables = TRUE;
        AcpiGbl_DbFilename = argv[optind];

        Status = AcpiDbGetAcpiTable (AcpiGbl_DbFilename);
        if (ACPI_FAILURE (Status))
        {
            printf ("**** Could not get input table, %s\n", AcpiFormatException (Status));
            goto enterloop;
        }


        AeBuildLocalTables ();
        Status = AeInstallTables ();
        if (ACPI_FAILURE (Status))
        {
            printf ("**** Could not load ACPI tables, %s\n", AcpiFormatException (Status));
            goto enterloop;
        }

        /*
         * TBD:
         * Need a way to call this after the "LOAD" command
         */
        Status = AeInstallHandlers ();
        if (ACPI_FAILURE (Status))
        {
            goto enterloop;
        }

        Status = AcpiEnableSubsystem (InitFlags);
        if (ACPI_FAILURE (Status))
        {
            printf ("**** Could not EnableSubsystem, %s\n", AcpiFormatException (Status));
            goto enterloop;
        }

        ReturnBuf.Length = 32;
        ReturnBuf.Pointer = Buffer;
        AcpiGetName (AcpiGbl_RootNode, ACPI_FULL_PATHNAME, &ReturnBuf);
        AcpiEnableEvent (ACPI_EVENT_GLOBAL, ACPI_EVENT_FIXED, 0);
        AcpiEnableEvent (0, ACPI_EVENT_GPE, 0);
    }

#if ACPI_MACHINE_WIDTH == 16
    else
    {
#include "16bit.h"

        Status = AfFindDsdt (NULL, NULL);
        if (ACPI_FAILURE (Status))
        {
            goto enterloop;
        }


        if (ACPI_FAILURE (Status))
        {
            printf ("**** Could not load ACPI tables, %s\n", AcpiFormatException (Status));
            goto enterloop;
        }


        Status = AcpiNsLoadNamespace ();
        if (ACPI_FAILURE (Status))
        {
            printf ("**** Could not load ACPI namespace, %s\n", AcpiFormatException (Status));
            goto enterloop;
        }

        /* TBD:
         * Need a way to call this after the "LOAD" command
         */
        Status = AeInstallHandlers ();
        if (ACPI_FAILURE (Status))
        {
            goto enterloop;
        }

        Status = AcpiEnableSubsystem (InitFlags);
        if (ACPI_FAILURE (Status))
        {
            printf ("**** Could not EnableSubsystem, %s\n", AcpiFormatException (Status));
            goto enterloop;
        }
     }
#endif

enterloop:

    /* Enter the debugger command loop */

    AcpiDbUserCommands (ACPI_DEBUGGER_COMMAND_PROMPT, NULL);

    return 0;
}
