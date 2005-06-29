
/******************************************************************************
 *
 * Module Name: aslmain - compiler main and utilities
 *              $Revision: 1.56 $
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


#define _DECLARE_GLOBALS

#include "aslcompiler.h"
#include "acnamesp.h"
#include "adisasm.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#define _COMPONENT          ACPI_COMPILER
        ACPI_MODULE_NAME    ("aslmain")

int                     optind;
NATIVE_CHAR             *optarg;

UINT32                  Gbl_ExceptionCount[ASL_NUM_REPORT_LEVELS] = {0,0,0};
char                    hex[] = {'0','1','2','3','4','5','6','7',
                                 '8','9','A','B','C','D','E','F'};


/*******************************************************************************
 *
 * FUNCTION:    Options
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Display option help message
 *
 ******************************************************************************/

void
Options (
    void)
{

    printf ("Options:\n");
    printf ("    -a               Create AML in a assembler source code file (*.asm)\n");
    printf ("    -c               Create AML in a C source code file (*.c)\n");
    printf ("    -d               Disassemble AML file to ASL source code file (*.dsl)\n");
    printf ("    -e               Less verbose errors and warnings for IDE\n");
    printf ("    -f               Disable constant folding\n");
    printf ("    -h               Additional help and compiler debug options\n");
    printf ("    -l               Create listing file (mixed ASL source and AML) (*.lst)\n");
    printf ("    -n               Create namespace file (*.nsp)\n");
    printf ("    -o <name>        Specify filename prefix for all output files\n");
    printf ("                             (including the .aml file)\n");
    printf ("    -qc              Display operators allowed in constant expressions\n");
    printf ("    -qr              Display ACPI reserved method names\n");
    printf ("    -s               Create combined (w/includes) ASL file (*.src)\n");
    printf ("    -t <a|c>         Create AML hex table in assembler or C (*.hex)\n");
}


/*******************************************************************************
 *
 * FUNCTION:    Usage
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Display help message
 *
 ******************************************************************************/

void
HelpMessage (
    void)
{

    printf ("AML output filename generation:\n");
    printf ("  Output filenames are generated by appending an extension to a common\n");
    printf ("  filename prefix.  The filename prefix is obtained via one of the\n");
    printf ("  following methods (in priority order):\n");
    printf ("    1) The -o option specifies the prefix\n");
    printf ("    2) The prefix of the AMLFileName in the ASL Definition Block\n");
    printf ("    3) The prefix of the input filename\n");
    printf ("\n");

    Options ();

    printf ("\nCompiler Debug Options:\n");
    printf ("    -b <p|t|b>       Create compiler debug/trace file (*.txt)\n");
    printf ("                             Types: Parse/Tree/Both\n");
    printf ("    -i               Ignore errors, always create AML output file(s)\n");
    printf ("    -p               Parse only, no output generation\n");
    printf ("    -x               Display compile times\n");
    printf ("    -v <trace level> Set debug level for trace output\n");
}


/*******************************************************************************
 *
 * FUNCTION:    Usage
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Display usage and option message
 *
 ******************************************************************************/

void
Usage (
    void)
{

    printf ("Usage:    %s <Options> <InputFile>\n\n", CompilerName);
    Options ();
}


/*******************************************************************************
 *
 * FUNCTION:    AslInitialize
 *
 * PARAMETERS:  None
 *
 * RETURN:      None
 *
 * DESCRIPTION: Initialize compiler globals
 *
 ******************************************************************************/

void
AslInitialize (void)
{
    UINT32              i;


#ifdef _DEBUG
    _CrtSetDbgFlag (_CRTDBG_CHECK_ALWAYS_DF | _CrtSetDbgFlag(0));
#endif

    AcpiDbgLevel = 0;

    for (i = 0; i < ASL_NUM_FILES; i++)
    {
        Gbl_Files[i].Handle = NULL;
        Gbl_Files[i].Filename = NULL;
    }

    Gbl_Files[ASL_FILE_STDOUT].Handle   = stdout;
    Gbl_Files[ASL_FILE_STDOUT].Filename = "STDOUT";

    Gbl_Files[ASL_FILE_STDERR].Handle   = stderr;
    Gbl_Files[ASL_FILE_STDERR].Filename = "STDERR";
}


/*******************************************************************************
 *
 * FUNCTION:    main
 *
 * PARAMETERS:  Standard argc/argv
 *
 * RETURN:      Program termination code
 *
 * DESCRIPTION: C main routine for the Asl Compiler.  Handle command line
 *              options and begin the compile.
 *
 ******************************************************************************/

int ACPI_SYSTEM_XFACE
main (
    int                 argc,
    char                **argv)
{
    int                 j;
    BOOLEAN             BadCommandLine = FALSE;
    int                 Status;
    BOOLEAN             AslToFile = TRUE;


    AslInitialize ();
    AslCompilerSignon (ASL_FILE_STDOUT);

    /* Minimum command line contains at least the input file */

    if (argc < 2)
    {
        Usage ();
        return 0;
    }

    /* Get the command line options */

    while ((j = getopt (argc, argv, "ab:cdefhilno:pq:rst:v:x")) != EOF) switch (j)
    {
    case 'a':

        /* Produce assembly code output file */

        Gbl_AsmOutputFlag = TRUE;
        break;

    case 'b':

        switch (optarg[0])
        {
        case 'b':
            AslCompilerdebug = 1; /* same as yydebug */
            break;

        case 'p':
            AslCompilerdebug = 1; /* same as yydebug */
            break;

        case 't':
            break;

        default:
            printf ("Unknown option: -d%s\n", optarg);
            BadCommandLine = TRUE;
        }

        /* Produce debug output file */

        Gbl_DebugFlag = TRUE;
        break;

    case 'c':

        /* Produce C hex output file */

        Gbl_C_OutputFlag = TRUE;
        break;

    case 'd':
        Gbl_DisasmFlag = TRUE;
        break;

    case 'e':

        /* Less verbose error messages */

        Gbl_VerboseErrors = FALSE;
        break;

    case 'f':

        /* Disable folding on "normal" expressions */

        Gbl_FoldConstants = FALSE;
        break;

    case 'h':

        HelpMessage ();
        return 0;

    case 'i':

        /* Ignore errors and always attempt to create aml file */

        Gbl_IgnoreErrors = TRUE;
        break;

    case 'l':

        /* Produce listing file (Mixed source/aml) */

        Gbl_ListingFlag = TRUE;
        break;

    case 'n':

        /* Produce namespace file */

        Gbl_NsOutputFlag = TRUE;
        break;

    case 'o':

        /* Override default AML output filename */

        Gbl_OutputFilenamePrefix = optarg;
        Gbl_UseDefaultAmlFilename = FALSE;
        break;

    case 'p':

        /* Parse only */

        Gbl_ParseOnlyFlag = TRUE;
        break;

    case 'q':

        switch (optarg[0])
        {
        case 'c':
            UtDisplayConstantOpcodes ();
            return (0);

        case 'r':
            /* reserved names */

            MpDisplayReservedNames ();
            return (0);

        default:
            printf ("Unknown option: -q%s\n", optarg);
            BadCommandLine = TRUE;
        }
        break;

    case 'r':
        AslToFile = FALSE;
        break;

    case 's':

        /* Produce combined source file */

        Gbl_SourceOutputFlag = TRUE;
        break;

    case 't':

        /* Produce hex table output file */

        switch (optarg[0])
        {
        case 'a':
            Gbl_HexOutputFlag = HEX_OUTPUT_ASM;
            break;

        case 'c':
            Gbl_HexOutputFlag = HEX_OUTPUT_C;
            break;

        default:
            printf ("Unknown option: -t%s\n", optarg);
            BadCommandLine = TRUE;
        }

        break;

    case 'v':

        AcpiDbgLevel = strtoul (optarg, NULL, 16);
        break;

    case 'x':

        /* Display compile time(s) */

        Gbl_CompileTimesFlag = TRUE;
        break;

    default:

        BadCommandLine = TRUE;
        break;
    }

    /* Next parameter must be the input filename */

    Gbl_Files[ASL_FILE_INPUT].Filename = argv[optind];
    if (!Gbl_Files[ASL_FILE_INPUT].Filename)
    {
        printf ("Missing input filename\n");
        BadCommandLine = TRUE;
    }

    /* Abort if anything went wrong on the command line */

    if (BadCommandLine)
    {
        printf ("\n");
        Usage ();
        return 0;
    }

    if ((optind + 1) < argc)
    {
        printf ("Warning: extra arguments (%d) after input filename are ignored\n\n", argc - optind - 1);
    }

    /* ACPI CA subsystem initialization */

    AcpiUtInitGlobals ();
    Status = AcpiUtMutexInitialize ();
    if (ACPI_FAILURE (Status))
    {
        return -1;
    }

    Status = AcpiNsRootInitialize ();
    if (ACPI_FAILURE (Status))
    {
        return -1;
    }

    /*
     * If -o not specified, we will use the input filename as the
     * output filename prefix
     */
    if (Gbl_UseDefaultAmlFilename)
    {
        Gbl_OutputFilenamePrefix = Gbl_Files[ASL_FILE_INPUT].Filename;
    }

    if (Gbl_DisasmFlag)
    {
        AcpiGbl_DbOpt_disasm = TRUE;
        Status = AdAmlDisassemble (AslToFile, Gbl_Files[ASL_FILE_INPUT].Filename);
        return (0);
    }

    Status = CmDoCompile ();
    return (0);
}


