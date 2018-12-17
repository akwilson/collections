SUBDIRS = src test

SUBCLEAN = $(addsuffix .cln, $(SUBDIRS))

.PHONY: clean install tests subdirs $(SUBDIRS) $(SUBCLEAN) $(SUBTESTS)

subdirs : $(SUBDIRS)

$(SUBDIRS) :
	$(MAKE) -C $@ --no-print-directory

test : src

clean : $(SUBCLEAN)

$(SUBCLEAN) :
	$(MAKE) clean -C $(basename $@) --no-print-directory

install : src
	$(MAKE) install -C src --no-print-directory

tests : test
	@$(MAKE) tests -C test --no-print-directory
