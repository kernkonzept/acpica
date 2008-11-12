
#include <cstdio>
#include <cstdlib>

#include "acpi_l4.h"

extern "C" {
#include "acpi.h"
#include "accommon.h"
#include "acresrc.h"
}

#define _COMPONENT		ACPI_BUS_COMPONENT
ACPI_MODULE_NAME("l4main");

enum Acpi_irq_model_id {
	ACPI_IRQ_MODEL_PIC = 0,
	ACPI_IRQ_MODEL_IOAPIC,
	ACPI_IRQ_MODEL_IOSAPIC,
	ACPI_IRQ_MODEL_PLATFORM,
	ACPI_IRQ_MODEL_COUNT
};

static int acpi_bus_init_irq(void)
{
  ACPI_STATUS status = AE_OK;
  ACPI_OBJECT arg = { ACPI_TYPE_INTEGER };
  ACPI_OBJECT_LIST arg_list = { 1, &arg };
  char const *message = NULL;


  //int acpi_irq_model = ACPI_IRQ_MODEL_PIC;
  int acpi_irq_model = ACPI_IRQ_MODEL_IOAPIC;
  /*
   * Let the system know what interrupt model we are using by
   * evaluating the \_PIC object, if exists.
   */

  switch (acpi_irq_model) {
    case ACPI_IRQ_MODEL_PIC:
      message = "PIC";
      break;
    case ACPI_IRQ_MODEL_IOAPIC:
      message = "IOAPIC";
      break;
    case ACPI_IRQ_MODEL_IOSAPIC:
      message = "IOSAPIC";
      break;
    case ACPI_IRQ_MODEL_PLATFORM:
      message = "platform specific model";
      break;
    default:
      printf("Unknown interrupt routing model\n");
      return -1;
  }

  printf("Using %s for interrupt routing\n", message);

  arg.Integer.Value = acpi_irq_model;

  status = AcpiEvaluateObject(NULL, "\\_PIC", &arg_list, NULL);
  if (ACPI_FAILURE(status) && (status != AE_NOT_FOUND)) {
      ACPI_EXCEPTION((AE_INFO, status, "Evaluating _PIC"));
      return -1;
  }

  return 0;
}

static ACPI_STATUS
get_irq_cb(ACPI_RESOURCE *res, void *ctxt)
{
  l4_uint32_t *irq = (l4_uint32_t*)ctxt;
  if (!res)
    return AE_OK;

  if (res->Type != ACPI_RESOURCE_TYPE_IRQ)
    return AE_OK;

  *irq = res->Data.Irq.Interrupts[0];
  return AE_OK;
}

static ACPI_STATUS
print_device_cb(ACPI_HANDLE obj, UINT32 nest, void *ctxt, void **ret)
{
  printf("ACPI DEV: %p\n", obj);

  ACPI_BUFFER buf;
  ACPI_STATUS status;

  buf.Length = ACPI_ALLOCATE_BUFFER;

  status = AcpiGetIrqRoutingTable(obj, &buf);

  if (status)
    {
      printf("ERROR: while getting PRT for %s\n", "hoo");
      return AE_OK;
    }

  printf("PRT: %lx, %p\n", buf.Length, buf.Pointer);
  char *p = (char*)buf.Pointer;
  char *e = (char*)buf.Pointer + buf.Length;
  unsigned i = 0;
  while (1)
    {
      ACPI_PCI_ROUTING_TABLE *prt = (ACPI_PCI_ROUTING_TABLE *)p;
      if (prt->Length == 0)
	break;

      if (p + prt->Length > e)
	break;

      printf(" entry %d: pin=INT%c ADR=%x ",
	     i, (char)('A' + prt->Pin),
	     (unsigned)ACPI_HIWORD(prt->Address));
      if (prt->Source)
	{
	  if (!prt->Source[0])
	    {
	      printf("GLOBAL_IRQ=%d\n", prt->SourceIndex);
	    }
	  else
	    {
	      printf(" (dev[%s][%d]) ", prt->Source, prt->SourceIndex);
	      ACPI_STATUS status;
	      ACPI_HANDLE link_dev_handle;
	      status = AcpiGetHandle(obj, prt->Source, &link_dev_handle);
	      if (ACPI_FAILURE(status))
		{
		  printf("\nhoo, could not find PCI IRQ Link Device...\n");
		  return AE_OK;
		}

	      l4_uint32_t irq_num;
	      status = AcpiWalkResources(link_dev_handle, "_CRS", get_irq_cb, &irq_num);
	      if (ACPI_FAILURE(status))
		{
		  printf("\nhoo, could not evaluate _CRS of PCI IRQ Link Device\n");
		  return AE_OK;
		}
	      printf("GLOBAL_IRQ=%d\n", irq_num);
	    }
	}
      else
	printf("\n");

      p += prt->Length;
      ++i;
    }

  return AE_OK;
}

int main(int argc, char *argv[])
{
  printf("Hello from L4-ACPICA\n");

  AcpiDbgLevel =
      ACPI_LV_INIT
    //| ACPI_LV_INFO
   // | ACPI_LV_FUNCTIONS
   //     | ACPI_LV_ALL_EXCEPTIONS 
    //    | ACPI_LV_LOAD 
    //    | ACPI_LV_INIT_NAMES 
        | ACPI_LV_TABLES 
    //    | ACPI_LV_RESOURCES 
    //    | ACPI_LV_NAMES
    //    | ACPI_LV_VALUES 
    //    | ACPI_LV_OPREGION  
        | ACPI_LV_VERBOSE_INFO 
    //    | ACPI_LV_PARSE
    //    | ACPI_LV_DISPATCH
    //    | ACPI_LV_EXEC
    //    | ACPI_LV_IO
    ;

//0. enable workarounds, see include/acglobals.h
  AcpiGbl_EnableInterpreterSlack = (1==1);
  ACPI_STATUS status;

  status = AcpiInitializeSubsystem();
  if(status!=AE_OK)
    {
      exit(status);
    }

  status = AcpiInitializeTables (0, 0, TRUE);
  status = AcpiReallocateRootTable ();
  status = AcpiLoadTables ();

  if(ACPI_FAILURE(status))
    exit(status);

  status =
    AcpiEnableSubsystem(ACPI_NO_HARDWARE_INIT | ACPI_NO_ACPI_ENABLE
	| ACPI_NO_HANDLER_INIT);
  if (ACPI_FAILURE(status))
    {
      printf("Unable to start the ACPI Interpreter\n");
      exit(status);
    }

  status = AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);
  if (ACPI_FAILURE(status)) {
      printf("Unable to initialize ACPI objects\n");
      exit(status);
  }

  printf("Interpreter enabled\n");


  status = AcpiEnableSubsystem (~(ACPI_NO_HARDWARE_INIT | ACPI_NO_ACPI_ENABLE));
  if(ACPI_FAILURE(status))
    {
      exit(status);
    }

  status = AcpiInitializeObjects (ACPI_FULL_INITIALIZATION);
  if(ACPI_FAILURE(status))
    {
      exit(status);
    }

  /*
   * Get the system interrupt model and evaluate \_PIC.
   */
  int result = acpi_bus_init_irq();
  if (result)
    {
      printf("Could not initialize ACPI IRQ stuff\n");
      exit(1);
    }

  status = AcpiSubsystemStatus();

  if(ACPI_FAILURE(status))
    {
      exit(status);
    }
  else
    printf("acpi_subsystem initialized\n");


  ACPI_BUFFER ret_buffer;
  ret_buffer.Length = ACPI_ALLOCATE_BUFFER;

  status = AcpiGetSystemInfo(&ret_buffer);

  if(ACPI_FAILURE(status))
    exit(status);

  acpi_print_system_info(ret_buffer.Pointer);

  AcpiOsFree(ret_buffer.Pointer);


  void* sb_handle=0;
  status = AcpiGetDevices("PNP0A03", print_device_cb, &sb_handle, 0);
};
