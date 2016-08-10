TARGET := source/mycss
SRCDIR := source/mycss

CC ?= gcc

SRCS := $(wildcard $(SRCDIR)/*.c)
HDRS := $(wildcard $(SRCDIR)/*.h)
OBJS := $(patsubst %.c,%.o,$(SRCS))

SUBDIRS := examples test
LIBNAME := mycss

all: create
	$(MAKE) -C $(SRCDIR) $@
	cp $(SRCDIR)/lib$(LIBNAME)*.* lib/
	for f in $(SUBDIRS); do $(MAKE) -C $$f all; done

clean:
	$(MAKE) -C $(SRCDIR) clean
	for f in $(SUBDIRS); do $(MAKE) -C $$f clean; done
	rm -rf lib/*

test:
	test/test test Selectors

clone: create
	find include -name "*.h" -exec rm -f {} \;
	cp $(SRCDIR)/*.h include/mycss
	cp $(SRCDIR)/selectors/*.h include/mycss/selectors
	cp $(SRCDIR)/namespace/*.h include/mycss/namespace
	cp $(SRCDIR)/media/*.h include/mycss/media
	cp $(SRCDIR)/property/*.h include/mycss/property
	cp $(SRCDIR)/values/*.h include/mycss/values
	cp $(SRCDIR)/declaration/*.h include/mycss/declaration
	find include -name "*.h" -exec sed -i '.bak' -E 's/^[ \t]*#[ \t]*include[ \t]*"([^"]+)"/#include <\1>/g' {} \;
	find include -name "*.h.bak" -exec rm -f {} \;

create:
	mkdir -p bin lib include/mycss
	mkdir -p bin lib include/mycss/selectors
	mkdir -p bin lib include/mycss/namespace
	mkdir -p bin lib include/mycss/media
	mkdir -p bin lib include/mycss/property
	mkdir -p bin lib include/mycss/values
	mkdir -p bin lib include/mycss/declaration

.PHONY:all clean test