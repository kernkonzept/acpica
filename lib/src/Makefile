PKGDIR ?= ../..
L4DIR  ?= $(PKGDIR)/../..

#LIBS := -lio-acpi -lio-direct -lsigma0

PRIVATE_INCDIR = $(PKGDIR)/server/lib-acpi/src/include $(PKGDIR)/server/lib-acpi/src/acpica/include
SRC_CC := init.cc

TARGET := libacpica.a

#L4_MULTITHREADED=y

CXXFLAGS += -fno-exceptions -fno-rtti

include $(L4DIR)/mk/lib.mk
