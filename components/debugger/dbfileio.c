/******************************************************************************
 * 
 * Module Name: dbfileio - Debugger file I/O commands.  These can't usually
 *                  be used when running the debugger in Ring 0 (Kernel mode)
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


#include <acpi.h>
#include <acapi.h>
#include <debugger.h>
#include <namesp.h>
#include <parser.h>

#ifdef ACPI_DEBUG

#define _COMPONENT          DEBUGGER
        MODULE_NAME         ("dbfileio");

ACPI_GENERIC_OP         *root;

#ifdef ACPI_APPLICATION
#include <stdio.h>
FILE                    *DebugFile = NULL;
#endif


/******************************************************************************
 * 
 * FUNCTION:    DbCloseDebugFile
 *
 * PARAMETERS:  None
 *
 * RETURN:      Status
 *
 * DESCRIPTION: If open, close the current debug output file
 *
 *****************************************************************************/

void
DbCloseDebugFile (
    void)
{

#ifdef ACPI_APPLICATION

    if (DebugFile)
    {
       fclose (DebugFile);
       DebugFile = NULL;
       OutputToFile = FALSE;
       OsdPrintf ("Debug output file %s closed\n", DebugFilename);
    }
#endif

}


/******************************************************************************
 * 
 * FUNCTION:    DbOpenDebugFile
 *
 * PARAMETERS:  Name                - Filename  
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Open a file where debug output will be directed.
 *
 *****************************************************************************/

void
DbOpenDebugFile (
    char                    *Name)
{

#ifdef ACPI_APPLICATION

    if (DebugLevel == DEBUG_DEFAULT)
    {
        DebugLevel = 0x0FFFFEFF;
    }
            
    DbCloseDebugFile ();
    DebugFile = fopen (Name, "w+");
    if (DebugFile)
    {
        OsdPrintf ("Debug output file %s opened\n", Name);
        STRCPY (DebugFilename, Name);
        OutputToFile = TRUE;
    }

#endif
}


#ifdef ACPI_APPLICATION
/******************************************************************************
 * 
 * FUNCTION:    DbLoadDsdt
 *
 * PARAMETERS:  
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Load the DSDT from the file pointer
 *
 *****************************************************************************/

ACPI_STATUS
DbLoadDsdt(
    FILE                    *fp, 
    UINT8                   **DsdtPtr, 
    UINT32                  *DsdtLength)
{
    ACPI_TABLE_HEADER       dsdt_hdr;
    UINT8                   *AmlPtr;
    UINT32                   AmlLength;


    if (fread (&dsdt_hdr, 1, sizeof (dsdt_hdr), fp) == sizeof (dsdt_hdr))
    {
        *DsdtLength = dsdt_hdr.Length;

        if (*DsdtLength)
        {
            *DsdtPtr = (UINT8*) malloc ((size_t) *DsdtLength);

            if (*DsdtPtr)
            {
                AmlPtr = *DsdtPtr + sizeof (dsdt_hdr);
                AmlLength = *DsdtLength - sizeof (dsdt_hdr);

                memcpy (*DsdtPtr, &dsdt_hdr, sizeof (dsdt_hdr));
                if ((UINT32) fread (AmlPtr, 1, (size_t) AmlLength, fp) == AmlLength)
                {
                    return AE_OK;
                }

                free(*DsdtPtr);
            }
        }
    }

    *DsdtPtr = NULL;
    *DsdtLength = 0;

    return AE_AML_ERROR;
}
#endif


/******************************************************************************
 * 
 * FUNCTION:    DbLoadAcpiTable
 *
 * PARAMETERS:  
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Load an ACPI table from a file
 *
 *****************************************************************************/

ACPI_STATUS
DbLoadAcpiTable (
    char                    *Filename)
{
#ifdef ACPI_APPLICATION
    FILE                    *fp;
    ACPI_STATUS             Status;


    AcpiInitialize (NULL);


    fp = fopen (Filename, "rb");
    if (!fp)
    {
        OsdPrintf ("Couldn't open file %s\n", Filename);
        return AE_ERROR;
    }

    OsdPrintf ("Loading DSDT from file %s\n", Filename);
    Status = DbLoadDsdt (fp, &DsdtPtr, &DsdtLength);
    fclose(fp);

    /* TBD: set real Dsdt ptr here.  BUT FIX this */

    Gbl_DSDT = (ACPI_TABLE_HEADER *) DsdtPtr;



    AmlPtr = DsdtPtr + sizeof (ACPI_TABLE_HEADER);
    AmlLength = DsdtLength - sizeof (ACPI_TABLE_HEADER);

    Status = NsSetup ();
    if (ACPI_FAILURE (Status))
    {
        return (Status);
    }


    Status = PsxLoadTable (AmlPtr, AmlLength);
    if (ACPI_SUCCESS (Status))
    {
        Status = DbSecondPassParse (root);
    }

#endif  /* ACPI_APPLICATION */  
    return AE_OK;
}


#endif  /* ACPI_DEBUG */

