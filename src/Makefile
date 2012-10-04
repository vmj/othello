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

shift.o: common.h comp.h board.h flippers.h user.h shift.h
user.o: camera.h common.h shift.h board.h flippers.h display.h user.h
display.o: common.h board.h flippers.h camera.h shift.h display.h
flippers.o: common.h board.h flippers.h display.h shift.h
comp.o: board.h common.h flippers.h shift.h comp.h
camera.o: board.h common.h flippers.h camera.h
main.o: display.h common.h keyboard.h board.h flippers.h camera.h shift.h
main.o: version.h
board.o: common.h board.h flippers.h
keyboard.o: common.h camera.h keyboard.h
