
# compiler and flags
CC        := gcc
OPTLVL    := -O2
CFLAGS    := $(OPTLVL) -Wall -Wextra -Wpointer-arith -std=c99 -MMD -MP
LIB       := 
INC       := -Iinc/

# directories
SRCDIR    := src
BUILDDIR  := obj
TARGETDIR := bin

TARGET    := vector
FTARGET   := $(TARGETDIR)/$(TARGET)

# file extentions
SRCEXT    := c
OBJEXT    := o

SRC       := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJ       := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SRC:.$(SRCEXT)=.$(OBJEXT)))
DEPS      := $(OBJ:.o=.d)

all: $(FTARGET)

clean:
	rm -rf $(BUILDDIR)/*

mrproper:
	rm -rf $(TARGETDIR) $(BUILDDIR)

run: $(FTARGET)
	@./$(FTARGET)

$(FTARGET): $(OBJ)
	@mkdir -p $(TARGETDIR)
	$(CC) $(LIB) $^ -o $@

$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

-include $(DEPS)

.PHONY: all clean mrproper run
