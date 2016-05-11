TARGET := source/mycss
SRCDIR := source/mycss

CC ?= gcc

SRCS := $(wildcard $(SRCDIR)/*.c)
HDRS := $(wildcard $(SRCDIR)/*.h)
OBJS := $(patsubst %.c,%.o,$(SRCS))

SUBDIRS := examples
LIBNAME := mycss

all: create
	$(MAKE) -C $(SRCDIR) $@
	cp $(SRCDIR)/lib$(LIBNAME)*.* lib/
	for f in $(SUBDIRS); do $(MAKE) -C $$f all; done

clean:
	$(MAKE) -C $(SRCDIR) clean
	for f in $(SUBDIRS); do $(MAKE) -C $$f clean; done
	rm -rf lib/*

clone: create
	cp $(SRCDIR)/*.h include/mycss

create:
	mkdir -p bin lib include/mycss

.PHONY:all clean