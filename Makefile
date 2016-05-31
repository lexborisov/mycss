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
	cp $(SRCDIR)/selectors/*.h include/mycss/selectors
	cp $(SRCDIR)/namespace/*.h include/mycss/namespace
	cp $(SRCDIR)/rules/*.h include/mycss/rules
	cp $(SRCDIR)/media/*.h include/mycss/media

create:
	mkdir -p bin lib include/mycss
	mkdir -p bin lib include/mycss/selectors
	mkdir -p bin lib include/mycss/namespace
	mkdir -p bin lib include/mycss/rules
	mkdir -p bin lib include/mycss/media

.PHONY:all clean