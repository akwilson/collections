BIN = ctest
SRCS = ctest.c

BIN1 = pq_test
SRCS1 = pq_test.c

BUILDDIR = ../build

BIN_OUT = $(addprefix $(BUILDDIR)/, $(BIN))
BIN1_OUT = $(addprefix $(BUILDDIR)/, $(BIN1))
BIN2_OUT = $(addprefix $(BUILDDIR)/, $(BIN2))
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
OBJS1 = $(SRCS1:.c=.o)
DEPS1 = $(SRCS1:.c=.d)
OBJS2 = $(SRCS2:.c=.o)
DEPS2 = $(SRCS2:.c=.d)
CFLAGS = -g -I. -I /usr/local/include
#LDFLAGS = -linker_flags
LIB_PATH = ../build
RUN_PATH = $(CURDIR)/../build
LIBS = -lclxns

$(BUILDDIR)/%.o : %.c
	@echo $(COMPILE.c) -MMD -o $@ $<
	@$(COMPILE.c) -MMD -o $@ $<
	@sed -i 's,\($(BUILDDIR)/$*\.o\)[ :]*\(.*\),$@ : $$\(wildcard \2\)\n\1 : \2,g' $(BUILDDIR)/$*.d

all : $(BIN_OUT) $(BIN1_OUT) $(BIN2_OUT)

$(BIN_OUT) : $(addprefix $(BUILDDIR)/, $(OBJS))
	$(LINK.c) $^ -L$(LIB_PATH) -Wl,-rpath=$(RUN_PATH) $(LIBS) -o $@

$(BIN1_OUT) : $(addprefix $(BUILDDIR)/, $(OBJS1))
	$(LINK.c) $^ -L$(LIB_PATH) -Wl,-rpath=$(RUN_PATH) $(LIBS) -o $@

$(BIN2_OUT) : $(addprefix $(BUILDDIR)/, $(OBJS2))
	$(LINK.c) $^ -L$(LIB_PATH) -Wl,-rpath=$(RUN_PATH) $(LIBS) -o $@

.PHONY: clean
clean :
	rm $(addprefix $(BUILDDIR)/, $(OBJS)) \
		$(addprefix $(BUILDDIR)/, $(DEPS)) \
		$(addprefix $(BUILDDIR)/, $(OBJS1)) \
		$(addprefix $(BUILDDIR)/, $(DEPS1)) \
		$(BIN_OUT) \
		$(BIN1_OUT)
