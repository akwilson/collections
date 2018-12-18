# Makefile to build a shared object and/or a binary executable and/or unit test executable
# Include this file in your own Makefile
# Places object, dependency files and output files in $(BUILDDIR)
# $(BUILDDIR) created if it does not exist
# Compiles using full warnings and rebuilds when header files are changed
# `make clean` will remove all output from $(BUILDDIR)
# `make tests` will run the unit test binary
# `make install` will install the files in /usr/local

ifeq ($(BUILDDIR),)
	BUILDDIR = build
endif

LIB1_OUT = $(addprefix $(BUILDDIR)/, $(LIB1))
LIB1_OBJS = $(LIB1_SRCS:%.c=$(addprefix $(BUILDDIR)/, %.o))
LIB1_DEPS = $(LIB1_OBJS:%.o=%.d)

BIN1_OUT = $(addprefix $(BUILDDIR)/, $(BIN1))
BIN1_OBJS = $(BIN1_SRCS:%.c=$(addprefix $(BUILDDIR)/, %.o))
BIN1_DEPS = $(BIN1_OBJS:%.o=%.d)

TST1_OUT = $(addprefix $(BUILDDIR)/, $(TST1))
TST1_OBJS = $(TST1_SRCS:%.c=$(addprefix $(BUILDDIR)/, %.o))
TST1_DEPS = $(TST1_OBJS:%.o=%.d)

CFLAGS = -g -I. -I /usr/local/include -Wall -Wextra
#LDFLAGS = -linker_flags

$(BUILDDIR)/%.o : %.c
	$(COMPILE.c) -MMD -fpic -o $@ $<

all : directories $(LIB1_OUT) $(BIN1_OUT) $(TST1_OUT)

$(LIB1_OUT) : $(LIB1_OBJS)
	$(LINK.c) $^ -shared -o $@

$(BIN1_OUT) : $(BIN1_OBJS)
	$(LINK.c) $^ -L$(LIB_PATH) $(LIBS) -o $@

$(TST1_OUT) : $(TST1_OBJS)
	$(LINK.c) $^ -L$(LIB_PATH) $(LIBS) -o $@

-include $(LIB1_DEPS) $(BIN1_DEPS) $(TST1_DEPS)

ifeq ($(PREFIX),)
	PREFIX = /usr/local
endif

.PHONY: directories clean test install
directories :
	@mkdir -p $(BUILDDIR)

clean :
	rm -f $(LIB1_OBJS) $(LIB1_DEPS) $(LIB1_OUT) \
		  $(BIN1_OBJS) $(BIN1_DEPS) $(BIN1_OUT) \
		  $(TST1_OBJS) $(TST1_DEPS) $(TST1_OUT)

tests :
	@LD_LIBRARY_PATH=$(LIB_PATH) DYLD_LIBRARY_PATH=$(LIB_PATH) $(TST1_OUT)

INSTALL_H = $(HEADERS:%.h=$(addprefix $(DESTDIR)$(PREFIX)/include/, %.h))
INSTALL_L = $(addprefix $(DESTDIR)$(PREFIX)/lib/, $(LIB1))
INSTALL_B = $(addprefix $(DESTDIR)$(PREFIX)/bin/, $(BIN1))

install : $(INSTALL_H) $(INSTALL_L) $(INSTALL_B)

$(INSTALL_H) : $(HEADERS)
	install -d $(DESTDIR)$(PREFIX)/include
	install -m 644 $^ $(DESTDIR)$(PREFIX)/include

$(INSTALL_L) : $(LIB1_OUT)
	install -d $(DESTDIR)$(PREFIX)/lib
	install -m 644 $(LIB1_OUT) $(DESTDIR)$(PREFIX)/lib
	@if [ `uname` = "Linux" ]; then echo "*** Library installation complete. You may need to run 'sudo ldconfig' ***"; fi

$(INSTALL_B) : $(BIN1_OUT)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(BIN1_OUT) $(DESTDIR)$(PREFIX)/bin
