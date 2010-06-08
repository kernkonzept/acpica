/******************************************************************************
 *
 * Module Name: dmtbinfo - Table info for non-AML tables
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999 - 2010, Intel Corp.
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

#include "acpi.h"
#include "accommon.h"
#include "acdisasm.h"

/* This module used for application-level code only */

#define _COMPONENT          ACPI_CA_DISASSEMBLER
        ACPI_MODULE_NAME    ("dmtbinfo")

/*
 * Macros used to generate offsets to specific table fields
 */
#define ACPI_FACS_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_FACS,f)
#define ACPI_GAS_OFFSET(f)              (UINT8) ACPI_OFFSET (ACPI_GENERIC_ADDRESS,f)
#define ACPI_HDR_OFFSET(f)              (UINT8) ACPI_OFFSET (ACPI_TABLE_HEADER,f)
#define ACPI_RSDP_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_RSDP,f)
#define ACPI_BOOT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_BOOT,f)
#define ACPI_BERT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_BERT,f)
#define ACPI_CPEP_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_CPEP,f)
#define ACPI_DBGP_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_DBGP,f)
#define ACPI_DMAR_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_DMAR,f)
#define ACPI_ECDT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_ECDT,f)
#define ACPI_EINJ_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_EINJ,f)
#define ACPI_ERST_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_ERST,f)
#define ACPI_HEST_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_HEST,f)
#define ACPI_HPET_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_HPET,f)
#define ACPI_IVRS_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_IVRS,f)
#define ACPI_MADT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_MADT,f)
#define ACPI_MCFG_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_MCFG,f)
#define ACPI_MCHI_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_MCHI,f)
#define ACPI_MSCT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_MSCT,f)
#define ACPI_SBST_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_SBST,f)
#define ACPI_SLIT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_SLIT,f)
#define ACPI_SPCR_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_SPCR,f)
#define ACPI_SPMI_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_SPMI,f)
#define ACPI_SRAT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_SRAT,f)
#define ACPI_TCPA_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_TCPA,f)
#define ACPI_UEFI_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_UEFI,f)
#define ACPI_WAET_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_WAET,f)
#define ACPI_WDAT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_WDAT,f)
#define ACPI_WDDT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_WDDT,f)
#define ACPI_WDRT_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_TABLE_WDRT,f)

/* Subtables */

#define ACPI_ASF0_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_ASF_INFO,f)
#define ACPI_ASF1_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_ASF_ALERT,f)
#define ACPI_ASF1a_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_ASF_ALERT_DATA,f)
#define ACPI_ASF2_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_ASF_REMOTE,f)
#define ACPI_ASF2a_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_ASF_CONTROL_DATA,f)
#define ACPI_ASF3_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_ASF_RMCP,f)
#define ACPI_ASF4_OFFSET(f)             (UINT8) ACPI_OFFSET (ACPI_ASF_ADDRESS,f)
#define ACPI_CPEP0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_CPEP_POLLING,f)
#define ACPI_DMARS_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_DMAR_DEVICE_SCOPE,f)
#define ACPI_DMAR0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_DMAR_HARDWARE_UNIT,f)
#define ACPI_DMAR1_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_DMAR_RESERVED_MEMORY,f)
#define ACPI_DMAR2_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_DMAR_ATSR,f)
#define ACPI_DMAR3_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_DMAR_RHSA,f)
#define ACPI_EINJ0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_WHEA_HEADER,f)
#define ACPI_ERST0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_WHEA_HEADER,f)
#define ACPI_HEST0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_HEST_IA_MACHINE_CHECK,f)
#define ACPI_HEST1_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_HEST_IA_CORRECTED,f)
#define ACPI_HEST2_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_HEST_IA_NMI,f)
#define ACPI_HEST6_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_HEST_AER_ROOT,f)
#define ACPI_HEST7_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_HEST_AER,f)
#define ACPI_HEST8_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_HEST_AER_BRIDGE,f)
#define ACPI_HEST9_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_HEST_GENERIC,f)
#define ACPI_HESTN_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_HEST_NOTIFY,f)
#define ACPI_HESTB_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_HEST_IA_ERROR_BANK,f)
#define ACPI_IVRSH_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_IVRS_HEADER,f)
#define ACPI_IVRS0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_IVRS_HARDWARE,f)
#define ACPI_IVRS1_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_IVRS_MEMORY,f)
#define ACPI_IVRSD_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_IVRS_DE_HEADER,f)
#define ACPI_IVRS8A_OFFSET(f)           (UINT8) ACPI_OFFSET (ACPI_IVRS_DEVICE8A,f)
#define ACPI_IVRS8B_OFFSET(f)           (UINT8) ACPI_OFFSET (ACPI_IVRS_DEVICE8B,f)
#define ACPI_IVRS8C_OFFSET(f)           (UINT8) ACPI_OFFSET (ACPI_IVRS_DEVICE8C,f)
#define ACPI_MADT0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_LOCAL_APIC,f)
#define ACPI_MADT1_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_IO_APIC,f)
#define ACPI_MADT2_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_INTERRUPT_OVERRIDE,f)
#define ACPI_MADT3_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_NMI_SOURCE,f)
#define ACPI_MADT4_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_LOCAL_APIC_NMI,f)
#define ACPI_MADT5_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_LOCAL_APIC_OVERRIDE,f)
#define ACPI_MADT6_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_IO_SAPIC,f)
#define ACPI_MADT7_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_LOCAL_SAPIC,f)
#define ACPI_MADT8_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_INTERRUPT_SOURCE,f)
#define ACPI_MADT9_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MADT_LOCAL_X2APIC,f)
#define ACPI_MADT10_OFFSET(f)           (UINT8) ACPI_OFFSET (ACPI_MADT_LOCAL_X2APIC_NMI,f)
#define ACPI_MADTH_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_SUBTABLE_HEADER,f)
#define ACPI_MCFG0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MCFG_ALLOCATION,f)
#define ACPI_MSCT0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_MSCT_PROXIMITY,f)
#define ACPI_SRATH_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_SUBTABLE_HEADER,f)
#define ACPI_SRAT0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_SRAT_CPU_AFFINITY,f)
#define ACPI_SRAT1_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_SRAT_MEM_AFFINITY,f)
#define ACPI_SRAT2_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_SRAT_X2APIC_CPU_AFFINITY,f)
#define ACPI_WDAT0_OFFSET(f)            (UINT8) ACPI_OFFSET (ACPI_WDAT_ENTRY,f)

/*
 * Simplify access to flag fields by breaking them up into bytes
 */
#define ACPI_FLAG_OFFSET(d,f,o)         (UINT8) (ACPI_OFFSET (d,f) + o)

/* Flags */

#define ACPI_FADT_FLAG_OFFSET(f,o)      ACPI_FLAG_OFFSET (ACPI_TABLE_FADT,f,o)
#define ACPI_FACS_FLAG_OFFSET(f,o)      ACPI_FLAG_OFFSET (ACPI_TABLE_FACS,f,o)
#define ACPI_HPET_FLAG_OFFSET(f,o)      ACPI_FLAG_OFFSET (ACPI_TABLE_HPET,f,o)
#define ACPI_SRAT0_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_SRAT_CPU_AFFINITY,f,o)
#define ACPI_SRAT1_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_SRAT_MEM_AFFINITY,f,o)
#define ACPI_SRAT2_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_SRAT_X2APIC_CPU_AFFINITY,f,o)
#define ACPI_MADT_FLAG_OFFSET(f,o)      ACPI_FLAG_OFFSET (ACPI_TABLE_MADT,f,o)
#define ACPI_MADT0_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_MADT_LOCAL_APIC,f,o)
#define ACPI_MADT2_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_MADT_INTERRUPT_OVERRIDE,f,o)
#define ACPI_MADT3_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_MADT_NMI_SOURCE,f,o)
#define ACPI_MADT4_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_MADT_LOCAL_APIC_NMI,f,o)
#define ACPI_MADT7_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_MADT_LOCAL_SAPIC,f,o)
#define ACPI_MADT8_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_MADT_INTERRUPT_SOURCE,f,o)
#define ACPI_MADT9_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_MADT_LOCAL_X2APIC,f,o)
#define ACPI_MADT10_FLAG_OFFSET(f,o)    ACPI_FLAG_OFFSET (ACPI_MADT_LOCAL_X2APIC_NMI,f,o)
#define ACPI_WDDT_FLAG_OFFSET(f,o)      ACPI_FLAG_OFFSET (ACPI_TABLE_WDDT,f,o)
#define ACPI_EINJ0_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_WHEA_HEADER,f,o)
#define ACPI_ERST0_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_WHEA_HEADER,f,o)
#define ACPI_HEST0_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_HEST_IA_MACHINE_CHECK,f,o)
#define ACPI_HEST1_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_HEST_IA_CORRECTED,f,o)
#define ACPI_HEST6_FLAG_OFFSET(f,o)     ACPI_FLAG_OFFSET (ACPI_HEST_AER_ROOT,f,o)

/*
 * Required terminator for all tables below
 */
#define ACPI_DMT_TERMINATOR             {ACPI_DMT_EXIT, 0, NULL, 0}


/*
 * ACPI Table Information, used to dump formatted ACPI tables
 *
 * Each entry is of the form:  <Field Type, Field Offset, Field Name>
 */

/*******************************************************************************
 *
 * Common ACPI table header
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoHeader[] =
{
    {ACPI_DMT_SIG,      ACPI_HDR_OFFSET (Signature[0]),             "Signature", 0},
    {ACPI_DMT_UINT32,   ACPI_HDR_OFFSET (Length),                   "Table Length", DT_LENGTH},
    {ACPI_DMT_UINT8,    ACPI_HDR_OFFSET (Revision),                 "Revision", 0},
    {ACPI_DMT_CHKSUM,   ACPI_HDR_OFFSET (Checksum),                 "Checksum", 0},
    {ACPI_DMT_NAME6,    ACPI_HDR_OFFSET (OemId[0]),                 "Oem ID", 0},
    {ACPI_DMT_NAME8,    ACPI_HDR_OFFSET (OemTableId[0]),            "Oem Table ID", 0},
    {ACPI_DMT_UINT32,   ACPI_HDR_OFFSET (OemRevision),              "Oem Revision", 0},
    {ACPI_DMT_NAME4,    ACPI_HDR_OFFSET (AslCompilerId[0]),         "Asl Compiler ID", 0},
    {ACPI_DMT_UINT32,   ACPI_HDR_OFFSET (AslCompilerRevision),      "Asl Compiler Revision", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * GAS - Generic Address Structure
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoGas[] =
{
    {ACPI_DMT_SPACEID,  ACPI_GAS_OFFSET (SpaceId),                  "Space ID", 0},
    {ACPI_DMT_UINT8,    ACPI_GAS_OFFSET (BitWidth),                 "Bit Width", 0},
    {ACPI_DMT_UINT8,    ACPI_GAS_OFFSET (BitOffset),                "Bit Offset", 0},
    {ACPI_DMT_UINT8,    ACPI_GAS_OFFSET (AccessWidth),              "Access Width", 0},
    {ACPI_DMT_UINT64,   ACPI_GAS_OFFSET (Address),                  "Address", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * RSDP - Root System Description Pointer (Signature is "RSD PTR ")
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoRsdp1[] =
{
    {ACPI_DMT_NAME8,    ACPI_RSDP_OFFSET (Signature[0]),            "Signature", 0},
    {ACPI_DMT_UINT8,    ACPI_RSDP_OFFSET (Checksum),                "Checksum", 0},
    {ACPI_DMT_NAME6,    ACPI_RSDP_OFFSET (OemId[0]),                "Oem ID", 0},
    {ACPI_DMT_UINT8,    ACPI_RSDP_OFFSET (Revision),                "Revision", 0},
    {ACPI_DMT_UINT32,   ACPI_RSDP_OFFSET (RsdtPhysicalAddress),     "RSDT Address", 0},
    ACPI_DMT_TERMINATOR
};

/* ACPI 2.0+ Extensions */

ACPI_DMTABLE_INFO           AcpiDmTableInfoRsdp2[] =
{
    {ACPI_DMT_UINT32,   ACPI_RSDP_OFFSET (Length),                  "Length", DT_LENGTH},
    {ACPI_DMT_UINT64,   ACPI_RSDP_OFFSET (XsdtPhysicalAddress),     "XSDT Address", 0},
    {ACPI_DMT_UINT8,    ACPI_RSDP_OFFSET (ExtendedChecksum),        "Extended Checksum", 0},
    {ACPI_DMT_UINT24,   ACPI_RSDP_OFFSET (Reserved[0]),             "Reserved", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * FACS - Firmware ACPI Control Structure
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoFacs[] =
{
    {ACPI_DMT_NAME4,    ACPI_FACS_OFFSET (Signature[0]),            "Signature", 0},
    {ACPI_DMT_UINT32,   ACPI_FACS_OFFSET (Length),                  "Length", DT_LENGTH},
    {ACPI_DMT_UINT32,   ACPI_FACS_OFFSET (HardwareSignature),       "Hardware Signature", 0},
    {ACPI_DMT_UINT32,   ACPI_FACS_OFFSET (FirmwareWakingVector),    "32 Firmware Waking Vector", 0},
    {ACPI_DMT_UINT32,   ACPI_FACS_OFFSET (GlobalLock),              "Global Lock", 0},
    {ACPI_DMT_UINT32,   ACPI_FACS_OFFSET (Flags),                   "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_FACS_FLAG_OFFSET (Flags,0),            "S4BIOS Support Present", 0},
    {ACPI_DMT_FLAG1,    ACPI_FACS_FLAG_OFFSET (Flags,0),            "64-bit Wake Supported (V2)", 0},
    {ACPI_DMT_UINT64,   ACPI_FACS_OFFSET (XFirmwareWakingVector),   "64 Firmware Waking Vector", 0},
    {ACPI_DMT_UINT8,    ACPI_FACS_OFFSET (Version),                 "Version", 0},
    {ACPI_DMT_UINT24,   ACPI_FACS_OFFSET (Reserved[0]),             "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_FACS_OFFSET (OspmFlags),               "OspmFlags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_FACS_FLAG_OFFSET (OspmFlags,0),        "64-bit Wake Env Required (V2)", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * FADT - Fixed ACPI Description Table (Signature is FACP)
 *
 ******************************************************************************/

/* ACPI 1.0 FADT (Version 1) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoFadt1[] =
{
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Facs),                    "FACS Address", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Dsdt),                    "DSDT Address", DT_NON_ZERO},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (Model),                   "Model", 0},
    {ACPI_DMT_FADTPM,   ACPI_FADT_OFFSET (PreferredProfile),        "PM Profile", 0},
    {ACPI_DMT_UINT16,   ACPI_FADT_OFFSET (SciInterrupt),            "SCI Interrupt", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (SmiCommand),              "SMI Command Port", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (AcpiEnable),              "ACPI Enable Value", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (AcpiDisable),             "ACPI Disable Value", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (S4BiosRequest),           "S4BIOS Command", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (PstateControl),           "P-State Control", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Pm1aEventBlock),          "PM1A Event Block Address", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Pm1bEventBlock),          "PM1B Event Block Address", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Pm1aControlBlock),        "PM1A Control Block Address", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Pm1bControlBlock),        "PM1B Control Block Address", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Pm2ControlBlock),         "PM2 Control Block Address", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (PmTimerBlock),            "PM Timer Block Address", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Gpe0Block),               "GPE0 Block Address", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Gpe1Block),               "GPE1 Block Address", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (Pm1EventLength),          "PM1 Event Block Length", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (Pm1ControlLength),        "PM1 Control Block Length", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (Pm2ControlLength),        "PM2 Control Block Length", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (PmTimerLength),           "PM Timer Block Length", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (Gpe0BlockLength),         "GPE0 Block Length", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (Gpe1BlockLength),         "GPE1 Block Length", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (Gpe1Base),                "GPE1 Base Offset", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (CstControl),              "_CST Support", 0},
    {ACPI_DMT_UINT16,   ACPI_FADT_OFFSET (C2Latency),               "C2 Latency", 0},
    {ACPI_DMT_UINT16,   ACPI_FADT_OFFSET (C3Latency),               "C3 Latency", 0},
    {ACPI_DMT_UINT16,   ACPI_FADT_OFFSET (FlushSize),               "CPU Cache Size", 0},
    {ACPI_DMT_UINT16,   ACPI_FADT_OFFSET (FlushStride),             "Cache Flush Stride", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (DutyOffset),              "Duty Cycle Offset", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (DutyWidth),               "Duty Cycle Width", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (DayAlarm),                "RTC Day Alarm Index", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (MonthAlarm),              "RTC Month Alarm Index", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (Century),                 "RTC Century Index", 0},
    {ACPI_DMT_UINT16,   ACPI_FADT_OFFSET (BootFlags),               "Boot Flags (decoded below)", DT_FLAG},

    /* Boot Architecture Flags byte 0 */

    {ACPI_DMT_FLAG0,    ACPI_FADT_FLAG_OFFSET (BootFlags,0),        "Legacy Devices Supported (V2)", 0},
    {ACPI_DMT_FLAG1,    ACPI_FADT_FLAG_OFFSET (BootFlags,0),        "8042 Present on ports 60/64 (V2)", 0},
    {ACPI_DMT_FLAG2,    ACPI_FADT_FLAG_OFFSET (BootFlags,0),        "VGA Not Present (V4)", 0},
    {ACPI_DMT_FLAG3,    ACPI_FADT_FLAG_OFFSET (BootFlags,0),        "MSI Not Supported (V4)", 0},
    {ACPI_DMT_FLAG4,    ACPI_FADT_FLAG_OFFSET (BootFlags,0),        "PCIe ASPM Not Supported (V4)", 0},

    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (Reserved),                "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_FADT_OFFSET (Flags),                   "Flags (decoded below)", DT_FLAG},

    /* Flags byte 0 */

    {ACPI_DMT_FLAG0,    ACPI_FADT_FLAG_OFFSET (Flags,0),            "WBINVD instruction is operational (V1)", 0},
    {ACPI_DMT_FLAG1,    ACPI_FADT_FLAG_OFFSET (Flags,0),            "WBINVD flushes all caches (V1)", 0},
    {ACPI_DMT_FLAG2,    ACPI_FADT_FLAG_OFFSET (Flags,0),            "All CPUs support C1 (V1)", 0},
    {ACPI_DMT_FLAG3,    ACPI_FADT_FLAG_OFFSET (Flags,0),            "C2 works on MP system (V1)", 0},
    {ACPI_DMT_FLAG4,    ACPI_FADT_FLAG_OFFSET (Flags,0),            "Control Method Power Button (V1)", 0},
    {ACPI_DMT_FLAG5,    ACPI_FADT_FLAG_OFFSET (Flags,0),            "Control Method Sleep Button (V1)", 0},
    {ACPI_DMT_FLAG6,    ACPI_FADT_FLAG_OFFSET (Flags,0),            "RTC wake not in fixed reg space (V1)", 0},
    {ACPI_DMT_FLAG7,    ACPI_FADT_FLAG_OFFSET (Flags,0),            "RTC can wake system from S4 (V1)", 0},

    /* Flags byte 1 */

    {ACPI_DMT_FLAG0,    ACPI_FADT_FLAG_OFFSET (Flags,1),            "32-bit PM Timer (V1)", 0},
    {ACPI_DMT_FLAG1,    ACPI_FADT_FLAG_OFFSET (Flags,1),            "Docking Supported (V1)", 0},
    {ACPI_DMT_FLAG2,    ACPI_FADT_FLAG_OFFSET (Flags,1),            "Reset Register Supported (V2)", 0},
    {ACPI_DMT_FLAG3,    ACPI_FADT_FLAG_OFFSET (Flags,1),            "Sealed Case (V3)", 0},
    {ACPI_DMT_FLAG4,    ACPI_FADT_FLAG_OFFSET (Flags,1),            "Headless - No Video (V3)", 0},
    {ACPI_DMT_FLAG5,    ACPI_FADT_FLAG_OFFSET (Flags,1),            "Use native instr after SLP_TYPx (V3)", 0},
    {ACPI_DMT_FLAG6,    ACPI_FADT_FLAG_OFFSET (Flags,1),            "PCIEXP_WAK Bits Supported (V4)", 0},
    {ACPI_DMT_FLAG7,    ACPI_FADT_FLAG_OFFSET (Flags,1),            "Use Platform Timer (V4)", 0},

    /* Flags byte 2 */

    {ACPI_DMT_FLAG0,    ACPI_FADT_FLAG_OFFSET (Flags,2),            "RTC_STS valid on S4 wake (V4)", 0},
    {ACPI_DMT_FLAG1,    ACPI_FADT_FLAG_OFFSET (Flags,2),            "Remote Power-on capable (V4)", 0},
    {ACPI_DMT_FLAG2,    ACPI_FADT_FLAG_OFFSET (Flags,2),            "Use APIC Cluster Model (V4)", 0},
    {ACPI_DMT_FLAG3,    ACPI_FADT_FLAG_OFFSET (Flags,2),            "Use APIC Physical Destination Mode (V4)", 0},
    ACPI_DMT_TERMINATOR
};

/* ACPI 1.0 MS Extensions (FADT version 2) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoFadt2[] =
{
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (ResetRegister),           "Reset Register", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (ResetValue),              "Value to cause reset", 0},
    {ACPI_DMT_UINT24,   ACPI_FADT_OFFSET (Reserved4[0]),            "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

/* ACPI 2.0+ Extensions (FADT version 3+) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoFadt3[] =
{
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (ResetRegister),           "Reset Register", 0},
    {ACPI_DMT_UINT8,    ACPI_FADT_OFFSET (ResetValue),              "Value to cause reset", 0},
    {ACPI_DMT_UINT24,   ACPI_FADT_OFFSET (Reserved4[0]),            "Reserved", 0},
    {ACPI_DMT_UINT64,   ACPI_FADT_OFFSET (XFacs),                   "FACS Address", 0},
    {ACPI_DMT_UINT64,   ACPI_FADT_OFFSET (XDsdt),                   "DSDT Address", 0},
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (XPm1aEventBlock),         "PM1A Event Block", 0},
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (XPm1bEventBlock),         "PM1B Event Block", 0},
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (XPm1aControlBlock),       "PM1A Control Block", 0},
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (XPm1bControlBlock),       "PM1B Control Block", 0},
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (XPm2ControlBlock),        "PM2 Control Block", 0},
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (XPmTimerBlock),           "PM Timer Block", 0},
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (XGpe0Block),              "GPE0 Block", 0},
    {ACPI_DMT_GAS,      ACPI_FADT_OFFSET (XGpe1Block),              "GPE1 Block", 0},
    ACPI_DMT_TERMINATOR
};


/*
 * Remaining tables are not consumed directly by the ACPICA subsystem
 */

/*******************************************************************************
 *
 * ASF - Alert Standard Format table (Signature "ASF!")
 *
 ******************************************************************************/

/* Common Subtable header (one per Subtable) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoAsfHdr[] =
{
    {ACPI_DMT_ASF,      ACPI_ASF0_OFFSET (Header.Type),             "Subtable Type", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF0_OFFSET (Header.Reserved),         "Reserved", 0},
    {ACPI_DMT_UINT16,   ACPI_ASF0_OFFSET (Header.Length),           "Length", DT_LENGTH},
    ACPI_DMT_TERMINATOR
};

/* 0: ASF Information */

ACPI_DMTABLE_INFO           AcpiDmTableInfoAsf0[] =
{
    {ACPI_DMT_UINT8,    ACPI_ASF0_OFFSET (MinResetValue),           "Minimum Reset Value", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF0_OFFSET (MinPollInterval),         "Minimum Polling Interval", 0},
    {ACPI_DMT_UINT16,   ACPI_ASF0_OFFSET (SystemId),                "System ID", 0},
    {ACPI_DMT_UINT32,   ACPI_ASF0_OFFSET (MfgId),                   "Manufacturer ID", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF0_OFFSET (Flags),                   "Flags", 0},
    {ACPI_DMT_UINT24,   ACPI_ASF0_OFFSET (Reserved2[0]),            "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

/* 1: ASF Alerts */

ACPI_DMTABLE_INFO           AcpiDmTableInfoAsf1[] =
{
    {ACPI_DMT_UINT8,    ACPI_ASF1_OFFSET (AssertMask),              "AssertMask", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1_OFFSET (DeassertMask),            "DeassertMask", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1_OFFSET (Alerts),                  "Alert Count", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1_OFFSET (DataLength),              "Alert Data Length", 0},
    ACPI_DMT_TERMINATOR
};

/* 1a: ASF Alert data */

ACPI_DMTABLE_INFO           AcpiDmTableInfoAsf1a[] =
{
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (Address),                "Address", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (Command),                "Command", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (Mask),                   "Mask", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (Value),                  "Value", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (SensorType),             "SensorType", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (Type),                   "Type", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (Offset),                 "Offset", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (SourceType),             "SourceType", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (Severity),               "Severity", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (SensorNumber),           "SensorNumber", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (Entity),                 "Entity", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF1a_OFFSET (Instance),               "Instance", 0},
    ACPI_DMT_TERMINATOR
};

/* 2: ASF Remote Control */

ACPI_DMTABLE_INFO           AcpiDmTableInfoAsf2[] =
{
    {ACPI_DMT_UINT8,    ACPI_ASF2_OFFSET (Controls),                "Control Count", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF2_OFFSET (DataLength),              "Control Data Length", 0},
    {ACPI_DMT_UINT16,   ACPI_ASF2_OFFSET (Reserved2),               "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

/* 2a: ASF Control data */

ACPI_DMTABLE_INFO           AcpiDmTableInfoAsf2a[] =
{
    {ACPI_DMT_UINT8,    ACPI_ASF2a_OFFSET (Function),               "Function", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF2a_OFFSET (Address),                "Address", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF2a_OFFSET (Command),                "Command", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF2a_OFFSET (Value),                  "Value", 0},
    ACPI_DMT_TERMINATOR
};

/* 3: ASF RMCP Boot Options */

ACPI_DMTABLE_INFO           AcpiDmTableInfoAsf3[] =
{
    {ACPI_DMT_UINT56,   ACPI_ASF3_OFFSET (Capabilities[0]),         "Capabilities", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF3_OFFSET (CompletionCode),          "Completion Code", 0},
    {ACPI_DMT_UINT32,   ACPI_ASF3_OFFSET (EnterpriseId),            "Enterprise ID", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF3_OFFSET (Command),                 "Command", 0},
    {ACPI_DMT_UINT16,   ACPI_ASF3_OFFSET (Parameter),               "Parameter", 0},
    {ACPI_DMT_UINT16,   ACPI_ASF3_OFFSET (BootOptions),             "Boot Options", 0},
    {ACPI_DMT_UINT16,   ACPI_ASF3_OFFSET (OemParameters),           "Oem Parameters", 0},
    ACPI_DMT_TERMINATOR
};

/* 4: ASF Address */

ACPI_DMTABLE_INFO           AcpiDmTableInfoAsf4[] =
{
    {ACPI_DMT_UINT8,    ACPI_ASF4_OFFSET (EpromAddress),            "Eprom Address", 0},
    {ACPI_DMT_UINT8,    ACPI_ASF4_OFFSET (Devices),                 "Device Count", DT_COUNT},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * BERT -  Boot Error Record table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoBert[] =
{
    {ACPI_DMT_UINT32,   ACPI_BERT_OFFSET (RegionLength),            "Boot Error Region Length", 0},
    {ACPI_DMT_UINT64,   ACPI_BERT_OFFSET (Address),                 "Boot Error Region Address", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * BOOT - Simple Boot Flag Table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoBoot[] =
{
    {ACPI_DMT_UINT8,    ACPI_BOOT_OFFSET (CmosIndex),               "Boot Register Index", 0},
    {ACPI_DMT_UINT24,   ACPI_BOOT_OFFSET (Reserved[0]),             "Reserved", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * CPEP - Corrected Platform Error Polling table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoCpep[] =
{
    {ACPI_DMT_UINT64,   ACPI_CPEP_OFFSET (Reserved),                "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

ACPI_DMTABLE_INFO           AcpiDmTableInfoCpep0[] =
{
    {ACPI_DMT_UINT8,    ACPI_CPEP0_OFFSET (Header.Type),            "Subtable Type", 0},
    {ACPI_DMT_UINT8,    ACPI_CPEP0_OFFSET (Header.Length),          "Length", DT_LENGTH},
    {ACPI_DMT_UINT8,    ACPI_CPEP0_OFFSET (Id),                     "Processor ID", 0},
    {ACPI_DMT_UINT8,    ACPI_CPEP0_OFFSET (Eid),                    "Processor EID", 0},
    {ACPI_DMT_UINT32,   ACPI_CPEP0_OFFSET (Interval),               "Polling Interval", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * DBGP - Debug Port
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoDbgp[] =
{
    {ACPI_DMT_UINT8,    ACPI_DBGP_OFFSET (Type),                    "Interface Type", 0},
    {ACPI_DMT_UINT24,   ACPI_DBGP_OFFSET (Reserved[0]),             "Reserved", 0},
    {ACPI_DMT_GAS,      ACPI_DBGP_OFFSET (DebugPort),               "Debug Port Register", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * DMAR - DMA Remapping table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoDmar[] =
{
    {ACPI_DMT_UINT8,    ACPI_DMAR_OFFSET (Width),                   "Host Address Width", 0},
    {ACPI_DMT_UINT8,    ACPI_DMAR_OFFSET (Flags),                   "Flags", 0},
    ACPI_DMT_TERMINATOR
};

/* Common Subtable header (one per Subtable) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoDmarHdr[] =
{
    {ACPI_DMT_DMAR,     ACPI_DMAR0_OFFSET (Header.Type),            "Subtable Type", 0},
    {ACPI_DMT_UINT16,   ACPI_DMAR0_OFFSET (Header.Length),          "Length", DT_LENGTH},
    ACPI_DMT_TERMINATOR
};

/* Common device scope entry */

ACPI_DMTABLE_INFO           AcpiDmTableInfoDmarScope[] =
{
    {ACPI_DMT_UINT8,    ACPI_DMARS_OFFSET (EntryType),              "Device Scope Entry Type", 0},
    {ACPI_DMT_UINT8,    ACPI_DMARS_OFFSET (Length),                 "Entry Length", DT_LENGTH},
    {ACPI_DMT_UINT16,   ACPI_DMARS_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT8,    ACPI_DMARS_OFFSET (EnumerationId),          "Enumeration ID", 0},
    {ACPI_DMT_UINT8,    ACPI_DMARS_OFFSET (Bus),                    "PCI Bus Number", 0},
    ACPI_DMT_TERMINATOR
};

/* DMAR Subtables */

/* 0: Hardware Unit Definition */

ACPI_DMTABLE_INFO           AcpiDmTableInfoDmar0[] =
{
    {ACPI_DMT_UINT8,    ACPI_DMAR0_OFFSET (Flags),                  "Flags", 0},
    {ACPI_DMT_UINT8,    ACPI_DMAR0_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT16,   ACPI_DMAR0_OFFSET (Segment),                "PCI Segment Number", 0},
    {ACPI_DMT_UINT64,   ACPI_DMAR0_OFFSET (Address),                "Register Base Address", 0},
    ACPI_DMT_TERMINATOR
};

/* 1: Reserved Memory Definition */

ACPI_DMTABLE_INFO           AcpiDmTableInfoDmar1[] =
{
    {ACPI_DMT_UINT16,   ACPI_DMAR1_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT16,   ACPI_DMAR1_OFFSET (Segment),                "PCI Segment Number", 0},
    {ACPI_DMT_UINT64,   ACPI_DMAR1_OFFSET (BaseAddress),            "Base Address", 0},
    {ACPI_DMT_UINT64,   ACPI_DMAR1_OFFSET (EndAddress),             "End Address (limit)", 0},
    ACPI_DMT_TERMINATOR
};

/* 2: Root Port ATS Capability Definition */

ACPI_DMTABLE_INFO           AcpiDmTableInfoDmar2[] =
{
    {ACPI_DMT_UINT8,    ACPI_DMAR2_OFFSET (Flags),                  "Flags", 0},
    {ACPI_DMT_UINT8,    ACPI_DMAR2_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT16,   ACPI_DMAR2_OFFSET (Segment),                "PCI Segment Number", 0},
    ACPI_DMT_TERMINATOR
};

/* 3: Remapping Hardware Static Affinity Structure */

ACPI_DMTABLE_INFO           AcpiDmTableInfoDmar3[] =
{
    {ACPI_DMT_UINT32,   ACPI_DMAR3_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT64,   ACPI_DMAR3_OFFSET (BaseAddress),            "Base Address", 0},
    {ACPI_DMT_UINT32,   ACPI_DMAR3_OFFSET (ProximityDomain),        "Proximity Domain", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * ECDT - Embedded Controller Boot Resources Table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoEcdt[] =
{
    {ACPI_DMT_GAS,      ACPI_ECDT_OFFSET (Control),                 "Command/Status Register", 0},
    {ACPI_DMT_GAS,      ACPI_ECDT_OFFSET (Data),                    "Data Register", 0},
    {ACPI_DMT_UINT32,   ACPI_ECDT_OFFSET (Uid),                     "UID", 0},
    {ACPI_DMT_UINT8,    ACPI_ECDT_OFFSET (Gpe),                     "GPE Number", 0},
    {ACPI_DMT_STRING,   ACPI_ECDT_OFFSET (Id[0]),                   "Namepath", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * EINJ - Error Injection table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoEinj[] =
{
    {ACPI_DMT_UINT32,   ACPI_EINJ_OFFSET (HeaderLength),            "Injection Header Length", DT_LENGTH},
    {ACPI_DMT_UINT8,    ACPI_EINJ_OFFSET (Flags),                   "Flags", 0},
    {ACPI_DMT_UINT24,   ACPI_EINJ_OFFSET (Reserved[0]),             "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_EINJ_OFFSET (Entries),                 "Injection Entry Count", 0},
    ACPI_DMT_TERMINATOR
};

ACPI_DMTABLE_INFO           AcpiDmTableInfoEinj0[] =
{
    {ACPI_DMT_EINJACT,  ACPI_EINJ0_OFFSET (Action),                 "Action", 0},
    {ACPI_DMT_EINJINST, ACPI_EINJ0_OFFSET (Instruction),            "Instruction", 0},
    {ACPI_DMT_UINT8,    ACPI_EINJ0_OFFSET (Flags),                  "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_EINJ0_FLAG_OFFSET (Flags,0),           "Preserve Register Bits", 0},

    {ACPI_DMT_UINT8,    ACPI_EINJ0_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_GAS,      ACPI_EINJ0_OFFSET (RegisterRegion),         "Register Region", 0},
    {ACPI_DMT_UINT64,   ACPI_EINJ0_OFFSET (Value),                  "Value", 0},
    {ACPI_DMT_UINT64,   ACPI_EINJ0_OFFSET (Mask),                   "Mask", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * ERST - Error Record Serialization table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoErst[] =
{
    {ACPI_DMT_UINT32,   ACPI_ERST_OFFSET (HeaderLength),            "Serialization Header Length", DT_LENGTH},
    {ACPI_DMT_UINT32,   ACPI_ERST_OFFSET (Reserved),                "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_ERST_OFFSET (Entries),                 "Instruction Entry Count", 0},
    ACPI_DMT_TERMINATOR
};

ACPI_DMTABLE_INFO           AcpiDmTableInfoErst0[] =
{
    {ACPI_DMT_ERSTACT,  ACPI_ERST0_OFFSET (Action),                 "Action", 0},
    {ACPI_DMT_ERSTINST, ACPI_ERST0_OFFSET (Instruction),            "Instruction", 0},
    {ACPI_DMT_UINT8,    ACPI_ERST0_OFFSET (Flags),                  "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_ERST0_FLAG_OFFSET (Flags,0),           "Preserve Register Bits", 0},

    {ACPI_DMT_UINT8,    ACPI_ERST0_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_GAS,      ACPI_ERST0_OFFSET (RegisterRegion),         "Register Region", 0},
    {ACPI_DMT_UINT64,   ACPI_ERST0_OFFSET (Value),                  "Value", 0},
    {ACPI_DMT_UINT64,   ACPI_ERST0_OFFSET (Mask),                   "Mask", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * HEST - Hardware Error Source table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoHest[] =
{
    {ACPI_DMT_UINT32,   ACPI_HEST_OFFSET (ErrorSourceCount),        "Error Source Count", 0},
    ACPI_DMT_TERMINATOR
};

/* Common HEST structures for subtables */

#define ACPI_DM_HEST_HEADER \
    {ACPI_DMT_HEST,     ACPI_HEST0_OFFSET (Header.Type),            "Subtable Type", 0}, \
    {ACPI_DMT_UINT16,   ACPI_HEST0_OFFSET (Header.SourceId),        "Source Id", 0}

#define ACPI_DM_HEST_AER \
    {ACPI_DMT_UINT16,   ACPI_HEST6_OFFSET (Aer.Reserved1),              "Reserved", 0}, \
    {ACPI_DMT_UINT8,    ACPI_HEST6_OFFSET (Aer.Flags),                  "Flags (decoded below)", DT_FLAG}, \
    {ACPI_DMT_FLAG0,    ACPI_HEST6_FLAG_OFFSET (Aer.Flags,0),           "Firmware First", 0}, \
    {ACPI_DMT_UINT8,    ACPI_HEST6_OFFSET (Aer.Enabled),                "Enabled", 0}, \
    {ACPI_DMT_UINT32,   ACPI_HEST6_OFFSET (Aer.RecordsToPreallocate),   "Records To Preallocate", 0}, \
    {ACPI_DMT_UINT32,   ACPI_HEST6_OFFSET (Aer.MaxSectionsPerRecord),   "Max Sections Per Record", 0}, \
    {ACPI_DMT_UINT32,   ACPI_HEST6_OFFSET (Aer.Bus),                    "Bus", 0}, \
    {ACPI_DMT_UINT16,   ACPI_HEST6_OFFSET (Aer.Device),                 "Device", 0}, \
    {ACPI_DMT_UINT16,   ACPI_HEST6_OFFSET (Aer.Function),               "Function", 0}, \
    {ACPI_DMT_UINT16,   ACPI_HEST6_OFFSET (Aer.DeviceControl),          "DeviceControl", 0}, \
    {ACPI_DMT_UINT16,   ACPI_HEST6_OFFSET (Aer.Reserved2),              "Reserved", 0}, \
    {ACPI_DMT_UINT32,   ACPI_HEST6_OFFSET (Aer.UncorrectableMask),      "Uncorrectable Mask", 0}, \
    {ACPI_DMT_UINT32,   ACPI_HEST6_OFFSET (Aer.UncorrectableSeverity),  "Uncorrectable Severity", 0}, \
    {ACPI_DMT_UINT32,   ACPI_HEST6_OFFSET (Aer.CorrectableMask),        "Correctable Mask", 0}, \
    {ACPI_DMT_UINT32,   ACPI_HEST6_OFFSET (Aer.AdvancedCapabilities),   "Advanced Capabilities", 0}


/* HEST Subtables */

/* 0: IA32 Machine Check Exception */

ACPI_DMTABLE_INFO           AcpiDmTableInfoHest0[] =
{
    ACPI_DM_HEST_HEADER,
    {ACPI_DMT_UINT16,   ACPI_HEST0_OFFSET (Reserved1),              "Reserved1", 0},
    {ACPI_DMT_UINT8,    ACPI_HEST0_OFFSET (Flags),                  "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_HEST0_FLAG_OFFSET (Flags,0),           "Firmware First", 0},

    {ACPI_DMT_UINT8,    ACPI_HEST0_OFFSET (Enabled),                "Enabled", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST0_OFFSET (RecordsToPreallocate),   "Records To Preallocate", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST0_OFFSET (MaxSectionsPerRecord),   "Max Sections Per Record", 0},
    {ACPI_DMT_UINT64,   ACPI_HEST0_OFFSET (GlobalCapabilityData),   "Global Capability Data", 0},
    {ACPI_DMT_UINT64,   ACPI_HEST0_OFFSET (GlobalControlData),      "Global Control Data", 0},
    {ACPI_DMT_UINT8,    ACPI_HEST0_OFFSET (NumHardwareBanks),       "Num Hardware Banks", 0},
    {ACPI_DMT_UINT56,   ACPI_HEST0_OFFSET (Reserved3[0]),           "Reserved2", 0},
    ACPI_DMT_TERMINATOR
};

/* 1: IA32 Corrected Machine Check */

ACPI_DMTABLE_INFO           AcpiDmTableInfoHest1[] =
{
    ACPI_DM_HEST_HEADER,
    {ACPI_DMT_UINT16,   ACPI_HEST1_OFFSET (Reserved1),              "Reserved1", 0},
    {ACPI_DMT_UINT8,    ACPI_HEST1_OFFSET (Flags),                  "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_HEST1_FLAG_OFFSET (Flags,0),           "Firmware First", 0},

    {ACPI_DMT_UINT8,    ACPI_HEST1_OFFSET (Enabled),                "Enabled", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST1_OFFSET (RecordsToPreallocate),   "Records To Preallocate", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST1_OFFSET (MaxSectionsPerRecord),   "Max Sections Per Record", 0},
    {ACPI_DMT_HESTNTFY, ACPI_HEST1_OFFSET (Notify),                 "Notify", 0},
    {ACPI_DMT_UINT8,    ACPI_HEST1_OFFSET (NumHardwareBanks),       "Num Hardware Banks", 0},
    {ACPI_DMT_UINT24,   ACPI_HEST1_OFFSET (Reserved2[0]),           "Reserved2", 0},
    ACPI_DMT_TERMINATOR
};

/* 2: IA32 Non-Maskable Interrupt */

ACPI_DMTABLE_INFO           AcpiDmTableInfoHest2[] =
{
    ACPI_DM_HEST_HEADER,
    {ACPI_DMT_UINT32,   ACPI_HEST2_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST2_OFFSET (RecordsToPreallocate),   "Records To Preallocate", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST2_OFFSET (MaxSectionsPerRecord),   "Max Sections Per Record", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST2_OFFSET (MaxRawDataLength),       "Max Raw Data Length", 0},
    ACPI_DMT_TERMINATOR
};

/* 6: PCI Express Root Port AER */

ACPI_DMTABLE_INFO           AcpiDmTableInfoHest6[] =
{
    ACPI_DM_HEST_HEADER,
    ACPI_DM_HEST_AER,
    {ACPI_DMT_UINT32,   ACPI_HEST6_OFFSET (RootErrorCommand),       "Root Error Command", 0},
    ACPI_DMT_TERMINATOR
};

/* 7: PCI Express AER (AER Endpoint) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoHest7[] =
{
    ACPI_DM_HEST_HEADER,
    ACPI_DM_HEST_AER,
    ACPI_DMT_TERMINATOR
};

/* 8: PCI Express/PCI-X Bridge AER */

ACPI_DMTABLE_INFO           AcpiDmTableInfoHest8[] =
{
    ACPI_DM_HEST_HEADER,
    ACPI_DM_HEST_AER,
    {ACPI_DMT_UINT32,   ACPI_HEST8_OFFSET (UncorrectableMask2),     "2nd Uncorrectable Mask", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST8_OFFSET (UncorrectableSeverity2), "2nd Uncorrectable Severity", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST8_OFFSET (AdvancedCapabilities2),  "2nd Advanced Capabilities", 0},
    ACPI_DMT_TERMINATOR
};

/* 9: Generic Hardware Error Source */

ACPI_DMTABLE_INFO           AcpiDmTableInfoHest9[] =
{
    ACPI_DM_HEST_HEADER,
    {ACPI_DMT_UINT16,   ACPI_HEST9_OFFSET (RelatedSourceId),        "Related Source Id", 0},
    {ACPI_DMT_UINT8,    ACPI_HEST9_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT8,    ACPI_HEST9_OFFSET (Enabled),                "Enabled", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST9_OFFSET (RecordsToPreallocate),   "Records To Preallocate", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST9_OFFSET (MaxSectionsPerRecord),   "Max Sections Per Record", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST9_OFFSET (MaxRawDataLength),       "Max Raw Data Length", 0},
    {ACPI_DMT_GAS,      ACPI_HEST9_OFFSET (ErrorStatusAddress),     "Error Status Address", 0},
    {ACPI_DMT_HESTNTFY, ACPI_HEST9_OFFSET (Notify),                 "Notify", 0},
    {ACPI_DMT_UINT32,   ACPI_HEST9_OFFSET (ErrorBlockLength),       "Error Status Block Length", 0},
    ACPI_DMT_TERMINATOR
};

ACPI_DMTABLE_INFO           AcpiDmTableInfoHestNotify[] =
{
    {ACPI_DMT_HESTNTYP, ACPI_HESTN_OFFSET (Type),                   "Notify Type", 0},
    {ACPI_DMT_UINT8,    ACPI_HESTN_OFFSET (Length),                 "Notify Length", DT_LENGTH},
    {ACPI_DMT_UINT16,   ACPI_HESTN_OFFSET (ConfigWriteEnable),      "Configuration Write Enable", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTN_OFFSET (PollInterval),           "PollInterval", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTN_OFFSET (Vector),                 "Vector", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTN_OFFSET (PollingThresholdValue),  "Polling Threshold Value", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTN_OFFSET (PollingThresholdWindow), "Polling Threshold Window", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTN_OFFSET (ErrorThresholdValue),    "Error Threshold Value", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTN_OFFSET (ErrorThresholdWindow),   "Error Threshold Window", 0},
    ACPI_DMT_TERMINATOR
};


/*
 * IA32 Error Bank(s) - Follows the ACPI_HEST_IA_MACHINE_CHECK and
 * ACPI_HEST_IA_CORRECTED structures.
 */
ACPI_DMTABLE_INFO           AcpiDmTableInfoHestBank[] =
{
    {ACPI_DMT_UINT8,    ACPI_HESTB_OFFSET (BankNumber),             "Bank Number", 0},
    {ACPI_DMT_UINT8,    ACPI_HESTB_OFFSET (ClearStatusOnInit),      "Clear Status On Init", 0},
    {ACPI_DMT_UINT8,    ACPI_HESTB_OFFSET (StatusFormat),           "Status Format", 0},
    {ACPI_DMT_UINT8,    ACPI_HESTB_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTB_OFFSET (ControlRegister),        "Control Register", 0},
    {ACPI_DMT_UINT64,   ACPI_HESTB_OFFSET (ControlData),            "Control Data", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTB_OFFSET (StatusRegister),         "Status Register", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTB_OFFSET (AddressRegister),        "Address Register", 0},
    {ACPI_DMT_UINT32,   ACPI_HESTB_OFFSET (MiscRegister),           "Misc Register", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * HPET - High Precision Event Timer table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoHpet[] =
{
    {ACPI_DMT_UINT32,   ACPI_HPET_OFFSET (Id),                      "Hardware Block ID", 0},
    {ACPI_DMT_GAS,      ACPI_HPET_OFFSET (Address),                 "Timer Block Register", 0},
    {ACPI_DMT_UINT8,    ACPI_HPET_OFFSET (Sequence),                "Sequence Number", 0},
    {ACPI_DMT_UINT16,   ACPI_HPET_OFFSET (MinimumTick),             "Minimum Clock Ticks", 0},
    {ACPI_DMT_UINT8,    ACPI_HPET_OFFSET (Flags),                   "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_HPET_FLAG_OFFSET (Flags,0),            "4K Page Protect", 0},
    {ACPI_DMT_FLAG1,    ACPI_HPET_FLAG_OFFSET (Flags,0),            "64K Page Protect", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * IVRS - I/O Virtualization Reporting Structure
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoIvrs[] =
{
    {ACPI_DMT_UINT32,   ACPI_IVRS_OFFSET (Info),                    "Virtualization Info", 0},
    {ACPI_DMT_UINT64,   ACPI_IVRS_OFFSET (Reserved),                "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

/* Common Subtable header (one per Subtable) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoIvrsHdr[] =
{
    {ACPI_DMT_IVRS,     ACPI_IVRSH_OFFSET (Type),                   "Subtable Type", 0},
    {ACPI_DMT_UINT8,    ACPI_IVRSH_OFFSET (Flags),                  "Flags", 0},
    {ACPI_DMT_UINT16,   ACPI_IVRSH_OFFSET (Length),                 "Length", DT_LENGTH},
    {ACPI_DMT_UINT16,   ACPI_IVRSH_OFFSET (DeviceId),               "DeviceId", 0},
    ACPI_DMT_TERMINATOR
};

/* IVRS subtables */

/* 0x10: I/O Virtualization Hardware Definition (IVHD) Block */

ACPI_DMTABLE_INFO           AcpiDmTableInfoIvrs0[] =
{
    {ACPI_DMT_UINT16,   ACPI_IVRS0_OFFSET (CapabilityOffset),       "Capability Offset", 0},
    {ACPI_DMT_UINT64,   ACPI_IVRS0_OFFSET (BaseAddress),            "Base Address", 0},
    {ACPI_DMT_UINT16,   ACPI_IVRS0_OFFSET (PciSegmentGroup),        "PCI Segment Group", 0},
    {ACPI_DMT_UINT16,   ACPI_IVRS0_OFFSET (Info),                   "Virtualization Info", 0},
    {ACPI_DMT_UINT32,   ACPI_IVRS0_OFFSET (Reserved),               "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

/* 0x20, 0x21, 0x22: I/O Virtualization Memory Definition (IVMD) Block */

ACPI_DMTABLE_INFO           AcpiDmTableInfoIvrs1[] =
{
    {ACPI_DMT_UINT16,   ACPI_IVRS1_OFFSET (AuxData),                "Auxiliary Data", 0},
    {ACPI_DMT_UINT64,   ACPI_IVRS1_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT64,   ACPI_IVRS1_OFFSET (StartAddress),           "Start Address", 0},
    {ACPI_DMT_UINT64,   ACPI_IVRS1_OFFSET (MemoryLength),           "Memory Length", 0},
    ACPI_DMT_TERMINATOR
};

/* Device entry header for IVHD block */

#define ACPI_DMT_IVRS_DE_HEADER \
    {ACPI_DMT_UINT8,    ACPI_IVRSD_OFFSET (Type),                   "Entry Type", 0}, \
    {ACPI_DMT_UINT16,   ACPI_IVRSD_OFFSET (Id),                     "Device ID", 0}, \
    {ACPI_DMT_UINT8,    ACPI_IVRSD_OFFSET (DataSetting),            "Data Setting", 0}

/* 4-byte device entry */

ACPI_DMTABLE_INFO           AcpiDmTableInfoIvrs4[] =
{
    ACPI_DMT_IVRS_DE_HEADER,
    {ACPI_DMT_EXIT,     0,                                          NULL, 0},
};

/* 8-byte device entry */

ACPI_DMTABLE_INFO           AcpiDmTableInfoIvrs8a[] =
{
    ACPI_DMT_IVRS_DE_HEADER,
    {ACPI_DMT_UINT8,    ACPI_IVRS8A_OFFSET (Reserved1),             "Reserved", 0},
    {ACPI_DMT_UINT16,   ACPI_IVRS8A_OFFSET (UsedId),                "Source Used Device ID", 0},
    {ACPI_DMT_UINT8,    ACPI_IVRS8A_OFFSET (Reserved2),             "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

/* 8-byte device entry */

ACPI_DMTABLE_INFO           AcpiDmTableInfoIvrs8b[] =
{
    ACPI_DMT_IVRS_DE_HEADER,
    {ACPI_DMT_UINT32,   ACPI_IVRS8B_OFFSET (ExtendedData),          "Extended Data", 0},
    ACPI_DMT_TERMINATOR
};

/* 8-byte device entry */

ACPI_DMTABLE_INFO           AcpiDmTableInfoIvrs8c[] =
{
    ACPI_DMT_IVRS_DE_HEADER,
    {ACPI_DMT_UINT8,    ACPI_IVRS8C_OFFSET (Handle),                "Handle", 0},
    {ACPI_DMT_UINT16,   ACPI_IVRS8C_OFFSET (UsedId),                "Source Used Device ID", 0},
    {ACPI_DMT_UINT8,    ACPI_IVRS8C_OFFSET (Variety),               "Variety", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * MADT - Multiple APIC Description Table and subtables
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt[] =
{
    {ACPI_DMT_UINT32,   ACPI_MADT_OFFSET (Address),                 "Local Apic Address", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT_OFFSET (Flags),                   "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_MADT_FLAG_OFFSET (Flags,0),            "PC-AT Compatibility", 0},
    ACPI_DMT_TERMINATOR
};

/* Common Subtable header (one per Subtable) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadtHdr[] =
{
    {ACPI_DMT_MADT,     ACPI_MADTH_OFFSET (Type),                   "Subtable Type", 0},
    {ACPI_DMT_UINT8,    ACPI_MADTH_OFFSET (Length),                 "Length", DT_LENGTH},
    ACPI_DMT_TERMINATOR
};

/* MADT Subtables */

/* 0: processor APIC */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt0[] =
{
    {ACPI_DMT_UINT8,    ACPI_MADT0_OFFSET (ProcessorId),            "Processor ID", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT0_OFFSET (Id),                     "Local Apic ID", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT0_OFFSET (LapicFlags),             "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_MADT0_FLAG_OFFSET (LapicFlags,0),      "Processor Enabled", 0},
    ACPI_DMT_TERMINATOR
};

/* 1: IO APIC */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt1[] =
{
    {ACPI_DMT_UINT8,    ACPI_MADT1_OFFSET (Id),                     "I/O Apic ID", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT1_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT1_OFFSET (Address),                "Address", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT1_OFFSET (GlobalIrqBase),          "Interrupt", 0},
    ACPI_DMT_TERMINATOR
};

/* 2: Interrupt Override */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt2[] =
{
    {ACPI_DMT_UINT8,    ACPI_MADT2_OFFSET (Bus),                    "Bus", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT2_OFFSET (SourceIrq),              "Source", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT2_OFFSET (GlobalIrq),              "Interrupt", 0},
    {ACPI_DMT_UINT16,   ACPI_MADT2_OFFSET (IntiFlags),              "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAGS0,   ACPI_MADT2_FLAG_OFFSET (IntiFlags,0),       "Polarity", 0},
    {ACPI_DMT_FLAGS2,   ACPI_MADT2_FLAG_OFFSET (IntiFlags,0),       "Trigger Mode", 0},
    ACPI_DMT_TERMINATOR
};

/* 3: NMI Sources */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt3[] =
{
    {ACPI_DMT_UINT16,   ACPI_MADT3_OFFSET (IntiFlags),              "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAGS0,   ACPI_MADT3_FLAG_OFFSET (IntiFlags,0),       "Polarity", 0},
    {ACPI_DMT_FLAGS2,   ACPI_MADT3_FLAG_OFFSET (IntiFlags,0),       "Trigger Mode", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT3_OFFSET (GlobalIrq),              "Interrupt", 0},
    ACPI_DMT_TERMINATOR
};

/* 4: Local APIC NMI */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt4[] =
{
    {ACPI_DMT_UINT8,    ACPI_MADT4_OFFSET (ProcessorId),            "Processor ID", 0},
    {ACPI_DMT_UINT16,   ACPI_MADT4_OFFSET (IntiFlags),              "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAGS0,   ACPI_MADT4_FLAG_OFFSET (IntiFlags,0),       "Polarity", 0},
    {ACPI_DMT_FLAGS2,   ACPI_MADT4_FLAG_OFFSET (IntiFlags,0),       "Trigger Mode", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT4_OFFSET (Lint),                   "Interrupt Input LINT", 0},
    ACPI_DMT_TERMINATOR
};

/* 5: Address Override */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt5[] =
{
    {ACPI_DMT_UINT16,   ACPI_MADT5_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT64,   ACPI_MADT5_OFFSET (Address),                "APIC Address", 0},
    ACPI_DMT_TERMINATOR
};

/* 6: I/O Sapic */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt6[] =
{
    {ACPI_DMT_UINT8,    ACPI_MADT6_OFFSET (Id),                     "I/O Sapic ID", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT6_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT6_OFFSET (GlobalIrqBase),          "Interrupt Base", 0},
    {ACPI_DMT_UINT64,   ACPI_MADT6_OFFSET (Address),                "Address", 0},
    ACPI_DMT_TERMINATOR
};

/* 7: Local Sapic */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt7[] =
{
    {ACPI_DMT_UINT8,    ACPI_MADT7_OFFSET (ProcessorId),            "Processor ID", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT7_OFFSET (Id),                     "Local Sapic ID", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT7_OFFSET (Eid),                    "Local Sapic EID", 0},
    {ACPI_DMT_UINT24,   ACPI_MADT7_OFFSET (Reserved[0]),            "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT7_OFFSET (LapicFlags),             "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_MADT7_FLAG_OFFSET (LapicFlags,0),      "Processor Enabled", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT7_OFFSET (Uid),                    "Processor UID", 0},
    {ACPI_DMT_STRING,   ACPI_MADT7_OFFSET (UidString[0]),           "Processor UID String", 0},
    ACPI_DMT_TERMINATOR
};

/* 8: Platform Interrupt Source */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt8[] =
{
    {ACPI_DMT_UINT16,   ACPI_MADT8_OFFSET (IntiFlags),              "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAGS0,   ACPI_MADT8_FLAG_OFFSET (IntiFlags,0),       "Polarity", 0},
    {ACPI_DMT_FLAGS2,   ACPI_MADT8_FLAG_OFFSET (IntiFlags,0),       "Trigger Mode", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT8_OFFSET (Type),                   "InterruptType", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT8_OFFSET (Id),                     "Processor ID", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT8_OFFSET (Eid),                    "Processor EID", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT8_OFFSET (IoSapicVector),          "I/O Sapic Vector", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT8_OFFSET (GlobalIrq),              "Interrupt", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT8_OFFSET (Flags),                  "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_MADT8_OFFSET (Flags),                  "CPEI Override", 0},
    ACPI_DMT_TERMINATOR
};

/* 9: Processor Local X2_APIC (ACPI 4.0) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt9[] =
{
    {ACPI_DMT_UINT16,   ACPI_MADT9_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT9_OFFSET (LocalApicId),            "Processor x2Apic ID", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT9_OFFSET (LapicFlags),             "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_MADT9_FLAG_OFFSET (LapicFlags,0),      "Processor Enabled", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT9_OFFSET (Uid),                    "Processor UID", 0},
    ACPI_DMT_TERMINATOR
};

/* 10: Local X2_APIC NMI (ACPI 4.0) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMadt10[] =
{
    {ACPI_DMT_UINT16,   ACPI_MADT10_OFFSET (IntiFlags),             "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAGS0,   ACPI_MADT10_FLAG_OFFSET (IntiFlags,0),      "Polarity", 0},
    {ACPI_DMT_FLAGS2,   ACPI_MADT10_FLAG_OFFSET (IntiFlags,0),      "Trigger Mode", 0},
    {ACPI_DMT_UINT32,   ACPI_MADT10_OFFSET (Uid),                   "Processor UID", 0},
    {ACPI_DMT_UINT8,    ACPI_MADT10_OFFSET (Lint),                  "Interrupt Input LINT", 0},
    {ACPI_DMT_UINT24,   ACPI_MADT10_OFFSET (Reserved[0]),           "Reserved", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * MCFG - PCI Memory Mapped Configuration table and Subtable
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoMcfg[] =
{
    {ACPI_DMT_UINT64,   ACPI_MCFG_OFFSET (Reserved[0]),             "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

ACPI_DMTABLE_INFO           AcpiDmTableInfoMcfg0[] =
{
    {ACPI_DMT_UINT64,   ACPI_MCFG0_OFFSET (Address),                "Base Address", 0},
    {ACPI_DMT_UINT16,   ACPI_MCFG0_OFFSET (PciSegment),             "Segment Group Number", 0},
    {ACPI_DMT_UINT8,    ACPI_MCFG0_OFFSET (StartBusNumber),         "Start Bus Number", 0},
    {ACPI_DMT_UINT8,    ACPI_MCFG0_OFFSET (EndBusNumber),           "End Bus Number", 0},
    {ACPI_DMT_UINT32,   ACPI_MCFG0_OFFSET (Reserved),               "Reserved", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * MCHI - Management Controller Host Interface table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoMchi[] =
{
    {ACPI_DMT_UINT8,    ACPI_MCHI_OFFSET (InterfaceType),           "Interface Type", 0},
    {ACPI_DMT_UINT8,    ACPI_MCHI_OFFSET (Protocol),                "Protocol", 0},
    {ACPI_DMT_UINT64,   ACPI_MCHI_OFFSET (ProtocolData),            "Protocol Data", 0},
    {ACPI_DMT_UINT8,    ACPI_MCHI_OFFSET (InterruptType),           "Interrupt Type", 0},
    {ACPI_DMT_UINT8,    ACPI_MCHI_OFFSET (Gpe),                     "Gpe", 0},
    {ACPI_DMT_UINT8,    ACPI_MCHI_OFFSET (PciDeviceFlag),           "Pci Device Flag", 0},
    {ACPI_DMT_UINT32,   ACPI_MCHI_OFFSET (GlobalInterrupt),         "Global Interrupt", 0},
    {ACPI_DMT_GAS,      ACPI_MCHI_OFFSET (ControlRegister),         "Control Register", 0},
    {ACPI_DMT_UINT8,    ACPI_MCHI_OFFSET (PciSegment),              "Pci Segment", 0},
    {ACPI_DMT_UINT8,    ACPI_MCHI_OFFSET (PciBus),                  "Pci Bus", 0},
    {ACPI_DMT_UINT8,    ACPI_MCHI_OFFSET (PciDevice),               "Pci Device", 0},
    {ACPI_DMT_UINT8,    ACPI_MCHI_OFFSET (PciFunction),             "Pci Function", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * MSCT - Maximum System Characteristics Table (ACPI 4.0)
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoMsct[] =
{
    {ACPI_DMT_UINT32,   ACPI_MSCT_OFFSET (ProximityOffset),         "Proximity Offset", 0},
    {ACPI_DMT_UINT32,   ACPI_MSCT_OFFSET (MaxProximityDomains),     "Max Proximity Domains", 0},
    {ACPI_DMT_UINT32,   ACPI_MSCT_OFFSET (MaxClockDomains),         "Max Clock Domains", 0},
    {ACPI_DMT_UINT64,   ACPI_MSCT_OFFSET (MaxAddress),              "Max Physical Address", 0},
    ACPI_DMT_TERMINATOR
};

/* Subtable - Maximum Proximity Domain Information. Version 1 */

ACPI_DMTABLE_INFO           AcpiDmTableInfoMsct0[] =
{
    {ACPI_DMT_UINT8,    ACPI_MSCT0_OFFSET (Revision),               "Revision", 0},
    {ACPI_DMT_UINT8,    ACPI_MSCT0_OFFSET (Length),                 "Length", DT_LENGTH},
    {ACPI_DMT_UINT32,   ACPI_MSCT0_OFFSET (RangeStart),             "Domain Range Start", 0},
    {ACPI_DMT_UINT32,   ACPI_MSCT0_OFFSET (RangeEnd),               "Domain Range End", 0},
    {ACPI_DMT_UINT32,   ACPI_MSCT0_OFFSET (ProcessorCapacity),      "Processor Capacity", 0},
    {ACPI_DMT_UINT64,   ACPI_MSCT0_OFFSET (MemoryCapacity),         "Memory Capacity", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * SBST - Smart Battery Specification Table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoSbst[] =
{
    {ACPI_DMT_UINT32,   ACPI_SBST_OFFSET (WarningLevel),            "Warning Level", 0},
    {ACPI_DMT_UINT32,   ACPI_SBST_OFFSET (LowLevel),                "Low Level", 0},
    {ACPI_DMT_UINT32,   ACPI_SBST_OFFSET (CriticalLevel),           "Critical Level", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * SLIC - Software Licensing Description Table. NOT FULLY IMPLEMENTED, do not
 * have the table definition.
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoSlic[] =
{
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * SLIT - System Locality Information Table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoSlit[] =
{
    {ACPI_DMT_UINT64,   ACPI_SLIT_OFFSET (LocalityCount),          "Localities", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * SPCR - Serial Port Console Redirection table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoSpcr[] =
{
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (InterfaceType),           "Interface Type", 0},
    {ACPI_DMT_UINT24,   ACPI_SPCR_OFFSET (Reserved[0]),             "Reserved", 0},
    {ACPI_DMT_GAS,      ACPI_SPCR_OFFSET (SerialPort),              "Serial Port Register", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (InterruptType),           "Interrupt Type", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (PcInterrupt),             "PCAT-compatible IRQ", 0},
    {ACPI_DMT_UINT32,   ACPI_SPCR_OFFSET (Interrupt),               "Interrupt", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (BaudRate),                "Baud Rate", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (Parity),                  "Parity", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (StopBits),                "Stop Bits", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (FlowControl),             "Flow Control", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (TerminalType),            "Terminal Type", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (Reserved2),               "Reserved", 0},
    {ACPI_DMT_UINT16,   ACPI_SPCR_OFFSET (PciDeviceId),             "PCI Device ID", 0},
    {ACPI_DMT_UINT16,   ACPI_SPCR_OFFSET (PciVendorId),             "PCI Vendor ID", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (PciBus),                  "PCI Bus", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (PciDevice),               "PCI Device", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (PciFunction),             "PCI Function", 0},
    {ACPI_DMT_UINT32,   ACPI_SPCR_OFFSET (PciFlags),                "PCI Flags", 0},
    {ACPI_DMT_UINT8,    ACPI_SPCR_OFFSET (PciSegment),              "PCI Segment", 0},
    {ACPI_DMT_UINT32,   ACPI_SPCR_OFFSET (Reserved2),               "Reserved", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * SPMI - Server Platform Management Interface table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoSpmi[] =
{
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (InterfaceType),           "Interface Type", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (Reserved),                "Reserved", 0},
    {ACPI_DMT_UINT16,   ACPI_SPMI_OFFSET (SpecRevision),            "IPMI Spec Version", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (InterruptType),           "Interrupt Type", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (GpeNumber),               "GPE Number", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (Reserved1),               "Reserved", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (PciDeviceFlag),           "PCI Device Flag", 0},
    {ACPI_DMT_UINT32,   ACPI_SPMI_OFFSET (Interrupt),               "Interrupt", 0},
    {ACPI_DMT_GAS,      ACPI_SPMI_OFFSET (IpmiRegister),            "IPMI Register", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (PciSegment),              "PCI Segment", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (PciBus),                  "PCI Bus", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (PciDevice),               "PCI Device", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (PciFunction),             "PCI Function", 0},
    {ACPI_DMT_UINT8,    ACPI_SPMI_OFFSET (Reserved2),               "Reserved", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * SRAT - System Resource Affinity Table and Subtables
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoSrat[] =
{
    {ACPI_DMT_UINT32,   ACPI_SRAT_OFFSET (TableRevision),           "Table Revision", 0},
    {ACPI_DMT_UINT64,   ACPI_SRAT_OFFSET (Reserved),                "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

/* Common Subtable header (one per Subtable) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoSratHdr[] =
{
    {ACPI_DMT_SRAT,     ACPI_SRATH_OFFSET (Type),                   "Subtable Type", 0},
    {ACPI_DMT_UINT8,    ACPI_SRATH_OFFSET (Length),                 "Length", DT_LENGTH},
    ACPI_DMT_TERMINATOR
};

/* SRAT Subtables */

/* 0: Processor Local APIC/SAPIC Affinity */

ACPI_DMTABLE_INFO           AcpiDmTableInfoSrat0[] =
{
    {ACPI_DMT_UINT8,    ACPI_SRAT0_OFFSET (ProximityDomainLo),      "Proximity Domain Low(8)", 0},
    {ACPI_DMT_UINT8,    ACPI_SRAT0_OFFSET (ApicId),                 "Apic ID", 0},
    {ACPI_DMT_UINT32,   ACPI_SRAT0_OFFSET (Flags),                  "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_SRAT0_FLAG_OFFSET (Flags,0),           "Enabled", 0},
    {ACPI_DMT_UINT8,    ACPI_SRAT0_OFFSET (LocalSapicEid),          "Local Sapic EID", 0},
    {ACPI_DMT_UINT24,   ACPI_SRAT0_OFFSET (ProximityDomainHi[0]),   "Proximity Domain High(24)", 0},
    {ACPI_DMT_UINT32,   ACPI_SRAT0_OFFSET (Reserved),               "Reserved", 0},
    ACPI_DMT_TERMINATOR
};

/* 1: Memory Affinity */

ACPI_DMTABLE_INFO           AcpiDmTableInfoSrat1[] =
{
    {ACPI_DMT_UINT32,   ACPI_SRAT1_OFFSET (ProximityDomain),        "Proximity Domain", 0},
    {ACPI_DMT_UINT16,   ACPI_SRAT1_OFFSET (Reserved),               "Reserved1", 0},
    {ACPI_DMT_UINT64,   ACPI_SRAT1_OFFSET (BaseAddress),            "Base Address", 0},
    {ACPI_DMT_UINT64,   ACPI_SRAT1_OFFSET (Length),                 "Address Length", 0},
    {ACPI_DMT_UINT32,   ACPI_SRAT1_OFFSET (Reserved1),              "Reserved2", 0},
    {ACPI_DMT_UINT32,   ACPI_SRAT1_OFFSET (Flags),                  "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_SRAT1_FLAG_OFFSET (Flags,0),           "Enabled", 0},
    {ACPI_DMT_FLAG1,    ACPI_SRAT1_FLAG_OFFSET (Flags,0),           "Hot Pluggable", 0},
    {ACPI_DMT_FLAG2,    ACPI_SRAT1_FLAG_OFFSET (Flags,0),           "Non-Volatile", 0},
    {ACPI_DMT_UINT64,   ACPI_SRAT1_OFFSET (Reserved2),              "Reserved3", 0},
    ACPI_DMT_TERMINATOR
};

/* 2: Processor Local X2_APIC Affinity (ACPI 4.0) */

ACPI_DMTABLE_INFO           AcpiDmTableInfoSrat2[] =
{
    {ACPI_DMT_UINT16,   ACPI_SRAT2_OFFSET (Reserved),               "Reserved1", 0},
    {ACPI_DMT_UINT32,   ACPI_SRAT2_OFFSET (ProximityDomain),        "Proximity Domain", 0},
    {ACPI_DMT_UINT32,   ACPI_SRAT2_OFFSET (ApicId),                 "Apic ID", 0},
    {ACPI_DMT_UINT32,   ACPI_SRAT2_OFFSET (Flags),                  "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_SRAT2_FLAG_OFFSET (Flags,0),           "Enabled", 0},
    {ACPI_DMT_UINT32,   ACPI_SRAT2_OFFSET (ClockDomain),            "Clock Domain", 0},
    {ACPI_DMT_UINT32,   ACPI_SRAT2_OFFSET (Reserved2),              "Reserved2", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * TCPA - Trusted Computing Platform Alliance table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoTcpa[] =
{
    {ACPI_DMT_UINT16,   ACPI_TCPA_OFFSET (Reserved),                "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_TCPA_OFFSET (MaxLogLength),            "Max Event Log Length", 0},
    {ACPI_DMT_UINT64,   ACPI_TCPA_OFFSET (LogAddress),              "Event Log Address", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * UEFI - UEFI Boot optimization Table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoUefi[] =
{
    {ACPI_DMT_BUF16,    ACPI_UEFI_OFFSET (Identifier[0]),           "UUID Identifier", 0},
    {ACPI_DMT_UINT16,   ACPI_UEFI_OFFSET (DataOffset),              "Data Offset", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * WAET - Windows ACPI Emulated devices Table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoWaet[] =
{
    {ACPI_DMT_UINT32,   ACPI_WAET_OFFSET (Flags),                   "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_WAET_OFFSET (Flags),                   "RTC needs no INT ack", 0},
    {ACPI_DMT_FLAG1,    ACPI_WAET_OFFSET (Flags),                   "PM timer, one read only", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * WDAT - Watchdog Action Table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoWdat[] =
{
    {ACPI_DMT_UINT32,   ACPI_WDAT_OFFSET (HeaderLength),            "Header Length", DT_LENGTH},
    {ACPI_DMT_UINT16,   ACPI_WDAT_OFFSET (PciSegment),              "PCI Segment", 0},
    {ACPI_DMT_UINT8,    ACPI_WDAT_OFFSET (PciBus),                  "PCI Bus", 0},
    {ACPI_DMT_UINT8,    ACPI_WDAT_OFFSET (PciDevice),               "PCI Device", 0},
    {ACPI_DMT_UINT8,    ACPI_WDAT_OFFSET (PciFunction),             "PCI Function", 0},
    {ACPI_DMT_UINT24,   ACPI_WDAT_OFFSET (Reserved[0]),             "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_WDAT_OFFSET (TimerPeriod),             "Timer Period", 0},
    {ACPI_DMT_UINT32,   ACPI_WDAT_OFFSET (MaxCount),                "Max Count", 0},
    {ACPI_DMT_UINT32,   ACPI_WDAT_OFFSET (MinCount),                "Min Count", 0},
    {ACPI_DMT_UINT8,    ACPI_WDAT_OFFSET (Flags),                   "Flags (decoded below)", DT_FLAG},
    {ACPI_DMT_FLAG0,    ACPI_WDAT_OFFSET (Flags),                   "Enabled", 0},
    {ACPI_DMT_FLAG7,    ACPI_WDAT_OFFSET (Flags),                   "Stopped When Asleep", 0},
    {ACPI_DMT_UINT24,   ACPI_WDAT_OFFSET (Reserved2[0]),            "Reserved", 0},
    {ACPI_DMT_UINT32,   ACPI_WDAT_OFFSET (Entries),                 "Watchdog Entry Count", 0},
    ACPI_DMT_TERMINATOR
};

/* WDAT Subtables - Watchdog Instruction Entries */

ACPI_DMTABLE_INFO           AcpiDmTableInfoWdat0[] =
{
    {ACPI_DMT_UINT8,    ACPI_WDAT0_OFFSET (Action),                 "Watchdog Action", 0},
    {ACPI_DMT_UINT8,    ACPI_WDAT0_OFFSET (Instruction),            "Instruction", 0},
    {ACPI_DMT_UINT16,   ACPI_WDAT0_OFFSET (Reserved),               "Reserved", 0},
    {ACPI_DMT_GAS,      ACPI_WDAT0_OFFSET (RegisterRegion),         "Register Region", 0},
    {ACPI_DMT_UINT32,   ACPI_WDAT0_OFFSET (Value),                  "Value", 0},
    {ACPI_DMT_UINT32,   ACPI_WDAT0_OFFSET (Mask),                   "Register Mask", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * WDDT - Watchdog Description Table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoWddt[] =
{
    {ACPI_DMT_UINT16,   ACPI_WDDT_OFFSET (SpecVersion),             "Specification Version", 0},
    {ACPI_DMT_UINT16,   ACPI_WDDT_OFFSET (TableVersion),            "Table Version", 0},
    {ACPI_DMT_UINT16,   ACPI_WDDT_OFFSET (PciVendorId),             "PCI Vendor ID", 0},
    {ACPI_DMT_GAS,      ACPI_WDDT_OFFSET (Address),                 "Timer Register", 0},
    {ACPI_DMT_UINT16,   ACPI_WDDT_OFFSET (MaxCount),                "Max Count", 0},
    {ACPI_DMT_UINT16,   ACPI_WDDT_OFFSET (MinCount),                "Min Count", 0},
    {ACPI_DMT_UINT16,   ACPI_WDDT_OFFSET (Period),                  "Period", 0},
    {ACPI_DMT_UINT16,   ACPI_WDDT_OFFSET (Status),                  "Status (decoded below)", 0},

    /* Status Flags byte 0 */

    {ACPI_DMT_FLAG0,    ACPI_WDDT_FLAG_OFFSET (Status,0),           "Available", 0},
    {ACPI_DMT_FLAG1,    ACPI_WDDT_FLAG_OFFSET (Status,0),           "Active", 0},
    {ACPI_DMT_FLAG2,    ACPI_WDDT_FLAG_OFFSET (Status,0),           "OS Owns", 0},

    /* Status Flags byte 1 */

    {ACPI_DMT_FLAG3,    ACPI_WDDT_FLAG_OFFSET (Status,1),           "User Reset", 0},
    {ACPI_DMT_FLAG4,    ACPI_WDDT_FLAG_OFFSET (Status,1),           "Timeout Reset", 0},
    {ACPI_DMT_FLAG5,    ACPI_WDDT_FLAG_OFFSET (Status,1),           "Power Fail Reset", 0},
    {ACPI_DMT_FLAG6,    ACPI_WDDT_FLAG_OFFSET (Status,1),           "Unknown Reset", 0},

    {ACPI_DMT_UINT16,   ACPI_WDDT_OFFSET (Capability),              "Capability (decoded below)", 0},

    /* Capability Flags byte 0 */

    {ACPI_DMT_FLAG0,    ACPI_WDDT_FLAG_OFFSET (Capability,0),       "Auto Reset", 0},
    {ACPI_DMT_FLAG1,    ACPI_WDDT_FLAG_OFFSET (Capability,0),       "Timeout Alert", 0},
    ACPI_DMT_TERMINATOR
};


/*******************************************************************************
 *
 * WDRT - Watchdog Resource Table
 *
 ******************************************************************************/

ACPI_DMTABLE_INFO           AcpiDmTableInfoWdrt[] =
{
    {ACPI_DMT_GAS,      ACPI_WDRT_OFFSET (ControlRegister),         "Control Register", 0},
    {ACPI_DMT_GAS,      ACPI_WDRT_OFFSET (CountRegister),           "Count Register", 0},
    {ACPI_DMT_UINT16,   ACPI_WDRT_OFFSET (PciDeviceId),             "PCI Device ID", 0},
    {ACPI_DMT_UINT16,   ACPI_WDRT_OFFSET (PciVendorId),             "PCI Vendor ID", 0},
    {ACPI_DMT_UINT8,    ACPI_WDRT_OFFSET (PciBus),                  "PCI Bus", 0},
    {ACPI_DMT_UINT8,    ACPI_WDRT_OFFSET (PciDevice),               "PCI Device", 0},
    {ACPI_DMT_UINT8,    ACPI_WDRT_OFFSET (PciFunction),             "PCI Function", 0},
    {ACPI_DMT_UINT8,    ACPI_WDRT_OFFSET (PciSegment),              "PCI Segment", 0},
    {ACPI_DMT_UINT16,   ACPI_WDRT_OFFSET (MaxCount),                "Max Count", 0},
    {ACPI_DMT_UINT8,    ACPI_WDRT_OFFSET (Units),                   "Counter Units", 0},
    ACPI_DMT_TERMINATOR
};
