LIB = libclxns.so
SRCS = common.c priority_queue.c resize_array.c hash_table.c

BUILDDIR = ../build

LIB_OUT = $(addprefix $(BUILDDIR)/, $(LIB))
OBJS = $(SRCS:%.c=$(addprefix $(BUILDDIR)/, %.o))
DEPS = $(OBJS:%.o=%.d)
CFLAGS = -g -I. -I /usr/local/include
#LDFLAGS = -linker_flags

$(BUILDDIR)/%.o : %.c
	@echo $(COMPILE.c) -MMD -fpic -o $@ $<
	@$(COMPILE.c) -MMD -fpic -o $@ $<

all : $(LIB_OUT)

$(LIB_OUT) : $(OBJS)
	$(LINK.c) $^ $(LIBS) -shared -o $@

-include $(DEPS)

.PHONY: clean
clean :
	rm -f $(OBJS) $(DEPS) $(LIB_OUT)
