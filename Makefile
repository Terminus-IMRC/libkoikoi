.PHONY: all
all:

TARGETS := libkoikoi.a libkoikoi.so
SRCS := $(addprefix src/, koikoi.c socket.c err.c)
INCDIRS := include include_local

CFLAGS_LOCAL := -Wall -Wextra -pipe -O2
ifneq '$(INCDIRS)' ''
 CFLAGS_LOCAL += $(addprefix -I, $(INCDIRS))
endif

OBJS := $(patsubst %.c, %.c.o, $(filter %.c, $(SRCS)))
DEPS := $(patsubst %.c, %.c.d, $(filter %.c, $(SRCS)))
ALLDEPS = $(MAKEFILE_LIST_SANS_DEPS)

CC := gcc
AR := ar
ARFLAGS := cr
RANLIB := ranlib
RM := rm -rf
MKDIR := mkdir -p
WC := wc -l -c
CTAGS := ctags

VALID_MAKECMDGOALS := all $(TARGETS) $(DEPS) $(OBJS) clean line tags
NONEED_DEP_MAKECMDGOALS := clean line tags

EXTRA_MAKECMDGOALS := $(filter-out $(VALID_MAKECMDGOALS), $(MAKECMDGOALS))
ifneq '$(EXTRA_MAKECMDGOALS)' ''
 $(error No rule to make target `$(firstword $(EXTRA_MAKECMDGOALS))')
else
 ifeq '$(filter-out $(NONEED_DEP_MAKECMDGOALS), $(MAKECMDGOALS))' '$(MAKECMDGOALS)'
  sinclude $(DEPS)
 endif
endif

MAKEFILE_LIST_SANS_DEPS := $(filter-out %.c.d, $(MAKEFILE_LIST))

LINK.o = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(TARGET_ARCH)
SHARED.o = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(TARGET_ARCH) $(LDLIBS) $(LDLIBS_LOCAL) -shared
COMPILE.c = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(TARGET_ARCH) -c
MKDEP.c = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(TARGET_ARCH) -M -MP -MT "$<.o"

all: $(TARGETS)

$(filter %.a, $(TARGETS)): $(OBJS) $(ALLDEPS)
	$(AR) $(ARFLAGS) "$@" $(OBJS)
	$(RANLIB) "$@"

$(filter %.so, $(TARGETS)): $(OBJS) $(ALLDEPS)
	$(SHARED.o) $(OBJS) -o "$@"

%.c.o: %.c $(ALLDEPS)
	$(COMPILE.c) "$<" -o "$@"

%.c.d: %.c $(ALLDEPS)
	$(MKDEP.c) "$<" -MF "$@"

.PHONY: clean
clean:
	$(RM) $(TARGETS)
	$(RM) $(OBJS)
	$(RM) $(DEPS)

.PHONY: line
line:
	$(WC) $(SRCS) $(MAKEFILE_LIST_SANS_DEPS)

.PHONY: tags
	$(CTAGS) -R .
