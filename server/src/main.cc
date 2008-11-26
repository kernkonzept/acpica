#include <cstdio>
#include <l4/acpica/acpica.h>

int main(void)
{
  int err;
  err = acpica_init();
  if (err < 0)
    printf("ERROR: could not initialize ACPI\n");

  err = acpica_dump_prt();
  if (err < 0)
    printf("ERROR: could not dump the PCI IRQ routing table\n");

  return 0;
};
