.PHONY: all
all:

TARGETS := test-server.c.elf test-client.c.elf
INCDIRS := ../../include

CFLAGS_LOCAL := -Wall -Wextra -pipe -O2
ifneq '$(INCDIRS)' ''
 CFLAGS_LOCAL += $(addprefix -I, $(INCDIRS))
endif
LDLIBS_LOCAL := ../../libkoikoi.a
ALLDEPS := $(MAKEFILE_LIST)

CC := gcc
RM := rm -rf

LINK.o = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(TARGET_ARCH)

all: $(TARGETS)

%.c.elf: %.c $(ALLDEPS)
	$(LINK.o) "$<" $(LDLIBS) $(LDLIBS_LOCAL) -o "$@"

.PHONY: clean
clean:
	$(RM) $(TARGETS)
