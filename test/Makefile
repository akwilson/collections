BIN = ctest
SRCS = ctest.c ra_tests.c pq_tests.c ht_tests.c

BUILDDIR = ../build

BIN_OUT = $(addprefix $(BUILDDIR)/, $(BIN))
OBJS = $(SRCS:%.c=$(addprefix $(BUILDDIR)/, %.o))
DEPS = $(OBJS:%.o=%.d)
CFLAGS = -g -I. -I /usr/local/include
#LDFLAGS = -linker_flags
LIB_PATH = ../build
LIBS = -lclxns

$(BUILDDIR)/%.o : %.c
	@echo $(COMPILE.c) -MMD -o $@ $<
	@$(COMPILE.c) -MMD -o $@ $<

all : $(BIN_OUT)

$(BIN_OUT) : $(OBJS)
	$(LINK.c) $^ -L$(LIB_PATH) $(LIBS) -o $@

-include $(DEPS)

.PHONY: clean
clean :
	rm -f $(OBJS) $(DEPS) $(BIN_OUT)
