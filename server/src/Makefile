PKGDIR ?= ../..
L4DIR  ?= $(PKGDIR)/../..

LIBS := -lio-acpi -lio-direct -lsigma0

PRIVATE_INCDIR = $(PKGDIR)/server/lib-acpi/src/include $(PKGDIR)/server/lib-acpi/src/acpica/include
SRC_CC := main.cc

TARGET := acpica

L4_MULTITHREADED=y

include $(L4DIR)/mk/prog.mk
