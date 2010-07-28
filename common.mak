# =============================================================================
# definition
# =============================================================================

# -------------------------------------------------------------------
# windows or linux
# -------------------------------------------------------------------
ifeq ($(TERM),cygwin)

# windows
ifeq ($(NAME),libts1)
POSTFIX = .a
else
POSTFIX = .exe
endif
INSTALL_DIR = /cygdrive/c/windows/system32

else # neq ($(TERM),cygwin)

# linux
ifeq ($(NAME),libts1)
POSTFIX = .a
else
POSTFIX =
endif
INSTALL_DIR = /usr/local/bin

endif # ($(TERM),cygwin)

# -------------------------------------------------------------------
# debug or release
# -------------------------------------------------------------------
BUILD_TYPE = release

CFLAGS = -Wall -W -Werror
CFLAGS += -std=c99

ifeq ($(BUILD_TYPE), release)
	CFLAGS += -O2
	OBJ_DIR = release
else
	CFLAGS += -g
	OBJ_DIR = debug
endif

# -------------------------------------------------------------------
# others
# -------------------------------------------------------------------
CC = gcc
CPPFLAGS = -I. -I../lib
CXXFLAGS = $(CFLAGS)
COMPILE = $(CC) $(CPPFLAGS) $(CFLAGS) -c

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))
DEPS := $(patsubst %.c, %.d, $(SRCS))

# =============================================================================
# wildcard rule
# =============================================================================
%.d: %.c
	@echo make dependency file for $<
	@$(CC) -MM $(CPPFLAGS) $< > $@
	@$(CC) -MM $(CPPFLAGS) $< | sed s/.o/.d/ >> $@

%.o: %.c
	$(COMPILE) -o $@ $<

%.1: $(INSTALL_DIR)/%$(POSTFIX)
	-help2man -o $@ $<

$(INSTALL_DIR)/%$(POSTFIX): %$(POSTFIX)

%.html: %.1
	-man2html $< > $@

../release/%.html: %.html
	cp $< $@

# =============================================================================
# supported aim
# =============================================================================
all: $(NAME)$(POSTFIX)

$(NAME).a: $(OBJS)
	ar r $@ $(OBJS)

$(NAME).exe: $(OBJS) $(DEPS) ../lib/libts1.a
	$(CC) -o $@ $(OBJS) -L../lib -lts1
	cp $@ ../$(OBJ_DIR)/$@
	cp $@ $(INSTALL_DIR)/$@

$(NAME): $(OBJS) $(DEPS) ../lib/libts1.a
	$(CC) -o $@ $(OBJS) -L../lib -lts1
	cp $@ ../$(OBJ_DIR)/$@
	cp $@ $(INSTALL_DIR)/$@

clean:
	-rm -f $(OBJS) $(DEPS) *~
	-rm -f $(NAME) $(NAME).exe $(NAME).a $(NAME).1 $(NAME).html

explain:
	@echo "    Source     files: $(SRCS)"
	@echo "    Object     files: $(OBJS)"
	@echo "    Dependency files: $(DEPS)"

depend: $(DEPS)
	@echo "Dependency files are now up-to-date."

-include $(DEPS)

doc: ../release/$(NAME).html

install: $(NAME)$(POSTFIX)

uninstall:
	-rm -f $(INSTALL_DIR)/$(NAME)$(POSTFIX)
	-rm -f ../debug/$(NAME)$(POSTFIX)
	-rm -f ../release/$(NAME)$(POSTFIX)
	-rm -f ../release/$(NAME).html

# =============================================================================
# THE END
# =============================================================================
