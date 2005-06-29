
/******************************************************************************
 *
 * Module Name: abcompare - compare AML files
 *              $Revision: 1.15 $
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

#include "acpibin.h"
#include <stdlib.h>


FILE                *File1;
FILE                *File2;
ACPI_TABLE_HEADER   Header1;
ACPI_TABLE_HEADER   Header2;
struct stat         Gbl_StatBuf;

#define BUFFER_SIZE 256
char                Buffer[BUFFER_SIZE];


#define DB_CONSOLE_OUTPUT            0x02
#define ACPI_DB_REDIRECTABLE_OUTPUT  0x01

extern FILE                        *AcpiGbl_DebugFile = NULL;
extern UINT8                       AcpiGbl_DbOutputFlags = DB_CONSOLE_OUTPUT ;


/******************************************************************************
 *
 * FUNCTION:    AbValidateHeader
 *
 * DESCRIPTION: Check for valid ACPI table header
 *
 ******************************************************************************/

BOOLEAN
AbValidateHeader (
    ACPI_TABLE_HEADER       *Header)
{

    if (!AcpiUtValidAcpiName (* (UINT32 *) &Header->Signature))
    {
        printf ("Header signature is invalid\n");
        return FALSE;
    }

    return TRUE;
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiTbChecksum
 *
 * PARAMETERS:  Buffer              - Buffer to checksum
 *              Length              - Size of the buffer
 *
 * RETURNS      8 bit checksum of buffer
 *
 * DESCRIPTION: Computes an 8 bit checksum of the buffer(length) and returns it.
 *
 ******************************************************************************/

UINT8
AcpiTbChecksum (
    void                    *Buffer,
    UINT32                  Length)
{
    const UINT8             *limit;
    const UINT8             *rover;
    UINT8                   sum = 0;


    if (Buffer && Length)
    {
        /* Buffer and Length are valid */

        limit = (UINT8 *) Buffer + Length;

        for (rover = Buffer; rover < limit; rover++)
        {
            sum = (UINT8) (sum + *rover);
        }
    }
    return (sum);
}


/******************************************************************************
 *
 * FUNCTION:    AbDisplayHeader
 *
 * DESCRIPTION: Display an ACPI table header
 *
 ******************************************************************************/

void
AbDisplayHeader (
    char                    *File1Path)
{
    UINT32                  Actual1;


    File1 = fopen (File1Path, "rb");
    if (!File1)
    {
        printf ("Could not open file %s\n", File1Path);
        return;
    }

    Actual1 = fread (&Header1, 1, sizeof (ACPI_TABLE_HEADER), File1);
    if (Actual1 < sizeof (ACPI_TABLE_HEADER))
    {
        printf ("File %s does not contain an ACPI table header\n", File1Path);
        return;
    }

    if (!AbValidateHeader (&Header1))
    {
        return;
    }

    /* Display header information */

    printf ("Signature         : %8.4s\n",    Header1.Signature);
    printf ("Length            : %8.8X\n",    Header1.Length);
    printf ("Revision          : % 8.2X\n",   Header1.Revision);
    printf ("Checksum          : % 8.2X\n",   Header1.Checksum);
    printf ("OEM ID            : %8.6s\n",    Header1.OemId);
    printf ("OEM Table ID      : %8.8s\n",    Header1.OemTableId);
    printf ("OEM Revision      : %8.8X\n",    Header1.OemRevision);
    printf ("ASL Compiler ID   : %8.4s\n",    Header1.AslCompilerId);
    printf ("Compiler Revision : %8.8X\n",    Header1.AslCompilerRevision);
    printf ("\n");
}


/******************************************************************************
 *
 * FUNCTION:    AbComputeChecksum
 *
 * DESCRIPTION: Compute proper checksum for an ACPI table
 *
 ******************************************************************************/

void
AbComputeChecksum (
    char                    *File1Path)
{
    UINT32                  Actual1;
    ACPI_TABLE_HEADER       *Table;
    UINT8                   Checksum;


    File1 = fopen (File1Path, "rb");
    if (!File1)
    {
        printf ("Could not open file %s\n", File1Path);
        return;
    }

    Actual1 = fread (&Header1, 1, sizeof (ACPI_TABLE_HEADER), File1);
    if (Actual1 < sizeof (ACPI_TABLE_HEADER))
    {
        printf ("File %s does not contain an ACPI table header\n", File1Path);
        return;
    }

    if (!AbValidateHeader (&Header1))
    {
        return;
    }

    if (!Gbl_TerseMode)
    {
        /* Display header information */

        printf ("Signature         : %8.4s\n",    Header1.Signature);
        printf ("Length            : %8.8X\n",    Header1.Length);
        printf ("Revision          : % 8.2X\n",   Header1.Revision);
        printf ("Checksum          : % 8.2X\n",   Header1.Checksum);
        printf ("OEM ID            : %8.6s\n",    Header1.OemId);
        printf ("OEM Table ID      : %8.8s\n",    Header1.OemTableId);
        printf ("OEM Revision      : %8.8X\n",    Header1.OemRevision);
        printf ("ASL Compiler ID   : %8.4s\n",    Header1.AslCompilerId);
        printf ("Compiler Revision : %8.8X\n",    Header1.AslCompilerRevision);
        printf ("\n");
    }

    Table = AcpiOsAllocate (Header1.Length);
    if (!Table)
    {
        printf ("could not allocate\n");
        return;
    }

    fseek (File1, 0, SEEK_SET);
    Actual1 = fread (Table, 1, Header1.Length, File1);
    if (Actual1 < Header1.Length)
    {
        printf ("could not read table\n");
        return;
    }

    Table->Checksum = 0;

    Checksum = (UINT8) (0 - AcpiTbChecksum (Table, Table->Length));
    printf ("Computed checksum: 0x%X\n\n", Checksum);

    if (Header1.Checksum == Checksum)
    {
        printf ("Checksum ok in AML file, not updating\n");
        return;
    }

    fclose (File1);
    File1 = fopen (File1Path, "r+b");
    if (!File1)
    {
        printf ("Could not open file %s for writing\n", File1Path);
        return;
    }

    Header1.Checksum = Checksum;

    Actual1 = fwrite (&Header1, 1, sizeof (ACPI_TABLE_HEADER), File1);
    if (Actual1 < sizeof (ACPI_TABLE_HEADER))
    {
        printf ("Could not write updated table header\n");
        return;
    }

    printf ("Wrote new checksum\n");
    return;
}


/******************************************************************************
 *
 * FUNCTION:    AbCompareAmlFiles
 *
 * DESCRIPTION: Compare two AML files
 *
 ******************************************************************************/

int
AbCompareAmlFiles (
    char                    *File1Path,
    char                    *File2Path)
{
    UINT32                  Actual1;
    UINT32                  Actual2;
    UINT32                  Offset;
    UINT8                   Char1;
    UINT8                   Char2;
    UINT8                   Mismatches = 0;
    BOOLEAN                 HeaderMismatch = FALSE;


    File1 = fopen (File1Path, "rb");
    if (!File1)
    {
        printf ("Could not open file %s\n", File1Path);
        return -1;
    }

    File2 = fopen (File2Path, "rb");
    if (!File2)
    {
        printf ("Could not open file %s\n", File2Path);
        return -1;
    }

    /* Read the ACPI header from each file */

    Actual1 = fread (&Header1, 1, sizeof (ACPI_TABLE_HEADER), File1);
    if (Actual1 < sizeof (ACPI_TABLE_HEADER))
    {
        printf ("File %s does not contain an ACPI table header\n", File1Path);
        return -1;
    }

    Actual2 = fread (&Header2, 1, sizeof (ACPI_TABLE_HEADER), File2);
    if (Actual2 < sizeof (ACPI_TABLE_HEADER))
    {
        printf ("File %s does not contain an ACPI table header\n", File2Path);
        return -1;
    }

    if ((!AbValidateHeader (&Header1)) ||
        (!AbValidateHeader (&Header2)))
    {
        return -1;
    }

    /* Table signatures must match */

    if ((UINT32) Header1.Signature != (UINT32) Header1.Signature)
    {
        printf ("Table signatures do not match\n");
        return -1;
    }

    if (!Gbl_TerseMode)
    {
        /* Display header information */

        printf ("Signature         : %8.4s  %8.4s\n",    Header1.Signature, Header2.Signature);
        printf ("Length            : %8.8X  %8.8X\n",    Header1.Length, Header2.Length);
        printf ("Revision          : % 8.2X  % 8.2X\n",  Header1.Revision, Header2.Revision);
        printf ("Checksum          : % 8.2X  % 8.2X\n",  Header1.Checksum, Header2.Checksum);
        printf ("OEM ID            : %8.6s  %8.6s\n",    Header1.OemId, Header2.OemId);
        printf ("OEM Table ID      : %8.8s  %8.8s\n",    Header1.OemTableId, Header2.OemTableId);
        printf ("OEM Revision      : %8.8X  %8.8X\n",    Header1.OemRevision, Header2.OemRevision);
        printf ("ASL Compiler ID   : %8.4s  %8.4s\n",    Header1.AslCompilerId, Header2.AslCompilerId);
        printf ("Compiler Revision : %8.8X  %8.8X\n",    Header1.AslCompilerRevision, Header2.AslCompilerRevision);
        printf ("\n");
    }

    if (memcmp (Header1.Signature, Header2.Signature, sizeof (ACPI_TABLE_HEADER)))
    {
        printf ("Headers do not match exactly\n");
        HeaderMismatch = TRUE;
    }

    /* Do the byte-by-byte compare */

    Actual1 = fread (&Char1, 1, 1, File1);
    Actual2 = fread (&Char2, 1, 1, File2);
    Offset = sizeof (ACPI_TABLE_HEADER);

    while (Actual1 && Actual2)
    {
        if (Char1 != Char2)
        {
            printf ("Error - Byte mismatch at offset %8.8X: 0x%2.2X 0x%2.2X\n", 
                Offset, Char1, Char2);
            Mismatches++;
            if (Mismatches > 100)
            {
                printf ("100 Mismatches: Too many mismatches\n");
                return -1;
            }
        }

        Offset++;
        Actual1 = fread (&Char1, 1, 1, File1);
        Actual2 = fread (&Char2, 1, 1, File2);
    }

    if (Actual1)
    {
        printf ("Error - file %s is longer than file %s\n", File1Path, File2Path);
        Mismatches++;
    }
    else if (Actual2)
    {
        printf ("Error - file %s is shorter than file %s\n", File1Path, File2Path);
        Mismatches++;
    }
    else if (!Mismatches)
    {
        if (HeaderMismatch)
        {
            printf ("Files compare exactly after header\n");
        }
        else
        {
            printf ("Files compare exactly\n");
        }
    }

    printf ("%d Mismatches found\n", Mismatches);
    return 0;
}


/******************************************************************************
 *
 * FUNCTION:    AsGetFile
 *
 * DESCRIPTION: Open a file and read it entirely into a an allocated buffer
 *
 ******************************************************************************/

char *
AbGetFile (
    char                    *Filename,
    UINT32                  *FileSize)
{

    int                     FileHandle;
    UINT32                  Size;
    char                    *Buffer = NULL;


    /* Binary mode leaves CR/LF pairs */

    FileHandle = open (Filename, O_BINARY | O_RDONLY);
    if (!FileHandle)
    {
        printf ("Could not open %s\n", Filename);
        return NULL;
    }

    if (fstat (FileHandle, &Gbl_StatBuf))
    {
        printf ("Could not get file status for %s\n", Filename);
        goto ErrorExit;
    }

    /*
     * Create a buffer for the entire file
     */
    Size = Gbl_StatBuf.st_size;
    Buffer = calloc (Size, 1);
    if (!Buffer)
    {
        printf ("Could not allocate buffer of size %d\n", Size);
        goto ErrorExit;
    }

    /* Read the entire file */

    Size = read (FileHandle, Buffer, Size);
    if (Size == -1)
    {
        printf ("Could not read the input file %s\n", Filename);
        free (Buffer);
        Buffer = NULL;
        goto ErrorExit;
    }

    *FileSize = Size;

ErrorExit:
    close (FileHandle);

    return (Buffer);
}


/******************************************************************************
 *
 * FUNCTION:    AbDumpAmlFile
 *
 * DESCRIPTION: Dump a binary AML file to a text file
 *
 ******************************************************************************/

int
AbDumpAmlFile (
    char                    *File1Path,
    char                    *File2Path)
{
    char                    *FileBuffer;
    UINT32                  FileSize;
    FILE                    *FileOutHandle;


    FileBuffer = AbGetFile (File1Path, &FileSize);
    printf ("File %s contains 0x%X bytes\n\n", File1Path, FileSize);

    FileOutHandle = fopen (File2Path, "wb");
    if (!FileOutHandle)
    {
        printf ("Could not open %s\n", File2Path);
        return -1;
    }

    if (!AbValidateHeader ((ACPI_TABLE_HEADER *) FileBuffer))
    {
        return -1;
    }

    AcpiGbl_DebugFile = FileOutHandle;
    AcpiGbl_DbOutputFlags = ACPI_DB_REDIRECTABLE_OUTPUT;

    AcpiOsPrintf ("%4.4s\n", ((ACPI_TABLE_HEADER *) FileBuffer)->Signature);
    AcpiDbgLevel = ACPI_UINT32_MAX;
    AcpiUtDumpBuffer (FileBuffer, FileSize, DB_BYTE_DISPLAY, ACPI_UINT32_MAX);

    return 0;
}


/******************************************************************************
 *
 * FUNCTION:    AbExtractAmlFile
 *
 * DESCRIPTION: Extract a binary AML file from a text file (as produced by the
 *              DumpAmlFile procedure or the "acpidmp" table utility.
 *
 ******************************************************************************/

int
AbExtractAmlFile (
    char                    *TableSig,
    char                    *File1Path,
    char                    *File2Path)
{
    char                    *Table;
    long                    Value;
    UINT32                  i;
    char                    *End;
    FILE                    *FileHandle;
    FILE                    *FileOutHandle;


    /* Open in/out files. Binary mode leaves CR/LF pairs */

    FileHandle = fopen (File1Path, "rb");
    if (!FileHandle)
    {
        printf ("Could not open %s\n", File1Path);
        return -1;
    }

    FileOutHandle = fopen (File2Path, "wb");
    if (!FileOutHandle)
    {
        printf ("Could not open %s\n", File2Path);
        return -1;
    }

    /* Force input table sig to uppercase */

    strupr (TableSig);

    /* We have an ascii file, grab one line at a time */

    while (fgets (Buffer, BUFFER_SIZE, FileHandle))
    {
        /* 4-char signature appears at the beginning of a line */

        if (!strncmp (Buffer, TableSig, 4))
        {
            printf ("Found table [%4.4s]\n", TableSig);

            /* 
             * Eat all lines in the table, of the form:
             * 
             * offset: <16 bytes of data, separated by spaces>, extra junk, newline 
             */
            while (fgets (Buffer, BUFFER_SIZE, FileHandle))
            {
                /* Get past the offset: */

                Table = strchr (Buffer, ':');
                if (!Table)
                {
                    return 0;
                }

                /* Convert up to 16 bytes per input line */

                Table += 2;
                for (i = 0; i < 16; i++)
                {
                    Value = strtoul (Table, &End, 16);
                    if (End == Table)
                    {
                        /* No conversion, all done */

                        return 0;
                    }

                    /* Write the converted byte */

                    Table += 3;
                    fwrite (&(char *) Value, 1, 1, FileOutHandle);
                }
            }

            /* No more lines, all done */

            return (0);
        }
    }

    /* Searched entire file, no match to table signature */

    printf ("Could not match table signature\n");
    return -1;
}


/******************************************************************************
 *
 * FUNCTION:    Stubs
 *
 * DESCRIPTION: For linkage
 *
 ******************************************************************************/

ACPI_STATUS
AeLocalGetRootPointer (
    UINT32                  Flags,
    ACPI_PHYSICAL_ADDRESS   *RsdpPhysicalAddress)
{
    return AE_OK;
}

