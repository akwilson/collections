LIB = libclxns.so
SRCS = common.c priority_queue.c resize_array.c hash_table.c

BUILDDIR = ../build

LIB_OUT = $(addprefix $(BUILDDIR)/, $(LIB))
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
CFLAGS = -g -I. -I /usr/local/include
#LDFLAGS = -linker_flags

$(BUILDDIR)/%.o : %.c
	@echo $(COMPILE.c) -MMD -fpic -o $@ $<
	@$(COMPILE.c) -MMD -fpic -o $@ $<

all : $(LIB_OUT)

$(LIB_OUT) : $(addprefix $(BUILDDIR)/, $(OBJS))
	$(LINK.c) $^ $(LIBS) -shared -o $@

.PHONY: clean
clean :
	rm $(addprefix $(BUILDDIR)/, $(OBJS)) \
		$(addprefix $(BUILDDIR)/, $(DEPS)) \
		$(LIB_OUT)
