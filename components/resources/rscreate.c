/******************************************************************************
 *
 * Module Name: rscreate - AcpiRsCreateResourceList
 *                         AcpiRsCreatePciRoutingTable
 *                         AcpiRsCreateByteStream
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


#define __RSCREATE_C__

#include "acpi.h"
#include "resource.h"

#define _COMPONENT          RESOURCE_MANAGER
        MODULE_NAME         ("rscreate");


/***************************************************************************
 * FUNCTION:    AcpiRsCreateResourceList
 *
 * PARAMETERS:
 *              ByteStreamBuffer        - Pointer to the resource byte stream
 *              OutputBuffer            - Pointer to the user's buffer
 *              OutputBufferLength      - Pointer to the size of OutputBuffer
 *
 * RETURN:      Status  AE_OK if okay, else a valid ACPI_STATUS code
 *                      If OutputBuffer is not large enough, OutputBufferLength
 *                        indicates how large OutputBuffer should be, else it
 *                        indicates how may UINT8 elements of OutputBuffer are
 *                        valid.
 *
 * DESCRIPTION: Takes the byte stream returned from a _CRS, _PRS control method
 *                  execution and parses the stream to create a linked list
 *                  of device resources.
 *
 ***************************************************************************/

ACPI_STATUS
AcpiRsCreateResourceList (
    ACPI_OBJECT_INTERNAL    *ByteStreamBuffer,
    UINT8                   *OutputBuffer,
    UINT32                  *OutputBufferLength)
{

    ACPI_STATUS             Status = AE_UNKNOWN_STATUS;
    UINT8                   *ByteStreamStart = NULL;
    UINT32                  ListSizeNeeded = 0;
    UINT32                  ByteStreamBufferLength = 0;


    FUNCTION_TRACE ("RsCreateResourceList");

    DEBUG_PRINT (VERBOSE_INFO, ("RsCreateResourceList: ByteStreamBuffer = %p\n",
                 ByteStreamBuffer));

    /*
     * Validate parameters:
     *
     *  1. If ByteStreamBuffer is NULL after we know that
     *      ByteSteamLength is not zero, or
     *  2. If OutputBuffer is NULL and OutputBufferLength
     *      is not zero
     *
     *  Return an error
     */
    if (!ByteStreamBuffer ||
       (!OutputBuffer && 0 != *OutputBufferLength))
    {
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    ByteStreamBufferLength = ByteStreamBuffer->Buffer.Length;
    ByteStreamStart = ByteStreamBuffer->Buffer.Pointer;

    /*
     * Pass the ByteStreamBuffer into a module that can calculate
     *  the buffer size needed for the linked list
     */
    Status = AcpiRsCalculateListLength (ByteStreamStart,
                                        ByteStreamBufferLength,
                                        &ListSizeNeeded);

    DEBUG_PRINT (VERBOSE_INFO, 
        ("RsCreateResourceList: Status=%d ListSizeNeeded=%d\n", 
        Status, ListSizeNeeded));

    /*
     * Exit with the error passed back
     */
    if (AE_OK != Status)
    {
        return_ACPI_STATUS (Status);
    }

    /*
     * If the linked list will fit into the available buffer
     *  call to fill in the list
     */

    if (ListSizeNeeded <= *OutputBufferLength)
    {
        /*
         * Zero out the return buffer before proceeding
         */
        MEMSET (OutputBuffer, 0x00, *OutputBufferLength);

        Status = AcpiRsByteStreamToList (ByteStreamStart,
                                         ByteStreamBufferLength,
                                         &OutputBuffer);

        /*
         * Exit with the error passed back
         */
        if (AE_OK != Status)
        {
            return_ACPI_STATUS (Status);
        }

        DEBUG_PRINT (VERBOSE_INFO, ("RsByteStreamToList: OutputBuffer = %p\n",
                                     OutputBuffer));
    }

    else
    {
        *OutputBufferLength = ListSizeNeeded;
        return_ACPI_STATUS (AE_BUFFER_OVERFLOW);
    }

    *OutputBufferLength = ListSizeNeeded;
    return_ACPI_STATUS (AE_OK);

}


/***************************************************************************
 * FUNCTION:    AcpiRsCreatePciRoutingTable
 *
 * PARAMETERS:
 *              PackageObject           - Pointer to an ACPI_OBJECT_INTERNAL
 *                                          package
 *              OutputBuffer            - Pointer to the user's buffer
 *              OutputBufferLength      - Size of OutputBuffer
 *
 * RETURN:      Status  AE_OK if okay, else a valid ACPI_STATUS code.
 *              If the OutputBuffer is too small, the error will be
 *                AE_BUFFER_OVERFLOW and OutputBufferLength will point
 *                to the size buffer needed.
 *
 * DESCRIPTION: Takes the ACPI_OBJECT_INTERNAL package and creates a
 *                  linked list of PCI interrupt descriptions
 *
 ***************************************************************************/

ACPI_STATUS
AcpiRsCreatePciRoutingTable (
    ACPI_OBJECT_INTERNAL    *PackageObject,
    UINT8                   *OutputBuffer,
    UINT32                  *OutputBufferLength)
{
    UINT8                   *Buffer = OutputBuffer;
    ACPI_OBJECT_INTERNAL    **TopObjectList = NULL;
    ACPI_OBJECT_INTERNAL    **SubObjectList = NULL;
    ACPI_OBJECT_INTERNAL    *PackageElement = NULL;
    UINT32                  BufferSizeNeeded = 0;
    UINT32                  NumberOfElements = 0;
    UINT32                  Index = 0;
    UINT8                   TableIndex = 0;
    BOOLEAN                 NameFound = FALSE;
    PCI_ROUTING_TABLE       *UserPrt = NULL;


    FUNCTION_TRACE ("RsCreatePciRoutingTable");

    /*
     * Validate parameters:
     *
     *  1. If MethodReturnObject is NULL, or
     *  2. If OutputBuffer is NULL and OutputBufferLength is not zero
     *
     *  Return an error
     */
    if (!PackageObject ||
       (!OutputBuffer && 0 != *OutputBufferLength))
    {
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    /*
     * Calculate the buffer size needed for the routing table.
     */
    NumberOfElements = PackageObject->Package.Count;

    /*
     * Properly calculate the size of the return buffer.
     *  The base size is the number of elements * the sizes of the
     *  structures.  Additional space for the strings is added below.
     *  The minus one is to subtract the size of the UINT8 Source[1]
     *  member because it is added below.
     * NOTE: The NumberOfElements is incremented by one to add an end
     *  table structure that is essentially a structure of zeros.
     */
    BufferSizeNeeded = (NumberOfElements + 1) *
                       (sizeof (PCI_ROUTING_TABLE) - 1);

    /*
     * But each PRT_ENTRY structure has a pointer to a string and
     *  the size of that string must be found.
     */
    TopObjectList = PackageObject->Package.Elements;

    for (Index = 0; Index < NumberOfElements; Index++)
    {
        /*
         * Dereference the sub-package
         */
        PackageElement = *TopObjectList;

        /*
         * The SubObjectList will now point to an array of the
         *  four IRQ elements: Address, Pin, Source and SourceIndex
         */
        SubObjectList = PackageElement->Package.Elements;

        /*
         * Scan the IrqTableElements for the Source Name String
         */
        NameFound = FALSE;

        for (TableIndex = 0; TableIndex < 4 && !NameFound; TableIndex++)
        {
            if (ACPI_TYPE_STRING == (*SubObjectList)->Common.Type)
            {
                NameFound = TRUE;
            }

            else
            {
                /*
                 * Look at the next element
                 */
                SubObjectList++;
            }
        }

        /*
         * Was a String type found?
         */
        if (TRUE == NameFound)
        {
            /*
             * The length String.Length field includes the
             *  terminating NULL
             */
            BufferSizeNeeded += ((*SubObjectList)->String.Length);
        }

        else
        {
            /*
             * If no name was found, then this is a NULL, which is
             *  translated as a UINT32 zero.
             */
            BufferSizeNeeded += sizeof(UINT32);
        }

        /*
         * Point to the next ACPI_OBJECT_INTERNAL
         */
        TopObjectList++;
    }

    DEBUG_PRINT (VERBOSE_INFO, 
        ("RsCreatePciRoutingTable: BufferSizeNeeded = %d\n",
        BufferSizeNeeded));

    /*
     * If the data will fit into the available buffer
     *  call to fill in the list
     */
    if (BufferSizeNeeded <= *OutputBufferLength)
    {
        /*
         * Zero out the return buffer before proceeding
         */
        MEMSET (OutputBuffer, 0x00, *OutputBufferLength);

        /*
         * Loop through the ACPI_INTERNAL_OBJECTS - Each object should
         *  contain a UINT32 Address, a UINT8 Pin, a Name and a UINT8
         *  SourceIndex.
         */
        TopObjectList = PackageObject->Package.Elements;

        NumberOfElements = PackageObject->Package.Count;

        UserPrt = (PCI_ROUTING_TABLE *)Buffer;

        for (Index = 0; Index < NumberOfElements; Index++)
        {
            /*
             * Point UserPrt past this current structure
             *
             * NOTE: On the first iteration, UserPrt->Length will
             *  be zero because we zero'ed out the return buffer
             *  earlier
             */
            Buffer += UserPrt->Length;

            UserPrt = (PCI_ROUTING_TABLE *)Buffer;

            /*
             * Fill in the Length field with the information we
             *  have at this point.
             *  The minus one is to subtract the size of the
             *  UINT8 Source[1] member because it is added below.
             */
            UserPrt->Length = (sizeof(PCI_ROUTING_TABLE) - 1);

            /*
             * Dereference the sub-package
             */
            PackageElement = *TopObjectList;

            /*
             * The SubObjectList will now point to an array of
             *  the four IRQ elements: Address, Pin, Source and
             *  SourceIndex
             */
            SubObjectList = PackageElement->Package.Elements;

            /*
             * Dereference the Address
             */
            if (ACPI_TYPE_NUMBER == (*SubObjectList)->Common.Type)
            {
                UserPrt->Data.Address =
                        (*SubObjectList)->Number.Value;
            }

            else
            {
                return_ACPI_STATUS (AE_BAD_DATA);
            }

            /*
             * Dereference the Pin
             */
            SubObjectList++;

            if (ACPI_TYPE_NUMBER == (*SubObjectList)->Common.Type)
            {
                UserPrt->Data.Pin =
                        (*SubObjectList)->Number.Value;
            }

            else
            {
                return_ACPI_STATUS (AE_BAD_DATA);
            }

            /*
             * Dereference the Source Name
             */
            SubObjectList++;

            if (ACPI_TYPE_STRING == (*SubObjectList)->Common.Type)
            {
                STRCPY(UserPrt->Data.Source,
                      (*SubObjectList)->String.Pointer);

                /*
                 * Add to the Length field the length of the string
                 */
                UserPrt->Length += (*SubObjectList)->String.Length;
            }

            else
            {
                /*
                 * If this is a number, then the Source Name
                 *  is NULL, since the entire buffer was zeroed
                 *  out, we can leave this alone.
                 */
                if (ACPI_TYPE_NUMBER == (*SubObjectList)->Common.Type)
                {
                    /*
                     * Add to the Length field the length of
                     *  the UINT32 NULL
                     */
                    UserPrt->Length += sizeof(UINT32);
                }

                else
                {
                    return_ACPI_STATUS (AE_BAD_DATA);
                }
            }

            /*
             * Dereference the Source Index
             */
            SubObjectList++;

            if (ACPI_TYPE_NUMBER == (*SubObjectList)->Common.Type)
            {
                UserPrt->Data.SourceIndex =
                        (*SubObjectList)->Number.Value;
            }

            else
            {
                return_ACPI_STATUS (AE_BAD_DATA);
            }

            /*
             * Point to the next ACPI_OBJECT_INTERNAL
             */
            TopObjectList++;
        }

        DEBUG_PRINT (VERBOSE_INFO, 
            ("RsCreatePciRoutingTable: OutputBuffer = %p\n", 
            OutputBuffer));
    }

    else
    {
        *OutputBufferLength = BufferSizeNeeded;

        return_ACPI_STATUS (AE_BUFFER_OVERFLOW);
    }

    /*
     * Report the amount of buffer used
     */
    *OutputBufferLength = BufferSizeNeeded;

    return_ACPI_STATUS (AE_OK);

}


/***************************************************************************
 * FUNCTION:    AcpiRsCreateByteStream
 *
 * PARAMETERS:
 *              LinkedListBuffer        - Pointer to the resource linked list
 *              OutputBuffer            - Pointer to the user's buffer
 *              OutputBufferLength      - Size of OutputBuffer
 *
 * RETURN:      Status  AE_OK if okay, else a valid ACPI_STATUS code.
 *              If the OutputBuffer is too small, the error will be
 *              AE_BUFFER_OVERFLOW and OutputBufferLength will point
 *              to the size buffer needed.
 *
 * DESCRIPTION: Takes the linked list of device resources and
 *              creates a bytestream to be used as input for the
 *              _SRS control method.
 *
 ***************************************************************************/

ACPI_STATUS
AcpiRsCreateByteStream (
    RESOURCE                *LinkedListBuffer,
    UINT8                   *OutputBuffer,
    UINT32                  *OutputBufferLength)
{
    ACPI_STATUS             Status = AE_UNKNOWN_STATUS;
    UINT32                  ByteStreamSizeNeeded = 0;


    FUNCTION_TRACE ("RsCreateByteStream");

    DEBUG_PRINT (VERBOSE_INFO, 
        ("RsCreateByteStream: LinkedListBuffer = %p\n",
        LinkedListBuffer));

    /*
     * Validate parameters:
     *
     *  1. If LinkedListBuffer is NULL, or
     *  2. If OutputBuffer is NULL and OutputBufferLength is not zero
     *
     *  Return an error
     */
    if (!LinkedListBuffer ||
       (!OutputBuffer && 0 != *OutputBufferLength))
    {
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    /*
     * Pass the LinkedListBuffer into a module that can calculate
     *  the buffer size needed for the byte stream.
     */
    Status = AcpiRsCalculateByteStreamLength (LinkedListBuffer,
                                              &ByteStreamSizeNeeded);

    DEBUG_PRINT (VERBOSE_INFO, 
        ("RsCreateByteStream: ByteStreamSizeNeeded=%d, %s\n",
        ByteStreamSizeNeeded,
        AcpiCmFormatException (Status)));

    /*
     * Exit with the error passed back
     */
    if (AE_OK != Status)
    {
        return_ACPI_STATUS (Status);
    }

    /*
     * If the linked list will fit into the available buffer
     *  call to fill in the list
     */

    if (ByteStreamSizeNeeded <= *OutputBufferLength)
    {
        /*
         * Zero out the return buffer before proceeding
         */
        MEMSET (OutputBuffer, 0x00, *OutputBufferLength);

        Status = AcpiRsListToByteStream (LinkedListBuffer,
                                         ByteStreamSizeNeeded,
                                         &OutputBuffer);

        /*
         * Exit with the error passed back
         */
        if (AE_OK != Status)
        {
            return_ACPI_STATUS (Status);
        }

        DEBUG_PRINT (VERBOSE_INFO, 
            ("RsListToByteStream: OutputBuffer = %p\n",
            OutputBuffer));
    }
    else
    {
        *OutputBufferLength = ByteStreamSizeNeeded;
        return_ACPI_STATUS (AE_BUFFER_OVERFLOW);
    }

    return_ACPI_STATUS (AE_OK);

}

