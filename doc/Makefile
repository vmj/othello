DESTDIR ?=
PREFIX ?= /usr/local
MANDIR ?= $(PREFIX)/man
MAN1DIR ?= $(MANDIR)/man1

MAN_TXT = $(wildcard *.[1-8].txt)

HTML = $(MAN_TXT:.txt=.html)

MAN_XML = $(MAN_TXT:.txt=.xml)
MAN = $(MAN_XML:.xml=)

MAN1 = $(filter %.1,$(MAN))


all: man html

clean:
	-@rm *~ $(MAN_XML) 2>/dev/null || true

distclean: clean
	-@rm $(MAN) $(HTML) 2>/dev/null || true

dist: man html

install: install-man

install-man: man
	mkdir -p $(DESTDIR)$(MAN1DIR)
	cp $(MAN1) $(DESTDIR)$(MAN1DIR)/

#install-html: html
#	mkdir -p $(DESTDIR)$(DOCDIR)
#	cp $(HTML) $(DESTDIR)$(DOCDIR)/

man: $(MAN)

%: %.xml
	xmlto man $<

%.xml: %.txt
	asciidoc -b docbook -d manpage -o $@ $<

html: $(HTML)

%.html: %.txt
	asciidoc -b html -d manpage -o $@ $<
