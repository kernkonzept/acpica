#pragma once

#include <l4/sys/compiler.h>

struct acpica_pci_irq
{
  unsigned int irq;
  unsigned char trigger;
  unsigned char polarity;
};

__BEGIN_DECLS

int L4_CV acpica_init(void);
int L4_CV acpica_pci_irq_find(int segment, int bus, int dev, int pin,
                              struct acpica_pci_irq **irq);
int L4_CV acpica_dump_prt(void);

__END_DECLS
