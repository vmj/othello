
CC:=gcc

SRC = $(wildcard *.c)
HDR = $(filter-out $(SRC:.c=.h),$(wildcard *.h))
OBJ = $(SRC:.c=.o)
LDFLAGS = -lglut

all: othello

othello: $(HDR) $(OBJ)
	$(CC) $(LDFLAGS) -o othello $(OBJ)

clean:
	-@rm *~ *.o 2>/dev/null || true

distclean: clean
	-@rm othello 2>/dev/null || true

depend:
	makedepend -Y. -- $(CFLAGS) -- $(SRC)

# DO NOT DELETE

shift.o: common.h comp.h board.h flippers.h input.h shift.h
display.o: common.h board.h flippers.h camera.h shift.h display.h
flippers.o: common.h board.h flippers.h display.h shift.h
comp.o: board.h common.h flippers.h shift.h comp.h
camera.o: board.h common.h flippers.h camera.h
main.o: display.h common.h input.h board.h flippers.h camera.h shift.h
board.o: common.h board.h flippers.h
input.o: common.h camera.h board.h flippers.h shift.h display.h input.h
