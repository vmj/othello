DESTDIR ?=
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

CC ?= gcc

SRC = $(wildcard *.c)
HDR = $(filter-out $(SRC:.c=.h),$(wildcard *.h))
OBJ = $(SRC:.c=.o)
LDFLAGS = -lm -lGL -lGLU -lglut

all: othello

othello: $(HDR) $(OBJ)
	$(CC) -o othello $(OBJ) $(LDFLAGS)

clean:
	-@rm *~ *.o 2>/dev/null || true

distclean: clean
	-@rm othello 2>/dev/null || true

dist:

install: all
	cp othello $(DESTDIR)$(BINDIR)/othello

# Linux style, except -bl instead of -br, -nce instead of -ce, -ci8
# instead of -ci4, -l74 instead of -l80, and -psl instead of -npsl.
# Also, added -nut and -bli0.
indent:
	@indent -npro -linux -bl -nce -ci8 -l74 -psl -nut -bli0 *.[hc]
	$(MAKE) clean

depend:
	makedepend -Y. -- $(CFLAGS) -- $(SRC)
	rm Makefile.bak

# DO NOT DELETE

shift.o: common.h comp.h flippers.h transform.h board.h user.h shift.h
user.o: global.h board.h common.h transform.h camera.h shift.h display.h
user.o: user.h
display.o: global.h board.h common.h transform.h camera.h flippers.h shift.h
display.o: display.h
flippers.o: global.h board.h common.h transform.h camera.h display.h shift.h
flippers.o: flippers.h
comp.o: global.h board.h common.h transform.h camera.h shift.h comp.h
camera.o: common.h camera.h board.h transform.h
main.o: global.h board.h common.h transform.h camera.h display.h keyboard.h
main.o: flippers.h shift.h version.h
board.o: common.h board.h transform.h
keyboard.o: global.h board.h common.h transform.h camera.h keyboard.h
