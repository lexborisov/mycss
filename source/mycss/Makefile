TARGET := source/mycss
SRCDIR := source/mycss

CC ?= gcc

LIBPOSTFIX := .so
LIBNAME    := mycss
LIBSTATIC_POSTFIX  := _static

MyCSS_OPTIMIZATION_LEVEL ?= -O2

CFLAGS ?= -Wall -Werror
CFLAGS += $(MyCSS_OPTIMIZATION_LEVEL) -fPIC --std=c99 -I..

LDFLAGS += -lmyhtml

MyCSS_BUILD_WITHOUT_THREADS ?= NO
ifeq ($(MyCSS_BUILD_WITHOUT_THREADS),YES)
    $(info Build without POSIX Threads)
	CFLAGS += -DMyCSS_BUILD_WITHOUT_THREADS
else
    $(info Build with POSIX Threads)
	CFLAGS += -pthread
endif

ifeq ($(OS),Windows_NT)
else
	UNAM := $(shell uname -s)
	ifeq ($(UNAM),Darwin)
		LIBPOSTFIX := .dylib
	else
		CFLAGS += -D_POSIX_C_SOURCE=199309L
	endif
endif

SRCS := $(wildcard *.c)
SRCS += $(wildcard selectors/*.c namespace/*.c media/*.c values/*.c property/*.c declaration/*.c)
HDRS := $(wildcard *.h)
HDRS += $(wildcard selectors/*.h namespace/*.h media/*.h values/*.h property/*.h declaration/*.h)
OBJS := $(patsubst %.c, %.o, $(SRCS))

all: shared static

shared: $(OBJS) $(HDRS)
	$(CC) -shared $(LDFLAGS) $(OBJS) -o lib$(LIBNAME)$(LIBPOSTFIX)

static: shared
	$(AR) crus lib$(LIBNAME)$(LIBSTATIC_POSTFIX).a $(OBJS)

clean:
	rm -rf *.o
	rm -rf selectors/*.o
	rm -rf namespace/*.o
	rm -rf media/*.o
	rm -rf values/*.o
	rm -rf property/*.o
	rm -rf declaration/*.o
	rm -rf *lib$(LIBNAME)*

.PHONY: all clean
