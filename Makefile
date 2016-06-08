export
DESTDIR ?=
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
MANDIR ?= $(PREFIX)/man
MAN1DIR ?= $(MANDIR)/man1

all: subdirs

clean: subdirs
	-@rm *~ 2>/dev/null || true

distclean: subdirs clean
	-@rm othello-*.tar.gz 2>/dev/null || true

dist: subdirs
	@$(MAKE) -s dist-internal VERSION=`grep _version src/version.h |sed -e 's/.* "//' -e 's/";//'`

install: subdirs

dist-internal:
	-@rm -rf othello-$(VERSION) othello-$(VERSION).tar.gz 2>/dev/null || true
	@mkdir othello-$(VERSION)
	@cp -r LICENSE.txt Makefile README.adoc doc src othello-$(VERSION)/
	@tar czf othello-$(VERSION).tar.gz othello-$(VERSION)
	@rm -rf othello-$(VERSION)

SUBDIRS = src doc

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@ $(MAKEFLAGS) $(MAKECMDGOALS)
