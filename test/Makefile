BIN = ctest
SRCS = ctest.c ra_tests.c pq_tests.c ht_tests.c

BUILDDIR = ../build

BIN_OUT = $(addprefix $(BUILDDIR)/, $(BIN))
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
CFLAGS = -g -I. -I /usr/local/include
#LDFLAGS = -linker_flags
LIB_PATH = ../build
RUN_PATH = $(CURDIR)/../build
LIBS = -lclxns

$(BUILDDIR)/%.o : %.c
	@echo $(COMPILE.c) -MMD -o $@ $<
	@$(COMPILE.c) -MMD -o $@ $<

all : $(BIN_OUT) $(BIN1_OUT) $(BIN2_OUT)

$(BIN_OUT) : $(addprefix $(BUILDDIR)/, $(OBJS))
	$(LINK.c) $^ -L$(LIB_PATH) $(LIBS) -o $@

.PHONY: clean
clean :
	rm $(addprefix $(BUILDDIR)/, $(OBJS)) \
		$(addprefix $(BUILDDIR)/, $(DEPS)) \
		$(BIN_OUT) \
		$(BIN1_OUT)
