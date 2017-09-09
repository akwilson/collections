SUBDIRS = src test

SUBCLEAN = $(addsuffix .cln, $(SUBDIRS))

.PHONY: clean subdirs $(SUBDIRS) $(SUBCLEAN)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

test: src

clean: $(SUBCLEAN)

$(SUBCLEAN):
	$(MAKE) clean -C $(basename $@)
