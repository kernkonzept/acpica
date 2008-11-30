#include <l4/sys/compiler.h>

__BEGIN_DECLS
#include "acpi.h"
#include "acpiosxf.h"
__END_DECLS

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#include <l4/util/util.h>
#if defined(ARCH_amd64) || defined(ARCH_x86)
#include <l4/util/port_io.h>
#endif

#include <l4/re/env>
#include <l4/re/rm>
#include <l4/io/io.h>

#include <l4/re/util/cap_alloc>

#include <l4/sigma0/sigma0.h>

#define DEBUG_OSL_PORT_IO 0

static l4_addr_t map_area;
static L4::Cap<void> sigma0_cap;

static int init_map_area()
{
  if (!map_area)
    {
      map_area = 0x80000000;
      int r = L4Re::Env::env()->rm()->reserve_area(&map_area, 0x400000);
      if (r < 0)
	return r;

      sigma0_cap = L4Re::Util::cap_alloc.alloc<void>();
      L4Re::Env::env()->names()->query("sigma0", sigma0_cap);

      printf("ACPICA: reserved map region: %lx\n", map_area);
    }
  return 0;
}

ACPI_STATUS
AcpiOsInitialize (void)
{
  return AE_OK;
}

ACPI_STATUS
AcpiOsTerminate (void)
{
  return AE_OK;
}

void *
AcpiOsAllocate (ACPI_SIZE size)
{
  return malloc(size);
}

void
AcpiOsFree (void * memory)
{
  free(memory);
  return;
}

ACPI_STATUS
AcpiOsInstallInterruptHandler (
	uint32_t                        /* interrupt_number */,
	ACPI_OSD_HANDLER                /* service_routine */,
	void                            * /*context*/)
{
  printf("%s:%d:%s: UNINPLEMENTED\n", __FILE__, __LINE__, __func__);
  return !AE_OK;
}

ACPI_STATUS
AcpiOsRemoveInterruptHandler (
	uint32_t                        /* interrupt_number */,
	ACPI_OSD_HANDLER                /* service_routine */)
{
  printf("%s:%d:%s: UNINPLEMENTED\n", __FILE__, __LINE__, __func__);
  return !AE_OK;
}

ACPI_STATUS
AcpiOsExecute (
	ACPI_EXECUTE_TYPE                /*type */,
	ACPI_OSD_EXEC_CALLBACK           /*function */,
	void                            * /*context */)
{
  printf("%s:%d:%s: UNINPLEMENTED\n", __FILE__, __LINE__, __func__);
  return !AE_OK;
}

void
AcpiOsSleep (ACPI_INTEGER milliseconds)
{
  l4_sleep(milliseconds);
}

void
AcpiOsStall (uint32_t microseconds)
{
  l4_usleep(microseconds);
}


#if defined(ARCH_amd64) || defined(ARCH_x86)
static l4_umword_t iobitmap[0x10000 / L4_MWORD_BITS];

static l4_umword_t get_iobit(unsigned port)
{
  return iobitmap[port / L4_MWORD_BITS] & (1UL << (port % L4_MWORD_BITS));
}

static void set_iobit(unsigned port)
{
  iobitmap[port / L4_MWORD_BITS] |= (1UL << (port % L4_MWORD_BITS));
}

static
int map_ioport(unsigned port, unsigned size)
{
  unsigned i;
  for (i = 0; i < size; ++i)
    {
      if (!get_iobit(port + i))
	{
	  int err = l4io_request_ioport(port + i, 1);
	  if (err < 0)
	    return err;

	  set_iobit(port + i);
	}
    }
  return 0;
}

/*
 * Platform and hardware-independent I/O interfaces
 */
ACPI_STATUS
AcpiOsReadPort (
	ACPI_IO_ADDRESS                 address,
	uint32_t                       *value,
	uint32_t                        width)
{
  if(0 && DEBUG_OSL_PORT_IO)
    printf("IN: adr=0x%x, width=%i\n", address, width);

  if (address == 0x80)
    return AE_OK;

  if (map_ioport(address, width / 8) < 0)
    return AE_BAD_PARAMETER;

  switch(width)
    {
    case 8:
      *value = l4util_in8((l4_uint16_t)address);
      break;
    case 16:
      *value = l4util_in16((l4_uint16_t)address);
      break;
    case 32:
      *value = l4util_in32((l4_uint16_t)address);
      break;
    default :
      return AE_BAD_PARAMETER;
    }
  if(DEBUG_OSL_PORT_IO)
    printf("\tport(0x%x)=>0x%x\n",address,*value);
  return AE_OK;
}

ACPI_STATUS
AcpiOsWritePort (
	ACPI_IO_ADDRESS                 address,
	uint32_t                        value,
	uint32_t                        width)
{
  if(DEBUG_OSL_PORT_IO)
    printf("\tport(0x%x)<=0x%x\n",address,value);

  if (address == 0x80)
    return AE_OK;

  if (map_ioport(address, width / 8) < 0)
    return AE_BAD_PARAMETER;

  switch(width)
    {
    case 8:
      l4util_out8((l4_uint8_t)value,(l4_uint16_t)address);
      break;
    case 16:
      l4util_out16((l4_uint16_t)value,(l4_uint16_t)address);
      break;
    case 32:
      l4util_out32((l4_uint32_t)value,(l4_uint32_t)address);
      break;
    default :
      return AE_BAD_PARAMETER;
    }
  return AE_OK;
}
#else

ACPI_STATUS
AcpiOsReadPort (
	ACPI_IO_ADDRESS                 /*address*/,
	uint32_t                       * /*value*/,
	uint32_t                        /*width*/)
{
  return AE_NO_MEMORY;
}

ACPI_STATUS
AcpiOsWritePort (
	ACPI_IO_ADDRESS                 /*address*/,
	uint32_t                        /*value*/,
	uint32_t                        /*width*/)
{
  return AE_NO_MEMORY;
}
#endif

/*
 * Platform and hardware-independent physical memory interfaces
 */
ACPI_STATUS
AcpiOsReadMemory (
	ACPI_PHYSICAL_ADDRESS           /*address*/,
	uint32_t                             * /*value*/,
	uint32_t                             /*width*/)
{
  printf("%s:%d:%s: UNINPLEMENTED\n", __FILE__, __LINE__, __func__);
  return !AE_OK;
}

ACPI_STATUS
AcpiOsWriteMemory (
	ACPI_PHYSICAL_ADDRESS            /*address */,
	uint32_t                            /*  value */,
	uint32_t                             /* width */)
{
  printf("%s:%d:%s: UNINPLEMENTED\n", __FILE__, __LINE__, __func__);
  return !AE_OK;
}


void *
AcpiOsMapMemory (
	ACPI_PHYSICAL_ADDRESS           where,
	ACPI_SIZE                       length)
{
  init_map_area();
  //LOG("where=%8.8x length=%x",where,length);
  int i;
  /* calc page-aligned pointers */
  l4_addr_t page_of_where = l4_trunc_page(where);
  length = l4_round_page(length+where-page_of_where);
  l4_addr_t virt = map_area;
  map_area += l4_round_page(length);

  i = l4sigma0_map_iomem(sigma0_cap.cap(), page_of_where, virt, length, 0);

  if (i < 0)
    {
      printf("DAMN\n");
      return 0;
    }

  printf("%s(%x, %x) = %lx\n", __func__, where, length, virt + (where - page_of_where));

  return (void*)(virt + (where - page_of_where));
}

void
AcpiOsUnmapMemory (
	void                            *logical_address,
	ACPI_SIZE                       size)
{
  l4io_release_iomem((l4_addr_t)logical_address, size);
  return;
}

/******************************************************************************
 *
 * FUNCTION:    AcpiOsReadPciConfiguration
 *
 * PARAMETERS:  PciId               Seg/Bus/Dev
 *              Register            Device Register
 *              Value               Buffer where value is placed
 *              Width               Number of bits
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read data from PCI configuration space
 *
 *****************************************************************************/

extern inline
l4_uint32_t
pci_conf_addr(l4_uint32_t bus, l4_uint32_t dev, l4_uint32_t fn, l4_uint32_t reg)
{ return 0x80000000 | (bus << 16) | (dev << 11) | (fn << 8) | (reg & ~3); }

ACPI_STATUS
AcpiOsReadPciConfiguration (
    ACPI_PCI_ID             *PciId,
    UINT32                  Register,
    void                    *Value,
    UINT32                  Width)
{
  //printf("%s: ...\n", __func__);

  AcpiOsWritePort(0xcf8, pci_conf_addr(PciId->Bus, PciId->Device, PciId->Function, Register), 32);

  return AcpiOsReadPort(0xcfc + (Register & 3), (uint32_t*)Value, Width);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsWritePciConfiguration
 *
 * PARAMETERS:  PciId               Seg/Bus/Dev
 *              Register            Device Register
 *              Value               Value to be written
 *              Width               Number of bits
 *
 * RETURN:      Status.
 *
 * DESCRIPTION: Write data to PCI configuration space
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsWritePciConfiguration (
    ACPI_PCI_ID             * /*PciId*/,
    UINT32                   /*Register*/,
    ACPI_INTEGER             /*Value*/,
    UINT32                   /*Width*/)
{
  //printf("%s: ...\n", __func__);

  return (AE_OK);
}

/******************************************************************************
 *
 * FUNCTION:    AcpiOsPredefinedOverride
 *
 * PARAMETERS:  InitVal     - Initial value of the predefined object
 *              NewVal      - The new value for the object
 *
 * RETURN:      Status, pointer to value.  Null pointer returned if not
 *              overriding.
 *
 * DESCRIPTION: Allow the OS to override predefined names
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsPredefinedOverride (
    const ACPI_PREDEFINED_NAMES *InitVal,
    ACPI_STRING                 *NewVal)
{

    if (!InitVal || !NewVal)
    {
        return (AE_BAD_PARAMETER);
    }

    *NewVal = NULL;
    return (AE_OK);
}


/******************************************************************************
 *
 * FUNCTION:    AcpiOsTableOverride
 *
 * PARAMETERS:  ExistingTable   - Header of current table (probably firmware)
 *              NewTable        - Where an entire new table is returned.
 *
 * RETURN:      Status, pointer to new table.  Null pointer returned if no
 *              table is available to override
 *
 * DESCRIPTION: Return a different version of a table if one is available
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsTableOverride (
    ACPI_TABLE_HEADER       *ExistingTable,
    ACPI_TABLE_HEADER       **NewTable)
{

    if (!ExistingTable || !NewTable)
    {
        return (AE_BAD_PARAMETER);
    }

    *NewTable = NULL;

#ifdef ACPI_EXEC_APP

    /* This code exercises the table override mechanism in the core */

    if (ACPI_COMPARE_NAME (ExistingTable->Signature, ACPI_SIG_DSDT))
    {
        /* override DSDT with itself */

        *NewTable = AcpiGbl_DbTablePtr;
    }
    return (AE_OK);
#else
    return AE_NO_ACPI_TABLES;
#endif
}

/*
 * ACPI Table interfaces
 */
ACPI_PHYSICAL_ADDRESS
AcpiOsGetRootPointer (void)
{
  ACPI_SIZE table_address = 0;
  printf("Find root Pointer\n");
  AcpiFindRootPointer(&table_address);
  printf("Find root Pointer: %x\n", table_address);
  return (ACPI_PHYSICAL_ADDRESS)table_address;
}

/******************************************************************************
 *
 * FUNCTION:    AcpiOsValidateInterface
 *
 * PARAMETERS:  Interface           - Requested interface to be validated
 *
 * RETURN:      AE_OK if interface is supported, AE_SUPPORT otherwise
 *
 * DESCRIPTION: Match an interface string to the interfaces supported by the
 *              host. Strings originate from an AML call to the _OSI method.
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsValidateInterface (
    char                    * /*Interface*/)
{

    return (AE_SUPPORT);
}


/* TEMPORARY STUB FUNCTION */
void
AcpiOsDerivePciId(
    ACPI_HANDLE             /* rhandle */,
    ACPI_HANDLE             /* chandle */,
    ACPI_PCI_ID             ** /* PciId */)
{

}

/******************************************************************************
 *
 * FUNCTION:    AcpiOsValidateAddress
 *
 * PARAMETERS:  SpaceId             - ACPI space ID
 *              Address             - Physical address
 *              Length              - Address length
 *
 * RETURN:      AE_OK if Address/Length is valid for the SpaceId. Otherwise,
 *              should return AE_AML_ILLEGAL_ADDRESS.
 *
 * DESCRIPTION: Validate a system address via the host OS. Used to validate
 *              the addresses accessed by AML operation regions.
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsValidateAddress (
    UINT8                   /* SpaceId */,
    ACPI_PHYSICAL_ADDRESS   /* Address */,
    ACPI_SIZE               /* Length */)
{

    return (AE_OK);
}




/******************************************************************************
 *
 * FUNCTION:    AcpiOsSignal
 *
 * PARAMETERS:  Function            ACPI CA signal function code
 *              Info                Pointer to function-dependent structure
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Miscellaneous functions
 *
 *****************************************************************************/

ACPI_STATUS
AcpiOsSignal (
    UINT32                  Function,
    void                    *Info)
{

    switch (Function)
    {
    case ACPI_SIGNAL_FATAL:
        break;

    case ACPI_SIGNAL_BREAKPOINT:

        if (Info)
        {
            AcpiOsPrintf ("AcpiOsBreakpoint: %s ****\n", Info);
        }
        else
        {
            AcpiOsPrintf ("At AcpiOsBreakpoint ****\n");
        }

        break;
    }


    return (AE_OK);
}

/******************************************************************************
 *
 * FUNCTION:    AcpiOsGetTimer
 *
 * PARAMETERS:  None
 *
 * RETURN:      Current time in 100 nanosecond units
 *
 * DESCRIPTION: Get the current system time
 *
 *****************************************************************************/

UINT64
AcpiOsGetTimer (void)
{
    struct timeval  time;

    gettimeofday(&time, NULL);

    /* Seconds * 10^7 = 100ns(10^-7), Microseconds(10^-6) * 10^1 = 100ns */

    return (((UINT64) time.tv_sec * 10000000) + ((UINT64) time.tv_usec * 10));
}


