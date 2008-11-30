
#include <cstdio>
#include <cstdlib>

#include "acpi_l4.h"

extern "C" {
#include "acpi.h"
#include "accommon.h"
#include "acresrc.h"
}

#include <errno.h>
#include <l4/acpica/acpica.h>
#include <l4/cxx/list>

#define _COMPONENT		ACPI_BUS_COMPONENT
ACPI_MODULE_NAME("l4main");

struct Prt_entry : public cxx::List_item
{
  ACPI_PCI_ID   id;
  unsigned char pin;
  acpica_pci_irq irq;
};

struct Pci_dev;

struct Device : public cxx::List_item
{
  l4_uint32_t _adr;
  Device *_parent;
  Device *_cld;
  ACPI_HANDLE acpi_dev;

  explicit Device(l4_uint32_t adr = ~0)
  : _adr(adr), _parent(0), _cld(0), acpi_dev(0) {}
};

struct Pci_dev : public Device
{
  l4_uint16_t vendor;
  l4_uint16_t device;
  l4_uint32_t cls;

  explicit Pci_dev(l4_uint32_t adr)
  : Device(adr), vendor(0), device(0), cls(0) {}

  bool is_bridge() const { return cls == 0x0604; }
};

struct Pci_bridge : public Pci_dev
{
  unsigned char pri;
  unsigned char num;
  unsigned char subordinate;

  explicit Pci_bridge(l4_uint32_t adr)
  : Pci_dev(adr), pri(0), num(0), subordinate(0) {}
};


static Prt_entry *pci_irq_table;
static Pci_bridge *pci_root_bridge;

enum Acpi_irq_model_id {
	ACPI_IRQ_MODEL_PIC = 0,
	ACPI_IRQ_MODEL_IOAPIC,
	ACPI_IRQ_MODEL_IOSAPIC,
	ACPI_IRQ_MODEL_PLATFORM,
	ACPI_IRQ_MODEL_COUNT
};

static ACPI_STATUS
get_irq_cb(ACPI_RESOURCE *res, void *ctxt)
{
  acpica_pci_irq *irq = (acpica_pci_irq*)ctxt;
  if (!res)
    return AE_OK;

  if (res->Type != ACPI_RESOURCE_TYPE_IRQ)
    return AE_OK;

  irq->irq = res->Data.Irq.Interrupts[0];
  irq->polarity = res->Data.Irq.Polarity;
  irq->trigger  = res->Data.Irq.Triggering;
  return AE_OK;
}

inline void *operator new (size_t s)
{ return malloc(s); }

static int
acpica_add_prt_entry(ACPI_HANDLE obj, int segment, int bus,
                     ACPI_PCI_ROUTING_TABLE *e)
{
  if (!e)
    return -EINVAL;

  Prt_entry *ne = new Prt_entry();
  if (!ne)
    return -ENOMEM;

  ne->id.Segment = segment;
  ne->id.Bus = bus;
  ne->id.Device = (e->Address >> 16) & 0xffff;
  ne->id.Function = e->Address & 0xffff;
  ne->pin = e->Pin;

  ne->irq.irq = e->SourceIndex;
  ne->irq.polarity = ACPI_ACTIVE_LOW;
  ne->irq.trigger = ACPI_LEVEL_SENSITIVE;
  if (e->Source[0])
    {
      ACPI_HANDLE link;
      printf(" (dev[%s][%d]) ", e->Source, e->SourceIndex);
      ACPI_STATUS status;
      status = AcpiGetHandle(obj, e->Source, &link);
      if (ACPI_FAILURE(status))
	{
	  printf("\nhoo, could not find PCI IRQ Link Device...\n");
	  return -ENODEV;
	}

      status = AcpiWalkResources(link, (char*)"_CRS", get_irq_cb, &ne->irq);
      if (ACPI_FAILURE(status))
	{
	  printf("\nhoo, could not evaluate _CRS of PCI IRQ Link Device\n");
	  return -ENODEV;
	}
    }
  pci_irq_table = cxx::List_item::push_back(pci_irq_table, ne);
  return 0;
}

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

  status = AcpiEvaluateObject(NULL, (char*)"\\_PIC", &arg_list, NULL);
  if (ACPI_FAILURE(status) && (status != AE_NOT_FOUND)) {
      ACPI_EXCEPTION((AE_INFO, status, "Evaluating _PIC"));
      return -1;
  }

  return 0;
}


static ACPI_STATUS
add_pci_irq_table(ACPI_HANDLE obj, int bus)
{
  ACPI_BUFFER buf;
  ACPI_STATUS status;
  char buffer[80];
  ACPI_BUFFER ret_buf;
  ACPI_HANDLE handle;

  status = AcpiGetHandle(obj, (char*)"_PRT", &handle);

  // no PRT!!
  if (ACPI_FAILURE(status))
    return AE_OK;

  ret_buf.Length = sizeof (buffer);
  ret_buf.Pointer = buffer;

  status = AcpiGetName (obj, ACPI_FULL_PATHNAME, &ret_buf);

  if (ACPI_FAILURE (status))
    AcpiOsPrintf ("Could not convert name to pathname\n");
  else
    AcpiOsPrintf ("ACPI: PCI IRQ routing [%s._PRT]\n", buffer);

  buf.Length = ACPI_ALLOCATE_BUFFER;

  status = AcpiGetIrqRoutingTable(obj, &buf);

  if (status)
    {
      printf("ERROR: while getting PRT for [%s]\n", "buffer");
      return AE_OK;
    }

  char *p = (char*)buf.Pointer;
  char *e = (char*)buf.Pointer + buf.Length;
  while (1)
    {
      ACPI_PCI_ROUTING_TABLE *prt = (ACPI_PCI_ROUTING_TABLE *)p;
      if (prt->Length == 0)
	break;

      if (p + prt->Length > e)
	break;

      int err = acpica_add_prt_entry(obj, 0, bus, prt);
      if (err < 0)
	return !AE_OK;

      p += prt->Length;
    }

  return AE_OK;
}

static ACPI_STATUS
get_pcidev(ACPI_HANDLE h, UINT32 /*nest*/, void *ctxt, void **ret)
{
  Pci_dev *dev = (Pci_dev*)ctxt;

  ACPI_OBJECT adr;
  ACPI_BUFFER ret_buf;

  ret_buf.Length = sizeof (adr);
  ret_buf.Pointer = &adr;

    {
      ACPI_HANDLE dummy;
      if (ACPI_FAILURE(AcpiGetHandle(h, (char*)"_ADR", &dummy)))
	return AE_OK;
    }

  if (ACPI_SUCCESS(AcpiEvaluateObject (h, (char*)"_ADR", NULL, &ret_buf)))
    {
      if (adr.Type != ACPI_TYPE_INTEGER)
	{
	  printf("_ADR is not an integer\n");
	  return AE_OK;
	}
      else if (dev->_adr == adr.Integer.Value)
	{
	  *((ACPI_HANDLE*)ret) = h;
	  return AE_CTRL_TRUE;
	}
      else
	return AE_OK;
    }
  else
    {
      printf("ERROR: could not evaluate '_ADR'...\n");
      return AE_ERROR;
    }
}

static int
find_acpi_dev(Pci_dev *dev, ACPI_HANDLE bus)
{
  ACPI_STATUS status;
  ACPI_HANDLE handle;
  status = AcpiWalkNamespace(ACPI_TYPE_DEVICE, bus, 1, get_pcidev, dev, &handle);
  if (status == AE_CTRL_TRUE)
    {
      // found device
      dev->acpi_dev = handle;
    }
  return 0;
}

static int
scan_pci_bus(Pci_bridge *bus)
{
  ACPI_PCI_ID dev;

  //printf("scan PCI bus pri=%02x sec=%02x sub=%02x\n", bus->pri, bus->num, bus->subordinate);

  dev.Device = 0;
  dev.Segment = 0;
  dev.Bus = bus->num;
  for (dev.Device = 0; dev.Device < 32; ++dev.Device)
    {
      l4_uint32_t hdr_type;
      AcpiOsReadPciConfiguration(&dev, 0x0e, &hdr_type, 8);
      int funcs = (hdr_type & 0x80) ? 8 : 1;
      for (dev.Function = 0; dev.Function < funcs; ++dev.Function)
	{
	  l4_uint32_t vendor, _class, _subclass;
	  AcpiOsReadPciConfiguration(&dev, 0x00, &vendor, 16);
	  if (vendor == 0xffff)
	    break;


	  AcpiOsReadPciConfiguration(&dev, 0x0b, &_class, 8);
	  AcpiOsReadPciConfiguration(&dev, 0x0a, &_subclass, 8);

	  _class = (_class << 8) | _subclass;

	  Pci_dev *d;
	  if (_class == 0x0604)
	    {
	      Pci_bridge *b = new Pci_bridge((dev.Device << 16) | dev.Function);
	      b->pri = bus->num;
	      b->num = ++bus->subordinate;
	      b->subordinate = b->num;
	      scan_pci_bus(b);
	      d = b;
	    }
	  else
	    d = new Pci_dev((dev.Device << 16) | dev.Function);

	  d->vendor = vendor;
	  d->_parent = bus;
	  d->cls = _class;
	  bus->_cld = cxx::List_item::push_back(bus->_cld, d);
	  
	  if (d->_parent->acpi_dev)
	    find_acpi_dev(d, d->_parent->acpi_dev);
	}
    }

  if (bus->_parent)
    static_cast<Pci_bridge*>(bus->_parent)->subordinate = bus->subordinate;

  return 0;
}

static
int
dump_pci_bus(Pci_bridge *b)
{
  printf("PCI bus[%x]: bridge %04x:%02x:%02x.%x\n",
         b->num, 0, b->_parent ? (int)static_cast<Pci_bridge*>(b->_parent)->num : 0,
	 b->_adr >> 16, b->_adr & 0xffff);

  for (Pci_dev::T_iter<Device> c = b->_cld; *c; ++c)
    {
      Pci_dev *p = static_cast<Pci_dev*>(*c);
      printf("%04x:%02x:%02x.%x: class = %x\n",
	  0, (int)b->num, c->_adr >> 16, c->_adr & 0xffff, (int)p->cls);
      if (p->is_bridge())
	dump_pci_bus(static_cast<Pci_bridge*>(p));
    }

  return 0;
};

static int
add_pci_irq_tables(Pci_bridge *bus)
{
  ACPI_STATUS status;
  if (bus->acpi_dev)
    status = add_pci_irq_table(bus->acpi_dev, bus->num);

  for (Pci_bridge::T_iter<Device> c = bus->_cld; *c; ++c)
    {
      Pci_dev *p = static_cast<Pci_dev*>(*c);
      if (p->is_bridge())
	status = add_pci_irq_tables(static_cast<Pci_bridge*>(p));
    }

  return AE_OK;
}


static ACPI_STATUS
print_device_cb(ACPI_HANDLE obj, UINT32 /*nest*/, void * /*ctxt*/, void ** /*ret*/)
{
  ACPI_STATUS status;

  pci_root_bridge = new Pci_bridge(0);
  pci_root_bridge->acpi_dev = obj;

  scan_pci_bus(pci_root_bridge);

  if (!pci_root_bridge)
    return !AE_OK;

  dump_pci_bus(pci_root_bridge);

  // add the root bus PRT
  status = add_pci_irq_tables(pci_root_bridge);
  if (ACPI_FAILURE(status))
    return status;
#if 0
  for (Pci_bridge::T_iter<Pci_bridge> c = b->sub_busses; *c; ++c)
    status = add_pci_irq_table(xxx, c->num);
#endif
#if 0
  if (ctxt)
    {
      ACPI_OBJECT adr;
      ret_buf.Length = sizeof (adr);
      ret_buf.Pointer = &adr;
      printf("Scan sub-bus...\n");
      if (ACPI_SUCCESS(AcpiEvaluateObject (obj, "_ADR", NULL, &ret_buf)))
	{
	  if (adr.Type != ACPI_TYPE_INTEGER)
	    printf("_ADR is not an integer\n");
	  else
	    {
	      bus = adr.Integer.Value;
	      printf("_ADR=%x\n", bus);
	      bus >>= 16;
	    }
	}
      else
	printf("ERROR: could not evaluate '_ADR'...\n");

      printf("Address=%d\n", bus);
    }

  char *p = (char*)buf.Pointer;
  char *e = (char*)buf.Pointer + buf.Length;
  while (1)
    {
      ACPI_PCI_ROUTING_TABLE *prt = (ACPI_PCI_ROUTING_TABLE *)p;
      if (prt->Length == 0)
	break;

      if (p + prt->Length > e)
	break;

      int err = acpica_add_prt_entry(obj, 0, bus, prt);
      if (err < 0)
	return !AE_OK;

      p += prt->Length;
    }
#if 1
  ACPI_HANDLE child = NULL;
  while (ACPI_SUCCESS(AcpiGetNextObject(ACPI_TYPE_DEVICE, obj, child, &child)))
    {
      print_device_cb(child, nest, &child, ret);
    }
#endif
#endif
  return AE_OK;
}

int L4_CV acpica_init()
{
  printf("Hello from L4-ACPICA\n");

  AcpiDbgLevel =
      ACPI_LV_INIT
    //| ACPI_LV_INFO
   // | ACPI_LV_FUNCTIONSkern/irq.cpp
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


  status = AcpiGetDevices((char*)"PNP0A03", print_device_cb, NULL, 0);

  return 0;
}

int L4_CV
acpica_pci_irq_find(int segment, int bus, int device, int pin,
                    struct acpica_pci_irq **irq)
{
  Prt_entry::T_iter<Prt_entry> c = pci_irq_table;
  while (*c)
    {
      if (c->id.Segment == segment
	  && c->id.Bus == bus
	  && c->id.Device == device
	  && c->pin == pin)
	{
	  *irq = &c->irq;
	  return 0;
	}

      ++c;
    }

  return -ENODEV;
}


int L4_CV
acpica_dump_prt()
{
  Prt_entry::T_iter<Prt_entry> c = pci_irq_table;
  while (*c)
    {
      printf("%04x:%02x:%02x.%x: INT %c -> GSI %d\n",
	     c->id.Segment, c->id.Bus, c->id.Device, 0, 'A' + c->pin,
	     c->irq.irq);

      ++c;
    }

  return 0;
}
