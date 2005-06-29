/******************************************************************************
 *
 * Module Name: dsfield - Dispatcher field routines
 *              $Revision: 1.51 $
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999, 2000, 2001, Intel Corp.
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

#define __DSFIELD_C__

#include "acpi.h"
#include "amlcode.h"
#include "acdispat.h"
#include "acinterp.h"
#include "acnamesp.h"
#include "acparser.h"


#define _COMPONENT          ACPI_DISPATCHER
        MODULE_NAME         ("dsfield")


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsCreateBufferField
 *
 * PARAMETERS:  Opcode              - The opcode to be executed
 *              Operands            - List of operands for the opcode
 *              WalkState           - Current state
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Execute the CreateField operators:
 *              CreateBitFieldOp,
 *              CreateByteFieldOp,
 *              CreateWordFieldOp,
 *              CreateDWordFieldOp,
 *              CreateQWordFieldOp,
 *              CreateFieldOp       (all of which define fields in buffers)
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsCreateBufferField (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_WALK_STATE         *WalkState)
{
    ACPI_PARSE_OBJECT       *Arg;
    ACPI_NAMESPACE_NODE     *Node;
    ACPI_STATUS             Status;
    ACPI_OPERAND_OBJECT     *ObjDesc;
    UINT32                  Flags;


    FUNCTION_TRACE ("DsCreateBufferField");


    /* Get the NameString argument */

    if (Op->Opcode == AML_CREATE_FIELD_OP)
    {
        Arg = AcpiPsGetArg (Op, 3);
    }
    else
    {
        /* Create Bit/Byte/Word/Dword field */

        Arg = AcpiPsGetArg (Op, 2);
    }

    if (!Arg)
    {
        return_ACPI_STATUS (AE_AML_NO_OPERAND);
    }

    /*
     * During the load phase, we want to enter the name of the field into
     * the namespace.  During the execute phase (when we evaluate the size
     * operand), we want to lookup the name
     */
    if (WalkState->ParseFlags & ACPI_PARSE_EXECUTE)
    {
        Flags = NS_NO_UPSEARCH | NS_DONT_OPEN_SCOPE;
    }
    else
    {
        Flags = NS_NO_UPSEARCH | NS_DONT_OPEN_SCOPE | NS_ERROR_IF_FOUND;
    }

    /*
     * Enter the NameString into the namespace
     */
    Status = AcpiNsLookup (WalkState->ScopeInfo, Arg->Value.String,
                            INTERNAL_TYPE_DEF_ANY, IMODE_LOAD_PASS1,
                            Flags,
                            WalkState, &(Node));
    if (ACPI_FAILURE (Status))
    {
        return_ACPI_STATUS (Status);
    }

    /* We could put the returned object (Node) on the object stack for later, but
     * for now, we will put it in the "op" object that the parser uses, so we
     * can get it again at the end of this scope
     */
    Op->Node = Node;

    /*
     * If there is no object attached to the node, this node was just created and
     * we need to create the field object.  Otherwise, this was a lookup of an
     * existing node and we don't want to create the field object again.
     */
    if (Node->Object)
    {
        /* No longer need the Extra field */

        AcpiUtRemoveReference ((Node->Object)->BufferField.Extra);
        (Node->Object)->BufferField.Extra = NULL;
        return_ACPI_STATUS (AE_OK);
    }

    /*
     * The Field definition is not fully parsed at this time.
     * (We must save the address of the AML for the buffer and index operands)
     */

    /* Create the buffer field object */

    ObjDesc = AcpiUtCreateInternalObject (ACPI_TYPE_BUFFER_FIELD);
    if (!ObjDesc)
    {
        Status = AE_NO_MEMORY;
        goto Cleanup;
    }

    /*
     * Allocate a method object for this field unit
     */
    ObjDesc->BufferField.Extra = AcpiUtCreateInternalObject (
                                    INTERNAL_TYPE_EXTRA);
    if (!ObjDesc->BufferField.Extra)
    {
        Status = AE_NO_MEMORY;
        goto Cleanup;
    }

    /*
     * Remember location in AML stream of the field unit
     * opcode and operands -- since the buffer and index
     * operands must be evaluated.
     */
    ObjDesc->BufferField.Extra->Extra.AmlStart  = ((ACPI_PARSE2_OBJECT *) Op)->Data;
    ObjDesc->BufferField.Extra->Extra.AmlLength = ((ACPI_PARSE2_OBJECT *) Op)->Length;
    ObjDesc->BufferField.Node = Node;

    /* Attach constructed field descriptor to parent node */

    Status = AcpiNsAttachObject (Node, ObjDesc, ACPI_TYPE_BUFFER_FIELD);


Cleanup:

    /* Remove local reference to the object */

    AcpiUtRemoveReference (ObjDesc);
    return_ACPI_STATUS (Status);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsGetFieldNames
 *
 * PARAMETERS:  Info            - CreateField info structure
 *  `           WalkState       - Current method state
 *              Arg             - First parser arg for the field name list
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Process all named fields in a field declaration.  Names are
 *              entered into the namespace.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsGetFieldNames (
    ACPI_CREATE_FIELD_INFO  *Info,
    ACPI_WALK_STATE         *WalkState,
    ACPI_PARSE_OBJECT       *Arg)
{
    ACPI_STATUS             Status;


    FUNCTION_TRACE_U32 ("DsGetFieldNames", Info);


    /* First field starts at bit zero */

    Info->FieldBitPosition = 0;

    /* Process all elements in the field list (of parse nodes) */

    while (Arg)
    {
        /*
         * Three types of field elements are handled:
         * 1) Offset - specifies a bit offset
         * 2) AccessAs - changes the access mode
         * 3) Name - Enters a new named field into the namespace
         */
        switch (Arg->Opcode)
        {
        case AML_INT_RESERVEDFIELD_OP:

            Info->FieldBitPosition += Arg->Value.Size;
            break;


        case AML_INT_ACCESSFIELD_OP:

            /*
             * Get a new AccessType and AccessAttribute for all
             * entries (until end or another AccessAs keyword)
             */
            Info->FieldFlags  = (UINT8) ((Info->FieldFlags & FIELD_ACCESS_TYPE_MASK) ||
                                        ((UINT8) (Arg->Value.Integer >> 8)));
            break;


        case AML_INT_NAMEDFIELD_OP:

            /* Lookup the name */

            Status = AcpiNsLookup (WalkState->ScopeInfo,
                            (NATIVE_CHAR *) &((ACPI_PARSE2_OBJECT *)Arg)->Name,
                            Info->FieldType, IMODE_EXECUTE,
                            NS_DONT_OPEN_SCOPE,
                            NULL, &Info->FieldNode);
            if (ACPI_FAILURE (Status))
            {
                if (Status != AE_ALREADY_EXISTS)
                {
                    return_ACPI_STATUS (Status);
                }

                REPORT_ERROR (("Field name [%4.4s] already exists in current scope\n",
                                &((ACPI_PARSE2_OBJECT *)Arg)->Name));
            }
            else
            {
                Arg->Node = Info->FieldNode;
                Info->FieldBitLength = Arg->Value.Size;

                /* Create and initialize an object for the new Field Node */

                Status = AcpiExPrepFieldValue (Info);
                if (ACPI_FAILURE (Status))
                {
                    return_ACPI_STATUS (Status);
                }
            }

            /* Keep track of bit position for the next field */

            Info->FieldBitPosition += Info->FieldBitLength;
            break;


        default:

            ACPI_DEBUG_PRINT ((ACPI_DB_ERROR, "Invalid opcode in field list: %X\n",
                Arg->Opcode));
            return_ACPI_STATUS (AE_AML_ERROR);
            break;
        }

        Arg = Arg->Next;
    }

    return_ACPI_STATUS (AE_OK);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsCreateField
 *
 * PARAMETERS:  Op              - Op containing the Field definition and args
 *              RegionNode      - Object for the containing Operation Region
 *  `           WalkState       - Current method state
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Create a new field in the specified operation region
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsCreateField (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_NAMESPACE_NODE     *RegionNode,
    ACPI_WALK_STATE         *WalkState)
{
    ACPI_STATUS             Status = AE_AML_ERROR;
    ACPI_PARSE_OBJECT       *Arg;
    ACPI_CREATE_FIELD_INFO  Info;


    FUNCTION_TRACE_PTR ("DsCreateField", Op);


    /* First arg is the name of the parent OpRegion (must already exist) */

    Arg = Op->Value.Arg;
    if (!RegionNode)
    {
        Status = AcpiNsLookup (WalkState->ScopeInfo, Arg->Value.Name,
                        ACPI_TYPE_REGION, IMODE_EXECUTE,
                        NS_SEARCH_PARENT, WalkState, &RegionNode);
        if (ACPI_FAILURE (Status))
        {
            return_ACPI_STATUS (Status);
        }
    }

    /* Second arg is the field flags */

    Arg = Arg->Next;
    Info.FieldFlags = Arg->Value.Integer8;

    /* Each remaining arg is a Named Field */

    Info.FieldType = INTERNAL_TYPE_REGION_FIELD;
    Info.RegionNode = RegionNode;

    Status = AcpiDsGetFieldNames (&Info, WalkState, Arg->Next);

    return_ACPI_STATUS (Status);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsInitFieldObjects
 *
 * PARAMETERS:  Op              - Op containing the Field definition and args
 *  `           WalkState       - Current method state
 *
 * RETURN:      Status
 *
 * DESCRIPTION: For each "Field Unit" name in the argument list that is
 *              part of the field declaration, enter the name into the
 *              namespace.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsInitFieldObjects (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_WALK_STATE         *WalkState)
{
    ACPI_STATUS             Status = AE_AML_ERROR;
    ACPI_PARSE_OBJECT       *Arg = NULL;
    ACPI_NAMESPACE_NODE     *Node;
    UINT8                   Type = 0;


    FUNCTION_TRACE_PTR ("DsInitFieldObjects", Op);


    switch (WalkState->Opcode)
    {
    case AML_FIELD_OP:
        Arg = AcpiPsGetArg (Op, 2);
        Type = INTERNAL_TYPE_REGION_FIELD;
        break;

    case AML_BANK_FIELD_OP:
        Arg = AcpiPsGetArg (Op, 4);
        Type = INTERNAL_TYPE_BANK_FIELD;
        break;

    case AML_INDEX_FIELD_OP:
        Arg = AcpiPsGetArg (Op, 3);
        Type = INTERNAL_TYPE_INDEX_FIELD;
        break;
    }

    /*
     * Walk the list of entries in the FieldList
     */
    while (Arg)
    {
        /* Ignore OFFSET and ACCESSAS terms here */

        if (Arg->Opcode == AML_INT_NAMEDFIELD_OP)
        {
            Status = AcpiNsLookup (WalkState->ScopeInfo,
                            (NATIVE_CHAR *) &((ACPI_PARSE2_OBJECT *)Arg)->Name,
                            Type, IMODE_LOAD_PASS1,
                            NS_NO_UPSEARCH | NS_DONT_OPEN_SCOPE | NS_ERROR_IF_FOUND,
                            NULL, &Node);
            if (ACPI_FAILURE (Status))
            {
                if (Status != AE_ALREADY_EXISTS)
                {
                    return_ACPI_STATUS (Status);
                }

                REPORT_ERROR (("Field name [%4.4s] already exists in current scope\n",
                                &((ACPI_PARSE2_OBJECT *)Arg)->Name));
            }

            Arg->Node = Node;
        }

        /* Move on to next field in the list */

        Arg = Arg->Next;
    }

    return_ACPI_STATUS (Status);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsCreateBankField
 *
 * PARAMETERS:  Op              - Op containing the Field definition and args
 *              RegionNode      - Object for the containing Operation Region
 *  `           WalkState       - Current method state
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Create a new bank field in the specified operation region
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsCreateBankField (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_NAMESPACE_NODE     *RegionNode,
    ACPI_WALK_STATE         *WalkState)
{
    ACPI_STATUS             Status = AE_AML_ERROR;
    ACPI_PARSE_OBJECT       *Arg;
    ACPI_CREATE_FIELD_INFO  Info;


    FUNCTION_TRACE_PTR ("DsCreateBankField", Op);


    /* First arg is the name of the parent OpRegion (must already exist) */

    Arg = Op->Value.Arg;
    if (!RegionNode)
    {
        Status = AcpiNsLookup (WalkState->ScopeInfo, Arg->Value.Name,
                        ACPI_TYPE_REGION, IMODE_EXECUTE,
                        NS_SEARCH_PARENT, WalkState, &RegionNode);
        if (ACPI_FAILURE (Status))
        {
            return_ACPI_STATUS (Status);
        }
    }

    /* Second arg is the Bank Register (must already exist) */

    Arg = Arg->Next;
    Status = AcpiNsLookup (WalkState->ScopeInfo, Arg->Value.String,
                    INTERNAL_TYPE_BANK_FIELD_DEFN, IMODE_EXECUTE,
                    NS_SEARCH_PARENT, WalkState, &Info.RegisterNode);
    if (ACPI_FAILURE (Status))
    {
        return_ACPI_STATUS (Status);
    }

    /* Third arg is the BankValue */

    Arg = Arg->Next;
    Info.BankValue = Arg->Value.Integer32;

    /* Fourth arg is the field flags */

    Arg = Arg->Next;
    Info.FieldFlags = Arg->Value.Integer8;

    /* Each remaining arg is a Named Field */

    Info.FieldType = INTERNAL_TYPE_BANK_FIELD;
    Info.RegionNode = RegionNode;

    Status = AcpiDsGetFieldNames (&Info, WalkState, Arg->Next);

    return_ACPI_STATUS (Status);
}


/*******************************************************************************
 *
 * FUNCTION:    AcpiDsCreateIndexField
 *
 * PARAMETERS:  Op              - Op containing the Field definition and args
 *              RegionNode      - Object for the containing Operation Region
 *  `           WalkState       - Current method state
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Create a new index field in the specified operation region
 *
 ******************************************************************************/

ACPI_STATUS
AcpiDsCreateIndexField (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_NAMESPACE_NODE     *RegionNode,
    ACPI_WALK_STATE         *WalkState)
{
    ACPI_STATUS             Status;
    ACPI_PARSE_OBJECT       *Arg;
    ACPI_CREATE_FIELD_INFO  Info;


    FUNCTION_TRACE_PTR ("DsCreateIndexField", Op);


    /* First arg is the name of the Index register (must already exist) */

    Arg = Op->Value.Arg;
    Status = AcpiNsLookup (WalkState->ScopeInfo, Arg->Value.String,
                    ACPI_TYPE_ANY, IMODE_EXECUTE,
                    NS_SEARCH_PARENT, WalkState, &Info.RegisterNode);
    if (ACPI_FAILURE (Status))
    {
        return_ACPI_STATUS (Status);
    }

    /* Second arg is the data register (must already exist) */

    Arg = Arg->Next;
    Status = AcpiNsLookup (WalkState->ScopeInfo, Arg->Value.String,
                    INTERNAL_TYPE_INDEX_FIELD_DEFN, IMODE_EXECUTE,
                    NS_SEARCH_PARENT, WalkState, &Info.DataRegisterNode);
    if (ACPI_FAILURE (Status))
    {
        return_ACPI_STATUS (Status);
    }

    /* Next arg is the field flags */

    Arg = Arg->Next;
    Info.FieldFlags = Arg->Value.Integer8;


    /* Each remaining arg is a Named Field */

    Info.FieldType = INTERNAL_TYPE_INDEX_FIELD;
    Info.RegionNode = RegionNode;

    Status = AcpiDsGetFieldNames (&Info, WalkState, Arg->Next);

    return_ACPI_STATUS (Status);
}


