CC      := gcc
AR      := ar
PREFIX  ?= /usr/local

LIBNAME := qvec

INC = -Iinc/
OPTLVL := -O2
CFLAGS  := $(OPTLVL) -Wall -Wextra -Wpointer-arith $(INC)
PICFLAGS := -fPIC

SRC     := $(wildcard src/qvec/*.c)

OBJ     := $(patsubst src/%.c,build/%.o,$(SRC))
PICOBJ  := $(patsubst src/%.c,build/%_pic.o,$(SRC))
HEADERS := $(wildcard inc/qvec/*.h)

STATIC_LIB := build/lib$(LIBNAME).a
SHARED_LIB := build/lib$(LIBNAME).so

all: $(STATIC_LIB) $(SHARED_LIB)

$(STATIC_LIB): $(OBJ)
	$(AR) rcs $@ $^

$(SHARED_LIB): $(PICOBJ)
	$(CC) -shared -o $@ $^

build/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

build/%_pic.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(PICFLAGS) -c $< -o $@

install: all
	install -d $(DESTDIR)$(PREFIX)/lib
	install -d $(DESTDIR)$(PREFIX)/include/$(LIBNAME)

	install -m 644 $(STATIC_LIB) $(DESTDIR)$(PREFIX)/lib/
	install -m 755 $(SHARED_LIB) $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(HEADERS) $(DESTDIR)$(PREFIX)/include/$(LIBNAME)/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/lib$(LIBNAME).a
	rm -f $(DESTDIR)$(PREFIX)/lib/lib$(LIBNAME).so
	rm -rf $(DESTDIR)$(PREFIX)/include/$(LIBNAME)

clean:
	rm -rf build

.PHONY: all install uninstall clean
